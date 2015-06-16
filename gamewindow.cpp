#include <QMessageBox>
#include <vector>

#include "mainwindow.h"
#include "gamewindow.h"
#include "ui_gamewindow.h"

extern MainWindow *w;

GameWindow::GameWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWindow)
{
    ui->setupUi(this);
    qsrand(QTime::currentTime().msec());
    //set some basic values first
    blockEdgeAmount = w->data->getBoardEdgeSizeValue();

    gapOfWidget = 10;   gapOfBlocks = 4;
    blockEdgeLength = 50;

    gameAreaEdgeLength = gapOfBlocks + blockEdgeAmount*(blockEdgeLength+gapOfBlocks);
    recordAreaWidth = 200;

    //Then start to set the properties of the form and its widgets
    setFixedSize(gapOfWidget+200+gapOfWidget+gameAreaEdgeLength+gapOfWidget,gapOfWidget+gameAreaEdgeLength+gapOfWidget);

    //Set the position or geometry of some widgets
    ui->pushButton_clickMe->setGeometry(40,height()-70-60,120,40);
    ui->pushButton_returnToMenu->setGeometry(40,height()-70,120,40);

    //set the graphicsView's(gameArea) properties
    ui->graphicsView->setGeometry(gapOfWidget+recordAreaWidth+gapOfWidget,gapOfWidget,gameAreaEdgeLength,gameAreaEdgeLength);
    ui->graphicsView->setStyleSheet("QGraphicsView{background-color : rgba(255,255,255,90)}");


    //Then start to set the graphicsView's contents
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


    //Initiailize some candy image data which would be added into graphicView later
    candyImage = new QPixmap[6];
    candySelectedImage = new QPixmap[6];
    for(int i=0;i<6;i++)
    {
        (candyImage+i)->load(":/image_candy/Resource/candy_"+QString::number(i+1)+".png");
        *(candyImage+i) = (candyImage+i)->scaled(blockEdgeLength,blockEdgeLength);
        (candySelectedImage+i)->load(":/image_candy/Resource/candy_"+QString::number(i+1)+"_selected.png");
        *(candySelectedImage+i) = (candySelectedImage+i)->scaled(blockEdgeLength,blockEdgeLength);
    }
    specialCandyColImage.load(":/image_candy/Resource/specialCandy_col.png");
    specialCandyColImage = specialCandyColImage.scaled(blockEdgeLength,blockEdgeLength);
    specialCandyRowImage.load(":/image_candy/Resource/specialCandy_row.png");
    specialCandyRowImage = specialCandyRowImage.scaled(blockEdgeLength,blockEdgeLength);
    specialCandyBombImage.load(":/image_candy/Resource/specialCandy_bomb.png");
    specialCandyBombImage = specialCandyBombImage.scaled(blockEdgeLength,blockEdgeLength);
    specialCandyStarImage.load(":/image_candy/Resource/specialCandy_star.png");
    specialCandyStarImage = specialCandyStarImage.scaled(blockEdgeLength,blockEdgeLength);
    collectCandyImage = new QPixmap[4];
    for(int i=0;i<4;i++)
    {
        (collectCandyImage+i)->load(":/image_candy/Resource/collectCandy_"+QString::number(i+1)+".png");
        *(collectCandyImage+i) = (collectCandyImage+i)->scaled(blockEdgeLength,blockEdgeLength);
    }

    isBlockSelected = new bool[power(blockEdgeAmount,2)];
    for(int i=0;i<power(blockEdgeAmount,2);i++)
        *(isBlockSelected+i) = false;

    candyImageHolder = new QLabel* [power(blockEdgeAmount,2)];
    for(int i=0;i<power(blockEdgeAmount,2);i++)
    {
        int row = i%blockEdgeAmount, col = i/blockEdgeAmount;
        (*(candyImageHolder+i)) = new QLabel;
        (*(candyImageHolder+i))->setStyleSheet("QLabel{background-color : transparent}");
        (*(candyImageHolder+i))->setPixmap(*(candySelectedImage+i%(w->data->getKindsOfCandyValue())));
        (*(candyImageHolder+i))->setGeometry(gapOfBlocks+col*(blockEdgeLength+gapOfBlocks),
                                             gapOfBlocks+row*(blockEdgeLength+gapOfBlocks),
                                             blockEdgeLength,
                                             blockEdgeLength);
        scene->addWidget((*(candyImageHolder+i)));
    }
    candyTypeRecorder = new char[power(blockEdgeAmount,2)];
    for(int i=0;i<power(blockEdgeAmount,2);i++)
        *(candyTypeRecorder+i) = -1;    //-1 means empty
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

    //Initialize the candyBoard
    initCandyBoard();
}

