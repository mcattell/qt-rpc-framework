#include <QtCore/QMetaObject>
#include <QtCore/QMetaMethod>
#include <QtCore/QDebug>
#include "metaservice.h"


MetaService::MetaService(QObject *subject
                         , qint16 port
                         , const QUuid& uuid
                         , const QByteArray& className)
    : QObject()
    ,m_subject(subject)
    ,m_port(port)
    ,m_uuid(uuid)
    ,m_className(className){

    qRegisterMetaType<Inventory>("Inventory");
}
MetaService::MetaService(const MetaService &rhs)
    : QObject()
    ,m_subject(rhs.m_subject)
    ,m_port(rhs.m_port)
    ,m_uuid(rhs.m_uuid)
    ,m_className(rhs.m_className)
    ,m_inventory(rhs.m_inventory){
}
MetaService::~MetaService() {
}
qint16 MetaService::port() const {

    return m_port;
}
QObject* MetaService::subject() const {

    return m_subject;
}
void MetaService::setSubject(QObject *object) {

    m_subject = object;
}
QUuid MetaService::uuid() const {

    return m_uuid;
}
void MetaService::setUuid(const QUuid &uuid) {

    m_uuid = uuid;
}
QByteArray MetaService::className() {

    return m_className;
}
void MetaService::setController(Controller *controller) {

    m_controller = controller;
}
Controller* MetaService::controller() const {

    return m_controller;
}
void MetaService::setBindingType(BindingType type) {

    m_bindingType = type;
}
BindingType MetaService::bindingType() const {

    return m_bindingType;
}
int MetaService::qt_metacall(QMetaObject::Call c, int id, void **arguments) {

    Q_UNUSED(c);
    Q_UNUSED(id);
    Q_UNUSED(arguments);
    return -1;
}
Inventory MetaService::inventory() const {

    return m_inventory;
}
void MetaService::setInventory(const Inventory &inventory) {

    m_inventory = inventory;
}

