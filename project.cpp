#include "project.h"

#include <qfile.h>
#include <qdebug.h>

Project::Project()
{

}

QString Project::getFileName() {
    return fileName;

}

int Project::create(QString fileName)
{
    try {
        QFile project(fileName);
        project.open(QIODevice::WriteOnly);
        project.close();
        images.clear();
        this->fileName = fileName;
    } catch (std::exception &e) {
        return -1;
    }
    return 0;
}

int Project::open(QString fileName)
{
    try {
        QFile project(fileName);
        QByteArray line;
        project.open(QIODevice::ReadOnly);
        images.clear();
        while (!(line = project.readLine()).isEmpty()) {
            images.append(QString::fromUtf8(line));
        }
        project.close();
        this->fileName = fileName;
    } catch (std::exception &e) {
        return -1;
    }
    return 0;
}

int Project::save(QString _fileName)
{
    if (_fileName.isEmpty()) _fileName = this->fileName;
    try {
        QFile project(_fileName);
        project.open(QIODevice::WriteOnly);
        for (auto image : images) {
            project.write(image.toUtf8());
            project.write("\n");
        }
        project.close();
        this->fileName = _fileName;
    } catch (std::exception &e) {
        return -1;
    }
    return 0;
}

QStringList Project::getImages()
{
    return images;
}

void Project::setImages(QStringList list)
{
    images.clear();
    for (auto image : list) {
        images.append(image);
    }
}
