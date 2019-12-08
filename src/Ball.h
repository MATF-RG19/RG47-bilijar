#include "Vec2.h"
#include <GL/glut.h>
#include <bitset>
#include <math.h>

using namespace std;
#ifndef BALL_H
#define BALL_H
class Ball{

    private:

        //Vektori koji predstavljaju poziciju u brzinu    
        Vec2 position;
        Vec2 velocity;

        double radius;

        bool onTable;

        //RGB kod boje
        GLfloat r;
        GLfloat g;
        GLfloat b;

        unsigned int bmaskTurnOn;
        unsigned int bmaskTurnOff;
        //Pomocna metoda za inicijalizaciju
        void setAll(Vec2 pos, Vec2 vel, double radius, GLfloat r, GLfloat g, GLfloat b, int i){
            this->position = pos;
            this->velocity = vel;
            this->r = r;
            this->g = g;
            this->b = b;
            this->onTable = true;
            this->radius = radius;
            
            bmaskTurnOn = 1 << (i);
            bmaskTurnOff = bmaskTurnOn ^ (-1);
        }
        
    public:
        Ball();
        Ball(int);
        Ball(Vec2, int);
        Ball(Vec2, Vec2, int);
        Ball(Vec2, Vec2, double, GLfloat, GLfloat, GLfloat, int);

        //Getteri i setteri
        Vec2 getPosition();
        void setPosition(Vec2);

        Vec2 getVelocity();
        void setVelocity(Vec2);

        //Pametne metode:
        void drawSelf();
        void updateSelf(unsigned int *);
        void cushionCollide(double, double, double, double);
        void collideWith(Ball &);    
        bool pocketCollide(double limUp, double limDown, double limLeft, double limRight, double pocketRadius);
        
        

        string toString(); 
        bool isActive();
        bool getMoving();
        void setMoving(bool);
        unsigned int getBitMaskTurnOn();
        bool getOnTable();
        bool setOnTable(bool);

        void dieCompletely(unsigned int *);


        
        
};

#endif