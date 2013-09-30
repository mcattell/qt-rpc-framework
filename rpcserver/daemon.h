#ifndef DAEMON_H
#define DAEMON_H

#include <QtNetwork/QTcpServer>

class Daemon : public QTcpServer
{
    Q_OBJECT
public:
    explicit Daemon(QObject *parent = 0);
    inline void setPort(qint16 port) {
        m_port = port;
    }
    inline int port() const {
        return m_port;
    }
signals:
    void notifyConnection(qintptr);

public slots:
    void start();

private:
    qint16 m_port;
    void incomingConnection(qintptr);
};

#endif // DAEMON_H
