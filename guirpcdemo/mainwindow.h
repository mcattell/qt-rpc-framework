#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>

class RpcServer;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void receiveImage(const QImage& image);

    
private:
    Ui::MainWindow *ui;
    RpcServer* m_rpcServer;
};

#endif // MAINWINDOW_H
