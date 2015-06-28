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
    //Set the seed of the qrand()
    qsrand(QTime::currentTime().msec());
    //set some basic geometry values first
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


    //Initialize isMousePressEventBlocked
    isMousePressEventBlocked = false;

    //Initialize isCandySelected
    isCandySelected = new bool[power(blockEdgeAmount,2)];
    for(int i=0;i<power(blockEdgeAmount,2);i++)
        *(isCandySelected+i) = false;

    //Initialize candyImageHolder
    candyImageHolder = new QLabel* [power(blockEdgeAmount,2)];
    for(int i=0;i<power(blockEdgeAmount,2);i++)
    {
        int row = i/blockEdgeAmount, col = i%blockEdgeAmount;
        (*(candyImageHolder+i)) = new QLabel;
        (*(candyImageHolder+i))->setStyleSheet("QLabel{background-color : transparent}");
        (*(candyImageHolder+i))->setPixmap(*(candySelectedImage+i%(w->data->getKindsOfCandyValue())));
        (*(candyImageHolder+i))->setGeometry(gapOfBlocks+col*(blockEdgeLength+gapOfBlocks),
                                             gapOfBlocks+row*(blockEdgeLength+gapOfBlocks),
                                             blockEdgeLength,
                                             blockEdgeLength);
        scene->addWidget((*(candyImageHolder+i)));
    }
    //Initialize candyTypeRecorder
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
    delete scene;   //delete scene would also delete the contents(blocks,candyImageHolder) of it

    delete [] candyImage;
    delete [] candySelectedImage;
    delete [] collectCandyImage;

    delete [] candyTypeRecorder;
    //delete [] candyImageHolder;
    delete [] isCandySelected;
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
                    for(int i=index;i<(ballot.size()-1);i++)
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
                //not exceed->向上檢查完畢，繼續往左檢查
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
                    for(int i=index;i<(ballot.size()-1);i++)
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
        //test
    /*
        row = 3;
        col = 4;
        *(candyTypeRecorder+col+row*blockEdgeAmount) = 13;
        row = 6;
        col = 4;
        *(candyTypeRecorder+col+row*blockEdgeAmount) = 12;

  */
        row = 6;
        col = 4;
        *(candyTypeRecorder+col+row*blockEdgeAmount) = 12;
        row = 6;
        col = 7;
        *(candyTypeRecorder+col+row*blockEdgeAmount) = 12;

        //test_END

        //candyTypeRecorder設置完成，接著更新candyImageHolder
        candyImageHolderUpdate();

        //qDebug(optional)
        qDebug() << "candyTypeRecorder:";
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

void GameWindow::checkIfAnyCandyIsSelected(int candyToSelectRow, int candyToSelectCol)
{
    bool isAnyCandySelected = false;
    for(int i=0;i<power(blockEdgeAmount,2);i++)
        if(*(isCandySelected+i))
            isAnyCandySelected = true;
    if(isAnyCandySelected)
        exchangeCandy(candyToSelectRow,candyToSelectCol);
    else
    {
        //No other candy is selected,so just select current candy
        //To select it, check the type of the candy first
        if(*(candyTypeRecorder+candyToSelectCol+candyToSelectRow*blockEdgeAmount)<10)
        {
            //normal candy:just select it
            //To select means to modify two variables: isCandySelected(recorder) and block(for visibility)
            *(isCandySelected+candyToSelectCol+candyToSelectRow*blockEdgeAmount) = true;
            (block+candyToSelectCol+candyToSelectRow*blockEdgeAmount)->setPen(QPen(Qt::red,4));
            (block+candyToSelectCol+candyToSelectRow*blockEdgeAmount)->setBrush(QColor(245,180,40,130));  //bright orange: QColor(245,180,40,170);
        }
        else if(*(candyTypeRecorder+candyToSelectCol+candyToSelectRow*blockEdgeAmount)<20)
        {
            //special candy: check if it is a star candy
            if(*(candyTypeRecorder+candyToSelectCol+candyToSelectRow*blockEdgeAmount)==10)  //star candy
            {
                //if it is a star candy, then just select it like the normal candy
                *(isCandySelected+candyToSelectCol+candyToSelectRow*blockEdgeAmount) = true;
                (block+candyToSelectCol+candyToSelectRow*blockEdgeAmount)->setPen(QPen(Qt::red,4));
                (block+candyToSelectCol+candyToSelectRow*blockEdgeAmount)->setBrush(QColor(245,180,40,130));  //bright orange: QColor(245,180,40,170);
            }
            else
            {
                //Trigger the special candy's ability(Call the special candy's fuction)
                useSpecialCandy(candyToSelectRow,candyToSelectCol);
            }
        }
        else if(*(candyTypeRecorder+candyToSelectCol+candyToSelectRow*blockEdgeAmount)<30)
        {
            //collect candy:could not be selected
            //deselect all of the candies
            cancelSelectedCandy();
        }
    }

}



