#include "Vec2.h"
#include <GL/glut.h>

#ifndef BALL_H
#define BALL_H
class Ball{

    private:

        static const unsigned int bitCheck = pow(2, 16) - 1;

        //Vektori koji predstavljaju poziciju u brzinu    
        Vec2 position;
        Vec2 velocity;

        double radius;

        bool onTable;

        //RGB kod boje
        double r;
        double g;
        double b;

        unsigned int bitMask;

        //Pomocna metoda za inicijalizaciju
        void setAll(Vec2 pos, Vec2 vel, double radius, double r, double g, double b, int i){
            counter++;
            this->position = pos;
            this->velocity = vel;
            this->r = r;
            this->g = g;
            this->b = b;
            this->onTable = true;
            this->radius = radius;
            moving = (vel.mag() > 0);
        }

        bool moving;
    public:
        Ball();
        Ball(int);
        Ball(Vec2, int);
        Ball(Vec2, Vec2, int);
        Ball(Vec2, Vec2, double, double, double, double, int);

        //Getteri i setteri
        Vec2 getPosition();
        void setPosition(Vec2);

        Vec2 getVelocity();
        void setVelocity(Vec2);

        //Metoda koja iscrtava loptu na sceni
        void drawSelf();

        //Metoda koja regulise koliziju sa drugom loptom
        void collideWith(Ball);    

        string toString(); 

        static int counter;

        bool isActive();

        bool getMoving();

        
};

#endif