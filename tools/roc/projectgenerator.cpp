#include <QtCore/QProcess>
#include <QtCore/QSettings>
#include "projectgenerator.h"
#include "filehelper.h"


ProjectGenerator::ProjectGenerator(QObject *parent
                                   , const QStringList& mandatory
                                   , const QStringList& includePaths
                                   , const QStringList& libraryPaths
                                   , const QStringList& dependencies
                                   , const QStringList& buildTools)
    : QObject(parent)
    , m_mandatory(mandatory)
    , m_includePaths(includePaths)
    , m_libPaths(libraryPaths)
    , m_dependencies(dependencies)
    , m_buildTools(buildTools){

    saveSettings();
    copySourceFiles();
    createLoaderProjectFile();
    createLoaderMain();
    if (buildSubProject()) {

        if (runSubProject()) {

            modifySourceProjectFile();
        }
    }
}

ProjectGenerator::~ProjectGenerator() {
}

/***********************************************************************************************************************************
 * Modifies the source project .pro file so that it contains the rpc_ version of the source .cpp QObject subclass which is the     *
 * candidate for acting as the client side RPC stub.                                                                               *
 ***********************************************************************************************************************************/
void ProjectGenerator::modifySourceProjectFile() {


    QTextStream proFileStream;
    QFile proFile;

    if (FileHelper::openReadOnlyTextStream(proFile, proFileStream, m_mandatory[PROJECT_DIRECTORY_PREFIX] , m_mandatory[PROJECT_FILE_NAME], true)) {
        qDebug() << "opened source project file, " << m_mandatory[PROJECT_DIRECTORY_PREFIX] + "/" + m_mandatory[PROJECT_FILE_NAME];
    }

    QTextStream newProFileStream;
    QFile newProFile;
    QString newProFileName = "temp.pro";

    if (!FileHelper::openWriteOnlyTextStream(newProFile, newProFileStream, QDir::currentPath(), newProFileName, false)) {

        qWarning() << "Opened temporary project file stream on " << newProFile.fileName();
    }

    while (!proFileStream.atEnd()) {

        QString current = proFileStream.readLine();
        if (current.contains(m_mandatory[SOURCE_IMPLEMENTATION_NAME]) && !current.contains("rpc_")) {

            current.replace(m_mandatory[SOURCE_IMPLEMENTATION_NAME],QString("rpc_%1").arg(m_mandatory[SOURCE_IMPLEMENTATION_NAME]));
        }
        newProFileStream << current << "\n";
    }
    // Add rpcclient library to the source project
    newProFileStream << "LIBS += -L" << m_buildTools[QTDIR] << "/lib -lrpcclient\n";

    proFile.remove();
    if (!newProFile.rename(QDir::currentPath() + "/" + "temp.pro", m_mandatory[PROJECT_DIRECTORY_PREFIX] + "/" + m_mandatory[PROJECT_FILE_NAME])) {

        qWarning() << "Coul not rename temporary file, temp.pro.";
    }
}
/***********************************************************************************************************************************
 * Creates the project file for the loader project from a template called loader.dat                                               *
 ***********************************************************************************************************************************/
void ProjectGenerator::createLoaderProjectFile() {

    // Open the template file, loader.dat
    QTextStream loaderDatStream;
    QFile loader;

    if (!FileHelper::openReadOnlyFromResource(loader, ":/sources/loader.dat", loaderDatStream)) {

         qWarning() << "Unable to open loader.dat from resource";
    }


    QTextStream projectFileStream;
    QFile file;
    QString projectFileName = "loader.pro";
    QString cwd = QDir::currentPath();
    cwd += "/subproject";


    if (!FileHelper::openWriteOnlyTextStream(file, projectFileStream, cwd, projectFileName, false)) {

        qWarning() << "Unable to open file stream on " << file.fileName();
    }

    createProjectFileReplacements();

    while (!loaderDatStream.atEnd()) {

        QString current = loaderDatStream.readLine();
        QString outputLine = current;
        for (int count = 0; count < LOADER_ELEMENTS; ++count) {

            if (current.contains(loaderElements[count])) {
                outputLine = current.replace(loaderElements[count], m_replacements[count]);

            } else continue;

        }
        projectFileStream << outputLine << "\n";
    }
}
/***********************************************************************************************************************************
 * Creates the main.cpp file for the loader project from a template called main.dat                                                *
 ***********************************************************************************************************************************/
