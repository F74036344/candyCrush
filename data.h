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

    void setComboBox_kindsOfCandyIndex(int index);
    int getComboBox_kindsOfCandyIndex();
    int getKindsOfCandyValue();
private:
    int mode;
    int step;
    int goal;
    int comboBox_boardEdgeSizeIndex;
    int comboBox_kindsOfCandyIndex;
};

#endif // DATA_H
