#ifndef SOURCETESTCLASSTWO_H
#define SOURCETESTCLASSTWO_H

#include <QtCore/QObject>
class QTimer;
class SourceTestClassTwo : public QObject
{
    Q_OBJECT
public:
    explicit SourceTestClassTwo(QObject *parent = 0);
    
signals:
    void testSignal1(int,int);
    void testSignal2(bool,float);
public slots:
    void timeout();
private:
    QTimer* m_timer;
    
};

#endif // SOURCETESTCLASSTWO_H