void GameWindow::exchangeCandy(int candyWantToExchangeRow, int candyWantToExchangeCol)
{
    //check the type of candyWantToExchange
    //Here I put the star candy and the other special candy into difference parts
    //candyWantToExchange:4 posibilities : normal,star,special,collect
    //candySelected:3 posibilities : normal,star,collect(Since special candy cannot be selected)


    if(*(candyTypeRecorder+candyWantToExchangeCol+candyWantToExchangeRow*blockEdgeAmount)>10
            && *(candyTypeRecorder+candyWantToExchangeCol+candyWantToExchangeRow*blockEdgeAmount)<20)
    //If the type of candyWantToExchange is special(star candy exclusive) -> have no effect -> deselect all of the candies
    {
        cancelSelectedCandy();
    }
    else
    {
        //If anyone of the candyWantToExchange and candySelected is star candy, then it needs more complex process
        //So I first calculate the amount of star candy
        int starCandyAmount = 0;
        //Find the selected candy and then check if the selected candy is star candy or not
        int candySelectedRow,candySelectedCol;
        for(int i=0;i<power(blockEdgeAmount,2);i++)
            if(*(isCandySelected+i))    //selected candy
            {
                if(*(candyTypeRecorder+i)==10)  //star candy->starCandyAmount++
                    starCandyAmount++;
                //Record the selected candy's row and col no matter the candy is a star candy or not
                candySelectedRow = i/blockEdgeAmount;
                candySelectedCol = i%blockEdgeAmount;
                break;
            }
        //Then check if candyWantToExchange is star candy or not
        if(*(candyTypeRecorder+candyWantToExchangeCol+candyWantToExchangeRow*blockEdgeAmount)==10)  //star candy
            starCandyAmount++;
        //依照starCandyAmount的值做出對應的動作
        switch(starCandyAmount)
        {
        case 0:
            {
            //None of the candies are star candies
            //If selectedCandy and candyWantToExchange are neighbors of each other, then just exchange them
            //Next to each other:two conditions:
            //1. rowDifference = 0,colDifference = 1 or -1
            //2. rowDifference = 1 or -1, colDifference = 0
            bool areTheTwoCandiesNeighbors = false;
            int rowDifference,colDifference;
            rowDifference = candySelectedRow - candyWantToExchangeRow;
            colDifference = candySelectedCol - candyWantToExchangeCol;
            if(rowDifference<0)
                rowDifference *= -1;    //change it from negative to positive
            if(colDifference<0)
                colDifference *= -1;    //change it from negative to positive
            //Then start to check rowDifference and colDifference
            //If the rowDifference and the colDifference conform the conditions 1. OR 2.,
            //set the areTheTwoCandiesNeighbors to true
            if(rowDifference==0)
                if(colDifference==1)
                    areTheTwoCandiesNeighbors = true;
            if(colDifference==0)
                if(rowDifference==1)
                    areTheTwoCandiesNeighbors = true;
            if(areTheTwoCandiesNeighbors)
            {
                //exchange the candies
                //First, deselect all of the candies
                cancelSelectedCandy();
                //Then exchange candies
                char candyType_tmp;
                candyType_tmp = *(candyTypeRecorder+candySelectedCol+candySelectedRow*blockEdgeAmount);
                *(candyTypeRecorder+candySelectedCol+candySelectedRow*blockEdgeAmount) = *(candyTypeRecorder+candyWantToExchangeCol+candyWantToExchangeRow*blockEdgeAmount);
                *(candyTypeRecorder+candyWantToExchangeCol+candyWantToExchangeRow*blockEdgeAmount) = candyType_tmp;
                //Then update the candyImage holder based on the candyTypeRecorder
                candyImageHolderUpdate();
                //And then eliminate the connected candy
                eliminateConnectedCandy();
            }
            else    //If they are Not neighbors, they can't be exchange -> deselect all of the candies
               cancelSelectedCandy();
            break;
            }
        case 1://One of the candies is a star candy
            //Trigger the star candy's ability(Call the starCandy's function)
            useStarCandy(candySelectedRow,candySelectedCol,candyWantToExchangeRow,candyWantToExchangeCol);
            break;
        case 2://Both of the candies are star candy -> have no effect
            //So just deselect all of the candies
            cancelSelectedCandy();
            break;
        }
    }


}




