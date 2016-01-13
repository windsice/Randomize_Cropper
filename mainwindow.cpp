#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDirIterator>
#include <QtConcurrent/qtconcurrentrun.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox_fileType->addItem(".jpg");
    ui->comboBox_fileType->addItem(".pgm");
    ui->comboBox_fileType->addItem(".png");
    ui->comboBox_SplitedFileType->addItem(".jpg");
    ui->comboBox_SplitedFileType->addItem(".pgm");
    ui->comboBox_SplitedFileType->addItem(".png");

    connect(this,SIGNAL(updateStatus(QString)),this,SLOT(onUpdateStatus(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButton_InputDir_clicked()
{
    QString filename;
    filename = QFileDialog::getExistingDirectory(this,"Input location","");
    if(filename != "")
    ui->lineEdit_InputDir->setText(filename);
}

void MainWindow::on_toolButton_OutputDir_clicked()
{
    QString filename;
    filename = QFileDialog::getExistingDirectory(this,"Output location","");
    if(filename != "")
    ui->lineEdit_OutputDir->setText(filename);
}

void MainWindow::onUpdateStatus(const QString &picture){
    ui->statusBar->showMessage(picture + " Done");
}

void MainWindow::on_pushButton_OK_clicked()
{
    if(SameIO())
        return;

    QFuture<void> thread = QtConcurrent::run(this,&MainWindow::StartRandomCut);
}

void MainWindow::StartRandomCut(){
    Cropped_Height = ui->spinBox_CroppedH->value();
    Cropped_Width = ui->spinBox_CroppedW->value();
    Crop_Need = ui->spinBox_CropNeed->value();

    QString picture;
    int randX,randY;
    int outputIndex = 0;
    QDirIterator it(ui->lineEdit_InputDir->text(),QDir::Files);
    while(it.hasNext())
    {
        it.next();
        QImage image(it.filePath());
        for(int i = outputIndex; i < Crop_Need + outputIndex; i++){

        randX = qrand() % image.size().width();
        if(randX > image.size().width()-Cropped_Width)
            randX -= Cropped_Width;

        randY = qrand() % image.size().height();
        if(randY > image.size().height()-Cropped_Height)
            randY -= Cropped_Height;

        picture = QString("%1%2%3").arg(ui->lineEdit_OutputDir->text() + "/")
                .arg(i,8,10,QLatin1Char('0')).arg(ui->comboBox_fileType->currentText());
        QImage copy = image.copy(abs(randX),abs(randY),Cropped_Width,Cropped_Height);
        copy.save(picture);

        emit MainWindow::updateStatus(picture);
        }
        outputIndex += Crop_Need;
    }
}

void MainWindow::on_pushButton_OK_Split_clicked()
{
    if(SameIO())
        return;

    QFuture<void> thread = QtConcurrent::run(this,&MainWindow::StartSplitCut);
}

void MainWindow::StartSplitCut(){
    int splitNeed = ui->spinBox_SplitNeed->value();
    int splitedH,splitedW;

    QString picture;
    int outputIndex = 0;
    QDirIterator it(ui->lineEdit_InputDir->text(),QDir::Files);
    while(it.hasNext())
    {
        it.next();
        QImage image(it.filePath());
        splitedH = image.size().height()/splitNeed;
        splitedW = image.size().width()/splitNeed;
        for(int i = 0; i < image.size().height(); i += splitedH)
        {
            for(int j = 0; j < image.size().width(); j += splitedW)
            {
                picture = QString("%1%2%3").arg(ui->lineEdit_OutputDir->text() + "/")
                        .arg(outputIndex,8,10,QLatin1Char('0')).arg(ui->comboBox_SplitedFileType->currentText());
                QImage copy = image.copy(j,i,splitedW,splitedH);
                copy.save(picture);

                emit MainWindow::updateStatus(picture);
                outputIndex++;
            }
        }
    }
}

void MainWindow::on_pushButton_Dup_clicked()
{
    if(SameIO())
        return;

    QFuture<void> thread = QtConcurrent::run(this,&MainWindow::StartDuplicating);
}

void MainWindow::StartDuplicating(){
    QString prefix = ui->lineEdit_DupPrefix->text();
    int DupNum = ui->spinBox_DupNum->value();
    QString dataFormat;

    QString picture;
    int outputIndex = 0;
    QDirIterator it(ui->lineEdit_InputDir->text(),QDir::Files);
    while(it.hasNext()){
        it.next();
        QImage image(it.filePath());
        dataFormat = QFileInfo(it.filePath()).completeSuffix();
        for(int i = 0; i < DupNum; i++){
            picture = QString("%1%2%3%4").arg(ui->lineEdit_OutputDir->text() + "/")
                      .arg(prefix).arg(outputIndex,8,10,QLatin1Char('0')).arg("."+ dataFormat);
            image.save(picture);

            emit MainWindow::updateStatus(picture);
            outputIndex++;
        }
    }
}

bool MainWindow::SameIO(){
    if(ui->lineEdit_InputDir == ui->lineEdit_OutputDir)
    {
        QMessageBox::critical(this,"Same IO Folder","Please do not use the same folder for input and output");
        return true;
    }
    return false;
}
