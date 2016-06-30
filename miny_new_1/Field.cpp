#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Square.h"
#include <stdio.h>
#include <vector>
#include "common.h"
#include "Field.h"
#include "Timer.h"
#include "Replay.h"

using namespace std;

extern void redisplay();

extern bool playReplay;
extern int gameState;
extern bool isFlagging;
extern Timer timer;
extern bool gamePaused;
extern Replay replay;
extern unsigned short hitMineX, hitMineY;
extern int squareSize;

extern void saveReplay(char *, Replay *);


extern void endGameWon();
extern void endGameLost();

/////xxx





////

//// xxx

//vector<Square> Field::surroundingSquares(int squareX, int squareY, int range) {
//
//    vector<Square> sqrs;
//
//    for (int x=squareX-range; x<=squareX+range; x++){
//        for (int y=squareY-range; y<=squareY+range; y++){
//            if (y<=height and y>=0 and x>=0 and x<=width) sqrs.push_back(allSquares[x][y]);
//        }
//    }
//    if (sqrs.size()==0 and range==2 ){
//
//    cout << squareX << " " <<squareY<<": " << endl;
//    cout << "size: " <<sqrs.size() << endl;
//    cout << "squareX-range: " <<squareX-range << endl;
//    cout << "squareX-range: " <<squareX+range << endl;
//    cout << "squareY-range: " <<squareY-range << endl;
//    cout << "squareY-range: " <<squareY+range << endl;
//
//        for (int x=squareX-range; x<=squareX+range; x++){
//            for (int y=squareY-range; y<=squareY+range; y++){
//                cout << x << y <<endl;
//            }
//        }
//    }
//    return sqrs;
//}

vector<Square*> Field::surroundingSquares(int squareX, int squareY, int range) {

    vector<Square*> sqrs;

    for (int x=squareX-range; x<=squareX+range; x++){
        for (int y=squareY-range; y<=squareY+range; y++){
            if (y<=height and y>=0 and x>=0 and x<=width) sqrs.push_back(&allSquares[x][y]);
        }
    }
    if (sqrs.size()==0 and range==2 ){
        for (int x=squareX-range; x<=squareX+range; x++){
            for (int y=squareY-range; y<=squareY+range; y++){
                cout << x << y <<endl;
            }
        }
    }
    return sqrs;
}

void Field::ffmProc(int tmpField[MAX_WIDTH][MAX_HEIGHT],int i,int j) {

   // cout << "ffmproc(" << i << ", " << j << ")" << endl;

    if (tmpField[i][j]!=3) {
        bool isZero=(tmpField[i][j]==0);

        tmpField[i][j]=3;

        if (isZero)
            floodFillMark(tmpField,i,j);

    }

}


void Field::floodFillMark(int tmpField[MAX_WIDTH][MAX_HEIGHT],int i,int j) {

    if (i>0) {
        if (j>0) ffmProc(tmpField,i-1,j-1);
        ffmProc(tmpField,i-1,j);
        if (j<height-1) ffmProc(tmpField,i-1,j+1);

    }

    if (j>0) ffmProc(tmpField,i,j-1);
    if (j<height-1) ffmProc(tmpField,i,j+1);

    if (i<width-1) {
        if (j>0) ffmProc(tmpField,i+1,j-1);
        ffmProc(tmpField,i+1,j);
        if (j<height-1) ffmProc(tmpField,i+1,j+1);
    }


}

bool Field::isMine(int x, int y) {

    if (x<0 || x>=width) {
        cerr << "x=="<<x<<" out of bounds"<<endl;
        exit(1);
    }
    else if (y<0 || y>=height) {
        cerr << "y=="<<y<<" out of bounds"<<endl;
        exit(1);
    }
    else
        return allSquares[x][y].mine==1;


    return false;
}