void GameWindow::useStarCandy(int candySelectedRow,int candySelectedCol,int candyWantToExchangeRow, int candyWantToExchangeCol)
{
    qDebug() << "useStarCandy_START";
    //Figure out which candy is normal ,and the other is star.
    int normalCandyRow,normalCandyCol,starCandyRow,starCandyCol;
    char candyTypeToEliminate;
    if(*(candyTypeRecorder+candySelectedCol+candySelectedRow*blockEdgeAmount)==10)  //candySelected is a star candy
    {
        qDebug() << "candySelected(former) is a star candy";
        //Then the other(candyWantToExchange) is a normal candy
        normalCandyRow = candyWantToExchangeRow;
        normalCandyCol = candyWantToExchangeCol;
        starCandyRow = candySelectedRow;
        starCandyCol = candySelectedCol;
    }
    else  //candyWantToExchange is a star candy -> Then the other(candySelected) is a normal candy
    {
        qDebug() << "candyWantToExchange(latter) is a star candy";
        normalCandyRow = candySelectedRow;
        normalCandyCol = candySelectedCol;
        starCandyRow = candyWantToExchangeRow;
        starCandyCol = candyWantToExchangeCol;
    }
    qDebug() << "starCandyPosition(col,row) : (" << '\0'+starCandyCol << ","<< '\0'+starCandyRow <<")";
    qDebug() << "normalCandyPosition(col,row) : (" << '\0'+normalCandyCol << ","<< '\0'+normalCandyRow <<")";
    //Store the type of the normal candy to eliminate
    candyTypeToEliminate = (*(candyTypeRecorder+normalCandyCol+normalCandyRow*blockEdgeAmount));
    qDebug() << "candyTypeToEliminate: " << '\0'+candyTypeToEliminate;
    //Then eliminate all the candies of the type
    for(int i=0;i<power(blockEdgeAmount,2);i++)
        if(*(candyTypeRecorder+i)==candyTypeToEliminate)
            *(candyTypeRecorder+i) = -1;    //set it to undefined type
    //And eliminate the star candy itself
     *(candyTypeRecorder+starCandyCol+starCandyRow*blockEdgeAmount) = -1;

    //Deselect all of the candies
    cancelSelectedCandy();

    //Star candy have been used, some candy on the board have been eliminated by the star candy's ability
    //So it's about time to generate the candy to fill in the blank on the candyboard
    makeCandyFall(0,0,0,0);

}

void GameWindow::useSpecialCandy(int candyToUseRow, int candyToUseCol)
{

    //Trigger the special candy's ability
    triggerSpecialCandyAbility(candyToUseRow,candyToUseCol);
    //Update the candyImageHolder
    candyImageHolderUpdate();
    //And deselect all of the candies
    cancelSelectedCandy();
    //Then make the remained candies falling and generate new candies
    makeCandyFall(0,0,0,0);

}