GameWindow::~GameWindow()
{
    delete ui;
    delete scene;   //delete scene would also delete the contents(blocks) of it

    delete [] candyImage;
    delete [] candySelectedImage;
    delete [] collectCandyImage;

    delete [] candyTypeRecorder;
    //delete [] candyImageHolder;
    delete [] isBlockSelected;
}

void GameWindow::initCandyBoard()   //初始化版面(要避免一開始就有三個或以上連在一起的糖果在版面上)
{
    int kindsOfCandy = w->data->getKindsOfCandyValue();
    std::vector<int> ballot;
    int index;
    int row,col,row_for_check,col_for_check;
    int combo=1;
    bool comboExceed = false;
    for(row = 0;row<blockEdgeAmount;row++)
        for(col=0;col<blockEdgeAmount;col++)
        {
            if(!comboExceed)    //If it is not in the comboExceed status, then just initialize the ballot
            {
               //Initialize the ballot
                ballot.clear();
                for(int i=0;i<kindsOfCandy;i++)
                    ballot.push_back(i);
            }
            index = qrand()%(ballot.size());
            *(candyTypeRecorder+col+row*blockEdgeAmount) = ballot.at(index);
            //向上及向左檢查
            //向上檢查
            comboExceed = false;
            combo=1;
            for(row_for_check=row-1;row_for_check>=0;row_for_check--)
            {
                if(*(candyTypeRecorder+col+row_for_check*blockEdgeAmount) == *(candyTypeRecorder+col+row*blockEdgeAmount))
                    combo++;
                else
                    break;
                if(combo>=3)
                {
                    //將籤從ballot中移除
                    for(int i=0;i<(ballot.size()-1);i++)
                        ballot.at(i) = ballot.at(i+1);
                    if(!ballot.empty())
                        ballot.pop_back();
                    comboExceed = true;
                    break;
                }
            }
            if(comboExceed)
            {
                //comboExceed->再找一個值再試一次
                //not exceed->繼續往左檢查
                col--;
                continue;
            }
            //向左檢查
            comboExceed = false;
            combo=1;
            for(col_for_check=col-1;col_for_check>=0;col_for_check--)
            {
                if(*(candyTypeRecorder+col_for_check+row*blockEdgeAmount) == *(candyTypeRecorder+col+row*blockEdgeAmount))
                    combo++;
                else
                    break;
                if(combo>=3)
                {
                    //將籤從ballot中移除
                    for(int i=0;i<(ballot.size()-1);i++)
                        ballot.at(i) = ballot.at(i+1);
                    if(ballot.size()>0)
                        ballot.pop_back();
                    comboExceed = true;
                    break;
                }
            }
            if(comboExceed)
            {
                //comboExceed->再找一個值再試一次
                //not exceed->換下一格
                col--;
                continue;
            }

        }
        //candyImageHolder根據candyTypeRecorder內容來設置對應的candy image
        for(int i=0;i<power(blockEdgeAmount,2);i++)
        {
            (*(candyImageHolder+i))->setPixmap(*(candyImage+*(candyTypeRecorder+i)));
        }
        for(int r=0;r<blockEdgeAmount;r++)
        {
            qDebug() << '\0'+*(candyTypeRecorder+0+r*blockEdgeAmount)<<" "
                     << '\0'+*(candyTypeRecorder+1+r*blockEdgeAmount)<<" "
                     << '\0'+*(candyTypeRecorder+2+r*blockEdgeAmount)<<" "
                     << '\0'+*(candyTypeRecorder+3+r*blockEdgeAmount)<<" "
                     << '\0'+*(candyTypeRecorder+4+r*blockEdgeAmount)<<" "
                     << '\0'+*(candyTypeRecorder+5+r*blockEdgeAmount)<<" "
                     << '\0'+*(candyTypeRecorder+6+r*blockEdgeAmount)<<" "
                     << '\0'+*(candyTypeRecorder+7+r*blockEdgeAmount)<<" "
                     << '\0'+*(candyTypeRecorder+8+r*blockEdgeAmount)<<" "
                     << '\0'+*(candyTypeRecorder+9+r*blockEdgeAmount);
        }
}

