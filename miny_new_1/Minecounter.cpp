#include "Minecounter.h"



Minecounter::~Minecounter()
{
    //dtor
}

void Minecounter::setStartMines(short sm){

    startMines = sm;
    remainingMines = sm;
}
void Minecounter::setStartSupermines(short ssm){

    startSupermines = ssm;
    remainingSupermines = ssm;
}
