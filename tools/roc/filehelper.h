#ifndef FILEHELPER_H
#define FILEHELPER_H

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

class FileHelper
{

public:
    static bool openReadOnlyTextStream(QFile& file, QTextStream& stream, const QString& dir, const QString& name, bool existing);
    static bool openWriteOnlyTextStream(QFile& file, QTextStream& stream, const QString& dir, const QString& name, bool existing);
    static bool openTextStream(QFile& file, QTextStream& stream, const QString& dir, const QString& name, QIODevice::OpenMode mode);
    static bool createFromResource(const QString& resource, const QString& path, const QString& name);
    static bool openReadOnlyFromResource(QFile& file, const QString& rpath, QTextStream& stream);
private:
    static bool checkIfExists(const QString& dir, const QString& name);
};

#endif // FILEHELPER_H
