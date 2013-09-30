#ifndef SERVICE_H
#define SERVICE_H
#include <QtCore/QUuid>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QDataStream>
#include <QtCore/QMetaMethod>
#include "rpc_global.h"

typedef int BindingType;

struct NumericSignalSlotPair {

    int sigId;
    int slotId;
    Rpc::BlockingMode mode;
};
struct StringSignalSlotPair {

    QByteArray sigName;
    QByteArray slotName;
    Rpc::BlockingMode mode;
};

struct SlotDescriptor{

    QByteArray name;
    int id;
    int paramCount;
};

struct SignalDescriptor{

    QByteArray name;
    int id;
    int paramCount;
};

struct MethodDescriptor {

    QByteArray name;
    int id;
    int paramCount;
    int ret;
};

struct Inventory {

    Inventory(){

        offset = 0;
        methodCount = 0;
        slotCount = 0;
        signalCount = 0;
    }
    ~Inventory(){}
    Inventory(const Inventory& rhs) {

        copy(rhs);
    }
    Inventory& operator=(const Inventory& rhs) {

        copy(rhs);
        return *this;
    }
    int idForName(const QByteArray& name, QMetaMethod::MethodType type) {

        int id = 0;
        switch (type) {
        case QMetaMethod::Method: {
            id = find(name, methodList);
        }break;
        case QMetaMethod::Slot: {
            id = find(name, slotList);
        }break;
        case QMetaMethod::Signal: {
            id = find(name, signalList);
        }break;
        default: {
            qWarning("Inventory::idForName: Unknown method type.");
        }break;
        }
        return id;
    }

    int offset;
    int methodCount;
    int slotCount;
    int signalCount;
    QList<MethodDescriptor> methodList;
    QList<SlotDescriptor> slotList;
    QList<SignalDescriptor> signalList;

private:
    void copy(const Inventory& rhs) {

        offset = rhs.offset;
        methodCount = rhs.methodCount;
        slotCount = rhs.slotCount;
        signalCount = rhs.signalCount;
        methodList = rhs.methodList;
        slotList = rhs.slotList;
        signalList = rhs.signalList;
    }

    template <class T>
    int find(const QByteArray name, const QList<T> list) {

        QListIterator<T> iterator(list);
        while (iterator.hasNext()) {
            T item = iterator.next();
            if (item.name == name) {
                return item.id;
            }
        }
        return -1;
    }
};
Q_DECLARE_METATYPE(Inventory)

class Controller;
class Invokation;
class MetaService : public QObject{


public:
    MetaService(QObject* subject
                ,qint16 port
                ,const QUuid& uuid
                ,const QByteArray& className
                );
    MetaService(const MetaService& rhs);
    ~MetaService();

    virtual bool bind(BindingType type) = 0;
    virtual bool connectSignalToRemoteSlot(QObject *source
                                           , const QByteArray& signal
                                           , const QByteArray& slot
                                           , Rpc::BlockingMode mode) = 0;

    virtual bool connectRemoteSignalToSlot(int signal
                                           , QObject* target
                                           , int slot
                                           , Rpc::BlockingMode mode) = 0;
    virtual bool emitForwardedSignal(int signal, void **args) = 0;
    virtual Invokation* createInvokation(int signalId, int slotId, const QList<QByteArray>& parameterTypes) = 0;
    virtual const NumericSignalSlotPair& numericSignalSlot() const = 0;
    virtual const StringSignalSlotPair& stringSignalSlot() const = 0;
    virtual int qt_metacall(QMetaObject::Call c, int id, void **arguments);

    Inventory inventory() const;
    void setInventory(const Inventory& inventory);

    qint16 port() const;
    QObject* subject() const;
    void setSubject(QObject* object);
    Controller* controller() const;
    void setController(Controller* controller);
    QUuid uuid() const;
    void setUuid(const QUuid& uuid);
    QByteArray className();
    void setBindingType(BindingType type);
    BindingType bindingType() const;


    friend const QDataStream& operator>>(QDataStream& stream, MetaService& service) {

        Inventory inventory;

        stream >> inventory.offset
               >> inventory.methodCount
               >> inventory.slotCount
               >> inventory.signalCount;

        for (int methods = 0; methods < inventory.methodCount; ++methods) {

            MethodDescriptor desc;
            stream >> desc.id
                   >> desc.name
                   >> desc.ret;
            inventory.methodList.append(desc);

        }

        for (int slotsc = 0; slotsc < inventory.slotCount; ++slotsc) {

            SlotDescriptor desc;
            stream >> desc.id
                   >> desc.name;
            inventory.slotList.append(desc);
        }

        for (int signalc = 0; signalc < inventory.signalCount; ++signalc) {

            SignalDescriptor desc;
            stream >> desc.id
                   >> desc.name;
            inventory.signalList.append(desc);
        }

        service.setInventory(inventory);
        return stream;
    }

    friend QDataStream& operator<<(QDataStream& stream, const MetaService& service) {

        Inventory inventory = service.inventory();

        stream << inventory.offset
               << inventory.methodCount
               << inventory.slotCount
               << inventory.signalCount;

        foreach (MethodDescriptor desc, inventory.methodList) {

            stream << desc.id
                   << desc.name
                   << desc.ret;
        }

        foreach (SlotDescriptor desc, inventory.slotList) {

            stream << desc.id
                   << desc.name;
        }

        foreach (SignalDescriptor desc, inventory.signalList) {

            stream << desc.id
                   << desc.name;
        }
        return stream;
    }

protected:
    virtual Inventory takeInventory() = 0;

private:
    QObject* m_subject;
    qint16 m_port;
    QUuid m_uuid;
    QByteArray m_className;
    Controller* m_controller;
    BindingType m_bindingType;
    Inventory m_inventory;
};
Q_DECLARE_METATYPE(MetaService*)
#endif // SERVICE_H
