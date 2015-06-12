#ifndef DATA_H
#define DATA_H


class Data
{
public:
    Data();
    ~Data();
    void setModeValue(int value);
    int getModeValue();
    void setStepValue(int value);
    int getStepValue();
    void setGoalValue(int value);
    int getGoalValue();
    void setComboBox_boardEdgeSizeIndex(int index);
    int getComboBox_boardEdgeSizeIndex();
    int getBoardEdgeSizeValue();
private:
    int mode;
    int step;
    int goal;
    int comboBox_boardEdgeSizeIndex;
};

#endif // DATA_H
