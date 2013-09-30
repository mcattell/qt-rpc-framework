#ifndef INVOKATION_H
#define INVOKATION_H
#include <QtCore/QList>
#include <QtCore/QVariant>

class QObject;
class Invokation {

public:
    virtual void call(QObject *sender, void **arguments) = 0;
    virtual const int& signalId() const = 0;
    virtual const int& slotId() const = 0;
    virtual const QVariantList& parameters() const = 0;
};

#endif // INVOKATION_H
