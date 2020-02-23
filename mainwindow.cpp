#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    project() {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onAddImageClick() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Image", "..", "Image Files (*.png *.jpg *.jpeg *.bmp)");
    addImage(fileName);
}

int MainWindow::addImage(QString fileName) {
    QIcon icon(fileName);
    QListWidgetItem *item = new QListWidgetItem(icon, fileName, ui->listWidget);
    ui->listWidget->addItem(item);
    return 0;
}

QStringList MainWindow::getImgPaths()
{
    QStringList images;
    for (int itemIndex = 0; itemIndex < ui->listWidget->count(); itemIndex++) {
        QListWidgetItem *item = ui->listWidget->item(itemIndex);
        images.append(item->text());
    }
    return images;
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
    exportToFile(fileName);
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
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "New project", "..", "Showmaker projects (*.show)");
    if (project.create(fileName) != -1) {
        ui->listWidget->clear();
    }
}

void MainWindow::upload()
{
    QStringList devices = Device::listDevices();
    if (devices.empty()) {
        QMessageBox::information(this, "Devices", "No devices found.", QMessageBox::Ok);
        return;
    }
    for (auto device : devices) {
        if (QMessageBox::question(
                    this,
                    "Uploading...",
                    QString("Upload bundle to %1?").arg(device),
                    QMessageBox::Yes,
                    QMessageBox::No) == QMessageBox::Yes) {
            exportToFile(device);
        }
    }
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

void MainWindow::onSaveClick()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Save project", "..", "Showmaker projects (*.show)");
    project.setImages(getImgPaths());
    project.save();
}

void MainWindow::onOpenClick()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open project", "..", "Showmaker projects (*.show)");
    if (project.open(fileName) != -1) {
        ui->listWidget->clear();
        for (auto image : project.getImages()) {
            addImage(image.trimmed());
        }
    }
}

void MainWindow::exportToFile(QString fileName)
{
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
