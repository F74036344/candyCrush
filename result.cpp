#include "result.h"
#include "ui_result.h"

Result::Result(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Result)
{
    ui->setupUi(this);
    setFixedSize(350,300);

}

Result::~Result()
{
    delete ui;
}
