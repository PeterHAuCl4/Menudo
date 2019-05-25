#include "API.h"
#include<math.h>
#define n 10
#define mid 4
struct maze{
    int val;
    bool visited , north , west , south , east , locked;
};
int posX = 0, posY = 0 , offx = 0 , offy = 0 , value = 255;
maze mat[n][n];
char heading = 'n' , dir = 'f';

void fwdOffSet(char);
void leftOffSet(char);
void rightOffSet(char);
void backOffSet(char);
void fwd(char,char);
void updateWalls(char, char);
void updateThisCell();
void showValues();
void isLocked(char);
void lock();
/*==================================================================================================================*/
int main() {
    int ai = n-2, bi = 0 , ci = 0;
    for(int i = 0; i < n/2; ++i){ //izquierda abajo
        for(int j = 0; j < n/2; ++j){
        mat[i][j].val = ai-j;
        }
        ai--;
    }
    ai++;
    for(int i = n/2; i < n; ++i){ //derecha abajo
        for(int j = 0; j < n/2; ++j){
        mat[i][j].val = ai-j;
        }
        ai++;
    }
    for(int i = 0; i < n/2; ++i){ //arriba izquierda
        for(int j = n-1; j >= n/2; --j){
            mat[i][j].val = j-bi-1;
        }
        bi++;
    }
    for(int i = n/2; i < n; ++i){
        for(int j = n/2;j < n; ++j){
            mat[i][j].val = ci;
            ci++;
        }
        ci-=mid;
    }
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++){
            mat[i][j].visited = false;
            mat[i][j].north = false;
            mat[i][j].west = false;
            mat[i][j].south = false;
            mat[i][j].east = false;
            mat[i][j].locked = false;
        }
    int aux = 0;
    API::setWall(0,0,'s');
    int numl = 0, numr = 0, numf = 0;
    bool gof = false, gol = false, gor = false , lockF = false , lockL = false , lockR = false;
    std::string ssa;

    while (true) {
aux = 0;
        if(API::wallFront())
            aux+=1;
        if(API::wallLeft())
            aux+=2;
        if(API::wallRight())
            aux+=4;

        switch (aux) {
        case 0:
            fwdOffSet(heading);
            numf = mat[posX+offx][posY+offy].val;
            gof = mat[posX+offx][posY+offy].visited;
            lockF = mat[posX+offx][posY+offy].locked;

            leftOffSet(heading);
            numl = mat[posX+offx][posY+offy].val;
            gol = mat[posX+offx][posY+offy].visited;
            lockL = mat[posX+offx][posY+offy].locked;

            rightOffSet(heading);
            numr = mat[posX+offx][posY+offy].val;
            gor = mat[posX+offx][posY+offy].visited;
            lockR = mat[posX+offx][posY+offy].locked;

            if(lockF){
                if(lockL){
                    dir = 'r'; API::turnRight(); fwd(dir, heading); //Tapado al frente e izquierda
                    if(lockR){
                        dir = 'b'; lock(); API::turnLeft(); API::turnLeft(); fwd(dir, heading); //Si todo está tapado
                    }
                }else{
                    dir = 'l'; API::turnLeft(); fwd(dir, heading);
                }
            }else if(lockR){
                if(lockL){
                    dir = 'f'; fwd(dir, heading);
                }
                dir = 'l'; API::turnLeft(); fwd(dir, heading);
            }else if (lockL) {
                    dir = 'f'; fwd(dir, heading);
            }else {
                if(!gof && !gol && !gor){ //Si ninguno ha sido visitado

                    if(numf<= numl){
                        if(numf <= numr){
                           dir = 'f'; fwd(dir, heading);
                        }else if(numl < numr){
                          API::setColor(posX,posY,'Y');
                            dir = 'l'; API::turnLeft(); fwd(dir, heading);
                        }else if(numr <numf){
                            dir = 'r'; API::turnRight(); fwd(dir, heading);
                        }
                    }else if (numf < numr) {
                            dir = 'f'; fwd(dir, heading);
                    }else{
                        dir = 'l'; API::turnLeft(); fwd(dir, heading);
                    }
                }else{
                    if(numf<= numl){
                        if(numl <= numr){
                            API::setColor(posX,posY,'Y');
                            dir = 'l'; API::turnLeft(); fwd(dir, heading);
                        }else {
                            dir = 'f'; fwd(dir, heading);
                        }
                    }else if (numf < numr) {
                            dir = 'f'; fwd(dir, heading);
                    }else{
                        dir = 'l'; API::turnLeft(); fwd(dir, heading);
                    }
                }
            }
isLocked(heading); break;
        case 1: //dir = bestPlace('l','r',heading);
//            ssa+=dir;
//            API::setText(posX+3,posY,ssa);
            leftOffSet(heading);
            numl = mat[posX+offx][posY+offy].val;
            gol = mat[posX+offx][posY+offy].visited;
            lockL = mat[posX+offx][posY+offy].locked;
            
            rightOffSet(heading);
            numr = mat[posX+offx][posY+offy].val;
            gor = mat[posX+offx][posY+offy].visited;
            lockR = mat[posX+offx][posY+offy].locked;

            updateWalls(dir, heading);

            if(lockL){
                if(lockR){
                    dir = 'b'; lock(); API::turnLeft(); API::turnLeft(); fwd(dir, heading); //Si todo está tapado
                }else{
                    dir = 'r';
                    API::turnRight(); fwd(dir, heading);
                }
            }else if(lockR){
                dir = 'l';
                API::turnLeft(); fwd(dir, heading);
            }else{
                if(!gor && !gol){ //Si ninguno ha sido visitado
                    if(numl <= numr){
                        dir = 'l';
                        API::turnLeft(); fwd(dir, heading);
                    }else{
                        dir = 'r';
                        API::turnRight(); fwd(dir, heading);
                    }
                }else if(!gor && gol){
                    dir = 'r';
                    API::turnRight(); fwd(dir, heading);
                }else if(gor && !gol){
                    dir = 'l';
                    API::turnLeft(); fwd(dir, heading);
                }else{
                    if(numl <= numr){
                        dir = 'l';
                        API::turnLeft(); fwd(dir, heading);
                    }else{
                        dir = 'r';
                        API::turnRight(); fwd(dir, heading);
                    }
                }
            }
isLocked(heading); break;

        case 2:  //Front, right
            fwdOffSet(heading);
            numf = mat[posX+offx][posY+offy].val;
            gof = mat[posX+offx][posY+offy].visited;
            lockF = mat[posX+offx][posY+offy].locked;

            rightOffSet(heading);
            numr = mat[posX+offx][posY+offy].val;
            gor = mat[posX+offx][posY+offy].visited;
            lockR = mat[posX+offx][posY+offy].locked;
            updateWalls(dir, heading);
            
            if(lockF){
                if(lockR){
                     dir = 'b'; lock(); API::turnLeft(); API::turnLeft(); fwd(dir, heading);
                }else{
                    dir = 'r';
                    API::turnRight(); fwd(dir, heading);
                }
            }else if(lockR){
                dir = 'f';
                fwd(dir, heading);
            }else{
                if(!gof && !gor){ //Si ninguno ha sido visitado
                    if(numf <= numr){
                        dir = 'f';
                        fwd(dir, heading);
                    }else{
                        dir = 'r';
                        API::turnRight(); fwd(dir, heading);
                    }
                }else if(!gor && gof){
                    dir = 'r';
                    API::turnRight(); fwd(dir, heading);
                }else if(gor && !gof){
                    dir = 'f';
                    fwd(dir, heading);
                }else{
                    if(numf <= numr){
                        dir = 'f';
                        fwd(dir, heading);
                    }else{
                        dir = 'r';
                        API::turnRight(); fwd(dir, heading);
                    }
                }
            }
           isLocked(heading); break;

        case 3: //Only turn right
            rightOffSet(heading);
            if(mat[posX+offx][posY+offy].locked){
                dir = 'b';
                updateWalls(dir, heading); lock(); API::turnLeft(); API::turnLeft(); fwd(dir, heading);
            }else{
                dir = 'r';
                updateWalls(dir, heading); API::turnRight(); fwd(dir, heading);
            }
            isLocked(heading); break;
        case 4://front left

            fwdOffSet(heading);
            numf = mat[posX+offx][posY+offy].val;
            gof = mat[posX+offx][posY+offy].visited;
            lockF = mat[posX+offx][posY+offy].locked;
            
            leftOffSet(heading);
            numl = mat[posX+offx][posY+offy].val;
            gol = mat[posX+offx][posY+offy].visited;
            lockL = mat[posX+offx][posY+offy].locked;
            updateWalls(dir, heading);
            
            if(lockF){
                if(lockL){
                     dir = 'b'; lock(); API::turnLeft(); API::turnLeft(); fwd(dir, heading);
                }else {
                    dir = 'l';
                    API::turnLeft(); fwd(dir, heading);
                }
            }else if (lockL) {
                dir = 'f';
                fwd(dir, heading);
            }else{
                if(!gof && !gol){ //Si ninguno ha sido visitado
                    if(numf <= numl){
                        dir = 'f';
                        fwd(dir, heading);
                    }else{
                        dir = 'l';
                        API::turnLeft(); fwd(dir, heading);
                    }
                }else if(!gol && gof){
                    dir = 'l';
                    API::turnLeft(); fwd(dir, heading);
                }else if(gol  && !gof){
                    dir = 'f';
                    fwd(dir, heading);
                }else{
                    if(numf <= numr){
                        dir = 'f';
                        fwd(dir, heading);
                    }else{
                        dir = 'l';
                        API::turnLeft(); fwd(dir, heading);
                    }
                }
            }
           isLocked(heading); break;

        case 5: //Only turn left
            leftOffSet(heading);
            if(mat[posX+offx][posY+offy].locked){
                dir = 'b';
                updateWalls(dir, heading); lock(); API::turnLeft(); API::turnLeft(); fwd(dir, heading);
            }else{
                dir = 'l';
                updateWalls(dir, heading); API::turnLeft(); fwd(dir, heading);
            }
isLocked(heading); break;
        case 6: //Front is Clear
            fwdOffSet(heading);
            if(mat[posX+offx][posY+offy].locked){
                dir = 'b';
                updateWalls(dir, heading); lock(); API::turnLeft(); API::turnLeft(); fwd(dir, heading);
            }else{
                dir = 'f'; isLocked(heading);
                updateWalls(dir, heading); fwd(dir, heading);
            }
isLocked(heading); break;

        case 7: //Back is clear
            dir = 'b';
            updateWalls(dir, heading); lock(); API::turnLeft(); API::turnLeft(); fwd(dir, heading); break;
        }
        aux = 0; numl = 0; numr = 0; numf = 0;
//        if((posX == 4 || posX == 5) && (posY == 4 || posY == 5)){
//        API::setColor(4,4,'g');
//        API::setColor(4,5,'g');
//        API::setColor(5,4,'g');
//        API::setColor(5,5,'g');

        if((posX == mid || posX == mid+1) && (posY == mid || posY == mid+1)){
            API::setColor(mid,mid,'g');
            API::setColor(mid,mid+1,'g');
            API::setColor(mid+1,mid,'g');
            API::setColor(mid+1,mid+1,'g');
            break;
        }
    }
}
/*====================================================================================================*/
void lock(){
    mat[posX][posY].locked = true;
    API::setColor(posX,posY,'R');
}
void isLocked(char head){

    bool way = false;
    int auxiX = 0 , auxiY = 0;
        switch (head) {
        case 'n': auxiY = -1; break;
        case 'w': auxiX = 1; break;
        case 's': auxiY = 1; break;
        case 'e': auxiX = -1; break;
        }
        if(!API::wallFront() && API::wallLeft() && API::wallRight())
            way = true;
        if(mat[posX+auxiX][posY+auxiY].locked == true && way){
            mat[posX][posY].locked = true;
            API::setColor(posX,posY,'R');
        }

}
void updateWalls(char dire, char head){
    switch (head) {
    case 'n':
        switch(dire){
        case 'f':
            if(API::wallLeft()){
                API::setWall(posX,posY,'w'); mat[posX][posY].west = true;
            }if(API::wallRight()){
                API::setWall(posX,posY,'e'); mat[posX][posY].east = true;
            }if(API::wallFront()){
                API::setWall(posX,posY,'n'); mat[posX][posY].north = true;
            }break;
        case 'r':
            if(API::wallFront()){
                API::setWall(posX,posY,'n'); mat[posX][posY].north = true;
            }if(API::wallLeft()){
                API::setWall(posX, posY,'w');mat[posX][posY].west = true;
            }break;
        case 'l':
            if(API::wallFront()){
                API::setWall(posX,posY,'n'); mat[posX][posY].north = true;
            }if(API::wallRight()){
                API::setWall(posX, posY,'e'); mat[posX][posY].east = true;
            }break;
        case 'b':
            API::setWall(posX,posY,'n'); mat[posX][posY].north = true;
            API::setWall(posX,posY,'w'); mat[posX][posY].west = true;
            API::setWall(posX,posY,'e'); mat[posX][posY].east = true;
            break;
        } break;
    case 'w':
        switch (dire) {
        case 'f':
            if(API::wallLeft()){
                API::setWall(posX,posY,'s'); mat[posX][posY].south = true;
            }if(API::wallRight()){
                API::setWall(posX,posY,'n'); mat[posX][posY].north = true;
            }if(API::wallFront()){
                API::setWall(posX,posY,'w'); mat[posX][posY].west = true;
            }break;
        case 'r':
            if(API::wallFront()){
                API::setWall(posX,posY,'w'); mat[posX][posY].west = true;
            }if(API::wallLeft()){
                API::setWall(posX, posY,'s'); mat[posX][posY].south = true;
            }break;
        case 'l':
            if(API::wallFront()){
                API::setWall(posX,posY,'w'); mat[posX][posY].west = true;
            }if(API::wallRight()){
                API::setWall(posX, posY,'n'); mat[posX][posY].north = true;
            }break;
        case 'b':
            API::setWall(posX,posY,'n'); mat[posX][posY].north = true;
            API::setWall(posX,posY,'w'); mat[posX][posY].west = true;
            API::setWall(posX,posY,'s'); mat[posX][posY].south = true;
            break;
        } break;
    case 's':
        switch (dire) {
        case 'f':
            if(API::wallLeft()){
                API::setWall(posX,posY,'e'); mat[posX][posY].east = true;
            }if(API::wallRight()){
                API::setWall(posX,posY,'w'); mat[posX][posY].west = true;
            }if(API::wallFront()){
                API::setWall(posX,posY,'s'); mat[posX][posY].south = true;
                }break;
        case 'r':
            if(API::wallFront()){
                API::setWall(posX,posY,'s'); mat[posX][posY].south = true;
            }if(API::wallLeft()){
                API::setWall(posX, posY,'e'); mat[posX][posY].east = true;
            }break;
        case 'l':
            if(API::wallFront()){
                API::setWall(posX,posY,'s'); mat[posX][posY].south = true;
            }if(API::wallRight()){
                API::setWall(posX, posY,'w'); mat[posX][posY].west = true;
            }break;
        case 'b':
            API::setWall(posX,posY,'s'); mat[posX][posY].south = true;
            API::setWall(posX,posY,'w'); mat[posX][posY].west = true;
            API::setWall(posX,posY,'e'); mat[posX][posY].east = true;
            break;
        } break;
    case 'e':
        switch (dire) {
        case 'f':
            if(API::wallLeft()){
                API::setWall(posX,posY,'n'); mat[posX][posY].north = true;
            }if(API::wallRight()){
                API::setWall(posX,posY,'s'); mat[posX][posY].south = true;
            }if(API::wallFront()){
                API::setWall(posX,posY,'e'); mat[posX][posY].east = true;
            }break;
        case 'r':
            if(API::wallFront()){
                API::setWall(posX,posY,'e'); mat[posX][posY].east = true;
            }if(API::wallLeft()){
                API::setWall(posX, posY,'n'); mat[posX][posY].north = true;
            }break;
        case 'l':
            if(API::wallFront()){
                API::setWall(posX,posY,'e'); mat[posX][posY].east = true;
            }if(API::wallRight()){
                API::setWall(posX, posY,'s'); mat[posX][posY].south = true;
            }break;
        case 'b':
            API::setWall(posX,posY,'n'); mat[posX][posY].north = true;
            API::setWall(posX,posY,'s'); mat[posX][posY].south = true;
            API::setWall(posX,posY,'e'); mat[posX][posY].east = true;
            break;
        } break;
    }
}
void fwd(char dire, char head){

    //mat[posX][posY].val = value;
    updateThisCell();
    showValues();
    switch (dire) {
    case 'f':
        switch (head) {
        case 'n': posY++; heading = 'n'; dir = 'f'; break;
        case 'w': posX--; heading = 'w'; dir = 'f'; break;
        case 's': posY--; heading = 's'; dir = 'f'; break;
        case 'e': posX++; heading = 'e'; dir = 'f'; break;
        } break;
    case 'r':
        switch (head) {
        case 'n': posX++; heading = 'e'; dir = 'f'; break;
        case 'w': posY++; heading = 'n'; dir = 'f'; break;
        case 's': posX--; heading = 'w'; dir = 'f'; break;
        case 'e': posY--; heading = 's'; dir = 'f'; break;
        } break;
    case 'l':
        switch (head) {
        case 'n': posX--; heading = 'w'; dir = 'f'; break;
        case 'w': posY--; heading = 's'; dir = 'f'; break;
        case 's': posX++; heading = 'e'; dir = 'f'; break;
        case 'e': posY++; heading = 'n'; dir = 'f'; break;
        } break;
    case 'b':
        switch (head) {
        case 'n': posY--; heading = 's'; dir = 'f'; break;
        case 'w': posX++; heading = 'e'; dir = 'f'; break;
        case 's': posY++; heading = 'n'; dir = 'f'; break;
        case 'e': posX--; heading = 'w'; dir = 'f'; break;
        } break;
    }
API::moveForward();
}
void showValues(){
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j){
          std::string resul = std::to_string(mat[i][j].val);
          API::setText(i, j, resul);
       }
}
void updateThisCell(){
    mat[posX][posY].visited = true;
}
void fwdOffSet(char heading){
    offx = 0; offy = 0;
    switch(heading){
        case 'n': offy = 1; break;
        case 'w': offx = -1; break;
        case 's': offy = -1;break;
        case 'e': offx = 1; break;
    }
}
void leftOffSet(char heading){
offx = 0; offy = 0;
switch(heading){
    case 'n': offx = -1; break;
    case 'w': offy = -1; break;
    case 's': offx = 1;break;
    case 'e': offy = 1; break;
    }
}
void rightOffSet(char heading){
    offx = 0; offy = 0;
    switch(heading){
        case 'n': offx = 1; break;
        case 'w': offy = 1; break;
        case 's': offx = -1;break;
        case 'e': offy = -1; break;
    }
}
void backOffSet(char heading){
offx = 0; offy = 0;
    switch(heading){
        case 'n': offy = -1; break;
        case 'w': offx = 1; break;
        case 's': offy = 1;break;
        case 'e': offx = -1; break;
    }
}
