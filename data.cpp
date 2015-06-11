#include "data.h"

Data::Data()
{
    //set default mode value
    mode = 0;
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

