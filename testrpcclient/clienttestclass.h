#ifndef CLIENTTESTCLASS_H
#define CLIENTTESTCLASS_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>

class QTimer;
class ClientTestClass : public QObject
{
    Q_OBJECT
public:
    explicit ClientTestClass(QObject *parent = 0);
    
signals:
    void another();
    void testSignal1(int,bool);
    void testSignal2(QString);
    void testSignal3(double);
    void another1(bool);
    void testSignal4(int);
public slots:
    void testSlot1(int);
    void testSlot2(bool);
    void debugSlot(int,bool);
    void debugSlot2(int,bool);
    void testSlot3();
    void anotherSlot1(int a,int b);
    void testSlot4(bool,float);
    void debugSlot2(QString);
    void debugSlot3(QVariant);
    void debugSlot4(QVariant);
private slots:
    void timerExpired();
private:
    QTimer* m_testTimer;
};

#endif // CLIENTTESTCLASS_H