void ProjectGenerator::createLoaderMain() {

    // Open the template file, loader.dat
    QTextStream mainStream;
    QFile main;

    if (!FileHelper::openReadOnlyFromResource(main, ":/sources/main.dat", mainStream)) {

         qDebug() << "Unable to open main.dat from resource";
    }


    QTextStream mainFileStream;
    QFile file;
    QString mainFileName = "main.cpp";
    QString cwd = QDir::currentPath();
    cwd += "/subproject";


    if (!FileHelper::openWriteOnlyTextStream(file, mainFileStream, cwd, mainFileName, false)) {

        qWarning() << "Unable to open file stream on " << file.fileName();
    }

    while (!mainStream.atEnd()) {

        QString current = mainStream.readLine();
        QString outputLine = current;

        if (current.contains("#ADDITIONAL_HEADER")) {

            outputLine = insertAdditionalHeaders();
        }
        if (current.contains("CANDIDATE_CLASS_NAME")) {

            outputLine = current.replace("CANDIDATE_CLASS_NAME", m_mandatory[SOURCE_CLASS_NAME]);
        }

        mainFileStream << outputLine << "\n";
    }
}


/***********************************************************************************************************************************
 * Create replacement sections and insertions for project file                                                                     *
 ***********************************************************************************************************************************/
void ProjectGenerator::createProjectFileReplacements() {

    for (int count = 0; count < LOADER_ELEMENTS; ++count) {

        switch (count) {

        case  INCLUDEMARKER : {

            createIncludeSection();
        }break;
        case  LIBSMARKER    :   {

            createLibsSection();
        }break;
        case  RPC_QOBJECT_SOURCE : {

            createSourceSection();
        }break;
        case  RPC_QOBJECT_HEADER : {

            createHeaderSection();
        }break;
        case DEPENDPATHMARKER : {

            createDependSection();
        }break;
        }

    }
}
/***********************************************************************************************************************************
 * Append items to the INCLUDE section of the .pro file                                                                            *
 ***********************************************************************************************************************************/
void ProjectGenerator::createIncludeSection() {

    m_replacements[INCLUDEMARKER] = " \\\n" +m_mandatory[PROJECT_DIRECTORY_PREFIX] + "\n";


    if (!m_includePaths.isEmpty()) {

        m_replacements[INCLUDEMARKER] += " \\\n";

        int size = m_includePaths.size();
        for (int count = 0; count < size; count +=2) {

            m_replacements[INCLUDEMARKER] += "          ";
            m_replacements[INCLUDEMARKER] += m_includePaths[count];
            if (count < size-2)
                m_replacements[INCLUDEMARKER] += " \\";
            m_replacements[INCLUDEMARKER] += " \n";
        }
    }
}
/***********************************************************************************************************************************
 * Append items to the LIBS section of the .pro file                                                                               *
 ***********************************************************************************************************************************/
void ProjectGenerator::createLibsSection() {

    if (!m_libPaths.isEmpty()) {

        m_replacements[LIBSMARKER] = " \\\n";

        int size = m_libPaths.size();

        for (int count = 0; count < size; count += 2) {

            m_replacements[LIBSMARKER] += "         ";
            m_replacements[LIBSMARKER] += "-L";
            m_replacements[LIBSMARKER] += m_libPaths[count];

            m_replacements[LIBSMARKER] += " -l";
            m_replacements[LIBSMARKER] += m_libPaths[count+1];
            if (count < size-2)

                m_replacements[LIBSMARKER] += " \\";

            m_replacements[LIBSMARKER] += " \n";
        }
    }
}
/***********************************************************************************************************************************
 * Append items to the SOURCES section of the .pro file                                                                            *
 ***********************************************************************************************************************************/
void ProjectGenerator::createSourceSection() {

    m_replacements[RPC_QOBJECT_SOURCE] = m_mandatory[RPC_QOBJECT_SOURCE];

    if (!m_dependencies.isEmpty()) {

        m_replacements[RPC_QOBJECT_SOURCE] += " \\\n";

        int size = m_dependencies.size();

        for (int count = 0; count < size; count += 2) {

            m_replacements[RPC_QOBJECT_SOURCE] += "         ";
            m_replacements[RPC_QOBJECT_SOURCE] += m_dependencies[count+1];

            if (count < size-2)

                m_replacements[RPC_QOBJECT_SOURCE] += " \\";

            m_replacements[RPC_QOBJECT_SOURCE] += " \n";
        }
    }
}
/***********************************************************************************************************************************
 * Append items to the HEADERS section of the .pro file                                                                            *
 ***********************************************************************************************************************************/
void ProjectGenerator::createHeaderSection() {

    m_replacements[RPC_QOBJECT_HEADER] = m_mandatory[RPC_QOBJECT_HEADER];

    if (!m_includePaths.isEmpty()) {

        m_replacements[RPC_QOBJECT_HEADER] += " \\\n";

        int size = m_includePaths.size();

        for (int count = 0; count < size; count += 2) {

            m_replacements[RPC_QOBJECT_HEADER] += "         ";
            m_replacements[RPC_QOBJECT_HEADER] += m_includePaths[count+1];

            if (count < size-2)

                m_replacements[RPC_QOBJECT_HEADER] += " \\";

            m_replacements[RPC_QOBJECT_HEADER] += " \n";
        }
    }
}

