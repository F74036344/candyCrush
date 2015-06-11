#include "result.h"
#include "ui_result.h"

Result::Result(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Result)
{
    ui->setupUi(this);
    setFixedSize(350,300);


    ui->pushButton_returnToMenu->setGeometry(width()/2-120/2,height()-30-10,120,30);
}

Result::~Result()
{
    delete ui;
}
