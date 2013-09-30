#ifndef REWRITER_H
#define REWRITER_H
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include "callinterface.h"
#include "rpc_global.h"

#define SOURCE_HEADER_NAME          0
#define SOURCE_IMPLEMENTATION_NAME  1
#define PROJECT_FILE_NAME           2
#define PROJECT_DIRECTORY           3
#define CLASS_NAME                  4
#define DEPENDENCY_HEADER_NAME      5



class Rewriter
{

public:

    Rewriter(QObject* parent);
    ~Rewriter();

private:
    bool tryPass();
    void mod_project();
    void backup_orig(const QString&,const QString&);
    void read_config();
    void generate(const QString& hdrName
                  ,const QString& implName
                  ,const QString& projDir
                  );
    void introspect(const QString& hdr, const QDir& dir);
    void createNewImplementation(const QString& implName, QDir& dir);
    QString methodStartLine(QByteArray& line);
    void formatRpcCall(const QByteArray& line);
    int findIndex(const QString& method);
    QList<bool> checkForConst(const QString& line);
    QList<bool> checkForRef(const QString& line);
    void checkFor(const QString& line, QList<bool>&, const QString& pattern);
    methodData_t createCallList();
    QByteArray endIncludeSection(QByteArray& line);
    void modClientHeader(const QString& hdrName, QDir& dir);

    QObject *m_object;
    const QMetaObject *m_obj;
    QStringList m_methods;
    QString m_currentMethod;
    QMap<QString,int> m_methodIndexMap;
    QByteArray m_rpcCall, m_newMethodSig;
    QString m_paths[5];
    QStringList m_dependencies;
};


#endif // REWRITER_H
