#include "rewriter.h"
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QVariant>

Rewriter::Rewriter(QObject* parent)
    : m_object(parent) {


    read_config();
    backup_orig(m_paths[SOURCE_IMPLEMENTATION_NAME],m_paths[PROJECT_DIRECTORY]);
    generate(m_paths[SOURCE_HEADER_NAME]
             ,m_paths[SOURCE_IMPLEMENTATION_NAME]
             ,m_paths[PROJECT_DIRECTORY]);
    mod_project();
}
Rewriter::~Rewriter(){}

void Rewriter::read_config() {

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,"MLCSoft","rpcframework");
    settings.beginGroup("Paths");
    m_paths[CLASS_NAME] = settings.value("ClassName").toString();
    m_paths[SOURCE_HEADER_NAME] = settings.value("HeaderName").toString();
    m_paths[SOURCE_IMPLEMENTATION_NAME] = settings.value("SourceName").toString();
    m_paths[PROJECT_FILE_NAME] = settings.value("ProFileName").toString();
    m_paths[PROJECT_DIRECTORY] = settings.value("Prefix").toString();
    settings.endGroup();
    settings.sync();

}

void Rewriter::generate(const QString& hdrName
                        ,const QString& impName
                        ,const QString& projDir
                        ) {

    QDir projectDirectory(projDir);
    introspect(hdrName,projectDirectory);
    createNewImplementation(impName,projectDirectory);
    modClientHeader(m_paths[SOURCE_HEADER_NAME], projectDirectory);
}

void Rewriter::introspect(const QString& hdrName, const QDir& dir) {

    m_obj = m_object->metaObject();
    int methodCount = m_obj->methodCount();
    QFile hdrSource(dir.absoluteFilePath(hdrName));
    if (!hdrSource.open(QIODevice::ReadOnly)) {

        qWarning() << "Unable to open header file for processing.";
        return;
    }
    QTextStream inStream(&hdrSource);
    QString line;
    QString n;
    while (!inStream.atEnd()) {
        line = inStream.readLine();
        for (int i = 0; i < methodCount; ++i) {

            QString pattern = "";
            n = m_obj->method(i).name();
            pattern += n;
            pattern += "(";
            if (line.contains(pattern,Qt::CaseSensitive)) {

                QMetaMethod::MethodType mt = m_obj->method(i).methodType();
                switch (mt) {
                case QMetaMethod::Method: {
                    m_methodIndexMap.insert(n,i);
                    m_methods << n;
                } break;
                default: break;
                }
            }
        }
    }
    hdrSource.close();
}

void Rewriter::createNewImplementation(const QString& implName, QDir& dir) {

    QString fileName = "rpc_" + implName;
    QFile newImplementation(dir.absoluteFilePath(fileName));

    if (!newImplementation.open(QIODevice::WriteOnly)) {

        qWarning() << "Could not create implementation file: " << dir.absoluteFilePath(fileName);
        return;
    }
    QTextStream impStream(&newImplementation);

    QFile oldImp(dir.absoluteFilePath(implName));
    if (!oldImp.open(QIODevice::ReadOnly)) {

        qWarning() << "Could not open old implementation file: " << dir.absoluteFilePath(implName);
        return;
    }
    int depth = 0;
    int lineNum = 1;

    bool inFunction = false;
    bool written = false;
    QByteArray lineToWrite;
    while (!oldImp.atEnd()) {

        QByteArray currentLine = oldImp.readLine();

        m_currentMethod = methodStartLine(currentLine);
        if (!m_currentMethod.isEmpty()){

            inFunction = true;
        }
        if (currentLine.contains(QByteArray("{"))) {

            ++depth;
        }
        if (currentLine.contains(QByteArray("}"))) {

            --depth;
        }
        if (depth == 0 && inFunction) {

            inFunction = false;
            written = false;
        }
        if (!written && inFunction) {

            formatRpcCall(currentLine);
            lineToWrite = m_newMethodSig + "\n" + m_rpcCall + "\n";
            m_currentMethod = "";
            written = true;
        } else if (written && inFunction){

            lineToWrite = "";
        } else {

            lineToWrite = currentLine;
        }

        ++lineNum;
        impStream << lineToWrite;
    }

    newImplementation.close();
    oldImp.close();
}

QString Rewriter::methodStartLine(QByteArray& line) {

    const QString lineString(line);
    foreach (const QString method, m_methods) {

        QString pattern = m_obj->className();
        QString thisMethod = method;
        pattern += "::";
        pattern += thisMethod;
        pattern += "(";
        if ( lineString.contains(pattern) && lineString.contains("{")) {
            return thisMethod;
        }
    }
    return QString();
}

