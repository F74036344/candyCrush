#include "data.h"

Data::Data()
{
    //set default values
    setModeValue(1);
    setStepValue(30);
    setGoalValue(500);
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

