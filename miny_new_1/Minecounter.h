#ifndef MINECOUNTER_H
#define MINECOUNTER_H


class Minecounter
{
    public:
        Minecounter(short m=0, short sm=0) :
        startMines(m), startSupermines(sm), remainingMines(m), remainingSupermines(sm) {}
        virtual ~Minecounter();

        short startMines;
        short startSupermines;
        short remainingMines;
        short remainingSupermines;

        void setStartMines(short sm);
        void setStartSupermines(short ssm);

    protected:
    private:


};

#endif // MINECOUNTER_H