void Rewriter::formatRpcCall(const QByteArray &line) {

    QString convLine(line);

    QList<bool> constList = checkForConst(line);
    QList<bool> refList = checkForRef(line);

    QString ending = convLine.mid(convLine.indexOf(")"));
    methodData_t c = createCallList();
    QString tmp = QString("%1 %2::%3(").arg(c.returnType.constData()).arg(m_obj->className()).arg(c.methodName);
    QString normal;
    QString sigContent;

    for (int i = 0; i < c.paramTypesList.size(); ++i) {

        QString constPrefix;
        QString refSuffix;
        if (!constList.isEmpty())
            constPrefix = (constList.at(i) == true) ? "const " : "";
        if (!refList.isEmpty())
            refSuffix = (refList.at(i) == true) ? "&" : "";
        normal += QString("%1 %2%3 param_%4").arg(constPrefix).arg(c.paramTypesList.at(i).constData()).arg(refSuffix).arg(i);
        sigContent += QString("%1").arg(c.paramTypesList.at(i).constData());
        sigContent += QString("%1").arg(refSuffix);

        if (i < (c.paramTypesList.size() - 1)) {
            normal += ",";
            sigContent += ",";
        }
    }

    tmp += normal;
    tmp += ending;

    m_newMethodSig = tmp.toLatin1();

    QString body = "    methodData_t data;\n    bool ok = false;\n";
    if (c.returnType != "void")
        body += QString("    %1 retVal;\n").arg(c.returnType.constData());

    body += QString("    data.methodId = %1;\n").arg(m_methodIndexMap.value(c.methodName));
    if (c.returnType != "void")
        body += QString("    data.returnData = QVariant::fromValue(retVal);\n");

    int count = 0;
    foreach (QByteArray paramType, c.paramTypesList) {

        body += QString("    data.argList.append(QVariant::fromValue(param_%1));\n").arg(count++);
    }
    body += QString("    data.returnType = \"%1\";\n").arg(c.returnType.constData());
    if (ending.contains("const")) {

        body += QString("    ok = CallInterfaceFactory::instance(const_cast<%1*>(this))->call(&data);\n").arg(m_obj->className());
    } else {

        body += "    ok = CallInterfaceFactory::instance(this)->call(&data);\n";
    }
    body += QString("    if (!ok) qWarning() << \"%1 to %2 failed.\";\n").arg("RPC Call").arg(c.methodName);
    if (c.returnType != "void") {

       body += QString("    return data.returnData.value<%1>();\n").arg(c.returnType.constData());
    }

    m_rpcCall = body.toLatin1();
}
QList<bool> Rewriter::checkForConst(const QString& line) {

    QList<bool> list;
    checkFor(line,list,"const");

    return list;
}
QList<bool> Rewriter::checkForRef(const QString &line) {

    QList<bool> list;
    checkFor(line,list,"&");
    return list;
}
void Rewriter::checkFor(const QString& line, QList<bool>& list, const QString& pattern) {

    QStringList sigList;
    int start = line.indexOf("(");
    int end = line.indexOf(")");
    QString signature = line.mid(start+1, end-start-1);
    if (!signature.isEmpty()) {
        sigList = signature.split(",");
        foreach (QString param, sigList) {
            if (param.contains(pattern))
                list.append(true);
            else
                list.append(false);
        }
    }
}