int Field::calculate3BV() {

    int tmpField[MAX_WIDTH][MAX_HEIGHT];    // 0=0, 1=other number, 2=mine, 3=processed

    int tmp3BV=0;


    // temp zero all

    for (int i=0;i<width;i++)
        for (int j=0;j<height;j++)
            tmpField[i][j]=0;


    // mark "numbers" as 1

    for (int i=0;i<width;i++)
        for (int j=0;j<height;j++)
            if (allSquares[i][j].mine==1) {

                if (i>0) {
                    if (j>0) tmpField[i-1][j-1]=1;
                    tmpField[i-1][j]=1;
                    if (j<height-1) tmpField[i-1][j+1]=1;
                }

                if (j>0) tmpField[i][j-1]=1;
                if (j<height-1) tmpField[i][j+1]=1;

                if (i<width-1) {
                    if (j>0) tmpField[i+1][j-1]=1;
                    tmpField[i+1][j]=1;
                    if (j<height-1) tmpField[i+1][j+1]=1;
                }
            }

    // mark mines as 2

    for (int i=0;i<width;i++)
        for (int j=0;j<height;j++)
//            if (mine[i][j])
              if (allSquares[i][j].mine==1)
                  tmpField[i][j]=2;


    // count

    for (int i=0;i<width;i++)
        for (int j=0;j<height;j++)
            if (tmpField[i][j]==0) {
                tmp3BV++;
                tmpField[i][j]=3;

                floodFillMark(tmpField,i,j);
            }


    // count

    for (int i=0;i<width;i++)
        for (int j=0;j<height;j++)
            if (tmpField[i][j]==1)
                tmp3BV++;


   // cout << "3bv==" << tmp3BV << endl;

    return tmp3BV;

}

int Field::get3BV() {
    if (val3BV==0)
        val3BV=calculate3BV();

    return val3BV;

}


void Field::checkValues() {



    if (height<2)
        height=2;
    else if (height>MAX_HEIGHT)
        height=MAX_HEIGHT;


    if (width<2)
        width=2;
    else if (width>MAX_WIDTH)
        width=MAX_WIDTH;

    if (mc.startMines+mc.startSupermines>=height*width){
        mc.setStartMines((height*width-1)/2);
        mc.setStartSupermines((height*width-1)/2);
    }
    if (mc.startMines<1)
        mc.setStartMines(1);
    if (mc.startSupermines<1)
        mc.setStartSupermines(1);


}




bool Field::isSupermine(int x, int y) {

    if (x<0 || x>=width) {
        cerr << "x=="<<x<<" out of bounds"<<endl;
        exit(1);
    }
    else if (y<0 || y>=height) {
        cerr << "y=="<<y<<" out of bounds"<<endl;
        exit(1);
    }
    else
        return allSquares[x][y].mine==2;

    return false;
}



void Field::setMine(int x, int y) {

        allSquares[x][y].mine=1;
}

// xxx

void Field::setSupermine(int x, int y) {

    allSquares[x][y].mine=2;
}


void Field::placeMines(int firstClickX, int firstClickY) {

    for (int i=0;i<width;i++) {
        for (int j=0;j<width;j++) {
            allSquares[i][j].mine=0;
        }
    }

    for (int i=0;i<width;i++) {         // xxx2
        for (int j=0;j<width;j++) {
            allSquares[i][j].mine=0;
        }
    }



    for (int i=0;i<mc.startMines;i++) {
        int x=rand()%width;
        int y=rand()%height;
            if (allSquares[x][y].mine==1 or (firstClickX==x and firstClickY==y))
            i--;
        else
            allSquares[x][y].mine=1;
    }

    for (int i=0;i<mc.startSupermines;i++) {     // xxx2
        bool dontPlace=false;
        int x=rand()%width;
        int y=rand()%height;
        vector<Square*> vector1 =surroundingSquares(x,y,1);
        for (vector<Square*>::iterator it = vector1.begin(); it != vector1.end(); it++){

            if ((**it).posX==firstClickX and (**it).posY==firstClickY){
                dontPlace=true;
            }
        }

        if (allSquares[x][y].mine>0 or (firstClickX==x and firstClickY==y) or dontPlace)
            i--;
        else{

            allSquares[x][y].mine=2;
            vector<Square*> vector2 =surroundingSquares(x,y,1);
            for (vector<Square*>::iterator it = vector2.begin(); it != vector2.end(); it++){

                (*it)->adjacentToSuperMine=1;
            }
        }
    }

   // calculate3BV();
    val3BV=0;

    redisplay();

}

