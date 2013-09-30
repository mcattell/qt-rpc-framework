#ifndef SOURCETESTOBJECT_H
#define SOURCETESTOBJECT_H
#include <QtCore/QObject>

class QTimer;
class SourceTestObject : public QObject
{
    Q_OBJECT
public:
    explicit SourceTestObject(QObject *parent = 0);
    
signals:
    void testSignal1(int,int);
public slots:
    void timeout();
private:
    QTimer* m_timer;
};

#endif // SOURCETESTOBJECT_H
