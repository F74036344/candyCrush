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
private:
    int mode;
    int step;
    int goal;
};

#endif // DATA_H
