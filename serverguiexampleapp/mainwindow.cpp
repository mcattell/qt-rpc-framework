#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rpcclient.h"
#include <QtCore/QUuid>
#include <QtCore/QTimer>
#include <QtGui/QImage>
#include <QtCore/QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeout()));
    m_rpcClient = new RpcClient(this);
    QUuid id = m_rpcClient->bind("MainWindow",6000);
    m_rpcClient->connectSignalToRemoteSlot(this, "sendImage(QImage)","receiveImage(QImage)",id);
    m_timer->start(16);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::timeout() {
    QImage image1("../../rpc-project/serverguiexampleapp/4878_example.png");
    QImage image2("../../rpc-project/serverguiexampleapp/spike_example.png");

    if (image1.isNull())
        qWarning() << "Image file is not valid.";
    static bool flag = false;
    if (!flag)
        emit sendImage(image1);
    else
        emit sendImage(image2);
    flag = !flag;
}
