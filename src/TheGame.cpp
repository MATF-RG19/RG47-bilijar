#include "TheGame.h"
#include <GL/glut.h>
#include <string>

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
    switch (c){
        case 27:
            exit(1);
            break;
        case 'a':
            camTheta -= .02;
            glutPostRedisplay();
            break;
        case 'd':
            camTheta += .02;
            glutPostRedisplay();
            break;
        case 'w':
            camRho -= .02;
            glutPostRedisplay();
            break;
        case 's':
            camRho += .02;
            glutPostRedisplay();
            break;

        case 'i':
            camR -= .5;
            glutPostRedisplay();
            break;

        case 'k':
            camR += .5;
            glutPostRedisplay();
            break;
    }
}

TheGame::TheGame(){
    //prazan konsruktor
}

TheGame::TheGame(double tableLength){
    
    this->tableLength = tableLength;
    this->tableWidth = tableLength * tableRatio;
    cout << tableLength << " * " << tableRatio << " = " << tableLength * tableRatio << endl;

    //Objasnjeno u .h fajlu
    tableEdgeUp = tableLength/2;
    tableEdgeDown = - tableEdgeUp;
    tableEdgeRight = tableWidth/2;
    tableEdgeLeft = -tableEdgeRight;

    //Pocetna pozicija kamere
    camR = tableLength;
    camTheta = 1;
    camRho = 1;
    

    ballRadius = tableLength/47;

}

void TheGame::drawTable(){
    //Iscrtavanje stola (za sada samo kao obicni kvadar)
    //Eventualno ce biti dekorisan
    glColor3f(0.0, 1.0, 0);
    glPushMatrix();
    glTranslated(0, 0, tableHeight/2); //Translacija na odgovarajucu visinu

        glPushMatrix();
            glScaled(tableWidth, tableLength, tableHeight); //Skaliranje po dimenzijama
            glutSolidCube(1);
            glColor3f(0, 0, 0);
            glutWireCube(1);
        glPopMatrix();    

    glPopMatrix();
    
}

void TheGame::testBalls(){
    Ball b = Ball(Vec2(0, tableEdgeUp - 5), Vec2(0, 0), 1, 0, 0);
    Ball b1 = Ball(Vec2(0, tableEdgeUp - 10), Vec2(0, 0), 0, 0, 1);
    Ball b2 = Ball(Vec2(0, tableEdgeUp - 15), Vec2(0, 0), 1, 1, 0);

    glPushMatrix();
        glTranslated(0, 0, tableHeight + ballRadius/2);
        glScaled(ballRadius, ballRadius, ballRadius);
        b.drawSelf();
        b1.drawSelf();
        b2.drawSelf();
    glPopMatrix();
}


void TheGame::directCamera(){
    //Usmeravanje kamere na osnovu sfernih koordinata 
    //Centar sfere nije centar koordinatnog sistema vec centar stola, koji ima koordinate (0, 0, tableHeight). Zbog toga se tableHeight dodaje na poslednju koordinatu.
    gluLookAt(
        camR*cos(camTheta)*sin(camRho), camR*sin(camTheta)*sin(camRho), camR*cos(camRho) + tableHeight,
        0, 0, 0,
        0, 0, 1
    );
}