void GameWindow::triggerSpecialCandyAbility(int candyToUseRow, int candyToUseCol)
{
    //To trigger the special candy's ability, check the type of the special candy first
    int col,row;
    switch(*(candyTypeRecorder+candyToUseCol+candyToUseRow*blockEdgeAmount))
    {
    //star candy's(type Id:10) condition is defined in the exchangeCandy() and useStarCandy() function
    case 11:    //bomb candy
        //炸掉該candy的周圍區域(一個9x9的方格)
        *(candyTypeRecorder+candyToUseCol+candyToUseRow*blockEdgeAmount)=-1;   //先炸掉自己
        //先炸掉自己的原因:避免自己炸掉的candy中有和自己一樣type的special candy，炸掉時會觸發他的ability，該candy炸掉時又炸到自己，
        //然後自己的ability又被觸發，然後又炸掉他一次，這樣會陷入infinite recursive
        for(row=candyToUseRow-1;row<(candyToUseRow-1)+3;row++)
            for(col=candyToUseCol-1;col<(candyToUseCol-1)+3;col++)
                if(row>=0 && row<blockEdgeAmount)   //If candyToEliminate is in the range -> eliminate it
                    if(col>=0 && col<blockEdgeAmount)
                    {
                        if(row==candyToUseRow && col==candyToUseCol)    //炸掉周圍，遇到自己則跳過(自己已經被炸掉了，不須做任何動作)
                            continue;
                        //check the candy's type before eliminate it
                        if(*(candyTypeRecorder+col+row*blockEdgeAmount)>=10)
                        {
                            if(*(candyTypeRecorder+col+row*blockEdgeAmount)==10)
                            //如果要被炸掉的candy是star candy -> 不可炸掉 ->do nothing
                            {}
                            else if(*(candyTypeRecorder+col+row*blockEdgeAmount)<20)
                            {
                            //如果要被炸掉的candy中有bomb,row,或col candy -> 炸掉時連帶觸發該candy之ability
                                    triggerSpecialCandyAbility(row,col);

                            }
                            else
                            //如果要被炸掉的candy是collect candy -> 不可炸掉 -> do nothing
                            {}
                        }
                        else    //normal/undefined candy  -> just eliminate it
                        {
                            qDebug() << "eliminate normal candy";
                            *(candyTypeRecorder+col+row*blockEdgeAmount)=-1;
                        }
                    }

        break;
    case 12:    //row candy
        //炸掉該candy所在的row
        *(candyTypeRecorder+candyToUseCol+candyToUseRow*blockEdgeAmount)=-1;   //先炸掉自己
            row = candyToUseRow;
            for(col=0;col<blockEdgeAmount;col++)
            {
                if(col==candyToUseCol)
                    continue;   //遇到自己跳過
                //check the candy's type before eliminate it
                if(*(candyTypeRecorder+col+row*blockEdgeAmount)>=10)
                {
                    if(*(candyTypeRecorder+col+row*blockEdgeAmount)==10)
                    //如果要被炸掉的candy是star candy -> 不可炸掉 ->do nothing
                        ;
                    else if(*(candyTypeRecorder+col+row*blockEdgeAmount)<20)
                    //如果要被炸掉的candy中有bomb,row,或col candy -> 炸掉時連帶觸發該candy之ability
                            triggerSpecialCandyAbility(row,col);
                    else
                    //如果要被炸掉的candy是collect candy -> 不可炸掉 -> do nothing
                        ;
                }
                else    //normal candy  -> just eliminate it
                    *(candyTypeRecorder+col+row*blockEdgeAmount)=-1;
            }
        break;

    case 13:    //col candy
        //炸掉該candy所在的col
        *(candyTypeRecorder+candyToUseCol+candyToUseRow*blockEdgeAmount)=-1;   //先炸掉自己
            col = candyToUseCol;
            for(row = 0;row<blockEdgeAmount;row++)
            {
                if(row == candyToUseRow)    //遇到自己跳過
                    continue;
                //check the candy's type before eliminate it
                if(*(candyTypeRecorder+col+row*blockEdgeAmount)>=10)
                {
                    if(*(candyTypeRecorder+col+row*blockEdgeAmount)==10)
                    //如果要被炸掉的candy是star candy -> 不可炸掉 ->do nothing
                        ;
                    else if(*(candyTypeRecorder+col+row*blockEdgeAmount)<20)
                    //如果要被炸掉的candy中有bomb,row,或col candy -> 炸掉時連帶觸發該candy之ability
                            triggerSpecialCandyAbility(row,col);
                    else
                    //如果要被炸掉的candy是collect candy -> 不可炸掉 -> do nothing
                        ;
                }
                else    //normal candy  -> just eliminate it
                    *(candyTypeRecorder+col+row*blockEdgeAmount)=-1;
            }
        break;

    }

}

void GameWindow::candyImageHolderUpdate()
{
    //candyTypeRecorder設置完成後，candyImageHolder根據candyTypeRecorder內容來更新(重新設置對應的candy image)
    for(int i=0;i<power(blockEdgeAmount,2);i++)
    {
        //檢查candy之type, 並根據candy之type設置相對應的圖片
        if(*(candyTypeRecorder+i)>=0)
        {
            if(*(candyTypeRecorder+i)<10) //normal candy
                (*(candyImageHolder+i))->setPixmap(*(candyImage+*(candyTypeRecorder+i)));
            else if(*(candyTypeRecorder+i)<20)    //special candy
            {
                switch(*(candyTypeRecorder+i))
                {
                    case 10:(*(candyImageHolder+i))->setPixmap(specialCandyStarImage);  //star candy
                        break;
                    case 11:(*(candyImageHolder+i))->setPixmap(specialCandyBombImage);  //bomb candy
                        break;
                    case 12:(*(candyImageHolder+i))->setPixmap(specialCandyRowImage);   //row candy
                        break;
                    case 13:(*(candyImageHolder+i))->setPixmap(specialCandyColImage);   //col candy
                        break;
                }
                //Set special candy background    //color:30ffda
                (block+i)->setBrush(QColor(0x30,0xff,0xda,130));
            }
            else if(*(candyTypeRecorder+i)<30)    //collect candy
            {
                (*(candyImageHolder+i))->setPixmap(*(collectCandyImage+*(candyTypeRecorder+i)-20));
                //Set collect candy background   //color:36ff3b
                (block+i)->setBrush(QColor(0x36,0xff,0x3b,130));
            }
        }
        else    //undefined candy type -> just set the holder invisible
            (*(candyImageHolder+i))->clear();

    }


}

