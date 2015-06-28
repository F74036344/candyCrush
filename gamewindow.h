#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QtGui>
#include <QtCore>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QLabel>
#include <QPixmap>

#include "result.h"

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT

signals:
    void quit(int star,int score);

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

    void initCandyBoard();

    void checkIfAnyCandyIsSelected(int candyToSelectRow,int candyToSelectCol);
        void exchangeCandy(int candyWantToExchangeRow,int candyWantToExchangeCol);
            void useStarCandy(int candySelectedRow,int candySelectedCol,int candyWantToExchangeRow,int candyWantToExchangeCol);
        void useSpecialCandy(int candyToUseRow,int candyToUseCol);
            void triggerSpecialCandyAbility(int candyToUseRow,int candyToUseCol);

    void candyImageHolderUpdate();

    void eliminateConnectedCandy(bool isChecked);
        bool checkCanEliminateAnyCandy();

    void makeCandyFall(int specialStarCandyAmount,int specialBombCandyAmount,int specialRowCandyAmount,int specialColCandyAmount);

    void cancelSelectedCandy();

    void checkIfTheGameIsOver();


    //functions related to display
    //這些函式會同步更新ui中的QLabel及gamewindow.cpp中的相對應的變數
    void setGoalStatement();
    void setGoalValue(int value);
    void setScoreValue(int value);
    void setStepValue(int value);

    int power(int,int);
    Result *rslt;

protected:
    void mousePressEvent(QMouseEvent *event);

private slots:
    void on_pushButton_returnToMenu_clicked();

    void on_pushButton_clickMe_clicked();

private:
    Ui::GameWindow *ui;
    QPixmap backgroundImage;
    QGraphicsScene *scene;
    QGraphicsRectItem *block;
    int blockEdgeAmount;

    int gapOfWidget,gapOfBlocks;
    int blockEdgeLength;
    int gameAreaEdgeLength,recordAreaWidth;

    //variables to deal with candy images
    QPixmap *candyImage, *candySelectedImage;
    QPixmap specialCandyRowImage,specialCandyColImage,specialCandyBombImage,specialCandyStarImage;
    QPixmap *collectCandyImage;
    QLabel **candyImageHolder;
    char *candyTypeRecorder;    //candyType(char): 1.normalCandy: 0~5  2.specialCandy: 10~13  3.collectCandy: 20~23  4.undefined: -1
    bool *isCandySelected;

    bool isMousePressEventBlocked;

    //variables to store data such as score, step, etc

    int goalCandy_index;
    int collectCandyAmountOnBoard;

    int goal;
    int score;
    int step;
    int star;

};

#endif // GAMEWINDOW_H
