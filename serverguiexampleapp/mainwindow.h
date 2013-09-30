#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
class QTimer;
class RpcClient;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void sendImage(const QImage& image);
private slots:
    void timeout();
private:
    Ui::MainWindow *ui;
    RpcClient* m_rpcClient;
    QTimer* m_timer;
};

#endif // MAINWINDOW_H
