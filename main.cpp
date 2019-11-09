#include <iostream>
#include <math.h>

#include <GL/glut.h>

#include "Vec2.h"
#include "Ball.h"

using namespace std;

static void on_display(void);
static void on_reshape(int, int);
static void on_keyboard(unsigned char key,int x, int y);

int main(int argc, char ** argv){
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

}

static void on_reshape(int x, int y){

}

static void on_keyboard(unsigned char key,int x, int y){

}