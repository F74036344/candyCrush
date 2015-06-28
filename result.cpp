
#include <QPixmap>

#include "mainwindow.h"
#include "result.h"
#include "ui_result.h"

extern MainWindow *w;

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

    //set the title depending on the mode
    QPixmap pic_of_mode2(":/image_candy/Resource/candy_"+QString::number(w->gameWindow->goalCandy_index+1)+".png");
    QPixmap pic_of_mode3(":/image_candy/Resource/collectCandy_"+QString::number(w->gameWindow->goalCandy_index+1)+".png");
    switch(w->data->getModeValue())
    {
    case 1: //do nothing
        break;
    case 2:
        ui->label_item_pic->setGeometry(ui->label_item_pic->x()+10,ui->label_item_pic->y()+10,80,80);
        pic_of_mode2 = pic_of_mode2.scaled(80,80);
        ui->label_item_pic->setPixmap(pic_of_mode2);
        break;
    case 3:
        ui->label_item_pic->setGeometry(ui->label_item_pic->x()+10,ui->label_item_pic->y()+10,80,80);
        pic_of_mode3 = pic_of_mode3.scaled(80,80);
        ui->label_item_pic->setPixmap(pic_of_mode3);
        break;

    }


    //set the labels displaying your score and the goal
    ui->label_goalValue->setText(QString::number(w->data->getGoalValue()));
    ui->label_goalValue->setAlignment(Qt::AlignCenter);
    ui->label_yoursValue->setText(QString::number(w->gameWindow->getScoreValue()));
    ui->label_yoursValue->setAlignment(Qt::AlignCenter);

    //set the accomplish rate
    ui->label_accomplishRate->setAlignment(Qt::AlignCenter);
    int accomplish_rate = (1.0*w->gameWindow->getGoalValue()/w->data->getGoalValue())*100;
    if(accomplish_rate>100)
        accomplish_rate = 100;
    if(accomplish_rate<50)
        ui->label_accomplishRate->setStyleSheet("QLabel{color : red}");
    else if(accomplish_rate<75)
        ui->label_accomplishRate->setStyleSheet("QLabel{color : orange}");
    else if(accomplish_rate<100)
        ui->label_accomplishRate->setStyleSheet("QLabel{color : rgb(255,236,24)}");  //ffec18:yellow
    else
        ui->label_accomplishRate->setStyleSheet("QLabel{color : green}");
    ui->label_accomplishRate->setText(QString::number(accomplish_rate)+"%");


    //show the star
    QPixmap star_pic(":/image/Resource/star.png");
    QPixmap x_pic(":/image/Resource/X.png");
    star_pic = star_pic.scaled(35,35);
    x_pic = x_pic.scaled(35,35);
    switch(w->gameWindow->getStarValue())
    {
    case 3:
        ui->label_star3->setPixmap(star_pic);
    case 2:
        ui->label_star2->setPixmap(star_pic);
    case 1:
        ui->label_star1->setPixmap(star_pic);
        break;
    case 0:
        QFont font;
        font.setPointSize(18);
        font.setBold(true);
        ui->label_star2->setPixmap(x_pic);
        ui->label_star1->setGeometry(ui->label_star1->x(),ui->label_star1->y()-5,80,45);
        ui->label_star1->setText("No Star");
        ui->label_star1->setFont(font);
        ui->label_star1->setStyleSheet("QLabel{color : red}");
    }

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
