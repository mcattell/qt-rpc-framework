#include <QtCore/QString>
#include <Qt/QtTest>
#include <QtCore/QCoreApplication>
#include <QtCore/QProcess>
#include "rpcclient.h"
#include "rpctestclass.h"

const QString QtDir("/home/matthew/dev/qt5/install");
const QString makePath("/usr/bin/make");

class TestRpc : public QObject
{
    Q_OBJECT
    
public:
    TestRpc();
    
private Q_SLOTS:
    void initTestCase();
    void start_server();
    void client_bind();
    void call_method1();
    void cleanupTestCase();
public slots:
    void onClientStatusChanged(Rpc::Status status, const QUuid& target);


private:
    bool build_server();
    RpcClient* m_client;
    RpcTestClass* m_clientTestObject;
    QProcess* m_serverProcess;
    Rpc::Status m_status;
};

TestRpc::TestRpc() {
}

void TestRpc::initTestCase() {

    m_status = Rpc::ACTIVE;
    m_clientTestObject = new RpcTestClass(this);
    m_clientTestObject->setObjectName("TestObject");
    m_client = new RpcClient(this);
    connect(m_client,SIGNAL(clientStatusChanged(Rpc::Status,QUuid)),this,SLOT(onClientStatusChanged(Rpc::Status,QUuid)));
}
bool TestRpc::build_server() {

    QProcess qmake;
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("QTDIR", QtDir);
    env.insert("PATH", env.value("PATH") + QtDir + "/" + "bin");
    qmake.setProcessEnvironment(env);

    QStringList args;
    QString cwd = QDir::currentPath();
    qmake.setWorkingDirectory(cwd + "/server");
    QString arg0 = "server.pro";
    QString arg1 = "Makefile";

    args.append(arg0);
    args.append("-o");
    args.append(arg1);

    qmake.start("qmake", args);

    if (!qmake.waitForFinished()) {

        qWarning() << "qmake did not finish in time.";
        return false;
    }

    QProcess make;
    QStringList makeArgs;
    makeArgs.append("-j");
    makeArgs.append("tst_serverside");
    make.setProcessEnvironment(env);
    make.setWorkingDirectory(cwd + "/server");
    make.start("make", makeArgs);

    if (!make.waitForFinished(10000)) {

        qWarning() << "make did not finish in time.";
        return false;
    }
    return true;

}

void TestRpc::cleanupTestCase() {
}
void TestRpc::start_server() {

    bool built = build_server();
    QVERIFY(built);
    m_serverProcess = new QProcess(this);
    m_serverProcess->start("server/tst_serverside");
    if (!m_serverProcess->waitForStarted()) {

        qWarning("Could not start the server process.");
    }
}

void TestRpc::call_method1() {

    int ret = m_clientTestObject->getterTest1(10);
    QVERIFY(ret==10);
    while (m_status == Rpc::ACTIVE) {
        qApp->processEvents();
    }

    m_serverProcess->waitForFinished();

}

void TestRpc::client_bind() {

    bool ok = m_client->bind(m_clientTestObject,"192.168.0.196",5656);
    QVERIFY(ok);

}
void TestRpc::onClientStatusChanged(Rpc::Status status, const QUuid& target) {

    m_status = status; {
        if (status == Rpc::INACTIVE) {
            m_serverProcess->terminate();
        }
    }
    qDebug() << "Client status changed to: " << status;
}

QTEST_GUILESS_MAIN(TestRpc)

#include "tst_testrpc.moc"
