#ifndef RESULT_H
#define RESULT_H

#include <QWidget>
#include <QMainWindow>

namespace Ui {
class Result;
}

class Result : public QWidget
{
    Q_OBJECT

public:
    explicit Result(QWidget *parent = 0);
    ~Result();

private slots:
    void on_pushButton_returnToMenu_clicked();

private:
    Ui::Result *ui;
};

#endif // RESULT_H
