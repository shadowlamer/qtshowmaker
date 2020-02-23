#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

class Project
{
public:
    Project();
    QString getFileName();
    int create(QString fileName);
    int open(QString fileName);
    int save(QString fileName = "");
    QStringList getImages();
    void setImages(QStringList list);
private:
    QString fileName;
    QStringList images;
};

#endif // PROJECT_H