void Field::init() {

    for (int i=0;i<width;i++) {
        for (int j=0;j<height;j++) {
            allSquares[i][j] = Square();
            allSquares[i][j].posX=i;
            allSquares[i][j].posY=j;
            allSquaresV.push_back(&allSquares[i][j]);
        }
    }

    gameState=GAME_INITIALIZED;
    isFlagging=false;
    timer.reset();

    gamePaused=false;

    effectiveClicks=0;
    ineffectiveClicks=0;

    cout << endl;   // empty line in terminal between games


}

void Field::revealAround(int squareX, int squareY) {

    // reveal squares around a square

    if (squareX>0) {
        if (squareY>0) revealSquare(squareX-1,squareY-1);
        revealSquare(squareX-1,squareY);
        if (squareY<height-1) revealSquare(squareX-1,squareY+1);

    }

    if (squareY>0) revealSquare(squareX,squareY-1);
    if (squareY<height-1) revealSquare(squareX,squareY+1);

    if (squareX<width-1) {
        if (squareY>0) revealSquare(squareX+1,squareY-1);
        revealSquare(squareX+1,squareY);
        if (squareY<height-1) revealSquare(squareX+1,squareY+1);
    }

}

Square Field::isAroundThisSupermine(int squareX, int squareY){

    Square sqr;

    int posX = squareX;
    int posY = squareY;


    vector<Square*> vector1 = surroundingSquares(squareX, squareY, 1);

    for (vector<Square*>::iterator it = vector1.begin(); it != vector1.end(); it++)
    {


        if (allSquares[(*it)->posX][(*it)->posY].mine==2) { return **it; }

    }

    return allSquares[squareX][squareY];
}

void Field::revealSquare(int squareX, int squareY) {

    // clicked (or asked to reveal) unrevealed square

    const Square& closestSupermine = isAroundThisSupermine(squareX, squareY);


    if (allSquares[squareX][squareY].state==1 and allSquares[squareX][squareY].adjacentToSuperFlag==0) /* (state[squareX][squareY]==9 and state2[squareX][squareY]==9) */ {
            if (allSquares[squareX][squareY].mine==1) {
            hitMineX=squareX;
            hitMineY=squareY;
            timer.stop();
            replay.stopRecording();
            gameState=GAME_LOST;
            if (!playReplay) {
                endGameLost();
            }


            }else if(closestSupermine.mine==2) {
            //timeFinished=time(NULL);
            hitMineX=closestSupermine.posX;
            hitMineY=closestSupermine.posY;
            timer.stop();
            replay.stopRecording();
            gameState=GAME_LOST;
            if (!playReplay) {
                endGameLost();
            }


        }
        else {

            int adjacentMines=0;
            if (squareX>0) {
                if (squareY>0) adjacentMines+=(allSquares[squareX-1][squareY-1].mine==1)?1:0;
                adjacentMines+=(allSquares[squareX-1][squareY].mine==1)?1:0;
                if (squareY<height) adjacentMines+=(allSquares[squareX-1][squareY+1].mine==1)?1:0;

            }

            if (squareY>0) adjacentMines+=(allSquares[squareX][squareY-1].mine==1)?1:0;
            if (squareY<height-1) adjacentMines+=(allSquares[squareX][squareY+1].mine==1)?1:0;

            if (squareX<width-1) {
                if (squareY>0) adjacentMines+=(allSquares[squareX+1][squareY-1].mine==1)?1:0;
                adjacentMines+=(allSquares[squareX+1][squareY].mine==1)?1:0;
                if (squareY<height-1) adjacentMines+=(allSquares[squareX+1][squareY+1].mine==1)?1:0;
            }

            // xxx3



             int adjacentSupermines=0;
             int adjacentSupermines2=0;



                    vector<Square*> vector1 =surroundingSquares(squareX,squareY,2);

                    for (vector<Square*>::iterator it = vector1.begin(); it != vector1.end(); it++){

                        adjacentSupermines+=((**it).mine==2)?1:0 ;

                    }



            if (adjacentSupermines2!=adjacentSupermines){


//
//                    }
            };


            allSquares[squareX][squareY].state = 0;
            allSquares[squareX][squareY].surroundingMines = adjacentMines;
            allSquares[squareX][squareY].surroundingSupermines = adjacentSupermines;

            if (adjacentMines==0 and adjacentSupermines==0 ) {

                revealAround(squareX,squareY);
            }


            // test if game finished
            if (gameState==GAME_INITIALIZED or gameState==GAME_PLAYING) {
                bool finished=true;

                for (int i=0;i<width;i++)
                    for (int j=0;j<height;j++)

                        if (allSquares[i][j].state and !(allSquares[i][j].mine or allSquares[i][j].adjacentToSuperMine)){
                            finished=false;
                        }


                if (finished) {
                    endGameWon();
                }
            }
        }
    }
}

