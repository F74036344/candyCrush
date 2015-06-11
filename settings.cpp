#include "mainwindow.h"
#include "settings.h"
#include "ui_settings.h"


extern MainWindow *w;

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    //set the basic properties
    setFixedSize(450,450);

    //set the limits(max and in value) of the spinBox_step
    //**spinBox_goal's limits would be set later
    ui->spinBox_step->setMaximum(100);
    ui->spinBox_step->setMinimum(10);


    //load the pic of each option
    scoreMode_pic.load(":/image/Resource/score.png");
    scoreMode_pic_s.load(":/image/Resource/score.png");
    amountMode_pic.load(":/image/Resource/candy_3.png");
    amountMode_pic_s.load(":/image/Resource/candy_3.png");
    collectMode_pic.load(":/image/Resource/specialCandy_4.png");
    collectMode_pic_s.load(":/image/Resource/specialCandy_4.png");
    //And then set the pic and the pic_s to the proper size
    //_s first
    scoreMode_pic_s = scoreMode_pic_s.scaled(ui->label_scoreModeIcon->width(),
                                         ui->label_scoreModeIcon->height());
    amountMode_pic_s = amountMode_pic_s.scaled(ui->label_amountModeIcon->width(),
                                         ui->label_amountModeIcon->height());
    collectMode_pic_s = collectMode_pic_s.scaled(ui->label_collectModeIcon->width(),
                                         ui->label_collectModeIcon->height());
    //then the normal
    scoreMode_pic = scoreMode_pic.scaled(ui->label_goal_pic->width(),
                                         ui->label_goal_pic->height());
    amountMode_pic = amountMode_pic.scaled(ui->label_goal_pic->width(),
                                         ui->label_goal_pic->height());
    collectMode_pic = collectMode_pic.scaled(ui->label_goal_pic->width(),
                                         ui->label_goal_pic->height());

    //Read the checked radioButton info from data
    switch(w->data->getModeValue())
    {
    case 1:
        ui->radioButton_mode1->setChecked(true);
        //set the vlaue of label_goal_pic (which is next to the spinBox_goal)
        ui->label_goal_pic->clear();
        ui->label_goal_pic->setText("score");
        //reset the limits of the spinBox_goal
        ui->spinBox_goal->setMaximum(100000);
        ui->spinBox_goal->setMinimum(100);
        //reset the text of label_text_goal
        ui->label_text_goal->setText("goal:\n(100~100000)");
        //reset the text of label_mode_intro
        ui->label_mode_intro->setText("<html><head/><body><p>To play the game with the </p><p>goal <span style=\" font-weight:600;\">achieving a specific </span></p><p><span style=\" font-weight:600;\">score</span> in limited steps. </p></body></html>");
        break;
    case 2:
        ui->radioButton_mode2->setChecked(true);
        //set the vlaue of label_goal_pic (which is next to the spinBox_goal)
        ui->label_goal_pic->clear();
        ui->label_goal_pic->setPixmap(amountMode_pic);
        //reset the limits of the spinBox_goal
        ui->spinBox_goal->setMaximum(500);
        ui->spinBox_goal->setMinimum(10);
        //reset the text of label_text_goal
        ui->label_text_goal->setText("goal:\n(10~500)");
        //reset the text of label_mode_intro
        ui->label_mode_intro->setText("<html><head/><body><p>This mode is to play the </p><p>game with the goal </p><p><span style=\" font-weight:600;\">eliminating a certain kind</span></p><p><span style=\" font-weight:600;\">of candies to a particular </span></p><p><span style=\" font-weight:600;\">amount.</span></p></body></html>");
        break;
    case 3:
        ui->radioButton_mode3->setChecked(true);
        //set the vlaue of label_goal_pic (which is next to the spinBox_goal)
        ui->label_goal_pic->clear();
        ui->label_goal_pic->setPixmap(collectMode_pic);
        //reset the limits of the spinBox_goal
        ui->spinBox_goal->setMaximum(20);
        ui->spinBox_goal->setMinimum(1);
        //reset the text of label_text_goal
        ui->label_text_goal->setText("goal:\n(1~20)");
        //reset the text of label_mode_intro
        ui->label_mode_intro->setText("<html><head/><body><p>When selecting this mode, </p><p>a kind of <span style=\" font-weight:600;\">special candies </span></p><p>would appear, and you can </p><p><span style=\" font-weight:600;\">collect it by eliminating all the </span></p><p><span style=\" font-weight:600;\">candies below it</span> ,and it would</p><p><span style=\" font-weight:600;\">fall to the bottom </span>so that you </p><p>can collect it. Try your best</p><p>to reach the goal!</p></body></html>");
        break;
    }

    //Read the step value from data
    ui->spinBox_step->setValue(w->data->getStepValue());
    //Read the goal value from data
    ui->spinBox_goal->setValue(w->data->getGoalValue());

    //Assign them to the displaying label
    ui->label_scoreModeIcon->setPixmap(scoreMode_pic_s);
    ui->label_amountModeIcon->setPixmap(amountMode_pic_s);
    ui->label_collectModeIcon->setPixmap(collectMode_pic_s);

}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pushButton_cancel_clicked()
{
    close();
}

