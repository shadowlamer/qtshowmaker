#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>
#include <QPainter>


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
  void showImage(QListWidgetItem*);

private:
  Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
