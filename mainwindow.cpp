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
#include "gamewindow.h"
#include "settings.h"

//include sources



extern MainWindow *w;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(600,470);
    setAttribute(Qt::WA_DeleteOnClose);

    //Initialize sound object
    sound = new Sound;
    //Initialize data object
    data = new Data;

    //set some widgets properties
    ui->pushButton_gameStart->setGeometry(width()/2-160/2,250,160,70);
    ui->pushButton_setting->setGeometry(width()/2-100/2,350,100,30);
    ui->pushButton_quit->setGeometry(width()/2-80/2,400,80,25);
    ui->pushButton_skip->setGeometry(width()-60,height()-60,40,40);

    //set background image
    backgroundImage.load(":/image/Resource/candyCrushBackground.png");
    backgroundImage = backgroundImage.scaled(width(),height());
    ui->label_background->setPixmap(backgroundImage);
    ui->label_background->setGeometry(0,0,width(),height());

    //start to set background mask animation properties
    ui->label_backgroundMask->setGeometry(0,0,width(),height());
    rgba_r = 255;
    rgba_g = 255;
    rgba_b = 255;
    rgba_a = 255;
    QString str = "rgba("+QString::number(rgba_r)+","+QString::number(rgba_g)+","+QString::number(rgba_b)+","+QString::number(rgba_a)+")";
    ui->label_backgroundMask->setStyleSheet("QLabel{background-color : " + str + "}");
    connect(&timer,SIGNAL(timeout()),this,SLOT(d_alpha_var_whiteState()));
    timer.start(15);

    //set the widgets order
    ui->label_title_Candy->lower();
    ui->label_title_Crush->lower();
    ui->pushButton_gameStart->lower();
    ui->pushButton_setting->lower();
    ui->pushButton_quit->lower();
    ui->label_background->raise();
    ui->label_backgroundMask->raise();
    ui->pushButton_skip->raise();

    qDebug() << "timer.start next line";

}

MainWindow::~MainWindow()
{
    delete ui;
    delete sound;
    delete data;
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
        timer.start(6);
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
        ui->pushButton_skip->deleteLater(); //delete the skip button
        disconnect(&timer,SIGNAL(timeout()),this,SLOT(d_alpha_var_blackState()));
        timer.stop();
        //Show the title: Candy Crush
        ui->label_title_Candy->raise();
        ui->label_title_Crush->raise();
        ui->pushButton_gameStart->raise();
        ui->pushButton_setting->raise();
        ui->pushButton_quit->raise();
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

void MainWindow::on_pushButton_gameStart_clicked()
{
    GameWindow *gameWindow = new GameWindow;
    gameWindow->setWindowModality(Qt::ApplicationModal);
    gameWindow->setAttribute(Qt::WA_DeleteOnClose);
    gameWindow->show();
}

void MainWindow::on_pushButton_quit_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Quit",
                                  "Quit the Game ?",
                          QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
        close();
}

void MainWindow::on_pushButton_skip_clicked()
{
    ui->pushButton_skip->deleteLater(); //delete the skip button
    disconnect(&timer,SIGNAL(timeout()),this,SLOT(d_alpha_var_whiteState()));
    disconnect(&timer,SIGNAL(timeout()),this,SLOT(d_alpha_var_blackState()));
    timer.stop();
    //set the background to the terminal condition
    rgba_r = 0;
    rgba_g = 0;
    rgba_b = 0;
    rgba_a = 180;
    QString str = "rgba("+QString::number(rgba_r)+","+QString::number(rgba_g)+","+QString::number(rgba_b)+","+QString::number(rgba_a)+")";
    ui->label_backgroundMask->setStyleSheet("QLabel{background-color : " + str + "}");
    //Show the title: Candy Crush
    ui->label_title_Candy->raise();
    ui->label_title_Crush->raise();
    ui->pushButton_gameStart->raise();
    ui->pushButton_setting->raise();
    ui->pushButton_quit->raise();
}

void MainWindow::on_pushButton_setting_clicked()
{
    Settings *settings = new Settings;
    settings->setAttribute(Qt::WA_DeleteOnClose);
    settings->setWindowModality(Qt::ApplicationModal);
    settings->show();
}