/***********************************************************************************************************************************
 * Append items to the DEPENDPATH section of the .pro file                                                                         *
 ***********************************************************************************************************************************/
void ProjectGenerator::createDependSection() {

    if (!m_dependencies.isEmpty()) {

        m_replacements[DEPENDPATHMARKER] += " \\\n";

        int size = m_dependencies.size();

        for (int count = 0; count < size; count += 2) {

            m_replacements[DEPENDPATHMARKER] += "           ";
            m_replacements[DEPENDPATHMARKER] += m_dependencies[count];

            if (count < size-2)

                m_replacements[DEPENDPATHMARKER] += " \\";

            m_replacements[DEPENDPATHMARKER] += " \n";
        }
    }
}
/***********************************************************************************************************************************
 * Returns a string containing the additional includes for main.cpp in loader project                                              *
 ***********************************************************************************************************************************/
QString ProjectGenerator::insertAdditionalHeaders() {

    QString inc("#include \"");
    inc += m_mandatory[RPC_QOBJECT_HEADER];
    inc += "\"\n";
    int size = m_includePaths.size();

    for (int i = 0; i < size; i += 2) {

        inc += m_includePaths[i+1];
        inc += "\"\n";
    }

    return inc;
}
/***********************************************************************************************************************************
 * Run qmake on and then build loader project in /subproject directory                                                             *
 ***********************************************************************************************************************************/
bool ProjectGenerator::buildSubProject() {

    if (!m_buildTools.isEmpty()) {

        QProcess qmake;

        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("QTDIR", m_buildTools.at(0));
        env.insert("PATH", env.value("PATH") + m_buildTools.at(0) + "/" + "bin");
        qmake.setProcessEnvironment(env);

        QStringList args;
        QString cwd = QDir::currentPath();
        qmake.setWorkingDirectory(cwd + "/subproject");
        QString arg0 = "loader.pro";
        QString arg1 = "Makefile";

        args.append(arg0);
        args.append("-o");
        args.append(arg1);

        qmake.start("qmake", args);

        if (!qmake.waitForFinished()) {

            qWarning() << "qmake did not finish in time.";
            return false;
        }

        QProcess make;
        QStringList makeArgs;
        makeArgs.append("-j2");
        makeArgs.append("loader");
        make.setProcessEnvironment(env);
        make.setWorkingDirectory(cwd + "/subproject");
        make.start("make", makeArgs);

        if (!make.waitForFinished(10000)) {

            qWarning() << "make did not finish in time.";
            return false;
        }
    }
    return true;
}
/***********************************************************************************************************************************
 * Run the loader to generate the rpc_ file for the candidate class                                                                *
 ***********************************************************************************************************************************/
bool ProjectGenerator::runSubProject() {

    QProcess loader;
    QString cwd = QDir::currentPath();

    loader.setWorkingDirectory(cwd + "/subproject");
    loader.start("./loader");

    if (!loader.waitForFinished(5000)) {

        qWarning() << "Loader did not finish rewriting source implementation.";
        return false;
    }
    return true;
}
/***********************************************************************************************************************************
 * Save the paths to be used by loader for autogeneration of stub implementation.                                                  *
 ***********************************************************************************************************************************/
void ProjectGenerator::saveSettings() {

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,"MLCSoft","rpcframework");
    settings.beginGroup("Paths");
    settings.setValue("ClassName", m_mandatory[SOURCE_CLASS_NAME]);
    settings.setValue("HeaderName", m_mandatory[SOURCE_HEADER_NAME]);
    settings.setValue("SourceName", m_mandatory[SOURCE_IMPLEMENTATION_NAME]);
    settings.setValue("ProFileName", m_mandatory[PROJECT_FILE_NAME]);
    settings.setValue("Prefix", m_mandatory[PROJECT_DIRECTORY_PREFIX]);
    settings.endGroup();
    settings.sync();
}
/***********************************************************************************************************************************
 * Recreate rewriter.h and rewriter.cpp in the current working directory.                                                          *
 ***********************************************************************************************************************************/
void ProjectGenerator::copySourceFiles() {

    QDir cwd(QDir::currentPath());
    if (!cwd.exists("subproject")) {

        if (!cwd.mkdir("subproject")) {
            qWarning("Attempt to create subproject directory failed.");
        }
    }
    FileHelper::createFromResource(":/sources/rewriter.h", cwd.path()+"/subproject", "rewriter.h");
    FileHelper::createFromResource(":/sources/rewriter.cpp", cwd.path()+"/subproject", "rewriter.cpp");
}
