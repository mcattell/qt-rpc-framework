#ifndef PROJECTGENERATOR_H
#define PROJECTGENERATOR_H

#include <QtCore/QStringList>
#include <QtCore/QObject>
#include <QtCore/QDebug>

#define PROJECT_DIRECTORY_PREFIX   0
#define PROJECT_FILE_NAME          1
#define SOURCE_IMPLEMENTATION_NAME 2
#define SOURCE_HEADER_NAME         3
#define SOURCE_CLASS_NAME          4
#define QTDIR                      0

typedef enum {
    INCLUDEMARKER,
    LIBSMARKER,
    RPC_QOBJECT_SOURCE,
    RPC_QOBJECT_HEADER,
    DEPENDPATHMARKER,
    LOADER_ELEMENTS
}Elements;

const QString loaderElements[LOADER_ELEMENTS] = {
  "#END_INCLUDEPATH_MARKER",
  "#END_LIBS_MARKER",
  "RPC_QOBJECT_CANDIDATE_SOURCE",
  "RPC_QOBJECT_CANDIDATE_HEADER",
  "#END_DEPENDPATH_MARKER"
};



class ProjectGenerator : public QObject
{
    Q_OBJECT
public:
    ProjectGenerator(QObject *parent = 0
            , const QStringList& mandatory = QStringList()
            , const QStringList& includePaths = QStringList()
            , const QStringList& libraryPaths = QStringList()
            , const QStringList& dependencies = QStringList()
            , const QStringList& buildTools = QStringList()
            );
   virtual ~ProjectGenerator();

private:
    void modifySourceProjectFile();
    void createLoaderProjectFile();
    void createLoaderMain();
    void createProjectFileReplacements();
    void createIncludeSection();
    void createLibsSection();
    void createHeaderSection();
    void createSourceSection();
    void createDependSection();
    QString insertAdditionalHeaders();
    bool buildSubProject();
    bool runSubProject();
    void saveSettings();
    void copySourceFiles();
    QStringList m_mandatory, m_includePaths, m_libPaths, m_dependencies, m_buildTools;
    QString m_replacements[LOADER_ELEMENTS];
};

#endif // PROJECTGENERATOR_H
