#include "putils.h"
#include "Vec2.h"
#include <GL/glut.h>
#include <math.h>

#include <iostream>
using namespace std;
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

bool withinBoundsSimple(double x, double * bounds){
    return (x >= bounds[0] && x <= bounds[1]);
}


void drawCircle(double radius){
    double t = 0;
    while(t < 2*M_PI){
        glBegin(GL_TRIANGLE_STRIP);
            glVertex2f(0, 0);
            glVertex2f(radius*cos(t), radius*sin(t));
            glVertex2f(radius*cos(t + 0.2), radius*sin(t + 0.2));
        glBegin(GL_TRIANGLE_STRIP);
        t += 0.2;
    }
    glEnd();
}

void drawEllipse(double a, double b, double t_from, double t_to){
    double deltaT = 0.2;
    while(t_from < t_to){
        glBegin(GL_TRIANGLE_STRIP);
            glVertex2f(0, 0);
            glVertex2f(a*cos(t_from), b*sin(t_from));
            glVertex2f(a*cos(t_from + deltaT), b*sin(t_from + deltaT));
        glEnd();
        t_from += deltaT;
    }
}

bool circleDrop(double bigX, double bigY, double bigRadius, double smallX, double smallY){
    Vec2 distVec = Vec2(bigX - smallX, bigY - smallY);
    double nrm = distVec.mag();
    //cout << "Distance from ball " << smallX << ", " << smallY << " and hole " << bigX << ", " << bigY << " is " << nrm << endl;

    return nrm <= bigRadius;
}