int GameWindow::power(int b, int n)
{
    int tmp = 1;
    for(int i=0;i<n;i++)
        tmp *= b;
    return tmp;
}

void GameWindow::mousePressEvent(QMouseEvent *event)
{
    int col,row;
    int x_pos,y_pos;
    x_pos = (event->x())-(ui->graphicsView->x());
    if(x_pos>=0)
        col = x_pos/(gapOfBlocks+blockEdgeLength);
    else
        col = -1;
    y_pos = (event->y())-(ui->graphicsView->y());
    if(y_pos>=0)
        row = y_pos/(gapOfBlocks+blockEdgeLength);
    else
        row = -1;

    if(0<=col && col<blockEdgeAmount)
    {
        if(0<=row && row<blockEdgeAmount)
        {
            qDebug() << "Mouse current position  (col,row) : " <<"("<<col<<","<<row<<")" ;
            for(int i=0;i<power(blockEdgeAmount,2);i++)
            {
                (block+i)->setPen(QPen(Qt::transparent,0));
                (block+i)->setBrush(QColor(0,0,0,130));
            }
            (block+col+row*blockEdgeAmount)->setPen(QPen(Qt::red,4));
            (block+col+row*blockEdgeAmount)->setBrush(QColor(245,180,40,170));  //bright orange: QColor(245,180,40,170); bright blue:QColor(20,230,245,170)

            if(col+1<blockEdgeAmount)
            {
                (block+(col+1)+row*blockEdgeAmount)->setPen(QPen(Qt::blue,4));
                (block+(col+1)+row*blockEdgeAmount)->setBrush(QColor(20,230,245,170));
            }
            if(col-1>=0)
            {
                (block+(col-1)+row*blockEdgeAmount)->setPen(QPen(Qt::blue,4));
                (block+(col-1)+row*blockEdgeAmount)->setBrush(QColor(20,230,245,170));
            }
            if(row+1<blockEdgeAmount)
            {
                (block+col+(row+1)*blockEdgeAmount)->setPen(QPen(Qt::blue,4));
                (block+col+(row+1)*blockEdgeAmount)->setBrush(QColor(20,230,245,170));
            }
            if(row-1>=0)
            {
                (block+col+(row-1)*blockEdgeAmount)->setPen(QPen(Qt::blue,4));
                (block+col+(row-1)*blockEdgeAmount)->setBrush(QColor(20,230,245,170));
            }

        }
        else
        {
            for(int i=0;i<power(blockEdgeAmount,2);i++)
            {
                (block+i)->setPen(QPen(Qt::transparent,0));
                (block+i)->setBrush(QColor(0,0,0,130));
            }
        }
    }
    else
    {
        for(int i=0;i<power(blockEdgeAmount,2);i++)
        {
            (block+i)->setPen(QPen(Qt::transparent,0));
            (block+i)->setBrush(QColor(0,0,0,130));
        }
    }
    qDebug() << "Mouse current position  x : " << event->x();
    qDebug() << "Mouse current position  y : " << event->y();
    qDebug() << "Mouse current position  (col,row) : " <<"("<<col<<","<<row<<")" ;

}

void GameWindow::on_pushButton_returnToMenu_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Return to Menu",
                                  "Do you really want to return to Menu?\n"
                                  "Current game state would be DISCARDED!!",
                          QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
        delete this;
}

void GameWindow::on_pushButton_clickMe_clicked()
{
    rslt = new Result;
    connect(rslt,SIGNAL(destroyed()),this,SLOT(close()));
    rslt->setAttribute(Qt::WA_DeleteOnClose);
    rslt->setWindowModality(Qt::ApplicationModal);
    rslt->show();
}
