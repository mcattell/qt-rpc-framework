#ifndef CLIENT_INVOKATION_H
#define CLIENT_INVOKATION_H
#include <QtCore/QByteArray>
#include "invokation.h"

class MetaService;
class ClientInvokation : public Invokation
{
public:
    ClientInvokation(MetaService* parent
                     , int signalId = 0
                     , int slotId = 0
                     , int methodId = 0
                     , int returnType = 0
                     , const QList<QByteArray>& parameterTypes = QList<QByteArray>());
    ~ClientInvokation();
    virtual void call(QObject *sender, void **arguments);
    const int& signalId() const;
    const int& slotId() const;
    const QVariantList& parameters() const;

private:
    void serialize(void** args);
    MetaService* m_parent;
    QList<QByteArray> m_parameterTypes;
    int m_signalId, m_slotId, m_methodId, m_returnType;
    QVariantList m_parameters;

};

#endif // CLIENT_INVOKATION_H