bool Field::adjacentMinesFlagged(int squareX,int squareY) {


    int flaggedAdjacentMines=0;

    if (squareX>0) {
        if (squareY>0) flaggedAdjacentMines+=allSquares[squareX-1][squareY-1].state>1?1:0;
        flaggedAdjacentMines+=allSquares[squareX-1][squareY].state>1?1:0;
        if (squareY<height) flaggedAdjacentMines+=allSquares[squareX-1][squareY+1].state>1?1:0;

    }

    if (squareY>0) flaggedAdjacentMines+=allSquares[squareX][squareY-1].state>1?1:0;
    if (squareY<height) flaggedAdjacentMines+=allSquares[squareX][squareY+1].state>1?1:0;

    if (squareX<width) {
        if (squareY>0) flaggedAdjacentMines+=allSquares[squareX+1][squareY-1].state>1?1:0;
        flaggedAdjacentMines+=allSquares[squareX+1][squareY].state>1?1:0;
        if (squareY<height) flaggedAdjacentMines+=allSquares[squareX+1][squareY+1].state>1?1:0;
    }



    if (flaggedAdjacentMines==allSquares[squareX][squareY].surroundingMines+allSquares[squareX][squareY].surroundingSupermines)
        return true;
    else
        return false;
}

int Field::calculateRemainingMines() {

    int remaining=mc.startMines;

    for (int i=0;i<width;i++)
        for (int j=0;j<width;j++)
            if (allSquares[i][j].state==2)
                remaining--;

    return remaining;
}

int Field::calculateRemainingSupermines() {

    int remaining=mc.startSupermines;



    for (int i=0;i<width;i++)
        for (int j=0;j<width;j++)
            if (allSquares[i][j].state==3)
                remaining--;

    return remaining;
}

void Field::viewClicks() {
    cout << "Clicks: "<<effectiveClicks<<" / "<<ineffectiveClicks<<endl;

    return;

    float progress=getGameProgress();

    if (progress==0) return;
    cout << "Est.:   "<<(int)(effectiveClicks/progress)<<" / "<<(int)(ineffectiveClicks/progress)<< " Time: " << (int)(timer.calculateElapsedTime()/progress)<<"  "<<(int)(progress*100)<<"%"<<endl;
}


