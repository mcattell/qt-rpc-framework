#ifndef TESTRPCCLASS_H
#define TESTRPCCLASS_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUuid>
#include "rpc_global.h"
class TestRpcClass : public QObject
{
    Q_OBJECT
public:
    explicit TestRpcClass(QObject *parent = 0);
    
    Q_INVOKABLE void testMethod(int);
signals:
    void testSignal1(int);
    void testSignal2(bool);
public slots:
    void testSlot1(int,bool);
    void testSlot2(QString);
    void testSlot3(int,bool);

    void onStatusChanged(Rpc::Status, const QUuid& id);
};

#endif // TESTRPCCLASS_H
