
#include <QPixmap>

#include "result.h"
#include "ui_result.h"

Result::Result(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Result)
{
    ui->setupUi(this);
    //set this form size first
    setFixedSize(350,300);

    //set the background properties
    QPixmap bgImage(":/image/Resource/candyBackground.jpg");
    bgImage = bgImage.scaled(width(),height());
    ui->label_backgroundImage->setGeometry(0,0,width(),height());
    ui->label_backgroundImage->setPixmap(bgImage);
    ui->label_backgroundImageMask->setGeometry(0,0,width(),height());
    ui->label_backgroundImageMask->setStyleSheet("QLabel{background-color : rgba(255,255,255,180)}");

    //set button properties
    ui->pushButton_returnToMenu->setGeometry(width()/2-120/2,height()-30-10,120,30);

    //set the order of the widgets
    ui->label_backgroundImageMask->lower();
    ui->label_backgroundImage->lower();
}

Result::~Result()
{
    delete ui;
}

void Result::on_pushButton_returnToMenu_clicked()
{
    delete this;
}
