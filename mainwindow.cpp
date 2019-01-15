#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_textEdit_textChanged() {
    QREncoder encoder;
    QImage qrCode = encoder.getQrCodeImage(
                ui->textEdit->toPlainText().toUtf8()
    );
    ui->graphicsView_qrCode->drawImage(qrCode);
}
