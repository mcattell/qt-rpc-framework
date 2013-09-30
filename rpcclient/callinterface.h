#ifndef CALLINTERFACE_H
#define CALLINTERFACE_H
#include <QtCore/QDebug>
#include <QtCore/QMetaMethod>
#include <QtCore/QDataStream>
#include <QtCore/QMultiMap>
#include <QtCore/QVariant>
#include "rpc_global.h"



struct methodData_t{

    methodData_t(){}
    ~methodData_t(){}
    methodData_t(const methodData_t& m) {

        methodName = m.methodName;
        methodId = m.methodId;
        methodSignature = m.methodSignature;
        methodType = m.methodType;
        paramTypesList = m.paramTypesList;
        argList = m.argList;
        ret = m.ret;
        returnType = m.returnType;
        returnData = m.returnData;
    }

    QString methodName;
    int methodId;
    QByteArray methodSignature;
    QMetaMethod::MethodType methodType;
    QList<QByteArray> paramTypesList;
    QList<QVariant> argList;
    QGenericReturnArgument ret;
    QByteArray returnType;
    QVariant returnData;

    friend const QDataStream& operator>>(QDataStream& stream, methodData_t& method) {

        int argCount = 0;

        stream >> method.methodId
               >> argCount;

        for (int i = 0; i < argCount; ++i) {

            QVariant arg;
            stream >> arg;
            method.argList.append(arg);
        }
        stream >> method.returnType;
        stream >> method.returnData;
        return stream;
    }

    friend QDataStream& operator<<(QDataStream& stream, const methodData_t& method) {

        stream   << method.methodId
                 << (int)method.argList.size();

        foreach (QVariant arg, method.argList) {

            stream << arg;
        }
        QVariant v(QVariant::nameToType(method.ret.name()));
        stream << method.returnType;
        stream << v;
        return stream;
    }
};

class Controller;
struct CallInterface {

    virtual bool call(methodData_t* data) = 0;
};

struct ClientCallInterface : public CallInterface {

    ClientCallInterface(Controller* controller);
    virtual bool call(methodData_t *data);

private:
    Controller* m_controller;
};

class CallInterfaceFactory {

    CallInterfaceFactory();

public:
    ~CallInterfaceFactory();
    static CallInterface* instance(QObject* subject);
    static CallInterfaceFactory* instance();
    CallInterface* create(Controller* controller);

private:
    static CallInterfaceFactory* s_instance;
    static QHash<QObject*, CallInterface*> s_interfaces;
};

#endif // CALLINTERFACE_H


