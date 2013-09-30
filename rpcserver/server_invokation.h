#ifndef SERVER_INVOKATION_H
#define SERVER_INVOKATION_H
#include "invokation.h"

class MetaService;
class ServerInvokation : public Invokation
{
public:
    ServerInvokation(MetaService* parent, int signalId, int slotId, const QList<QByteArray>& parameterTypes);
    ~ServerInvokation();
    virtual void call(QObject *sender, void **arguments);
    const int& signalId() const;
    const int& slotId() const;
    const QVariantList& parameters() const;

private:
    void deserialize(void** args);
    MetaService* m_parent;
    QList<QByteArray> m_parameterTypes;
    int m_signalId;
    int m_slotId;
    QVariantList m_parameters;

};
#endif // SERVER_INVOKATION_H
