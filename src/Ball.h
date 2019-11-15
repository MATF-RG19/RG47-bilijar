#include "Vec2.h"
#include <GL/glut.h>

#ifndef BALL_H
#define BALL_H
class Ball{

    private:

        //Vektori koji predstavljaju poziciju u brzinu    
        Vec2 position;
        Vec2 velocity;

        bool onTable;

        //RGB kod boje
        double r;
        double g;
        double b;


        //Pomocna metoda za inicijalizaciju
        void setAll(Vec2 pos, Vec2 vel, double r, double g, double b){
            this->position = pos;
            this->velocity = vel;
            this->r = r;
            this->g = g;
            this->b = b;
            this->onTable = true;
        }

    public:
        Ball();
        Ball(Vec2);
        Ball(Vec2, Vec2);
        Ball(Vec2, Vec2, double, double, double);

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
};

#endif