#include "TheGame.h"
#include <GL/glut.h>
#include <string>
#include<bits/stdc++.h> 




// u .h fajlu
inline void TheGame::sphereToDecartes(double r, double theta, double rho, double *x, double *y, double *z){
    *x = r*cos(theta)*sin(rho);
    *y = r*sin(theta)*sin(rho);
    *z = r*cos(rho);
}

void TheGame::handleKeyboard(char c){
    //Pri pritiskanju tastera WASD ili IK menjaju se odgovarajuce komponente sfernih koordinata kako bi se mogla kontrolisati kamera
    //W i S kontrolisu azimutni ugao
    //A i D kontrolisu polarni ugao
    //I i K kontrolisu poluprecnik
    //Za sada se promene vrse preko fiksnih vrednosti (pogledati deltaR, deltaRho, deltaTheta iz TheGame.h fajla)
    
}

TheGame::TheGame(){
    //prazan konsruktor
}








string TheGame::utilGetBits(unsigned int n){
    string s = "";

    while (n != 0){
        if (n%2 == 0) s += "0";
        else s += "1";
    }

    std::reverse(s.begin(), s.end());
    return s;


}

Vec2 TheGame::getViewDirection(){
    Vec2 v(lookingAtX - lookingFromX, lookingAtY - lookingFromY);
    v.normalize();

    return v;
}


static void TheGame::timerCallBack(int arg){
    glutPostRedisplay();
    if (arg == 0){
        cout << "Animating" << endl;
        glutTimerFunc(20, timerCallBack, arg);
    }
}