void Field::click(int x,int y,int button) {

    int squareX=(x-FIELD_X)/squareSize;
    int squareY=(y-FIELD_Y)/squareSize;

    if (button==GLUT_LEFT_BUTTON) {

        if (allSquares[squareX][squareY].state==1 and allSquares[squareX][squareY].adjacentToSuperFlag==0) {
            if (gameState==GAME_INITIALIZED) {
                if (!playReplay)
                    placeMines(squareX,squareY);



                timer.start();
                gameState=GAME_PLAYING;


                if (!replay.isRecording()) {
                    replay.deleteData();
                    replay.startRecording();
                }
            }
            effectiveClicks++;
            viewClicks();
            replay.recordEvent(x,y,GLUT_LEFT_BUTTON);
            revealSquare(squareX,squareY);

        }
           else if (!(allSquares[squareX][squareY].state)) {
            replay.recordEvent(x,y,GLUT_LEFT_BUTTON);

              if ((allSquares[squareX][squareY].surroundingMines or allSquares[squareX][squareY].state or allSquares[squareX][squareY].surroundingSupermines)
                    and adjacentMinesFlagged(squareX,squareY)) {
                effectiveClicks++;
                viewClicks();
                revealAround(squareX,squareY);

            }
            else {
                ineffectiveClicks++;
                viewClicks();
            }
        }
        else {  // flag?
            replay.recordEvent(x,y,GLUT_LEFT_BUTTON);
            ineffectiveClicks++;
            viewClicks();
        }

    }
    else if (button==GLUT_RIGHT_BUTTON) {

        if (!replay.isRecording()) {
            replay.deleteData();
            replay.startRecording();
        }

        replay.recordEvent(x,y,GLUT_RIGHT_BUTTON);
        // toggle flag or check and reveal surrounding squares

         if (allSquares[squareX][squareY].state==1) {   // unrevealed
            allSquares[squareX][squareY].state=2;
            if (!isFlagging and !playReplay)
                cout<<"You are now playing with flagging."<<endl;

            isFlagging=true;
   //         cout << "Remaining mines: " << calculateRemainingMines() << endl;

            mc.remainingMines--;

            effectiveClicks++;
            viewClicks();
        }
        else if (allSquares[squareX][squareY].state==2) { // flag


            mc.remainingMines++;
            mc.remainingSupermines--;

            effectiveClicks++;
            viewClicks();
            allSquares[squareX][squareY].state=3;

            vector<Square*> vector1 =surroundingSquares(squareX,squareY,1);
            for (vector<Square*>::iterator it = vector1.begin(); it != vector1.end(); it++){

                (*it)->adjacentToSuperFlag=1;
            }
        }
        else if (allSquares[squareX][squareY].state==3) { // flag

            mc.remainingSupermines++;
            effectiveClicks++;
            viewClicks();
            allSquares[squareX][squareY].state=1;

            vector<Square*> vector1 =surroundingSquares(squareX,squareY,1);
            for (vector<Square*>::iterator it = vector1.begin(); it != vector1.end(); it++){

                (*it)->adjacentToSuperFlag=0;
            }

            for (vector<Square*>::iterator it = allSquaresV.begin(); it != allSquaresV.end(); it++){

                if ((*it)->state==3){
                    vector<Square*> vector1 =surroundingSquares((*it)->posX,(*it)->posY,1);
                    for (vector<Square*>::iterator it = vector1.begin(); it != vector1.end(); it++){

                        (*it)->adjacentToSuperFlag=1;
                        cout << (*it)->adjacentToSuperFlag << endl;
                    }
                }
            }

        }
        else {  // number
            if ((allSquares[squareX][squareY].surroundingMines or allSquares[squareX][squareY].state or allSquares[squareX][squareY].surroundingSupermines) and adjacentMinesFlagged(squareX,squareY)) {
                effectiveClicks++;
                viewClicks();
                revealAround(squareX,squareY);

            }
            else {
                ineffectiveClicks++;
                viewClicks();
            }
        }


    }
    else if (button==GLUT_MIDDLE_BUTTON) {
        if (allSquares[squareX][squareY].surroundingMines or allSquares[squareX][squareY].surroundingSupermines) {  // number
            replay.recordEvent(x,y,GLUT_MIDDLE_BUTTON);

            if ((allSquares[squareX][squareY].surroundingMines or allSquares[squareX][squareY].state or allSquares[squareX][squareY].surroundingSupermines) and adjacentMinesFlagged(squareX,squareY)) {
                effectiveClicks++;
                revealAround(squareX,squareY);
            }
            else {
                ineffectiveClicks++;
            }
            viewClicks();
        }
        else {
            ineffectiveClicks++;
            viewClicks();

        }
    }



}



float Field::getGameProgress() {

    int count=0;
    for (int x=0;x<width;x++)
        for (int y=0;y<height;y++)
            if ((allSquares[x][y].surroundingMines or allSquares[x][y].state or allSquares[x][y].surroundingSupermines) and adjacentMinesFlagged(x,y))
                count++;

    if (count==0) return 0;

    //cout<<count<<endl;
    return (float)count/(width*height-mc.startMines-mc.startSupermines);

}
