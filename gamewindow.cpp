#include <QMessageBox>

#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    //set some basic values first
    blockEdgeAmount = 10;

    gapOfWidget = 10;   gapOfBlocks = 4;
    blockEdgeLength = 55;

    gameAreaEdgeLength = gapOfBlocks + blockEdgeAmount*(blockEdgeLength+gapOfBlocks);
    recordAreaWidth = 200;

    //Then start to set the properties of the form and its widgets
    setFixedSize(gapOfWidget+200+gapOfWidget+gameAreaEdgeLength+gapOfWidget,gapOfWidget+gameAreaEdgeLength+gapOfWidget);

    //Set the position or geometry of some widgets
    ui->pushButton_returnToMenu->setGeometry(40,height()-70,120,40);

    //set the graphicsView's(gameArea) properties
    ui->graphicsView->setGeometry(gapOfWidget+recordAreaWidth+gapOfWidget,gapOfWidget,gameAreaEdgeLength,gameAreaEdgeLength);
    ui->graphicsView->setStyleSheet("QGraphicsView{background-color : rgba(255,255,255,90)}");
    //set background's properties
    backgroundImage.load(":/image/Resource/candyCrushBackground.png");
    backgroundImage = backgroundImage.scaled(width(),height());
    ui->label_background->setPixmap(backgroundImage);
    ui->label_background->setGeometry(0,0,width(),height());
    //set backgroundMask's properties
    ui->label_backgroundMask->setStyleSheet("QLabel{background-color : rgba(0,0,0,80)}");
    ui->label_backgroundMask->setGeometry(0,0,width(),height());

    //set the display order
    ui->graphicsView->lower();
    ui->label_backgroundMask->lower();
    ui->label_background->lower();

    //start to set the graphicsView's contents
    scene = new QGraphicsScene;
    scene->setSceneRect(0,0,ui->graphicsView->width(),ui->graphicsView->height());
    block = new QGraphicsRectItem[power(blockEdgeAmount,2)];
    for(int row=0;row<blockEdgeAmount;row++)
        for(int col=0;col<blockEdgeAmount;col++)
        {
            (block+col+row*blockEdgeAmount)->setRect(0,0,blockEdgeLength,blockEdgeLength);
            (block+col+row*blockEdgeAmount)->setPos(gapOfBlocks+col*(blockEdgeLength+gapOfBlocks),
                                                    gapOfBlocks+row*(blockEdgeLength+gapOfBlocks));
            (block+col+row*blockEdgeAmount)->setPen(QPen(Qt::transparent,0));
            (block+col+row*blockEdgeAmount)->setBrush(QColor(0,0,0,130));
            scene->addItem((block+col+row*blockEdgeAmount));
        }
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



}

GameWindow::~GameWindow()
{
    delete ui;
    delete scene;   //delete scene would also delete the contents(blocks) of it
}

int GameWindow::power(int b, int n)
{
    int tmp = 1;
    for(int i=0;i<n;i++)
        tmp *= b;
    return tmp;
}

void GameWindow::on_pushButton_returnToMenu_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Return to Menu",
                                  "Do you really want to return to Menu?\n"
                                  "current game state would be DISCARDED!!",
                          QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
        close();
}