bool GameWindow::checkCanEliminateAnyCandy()
{
    int combo;
    //每個candy向右及向下檢查有沒有超過3個以上相連的
    for(int row=0;row<blockEdgeAmount;row++)
        for(int col=0;col<blockEdgeAmount;col++)
        {
            if(*(candyTypeRecorder+col+row*blockEdgeAmount)<10) //只需檢查normal candy，special candy,collect candy不用檢查
            {
                //先向右檢查
                combo=1;
                for(int col_for_check=col+1;col_for_check<blockEdgeAmount;col_for_check++)
                {
                    if(*(candyTypeRecorder+col+row*blockEdgeAmount)==*(candyTypeRecorder+col_for_check+row*blockEdgeAmount))
                        combo++;
                    else
                        break;
                    if(combo>=3)    //有超過三個以上相連 -> return true
                    {
                        return true;
                    }
                }
                //再向下檢查
                combo=1;
                for(int row_for_check=row+1;row_for_check<blockEdgeAmount;row_for_check++)
                {
                    if(*(candyTypeRecorder+col+row*blockEdgeAmount)==*(candyTypeRecorder+col+row_for_check*blockEdgeAmount))
                        combo++;
                    else
                        break;
                    if(combo>=3)    //有超過三個以上相連 ->return true
                    {
                           return true;
                    }
                }
            }

        }
    return false;   //上面檢查完都未有return true發生，則return false
}

