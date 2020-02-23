#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QPainter>
#include "device.h"
#include "project.h"
#include <QFileDialog>
#include <QtMath>
#include <QFile>
#include <QDebug>
#include <QStorageInfo>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void onAddImageClick();
  void removeImage();
  void exportAll();
  void moveUp();
  void moveDown();
  void removeAll();
  void upload();
  void showImage(QListWidgetItem*);
  void onOpenClick();
  void onSaveClick();

private:
  Ui::MainWindow *ui;
  void exportToFile(QString fileName);
  Project project;
  int addImage(QString fileName);
  QStringList getImgPaths();
};
#endif // MAINWINDOW_H
