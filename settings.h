#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_radioButton_mode1_clicked();

    void on_radioButton_mode2_clicked();

    void on_radioButton_mode3_clicked();

private:
    Ui::Settings *ui;
    QPixmap scoreMode_pic,amountMode_pic,collectMode_pic;
    QPixmap scoreMode_pic_s,amountMode_pic_s,collectMode_pic_s;
};

#endif // SETTINGS_H