void GameWindow::eliminateConnectedCandy()
{
    if(checkCanEliminateAnyCandy()) //如果有可以消除的candy，則開始進行消除
    {
        std::vector<bool> canThisCandyBeEliminated(power(blockEdgeAmount,2));
        //Initialize the recorder
        for(int i=0;i<power(blockEdgeAmount,2);i++)
            canThisCandyBeEliminated.at(i) = false;

        int specialStarCandyAmount = 0;
        int specialBombCandyAmount = 0;
        int specialRowCandyAmount = 0;
        int specialColCandyAmount = 0;
        int combo,type_tmp;

        //逐列檢查:檢查rowCandy,starCandy
        for(int row=0;row<blockEdgeAmount;row++)
        {
            //Initialize type_tmp and combo
            type_tmp=-99;
            combo = 1;
            for(int col=0;col<blockEdgeAmount;col++)
            {
                if(type_tmp==*(candyTypeRecorder+col+row*blockEdgeAmount))
                {
                    combo++;
                    if(combo>=3)    //當combo>=3時，代表三個以上相連 ->可消掉
                    {
                        if(combo==3)    //將此糖果、此糖果-1、此糖果-2的糖果的canThisCandyBeEliminated設成true
                            for(int j=0;j<3;j++)
                                canThisCandyBeEliminated.at((col-j)+row*blockEdgeAmount)=true;
                        else    //combo>3 -> 只須設現在的candy的canThisCandyBeEliminated為true即可
                            canThisCandyBeEliminated.at(col+row*blockEdgeAmount)=true;

                    }
                    if(combo==4)    //當combo=4時，specialRowCandyAmount++
                        specialRowCandyAmount++;
                    if(combo==5)    //當combo=5時，specialStarCandyAmount++,specialRowCandyAmount--
                    {
                        specialStarCandyAmount++;
                        specialRowCandyAmount--;
                    }
                }
                else
                {
                    type_tmp = *(candyTypeRecorder+col+row*blockEdgeAmount);
                    combo = 1;  //重置combo
                }
            }
        }

        //逐行檢查:檢查colCandyAmount,starCandyAmount
        for(int col=0;col<blockEdgeAmount;col++)
        {
            //Initialize type_tmp and combo
            type_tmp=-99;
            combo = 1;
            for(int row=0;row<blockEdgeAmount;row++)
            {
                if(type_tmp==*(candyTypeRecorder+col+row*blockEdgeAmount))
                {
                    combo++;
                    if(combo>=3)    //當combo>=3時，代表三個以上相連 ->可消掉
                    {
                        if(combo==3)    //將此糖果、此糖果-1、此糖果-2的糖果的canThisCandyBeEliminated設成true
                            for(int j=0;j<3;j++)
                                canThisCandyBeEliminated.at(col+(row-j)*blockEdgeAmount)=true;
                        else    //combo>3 -> 只須設現在的candy的canThisCandyBeEliminated為true即可
                            canThisCandyBeEliminated.at(col+row*blockEdgeAmount)=true;

                    }
                    if(combo==4)    //當combo=4時，specialRowCandyAmount++
                        specialColCandyAmount++;
                    if(combo==5)    //當combo=5時，specialStarCandyAmount++,specialColCandyAmount--
                    {
                        specialStarCandyAmount++;
                        specialColCandyAmount--;
                    }
                }
                else
                {
                    type_tmp = *(candyTypeRecorder+col+row*blockEdgeAmount);
                    combo = 1;  //重置combo
                }
            }

        }

        //逐個檢查:檢查bombCandyAmount
        //每一個candy往  1.上左  2.下右   檢查，  如果  comboCol>=3 && comboRow>=3 , 則bombCandyAmount++
        int comboCol,comboRow;
        for(int row=0;row<blockEdgeAmount;row++)
            for(int col=0;col<blockEdgeAmount;col++)
            {
                //1.往上左檢查
                //在進行檢查之前，initialize the combo values first
                comboRow=1;
                comboCol=1;
                //先往上
                for(int row_for_check=row-1;row_for_check>=0;row_for_check--)
                {
                    if(*(candyTypeRecorder+col+row*blockEdgeAmount)==*(candyTypeRecorder+col+row_for_check*blockEdgeAmount))
                        comboCol++;
                    else    //不一樣 -> combo中斷 -> 跳出loop
                        break;
                }
                //再往左
                for(int col_for_check=col-1;col_for_check>=0;col_for_check--)
                {
                    if(*(candyTypeRecorder+col+row*blockEdgeAmount)==*(candyTypeRecorder+col_for_check+row*blockEdgeAmount))
                        comboRow++;
                    else    //不一樣 -> combo中斷 -> 跳出loop
                        break;
                }
                //然後檢查有沒有同時符合comboRow>=3和comboCol>=3  有的話specialCandyAmount++
                if(comboRow>=3 && comboCol>=3)
                    specialBombCandyAmount++;


                //2.往下右檢查
                //在進行檢查之前，initialize the combo values first
                comboRow=1;
                comboCol=1;
                //先往下
                for(int row_for_check=row+1;row_for_check<blockEdgeAmount;row_for_check++)
                {
                    if(*(candyTypeRecorder+col+row*blockEdgeAmount)==*(candyTypeRecorder+col+row_for_check*blockEdgeAmount))
                        comboCol++;
                    else    //不一樣 -> combo中斷 -> 跳出loop
                        break;
                }
                //再往右
                for(int col_for_check=col+1;col_for_check<blockEdgeAmount;col_for_check++)
                {
                    if(*(candyTypeRecorder+col+row*blockEdgeAmount)==*(candyTypeRecorder+col_for_check+row*blockEdgeAmount))
                        comboRow++;
                    else    //不一樣 -> combo中斷 -> 跳出loop
                        break;
                }
                //然後檢查有沒有同時符合comboRow>=3和comboCol>=3  有的話specialCandyAmount++
                if(comboRow>=3 && comboCol>=3)
                    specialBombCandyAmount++;


            }
        //計算完要產生的specialCandy的數量了，待會便可以傳給makeCandyFall()使其產生相對應數量的candy

        //接著要將canThisCandyBeEliminated = true 的candy給消除
        for(int i=0;i<power(blockEdgeAmount,2);i++)
            if(canThisCandyBeEliminated.at(i))  //If the value is true
                *(candyTypeRecorder+i) = -1;    //then eliminate the candy


        //After eliminate candy, call the function makeCandyFall()
        makeCandyFall(specialStarCandyAmount,specialBombCandyAmount,specialRowCandyAmount,specialColCandyAmount);
    }
    else
    {
        cancelSelectedCandy();
        candyImageHolderUpdate();
    }
}

