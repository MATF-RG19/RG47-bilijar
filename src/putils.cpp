#include "putils.h"
#include "Vec2.h"
#include <GL/glut.h>
#include <math.h>


#define PI 3.1415926535
#define EPSILON 0.01

#include <iostream>
using namespace std;

double pistep = PI/20;
double twopi = 2*PI;

/**
     * Malo cudnovata funkcija, za vrlo specificnu potrebu
     * Pravi onaj efekat 'treperenje' ukoliko korisnik proba da zumira ili da pomeri kameru uvis/dole previse.
 * **/
bool withinBounds(double * x, double * bounds){
    
    if (*x <= bounds[0]){
        *x = bounds[0] + .02;
        return false;
    }
    if(*x >= bounds[1]){
        *x = bounds[1] - .02;
        return false;
    }
    return true;
}
//Proverava da li je vrednost unutar svojih granica. bounds[0] je donja, bounds[1] je gornja
bool withinBoundsSimple(double x, double * bounds){
    return (x >= bounds[0] && x <= bounds[1]);

}
//Cratnje kruga trigonometrijskom parametrizacijom
void drawCircle(double radius){
    double t = 0;
    while(t < twopi){
        glBegin(GL_TRIANGLE_STRIP);
            glVertex2f(0, 0);
            glVertex2f(radius*cos(t), radius*sin(t));
            glVertex2f(radius*cos(t + 0.2), radius*sin(t + 0.2));
        glBegin(GL_TRIANGLE_STRIP);
        t += 0.2;
    }
    glEnd();
}
//Proverava da li se centar jednog kruga nalazi u centru drugog kruga
bool circleDrop(double bigX, double bigY, double bigRadius, double smallX, double smallY){
    Vec2 distVec = Vec2(bigX - smallX, bigY - smallY);
    double nrm = distVec.mag();
    return nrm <= bigRadius;
}
//Postavlja normale za cilindar
inline void set_normal_and_vertex(float u, float v){
    glNormal3f(
            sin(v),
            cos(v),
            0
            );
    glVertex3f(
            sin(v),
            cos(v),
            u
        );
}
//Crta cilindar ili polucilindar
void draw_cylinder(double height,  bool half){
    
    float u, v;

    float lim = half?PI:twopi;

    for (u = 0; u < height; u +=pistep) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = 0; v <= lim + EPSILON; v +=pistep) {
            set_normal_and_vertex(u, v);
            set_normal_and_vertex(u + pistep, v);
        }
        glEnd();
    }

}
//Crta deo cilindra od ugla angleStart do angleEnd
void draw_cylinder2(double height, double angleStart, double angleEnd, bool half){
    
    float u, v;

    float lim = half?PI:twopi;
    for (u = 0; u < height; u +=pistep) {
        glBegin(GL_TRIANGLE_STRIP);
        for (v = angleStart; v <= angleEnd + EPSILON; v +=pistep) {
            set_normal_and_vertex(u, v);
            set_normal_and_vertex(u + pistep, v);
        }
        glEnd();
    }
}