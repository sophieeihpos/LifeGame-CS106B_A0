
/*Sophie Lawrence practice c++ assignment of Stanford SC106B.

/*Overview: The Game of Life is a simulation originally conceived by the British mathematician J. H. Conway in 1970 and popularized by Martin Gardner in his Scientific American column. The game models the life cycle of bacteria using a two-dimensional grid of cells. Given an initial pattern, the game simulates the birth and death of future generations of cells using a set of simple rules. In this assignment you will implement a simplified version of Conway's simulation and a basic user interface for watching the bacteria grow over time.*/

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;

int numRow;
int numCol;
Grid<bool> currentGrid;

string fileName;
int numGenerations;
int numMiliseconds;
bool ifWrap;
void setFromConsol(){
    cout<<\
           "Welcome to the CS 106B Game of Life, \n\
           a simulation of the lifecycle of a bacteria colony.\n\
           Cells (X) live and die by the following rules:\n\
           - A cell with 1 or fewer neighbors dies.\n\
           - Locations with 2 neighbors remain stable.\n\
           - Locations with 3 neighbors will create life.\n\
           - A cell with 4 or more neighbors dies."<<endl;
           cout<< "Grid input file path? (Enter R for random start)"<<endl;
    getline(cin,fileName);
    if(fileName!="r" && fileName!="R"){
        replace(fileName.begin(),fileName.end(),'\\','/');
    }else{
        fileName=_strlib_h::toLowerCase(fileName);
    }

    cout<<"How many generations in total?"<<endl;
    cin>>numGenerations;
    cout<<"How many miliseconds per generation?"<<endl;
    cin>>numMiliseconds;
    string wrap="";
    while (wrap!="y"&&wrap!="n"){
        cout<<"Simulation wrap around grid (y/n e.g. if the top bound sees bottom bound as its neigbour)?"<<endl;
        cin>>wrap;
        _strlib_h::toLowerCase(wrap);
        if (wrap=="y"){
            ifWrap=1;
        }else if(wrap=="n"){
            ifWrap=0;
        }
    }
}

void randomStartGrid(){
    if(fileName=="r"){
        numRow=randomInteger(1,100);
        numCol=randomInteger(1,100);
        currentGrid.resize(numRow,numCol);
        for (int r=0;r<numRow;r++){
            for(int c=0;c<numCol;c++){
                currentGrid[r][c]=randomBool();
            }
        }
    }
}

void loadFile(){
    if(fileName!="r"){
        ifstream inFileStream;
        inFileStream.open(fileName.c_str());
        if(inFileStream.fail()){
            cout<<"Could not open file."<<endl;
            return;
        }else{
            string line;
            getline(inFileStream,line);
            numRow =stoi(line);
            getline(inFileStream,line);
            numCol=stoi(line);
            currentGrid.resize(numRow,numCol);
            for (int r =0;r<numRow;r++){
                getline(inFileStream,line);
                if(inFileStream.fail()){
                    cout<<"Specified number of rows does not match graph."<<endl;
                    return;
                }else{
                    for(int c=0;c<numCol;c++){
                        if(c<line.length()){
                            if(line[c]=='-'){
                                currentGrid[r][c]=0;
                            }else{
                                currentGrid[r][c]=1;
                            }
                        }else{
                            cout<<"Specified number of columns does not match graph."<<endl;
                            return;
                        }
                    }
                }
            }
            inFileStream.close();
        }
    }
}

int getNeibours(int r,int c){

    int count=0;

    for(int i=r-1;i<=r+1;i++){
        for (int j=c-1;j<=c+1;j++){

            if(currentGrid.get((i+numRow)%numRow,(j+numCol)%numCol)==1){
                count++;
            }


        }
    }
    if(currentGrid.get(r,c)==1){
        count--;
    }
    return count;

}

bool decideAlive(int r, int c, int count){
    bool current= currentGrid.get(r,c);
    bool ifAlive=current;
    if (count==0 || count==1){
        ifAlive=0;
    }else if (count==2){
        ifAlive=current;
    }else if(count==3){
        ifAlive=1;
    }else if (count>=4){
        ifAlive=0;
    }
    return ifAlive;
}

void marchForward(){

    if(currentGrid.numCols()==numCol && currentGrid.numRows()==numRow){
        LifeGUI gui;
        gui.resize(numRow,numCol);

        Grid<bool> nextGrid(numRow,numCol);
        for (int i = 0;i<=numGenerations;i++){
            for (int r=0; r<numRow;r++){
                for(int c=0;c<numCol;c++){
                    gui.drawCell(r,c,currentGrid.get(r,c));
                    if(decideAlive(r,c,getNeibours(r,c))==0){
                        nextGrid[r][c]=0;
                    }else{
                        nextGrid[r][c]=1;
                    }
                }
            }

            currentGrid=nextGrid;
            pause(numMiliseconds);

        }
        cout<<"Bacteria simulation completed."<<endl;
    }else{
        return;
    }
}


int main() {
    setFromConsol();
    loadFile();
    randomStartGrid();
    marchForward();
    return 0;
}