void GameWindow::makeCandyFall(int specialStarCandyAmount,int specialBombCandyAmount,int specialRowCandyAmount,int specialColCandyAmount)
{
    //-1 means the block has no candy(empty)

    //step1: move all the candy downwards to fill the block with -1 type
    int moveStep;
    for(int col=0;col<blockEdgeAmount;col++)
        for(int row=blockEdgeAmount-1;row>=0;row--) //由下往上逐一檢查每個block
        {
            moveStep = 0;
            for(int row_for_check=row+1;row_for_check<blockEdgeAmount;row_for_check++)  //每一個block由上往下檢查
            {
                //If the next block below is empty, then moveStep++
                if(*(candyTypeRecorder+col+row_for_check*blockEdgeAmount)==-1)
                {
                    moveStep++;
                }
                else    //encounter a non-empty block -> break the loop
                {
                    break;
                }
            }

            //Next is to move the block downwards based on the moveStep
            if(moveStep>0)
            {
                //If the moveStep is not zero, then move the block downwards
                *(candyTypeRecorder+col+(row+moveStep)*blockEdgeAmount) = *(candyTypeRecorder+col+row*blockEdgeAmount);
                //And set the origin block to -1
                *(candyTypeRecorder+col+row*blockEdgeAmount) = -1;
            }
        }
    //step2:generate candies to fill the empty block(step1已將所有candy都movedown了，所以現在空的block都擠在上面)
    //和initCandyBoard十分類似的填法，不過這邊不必像initCandyBoard一樣需要檢查，因為落下的糖果可以相連
    //並且多一個條件:只填-1的格子
    //不過注意:要先填special candies, 再填normal candy

    //先填special candies : based on the parameters passed by the eliminateCandy()
    //製作空位的籤
    std::vector<int> empty_block_row_ballot;
    std::vector<int> empty_block_col_ballot;
    //將空位的pos記錄到empty_block_col_ballot和empty_block_row_ballot
    for(int i=0;i<power(blockEdgeAmount,2);i++)
        if(*(candyTypeRecorder+i) == -1)    //如果是空的 -> 將位置資訊記錄到empty_block_row_ballot和empty_block_col_ballot
        {
            empty_block_row_ballot.push_back(i/blockEdgeAmount);
            empty_block_col_ballot.push_back(i%blockEdgeAmount);
        }
    //接著開始抽籤啦~
    //抽籤順序:star->bomb->row->col(順序沒什麼差別，先抽後抽機率都一樣)
    int index;
    //starCandy
    for(int times=0;times<specialStarCandyAmount;times++)
    {
        index = qrand()%(empty_block_col_ballot.size());
        //將所抽到的格子設為specialStarCandy
        *(candyTypeRecorder+empty_block_col_ballot.at(index)+empty_block_row_ballot.at(index)*blockEdgeAmount) = 10;    //starCandy ID = 10
        //抽到後將該籤銷毀
        for(int i=index;i<empty_block_col_ballot.size()-1;i++)
        {
            empty_block_col_ballot.at(i) = empty_block_col_ballot.at(i+1);
            empty_block_row_ballot.at(i) = empty_block_row_ballot.at(i+1);
        }
        empty_block_col_ballot.pop_back();
        empty_block_row_ballot.pop_back();
    }
    //bombCandy
    for(int times=0;times<specialBombCandyAmount;times++)
    {
        index = qrand()%(empty_block_col_ballot.size());
        //將所抽到的格子設為specialCandy
        *(candyTypeRecorder+empty_block_col_ballot.at(index)+empty_block_row_ballot.at(index)*blockEdgeAmount) = 11;    //bombCandy ID = 11
        //抽到後將該籤銷毀
        for(int i=index;i<empty_block_col_ballot.size()-1;i++)
        {
            empty_block_col_ballot.at(i) = empty_block_col_ballot.at(i+1);
            empty_block_row_ballot.at(i) = empty_block_row_ballot.at(i+1);
        }
        empty_block_col_ballot.pop_back();
        empty_block_row_ballot.pop_back();
    }
    //rowCandy
    for(int times=0;times<specialRowCandyAmount;times++)
    {
        index = qrand()%(empty_block_col_ballot.size());
        //將所抽到的格子設為specialRowCandy
        *(candyTypeRecorder+empty_block_col_ballot.at(index)+empty_block_row_ballot.at(index)*blockEdgeAmount) = 12;    //rowCandy ID = 12
        //抽到後將該籤銷毀
        for(int i=index;i<empty_block_col_ballot.size()-1;i++)
        {
            empty_block_col_ballot.at(i) = empty_block_col_ballot.at(i+1);
            empty_block_row_ballot.at(i) = empty_block_row_ballot.at(i+1);
        }
        empty_block_col_ballot.pop_back();
        empty_block_row_ballot.pop_back();
    }
    //colCandy
    for(int times=0;times<specialColCandyAmount;times++)
    {
        index = qrand()%(empty_block_col_ballot.size());
        //將所抽到的格子設為specialColCandy
        *(candyTypeRecorder+empty_block_col_ballot.at(index)+empty_block_row_ballot.at(index)*blockEdgeAmount) = 13;    //colCandy ID = 13
        //抽到後將該籤銷毀
        for(int i=index;i<empty_block_col_ballot.size()-1;i++)
        {
            empty_block_col_ballot.at(i) = empty_block_col_ballot.at(i+1);
            empty_block_row_ballot.at(i) = empty_block_row_ballot.at(i+1);
        }
        empty_block_col_ballot.pop_back();
        empty_block_row_ballot.pop_back();
    }


    //剩下的空格子就可以亂填normal candy了
    int kindsOfCandy = w->data->getKindsOfCandyValue();
    int row,col;
    for(col = 0;col<blockEdgeAmount;col++)
        for(row = 0;row<blockEdgeAmount;row++)
        {
            if(*(candyTypeRecorder+col+row*blockEdgeAmount)==-1)
            {
                *(candyTypeRecorder+col+row*blockEdgeAmount) = qrand()%kindsOfCandy;
            }

        }

    //Update the candy image holder
    candyImageHolderUpdate();


    //After all of the candies have fallen, then its time to eliminate 相連的 candy
    eliminateConnectedCandy();

}

