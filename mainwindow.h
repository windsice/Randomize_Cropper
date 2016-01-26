#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static QString RANDOMCUT;
    static QString SPLITCUT;
    static QString DUPANDCON;

signals:
    void updateStatus(const QString&);

private slots:
    void onUpdateStatus(const QString&);

    void on_toolButton_InputDir_clicked();

    void on_toolButton_OutputDir_clicked();

    void on_pushButton_OK_clicked();

private:
    void StartRandomCut();
    void StartSplitCut();
    void StartDuplication();
    void StartResize();
    bool SameIO();

    Ui::MainWindow *ui;

    int Cropped_Width;
    int Cropped_Height;
    int Crop_Need;
};

#endif // MAINWINDOW_H
