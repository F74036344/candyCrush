#include <QtGui>
#include <QtCore>
#include <QMessageBox>
#include <QDialog>
#include <QMovie>           //Used to hold video data
#include <QLabel>           //Used to display videos or pictures
#include <QMediaPlayer>     //Used to play sound

//include forms
#include "mainwindow.h"
#include "ui_mainwindow.h"


//include sources



extern MainWindow *w;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(600,470);

    //Initialize sound object
    sound = new Sound;
    //Initialize data object
    data = new Data;

    //set some widgets properties
    ui->pushButton_gameStart->setGeometry(width()/2-160/2,250,160,70);

    //set background image
    backgroundImage.load(":/image/Resource/candyCrushBackground.png");
    ui->label_background->setPixmap(backgroundImage);
    ui->label_background->setGeometry(0,0,width(),450);

    //start to set background mask animation properties
    ui->label_backgroundMask->setGeometry(0,0,width(),height());
    rgba_r = 255;
    rgba_g = 255;
    rgba_b = 255;
    rgba_a = 255;
    QString str = "rgba("+QString::number(rgba_r)+","+QString::number(rgba_g)+","+QString::number(rgba_b)+","+QString::number(rgba_a)+")";
    ui->label_backgroundMask->setStyleSheet("QLabel{background-color : " + str + "}");
    ui->label_backgroundMask->raise();
    connect(&timer,SIGNAL(timeout()),this,SLOT(d_alpha_var_whiteState()));
    timer.start(18);

    //hide the widgets which would appear after the background mask animation)
    ui->label_title_Candy->lower();
    ui->label_title_Crush->lower();
    ui->pushButton_gameStart->lower();

    qDebug() << "timer.start next line";

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::d_alpha_var_whiteState()
{
    rgba_a -= 1;
    if(rgba_a <= 1)
    {
        disconnect(&timer,SIGNAL(timeout()),this,SLOT(d_alpha_var_whiteState()));
        timer.stop();
        rgba_r = 0;
        rgba_g = 0;
        rgba_b = 0;
        connect(&timer,SIGNAL(timeout()),this,SLOT(d_alpha_var_blackState()));  //whiteState finish. The next is blackState
        timer.start(5);
    }
    else
    {
        QString str = "rgba("+QString::number(rgba_r)+","+QString::number(rgba_g)+","+QString::number(rgba_b)+","+QString::number(rgba_a)+")";
        ui->label_backgroundMask->setStyleSheet("QLabel{background-color : " + str + "}");
    }
}

void MainWindow::d_alpha_var_blackState()
{
    rgba_a += 1;
    if(rgba_a >= 180)
    {
        disconnect(&timer,SIGNAL(timeout()),this,SLOT(d_alpha_var_blackState()));
        timer.stop();
        //Show the title: Candy Crush
        ui->label_title_Candy->raise();
        ui->label_title_Crush->raise();
        ui->pushButton_gameStart->raise();



    }
    else
    {
        QString str = "rgba("+QString::number(rgba_r)+","+QString::number(rgba_g)+","+QString::number(rgba_b)+","+QString::number(rgba_a)+")";
        ui->label_backgroundMask->setStyleSheet("QLabel{background-color : " + str + "}");
    }
}


void MainWindow::on_pushButton_test_clicked()
{
    result = new Result;
    result->setAttribute(Qt::WA_DeleteOnClose);
    result->setWindowModality(Qt::ApplicationModal);
    result->show();
}