int Rewriter::findIndex(const QString &method) {

    return m_methodIndexMap.value(method);
}
methodData_t Rewriter::createCallList() {

    methodData_t mc;
    mc.methodName = m_currentMethod;
    int index = findIndex(m_currentMethod);
    mc.returnType = m_obj->method(index).typeName();
    mc.paramTypesList = m_obj->method(index).parameterTypes();
    mc.methodType = m_obj->method(index).methodType();
    return mc;
}
void Rewriter::mod_project() {

    bool replaced = false, includePathPresent = false;
    QDir projectDirectory(m_paths[PROJECT_DIRECTORY]);
    int lineCount = 0;
    QFile projFile(projectDirectory.absoluteFilePath(m_paths[PROJECT_FILE_NAME]));
    backup_orig(m_paths[PROJECT_FILE_NAME],m_paths[PROJECT_DIRECTORY]);


    QFile rm(QString("%1.tmp").arg(projectDirectory.absoluteFilePath(m_paths[PROJECT_FILE_NAME])));
    if (!rm.open(QIODevice::ReadWrite)) {

        qWarning("Could not open temporary project file.");
    }

    QFile temp(QString("%1.tmp").arg(projectDirectory.absoluteFilePath(m_paths[PROJECT_FILE_NAME])));

    if (!temp.open(QIODevice::ReadWrite)) {

        qWarning() << "Could not create/open temporary file";
        return;
    }
    QTextStream tempStream(&temp);
    if (!projFile.open(QIODevice::ReadWrite)) {

        qWarning() << "Could not open .pro file: " << projectDirectory.absoluteFilePath(m_paths[PROJECT_FILE_NAME]);
        return;
    }
    QTextStream projFileStream(&projFile);
    QString next;
    while (!projFileStream.atEnd()) {
        next = projFileStream.readLine();
        if (next.contains("../shared")) {
            includePathPresent = true;
        }
        ++lineCount;
    }
    projFileStream.seek(0);
    while (!projFileStream.atEnd()) {

        next = projFileStream.readLine();
        if (next.contains(m_paths[SOURCE_IMPLEMENTATION_NAME]) && !next.contains("rpc_")) {

            next.replace(m_paths[SOURCE_IMPLEMENTATION_NAME],QString("rpc_%1").arg(m_paths[SOURCE_IMPLEMENTATION_NAME]));
            replaced = true;
        }
        tempStream << next << "\n";
    }
    if (!replaced) {

        qWarning() << "Nothing changed in the source project file.";
    }
    if (!includePathPresent) {
        next = QString("INCLUDEPATH += ../shared\n");
        tempStream << next;
    }

    tempStream.flush();
    projFile.close();
    projFile.remove();
    temp.rename(QString("%1.tmp").arg(projectDirectory.absoluteFilePath(m_paths[PROJECT_FILE_NAME])),projectDirectory.absoluteFilePath(m_paths[PROJECT_FILE_NAME]));
    temp.close();
}

void Rewriter::backup_orig(const QString& orig, const QString& dir) {

    QDir projectDirectory(dir);
    QFile backup(projectDirectory.absoluteFilePath(orig));
    if (!QFile::exists(projectDirectory.absoluteFilePath(QString("%1_backup").arg(orig)))) {
        if ( !backup.copy(projectDirectory.absoluteFilePath(QString("%1_backup").arg(orig))) ) {
            qWarning() << "Could not create backup file " << projectDirectory.absoluteFilePath(QString("%1_backup").arg(orig))
                       << " or the file already exists.";
            return;
        }
    }
    qWarning() << "Created backup of file as " << projectDirectory.absoluteFilePath(QString("%1_backup").arg(orig));
    backup.close();
}
bool Rewriter::tryPass() {

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,"MLCSoft","rpcframework");
    settings.beginGroup("Rewriter");
    QVariant hasRun = settings.value("hasRun");
    if (!hasRun.toBool()) {
        settings.setValue("hasRun",true);
    }
    settings.endGroup();
    settings.sync();
    return hasRun.toBool();
}
QByteArray Rewriter::endIncludeSection(QByteArray &line) {

    static bool inInclude = false;
    static bool looking = true;
    QString current(line);
    QString pattern = "#include";
    if (looking && !inInclude) {
        if (current.contains(pattern)) {
            inInclude = true;
        }
    }else if (looking && inInclude) {
        if (current.contains("callinterface.h")) {
            looking = false;
            return QByteArray();

        }
        if (!current.contains(pattern)) {
            looking = false;
            return QByteArray("#include \"callinterface.h\"\n");
        }
    }
    return QByteArray();
}
void Rewriter::modClientHeader(const QString& hdrName, QDir& dir){

    QFile hdrFile(dir.absoluteFilePath(hdrName));
    if (!hdrFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open header file: " << dir.absoluteFilePath(hdrName);
        return;
    }

    QFile tmpFile(dir.absoluteFilePath("header.tmp"));
    if (!tmpFile.open(QIODevice::WriteOnly)) {
        qWarning("Could not open header.tmp");
    }
    QTextStream tempStream(&tmpFile);

    QByteArray lineToWrite;
    while(!hdrFile.atEnd()) {
        QByteArray currentLine = hdrFile.readLine();
        QByteArray includeInsert = endIncludeSection(currentLine);
        if (!includeInsert.isEmpty()) {
            lineToWrite = includeInsert;
        } else {
            lineToWrite = currentLine;
        }
        tempStream << lineToWrite;
    }
    hdrFile.close();
    tmpFile.close();
    hdrFile.remove();
    tmpFile.rename(dir.absoluteFilePath("header.tmp"),dir.absoluteFilePath(hdrName));
}