void GameWindow::cancelSelectedCandy()
{
    //set all candies unselected
    for(int i=0;i<power(blockEdgeAmount,2);i++)
    {
        *(isCandySelected+i) = false;
         //And then initialize the block QGraphicsRectItem
        if(*(candyTypeRecorder+i)>=10)   //Check the candy's type.
        //If the candy's type is not normal(0~9), then for visibility's sake, set the candy with a certain background
        {
            if(*(candyTypeRecorder+i)<20) //special candy    //color:30ffda:bright blue
                (block+i)->setBrush(QColor(0x30,0xff,0xda,130));
            else if(*(candyTypeRecorder+i)<30)    //collect candy //color:36ff3b:bright green
                (block+i)->setBrush(QColor(0x36,0xff,0x3b,130));
        }
        else    //normal or undefined candy
        {
            (block+i)->setBrush(QColor(0,0,0,130));
        }
        //No matter what type the candy is, set the frame of the candy's block to invisible
        (block+i)->setPen(QPen(Qt::transparent,0));
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
    if(isMousePressEventBlocked)
    {
        ;//do nothing
        qDebug()<<"***mousePressEvent BLOCKED!!";
    }
    else
    {
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
                //If the mouse position is in the candyboard when the mouse is pressed, check if any candy is selected
                checkIfAnyCandyIsSelected(row,col);

/*
                if(col+1<blockEdgeAmount)
                {
                    (block+(col+1)+row*blockEdgeAmount)->setPen(QPen(Qt::blue,4));
                    (block+(col+1)+row*blockEdgeAmount)->setBrush(QColor(20,230,245,100));
                }
                if(col-1>=0)
                {
                    (block+(col-1)+row*blockEdgeAmount)->setPen(QPen(Qt::blue,4));
                    (block+(col-1)+row*blockEdgeAmount)->setBrush(QColor(20,230,245,100));
                }
                if(row+1<blockEdgeAmount)
                {
                    (block+col+(row+1)*blockEdgeAmount)->setPen(QPen(Qt::blue,4));
                    (block+col+(row+1)*blockEdgeAmount)->setBrush(QColor(20,230,245,100));
                }
                if(row-1>=0)
                {
                    (block+col+(row-1)*blockEdgeAmount)->setPen(QPen(Qt::blue,4));
                    (block+col+(row-1)*blockEdgeAmount)->setBrush(QColor(20,230,245,100));
                }
*/
            }
            else
            {
                //If the position of the mouse is not in the area of the gameboard, then deselect all candies
                cancelSelectedCandy();
            }
        }
        else
        {
            //If the position of the mouse is not in the area of the gameboard, then deselect all candies
            cancelSelectedCandy();
        }
        qDebug() << "candyTypeRecorder:";
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
        qDebug() << "Mouse current position  x : " << event->x();
        qDebug() << "Mouse current position  y : " << event->y();
        qDebug() << "Mouse current position  (col,row) : " <<"("<<col<<","<<row<<")" ;
    }


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
