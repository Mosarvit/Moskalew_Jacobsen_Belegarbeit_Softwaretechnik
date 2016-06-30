#ifndef SQUARE_H
#define SQUARE_H


class Square
{
    public:

        Square(short x=0, short y=0, short sM=0, short sSm=0, short mn=0, short st=1, bool adjSf=0, bool adjSm=0) :
        posX(x), posY(y), surroundingMines(sM), surroundingSupermines(sSm), mine(mn) , state(st) , adjacentToSuperFlag(adjSf), adjacentToSuperMine(adjSm) {}

        short posX;
        short posY;
        short mine;
        short surroundingMines;
        short surroundingSupermines;
        short state;
        bool adjacentToSuperFlag;
        bool adjacentToSuperMine;
    protected:
    private:
};

#endif // SQUARE_H
