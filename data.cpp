#include "data.h"

Data::Data()
{
    //set default values
    setModeValue(1);
    setStepValue(30);
    setGoalValue(500);
    setComboBox_boardEdgeSizeIndex(2);
    setComboBox_kindsOfCandyIndex(1);
}

Data::~Data()
{

}







void Data::setModeValue(int value)
{
    mode = value;
}

int Data::getModeValue()
{
    return mode;
}

void Data::setStepValue(int value)
{
    step = value;
}

int Data::getStepValue()
{
    return step;
}

void Data::setGoalValue(int value)
{
    goal = value;
}

int Data::getGoalValue()
{
    return goal;
}

void Data::setComboBox_boardEdgeSizeIndex(int index)
{
    comboBox_boardEdgeSizeIndex = index;
}

int Data::getComboBox_boardEdgeSizeIndex()
{
    return comboBox_boardEdgeSizeIndex;
}

int Data::getBoardEdgeSizeValue()
{
    return comboBox_boardEdgeSizeIndex+8;
}

void Data::setComboBox_kindsOfCandyIndex(int index)
{
    comboBox_kindsOfCandyIndex = index;
}

int Data::getComboBox_kindsOfCandyIndex()
{
    return comboBox_kindsOfCandyIndex;
}

int Data::getKindsOfCandyValue()
{
    return comboBox_kindsOfCandyIndex+4;
}
