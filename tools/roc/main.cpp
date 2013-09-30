#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtCore/QResource>
#include "projectgenerator.h"



/***********************************************************************************************************************************
*                                                                                                                                  *
*  Usage: roc -prefix <path to source>                                                                                             *
*             -pro <project file name>                                                                                             *
*             -si <implementation name>.cpp                                                                                        *
*             -sh <header name>.h                                                                                                  *
*             -class <source class name>                                                                                           *
*             -qtdir [qt-directory]                                                                                                *
*             -make [path-to-make]                                                                                                 *
*             [-inc [-I[include-path] -i[include-name]]...]                                                                        *
*             [-lib [-L[library-path] -l[library-name]]...]                                                                        *
*             [-dep [-dpath[dependency-path] -d[dependency-name]]...]                                                              *
*                                                                                                                                  *
***********************************************************************************************************************************/

int main(int argc, char** argv) {

    QCoreApplication app(argc,argv);
    QResource::registerResource("sources.rcc");
    QStringList arguments = app.arguments();
    QStringList mandatoryArgs, includes, libs, deps, buildtools;

    if (arguments.size() >= 6) {

        int count = 1;

        if (arguments.at(1).contains("-prefix")) {

            ++count;
            mandatoryArgs.append(arguments.at(count));
            ++count;
        }
        if (arguments.at(count) == "-pro") {

            ++count;
            mandatoryArgs.append(arguments.at(count));
            ++count;
        }
        if (arguments.at(count) == "-si") {

            ++count;
            mandatoryArgs.append(arguments.at(count));
            ++count;
        }
        if (arguments.at(count) == "-sh") {

            ++count;
            mandatoryArgs.append(arguments.at(count));
            ++count;
        }
        if (arguments.at(count) == "-class") {

            ++count;
            mandatoryArgs.append(arguments.at(count));
            ++count;
        }
        if (arguments.at(count) == "-qtdir") {

            ++count;
            buildtools.append(arguments.at(count));
            ++count;
        }if (arguments.at(count) == "-make") {

            ++count;
            buildtools.append(arguments.at(count));
            ++count;
        }
        while (arguments.size() > count) {

            if (arguments.at(count) == "-inc") {

                ++count;
                if (arguments.at(count).mid(0,2) == "-I") {

                    includes.append(arguments.at(count).mid(2));
                    ++count;
                } else {

                    qWarning("missing -I prefix for include directory.");
                    break;
                }
                if (arguments.at(count).mid(0,2) == "-i") {

                    includes.append(arguments.at(count).mid(2));
                    ++count;
                } else {

                    qWarning("missing -i prefix for include name.");
                }
            } else if (arguments.at(count) == "-lib") {

                ++count;
                if (arguments.at(count).mid(0,2) == "-L") {

                    libs.append(arguments.at(count).mid(2));
                    ++count;
                } else {

                    qWarning("missing -L prefix for library directory.");
                    break;
                }
                if (arguments.at(count).mid(0,2) == "-l") {

                    libs.append(arguments.at(count).mid(2));
                    ++count;
                } else {

                    qWarning("missing -l prefix for library name.");
                }
            } else if (arguments.at(count) == "-dep") {

                ++count;
                if (arguments.at(count).mid(0,6) == "-dpath") {

                    deps.append(arguments.at(count).mid(6));
                    ++count;
                } else {

                    qWarning("missing -dpath prefix for dependency directory.");
                    break;
                }
                if (arguments.at(count).mid(0,2) == "-d") {

                    deps.append(arguments.at(count).mid(2));
                    ++count;
                } else {

                    qWarning("missing -d prefix for dependency name.");
                }
            }
        }
        ProjectGenerator p(0, mandatoryArgs, includes, libs, deps, buildtools);
    } else {

        qWarning("No arguments or too few arguments supplied.");
        qWarning("Usage: \n");
        qWarning() << "Usage: roc -prefix <path to source>\n\
           -pro <project file name>\n\
           -si <implementation name>.cpp\n\
           -sh <header name>.h\n\
           -class <source class name>\n\
           -qtdir [qt-directory]\n\
           -make [path-to-make]\n\
           [-inc [-I[include-path] -i[include-name]]...]\n\
           [-lib [-L[library-path] -l[library-name]]...]\n\
           [-dep [-dpath[dependency-path] -d[dependency-name]]...]\n";
    }

    return 0;
}
