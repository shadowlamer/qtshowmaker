#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QPainter>
#include "device.h"
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
  void addImage();
  void removeImage();
  void exportAll();
  void moveUp();
  void moveDown();
  void removeAll();
  void upload();
  void showImage(QListWidgetItem*);

private:
  Ui::MainWindow *ui;
  void exportToFile(QString fileName);
};
#endif // MAINWINDOW_H
