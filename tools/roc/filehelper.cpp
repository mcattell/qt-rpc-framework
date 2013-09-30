#include "filehelper.h"
#include <QtCore/QDebug>

bool FileHelper::openReadOnlyTextStream(QFile& file, QTextStream& stream, const QString& dir, const QString& name, bool existing) {

    QDir directory(dir);

    if (existing) {
       if (!checkIfExists(dir, name)) {
           return false;
       }
    }

    return openTextStream(file, stream, dir, name, QIODevice::ReadOnly);
}

bool FileHelper::openWriteOnlyTextStream(QFile& file, QTextStream& stream, const QString& dir, const QString& name, bool existing) {

    QDir directory(dir);

    if(!existing) {
        if (checkIfExists(dir, name)) {
            qWarning() << "File already exists, deleting...";
            QFile removed(directory.absoluteFilePath(name));
            if (!removed.remove()) {
                qWarning() << "Could not remove old file, " << directory.absoluteFilePath(name);
                return false;
            }
        }
    }
    return openTextStream(file, stream, dir, name, QIODevice::WriteOnly);
}

bool FileHelper::openTextStream(QFile& file, QTextStream& stream, const QString& dir, const QString& name, QIODevice::OpenMode mode) {

    QDir directory(dir);

    file.setFileName(directory.absoluteFilePath(name));
    stream.setDevice(&file);

    if (!file.open(mode)) {

        qWarning() << "Cannot create/open file: " << directory.absoluteFilePath(name);
        return false;
    }

    return (stream.status() == QTextStream::Ok);
}

bool FileHelper::checkIfExists(const QString& dir, const QString& name) {

    QDir directory(dir);

    if (!QFile::exists(directory.absoluteFilePath(name))) {

        qWarning() << "The file specified, " << directory.absoluteFilePath(name) << " doesn't exist.";
        return false;
    }
    return true;
}
bool FileHelper::createFromResource(const QString& resource, const QString& path, const QString& name) {


    QFile source(resource);
    QTextStream stream(&source);

    if (!source.open(QIODevice::ReadOnly)) {

        qWarning() << "Failed to open resource " << resource;
        return false;
    }

    QFile dest(path + "/" + name);
    QTextStream out(&dest);

    if (!dest.open(QIODevice::WriteOnly)) {

        qWarning() << "Failed to open destination file " << dest.fileName();
        return false;
    }
    while (!stream.atEnd()) {

        QString current = stream.readLine() + "\n";
        out << current;
    }
    return true;
}
bool FileHelper::openReadOnlyFromResource(QFile& file, const QString& rpath, QTextStream& stream) {

    file.setFileName(rpath);

    if (!file.open(QIODevice::ReadOnly)) {

        qWarning() << "Unable to open resource file " << rpath << " for reading.";
    }
    stream.setDevice(&file);
    return (stream.status() == QTextStream::Ok);
}