void Settings::on_pushButton_ok_clicked()
{
    //Save the current settings to the data
    //save the mode value to the data
    if(ui->radioButton_mode1->isChecked())
        w->data->setModeValue(1);
    else if(ui->radioButton_mode2->isChecked())
        w->data->setModeValue(2);
    else if(ui->radioButton_mode3->isChecked())
        w->data->setModeValue(3);
    //save the step value to the data
    w->data->setStepValue(ui->spinBox_step->value());
    //save the goal value to the data
    w->data->setGoalValue(ui->spinBox_goal->value());

    close();

}

void Settings::on_radioButton_mode1_clicked()
{
    ui->label_goal_pic->clear();
    ui->label_goal_pic->setText("score");
    //reset the limits of the spinBox_goal
    ui->spinBox_goal->setMaximum(100000);
    ui->spinBox_goal->setMinimum(100);
    //reset the text of label_text_goal
    ui->label_text_goal->setText("goal:\n(100~100000)");
    //reset the text of label_mode_intro
    ui->label_mode_intro->setText("<html><head/><body><p>To play the game with the </p><p>goal <span style=\" font-weight:600;\">achieving a specific </span></p><p><span style=\" font-weight:600;\">score</span> in limited steps. </p></body></html>");
}

void Settings::on_radioButton_mode2_clicked()
{
    //set the vlaue of label_goal_pic (which is next to the spinBox_goal)
    ui->label_goal_pic->clear();
    ui->label_goal_pic->setPixmap(amountMode_pic);
    //reset the limits of the spinBox_goal
    ui->spinBox_goal->setMaximum(500);
    ui->spinBox_goal->setMinimum(10);
    //reset the text of label_text_goal
    ui->label_text_goal->setText("goal:\n(10~500)");
    //reset the text of label_mode_intro
    ui->label_mode_intro->setText("<html><head/><body><p>This mode is to play the </p><p>game with the goal </p><p><span style=\" font-weight:600;\">eliminating a certain kind</span></p><p><span style=\" font-weight:600;\">of candies to a particular </span></p><p><span style=\" font-weight:600;\">amount.</span></p></body></html>");
}

void Settings::on_radioButton_mode3_clicked()
{
    //set the vlaue of label_goal_pic (which is next to the spinBox_goal)
    ui->label_goal_pic->clear();
    ui->label_goal_pic->setPixmap(collectMode_pic);
    //reset the limits of the spinBox_goal
    ui->spinBox_goal->setMaximum(20);
    ui->spinBox_goal->setMinimum(1);
    //reset the text of label_text_goal
    ui->label_text_goal->setText("goal:\n(1~20)");
    //reset the text of label_mode_intro
    ui->label_mode_intro->setText("<html><head/><body><p>When selecting this mode, </p><p>a kind of <span style=\" font-weight:600;\">special candies </span></p><p>would appear, and you can </p><p><span style=\" font-weight:600;\">collect it by eliminating all the </span></p><p><span style=\" font-weight:600;\">candies below it</span> ,and it would</p><p><span style=\" font-weight:600;\">fall to the bottom </span>so that you </p><p>can collect it. Try your best</p><p>to reach the goal!</p></body></html>");
}
