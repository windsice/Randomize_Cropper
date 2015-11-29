#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox_fileType->addItem(".jpg");
    ui->comboBox_fileType->addItem(".pgm");
    ui->comboBox_fileType->addItem(".png");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_PictureSel_clicked()
{
    QString filename;
    filename = QFileDialog::getOpenFileName(this,"Picture Selection","");
    if(filename != "")
    ui->lineEdit_PictureSel->setText(filename);
}

void MainWindow::on_toolButton_OutputDir_clicked()
{
    QString filename;
    filename = QFileDialog::getExistingDirectory(this,"Output location","");
    if(filename != "")
    ui->lineEdit_OutputDir->setText(filename + "/Cropped");
}

void MainWindow::on_pushButton_OK_clicked()
{
    Cropped_Height = ui->spinBox_CroppedH->value();
    Cropped_Width = ui->spinBox_CroppedW->value();
    Crop_Need = ui->spinBox_CropNeed->value();

    //making folder
    QString workingDir = ui->lineEdit_OutputDir->text();
    QDir dir(workingDir);
    if(!dir.exists()){
        dir.mkdir(".");
        ui->statusBar->showMessage("making folder");
    }

    QImage image(ui->lineEdit_PictureSel->text());

    QString picture;
    int randX,randY;
    for(int i = 0; i < Crop_Need; i++){

    randX = qrand() % image.size().width();
    if(randX > image.size().width()-Cropped_Width)
        randX -= Cropped_Width;

    randY = qrand() % image.size().height();
    if(randY > image.size().height()-Cropped_Height)
        randY -= Cropped_Height;

    picture = QString("%1%2%3").arg(workingDir + "/").arg(i,4,10,QLatin1Char('0')).arg(ui->comboBox_fileType->currentText());
    QImage copy = image.copy(abs(randX),abs(randY),Cropped_Width,Cropped_Height);
    copy.save(picture);

    ui->statusBar->showMessage(picture + " Done");
    }
}
