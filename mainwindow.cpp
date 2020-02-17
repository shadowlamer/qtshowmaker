#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QtMath>
#include <QFile>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addImage() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Image", "..", "Image Files (*.png *.jpg *.jpeg *.bmp)");
    QIcon icon(fileName);
    QListWidgetItem *item = new QListWidgetItem(icon, fileName, ui->listWidget);
    ui->listWidget->addItem(item);
}

void MainWindow::removeImage() {
    int row = ui->listWidget->currentRow();
    if (row >= 0) {
        ui->listWidget->takeItem(row);
    }
    row = ui->listWidget->currentRow();
    showImage(ui->listWidget->item(row));
}

void iconToColors(QIcon icon, QSize size, QColor *buf) {
    int maxDim = qMax(size.width(), size.height());
    QPixmap iconPixmap(icon.pixmap(icon.actualSize(QSize(maxDim, maxDim))));
    QImage iconImage = iconPixmap.scaledToHeight(size.height()).toImage();
    for (int x = 0; x < size.width(); x++) {
        for (int y = 0; y < size.height(); y++) {
            int imgX = x % iconImage.width();
            QColor color = iconImage.pixelColor(imgX, y);
            int index = size.height() * x + y;
            buf[index] = color;
        }
    }
}

void MainWindow::exportAll() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save bundle", "..", "Disk images (*.img)");
    QFile bundle(fileName);
    bundle.open(QIODevice::WriteOnly);
    for (int itemIndex = 0; itemIndex < ui->listWidget->count(); itemIndex++) {
        QListWidgetItem *item = ui->listWidget->item(itemIndex);
        QColor colors[1024 * 36];
        uint32_t bytes[1024 * 36];
        iconToColors(item->icon(), QSize(1024, 36),colors);
        for (int i = 0; i < 1024 * 36; i++) {

            QColor color = colors[i];
            int r,g,b;
            color.getRgb(&r, &g, &b);
            bytes[i] = (r & 0xff) | (g & 0xff) << 8 | (b & 0xff) << 16;
        }
        bundle.write((char *)bytes, 1024 * 36 * 4);
    }
    bundle.close();
}

void MainWindow::moveUp()
{
    int row = ui->listWidget->currentRow();
    if (row > 0) {
        QListWidgetItem *item = ui->listWidget->takeItem(row);
        row -= 1;
        ui->listWidget->insertItem(row,item);
        ui->listWidget->setCurrentRow(row);
    }
}

void MainWindow::moveDown()
{
    int row = ui->listWidget->currentRow();
    if (row >= 0) {
        QListWidgetItem *item = ui->listWidget->takeItem(row);
        row += 1;
        ui->listWidget->insertItem(row,item);
        ui->listWidget->setCurrentRow(row);
    }
}

void MainWindow::removeAll()
{
    ui->listWidget->clear();
}

void MainWindow::showImage(QListWidgetItem *item) {

    QPixmap previewPixmap(QSize(300,300));
    QPainter painter(&previewPixmap);
    painter.setPen(Qt::red);
    painter.setBackground(Qt::black);
    painter.eraseRect(previewPixmap.rect());

    if (item != nullptr) {
        QColor colors[1024 * 36];
        iconToColors(item->icon(), QSize(1024, 36),colors);

        for (int row = 0; row < 1024; row++) {
            for (int led = 0; led < 36; led++) {
                double phi = (double) row / 1024 * 2.0 * 3.14159265358;
                int x = (int) (qCos(phi) * (150.0 - led)) + 150;
                int y = (int) (qSin(phi) * (150.0 - led)) + 150;
                painter.setPen(colors[36 * row + led]);
                painter.drawPoint(x, y);
            }
        }
    }
    ui->label->setPixmap(previewPixmap);
}
