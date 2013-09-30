#include "server_handler.h"
#include "packet.h"
#include "callinterface.h"
#include "server_controller.h"
#include "server_metaservice.h"

ServerMethodCallRequestHandler::ServerMethodCallRequestHandler(Controller *controller)
    : RequestHandler(controller) {
}
ServerMethodCallRequestHandler::~ServerMethodCallRequestHandler(){}

ServerMethodCallConfirmationHandler::ServerMethodCallConfirmationHandler(Controller *controller)
    : ConfirmationHandler(controller) {}

ServerMethodCallConfirmationHandler::~ServerMethodCallConfirmationHandler() {}

void ServerMethodCallRequestHandler::handle(Packet & packet) {

    int count = 0;
    QUuid key;
    methodData_t method;

    BEGIN_DESERIALIZER
    stream >> key
           >> method;
    END_DESERIALIZER

    count = method.argList.size();

    QVector<void*> args;

    char *tmp = 0;
    QVariant ret;
    if (method.returnType != "void") {

        tmp = new char[QMetaType::sizeOf(QVariant::nameToType(method.returnType))];
        ret = QVariant(QVariant::nameToType(method.returnType),(void*)tmp);
        args.append(ret.data());
    } else {

        args.append(0);
    }

    for (int i = 0; i < count; ++i) {

        args.append((void*)method.argList.at(i).data());
    }
    if (key == m_controller->metaService()->uuid()) {

        int r = m_controller->metaService()->qt_metacall(QMetaObject::InvokeMetaMethod, method.methodId, args.data());

        r += (m_controller->metaService()->inventory().offset + m_controller->metaService()->inventory().methodCount);

        if (r == method.methodId) {

            BEGIN_SERIALIZER
                    stream << ret;
            END_SERIALIZER

            packet.setMsg(_RETURN_FROM_CALL_);
            packet.setType(_CONFIRMATION_);
            packet.setPayload(data);

            m_controller->confirm(packet);
        }

    } else {

        qWarning("Received a method call request with an invalid or incorrect UUID.");
    }
    if (tmp)
        delete tmp;
}


void ServerMethodCallConfirmationHandler::handle(Packet & packet) {
    Q_UNUSED(packet);
}
