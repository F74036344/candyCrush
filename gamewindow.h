#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QtGui>
#include <QtCore>

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget
{
    Q_OBJECT



public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();
    int power(int,int);

signals:
    void quit(int star,int score);


private:
    Ui::GameWindow *ui;
    QPixmap backgroundImage;
    QGraphicsScene *scene;
    QGraphicsRectItem *block;
    int blockEdgeAmount;

    int gapOfWidget,gapOfBlocks;
    int blockEdgeLength;
    int gameAreaEdgeLength,recordAreaWidth;
};

#endif // GAMEWINDOW_H
