#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "rpcserver.h"
#include <QtCore/QDebug>
#include <QtGui/QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    m_rpcServer = new RpcServer(this);

    if (!m_rpcServer->registerReceiver(this, 6000)) {
        qWarning("Unable to register receiver.");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::receiveImage(const QImage& image) {

    ui->imagePanel->setPixmap(QPixmap::fromImage(image));
}
