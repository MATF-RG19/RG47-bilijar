#include <iostream>
#include <math.h>
#include <GL/glut.h>


#include "Vec2.h"
#include "Ball.h"
#include "TheGame.h"
using namespace std;

static void on_display(void);
static void on_reshape(int, int);
static void on_keyboard(unsigned char key,int x, int y);

TheGame g; //Upravljacki objekat igrice. Pogledati TheGame.h

//Funkcija koja crta koordinatni sistem, sluzi kao pomoc pri debagovanju.
void drawCoord(){
    glLineWidth(1);
    glColor3f(0, 0, 1.0);
    //X AXIS IS BLUE
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(100.0, 0, 0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-100.0, 0, 0);
    glEnd();
    glColor3f(0, 1.0, 0);
    //Y AXIS IS GREEN
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 100.0, 0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -100.0, 0);
    glEnd();

    glColor3f(1.0, 0, 0);
    //Z AXIS IS RED
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 100.0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -100.0);
    glEnd();
}

int main(int argc, char ** argv){

    g = TheGame(50);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutGet(GLUT_SCREEN_HEIGHT);
    glutGet(GLUT_SCREEN_WIDTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(200, 200);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);

    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);

    glLineWidth(2);

    glutMainLoop();

    

    return 0;
}

static void on_display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 800); //Dalji clipping-plane je za sada vrlo daleko zbog cestih promena dimenzija objekata radi testiranja. Bice racunat na osnovu dimenzija

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    
    
    g.directCamera();   ///usmeravanje kamere
    g.drawTable();      //crtanje stola
    g.testBalls();      //test iscrtavanja kugli, privremeno
    drawCoord();        //crtanje koordinatnog sistema
    

    glutSwapBuffers();
}

static void on_reshape(int x, int y){
    
}

static void on_keyboard(unsigned char key,int x, int y){
    g.handleKeyboard(key);
}