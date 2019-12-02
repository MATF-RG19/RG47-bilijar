#include "Vec2.h"
#include <GL/glut.h>
#include <bitset>
#include <math.h>

using namespace std;
#ifndef BALL_H
#define BALL_H
class Ball{

    private:

        int stupidCounter;
        int stupidIndex;

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
            counter++;
            this->position = pos;
            this->velocity = vel;
            this->r = r;
            this->g = g;
            this->b = b;
            this->onTable = true;
            this->radius = radius;
            
            stupidCounter = 0;
            stupidIndex = i;

            bmaskTurnOn = 1 << (i);
            bmaskTurnOff = bmaskTurnOn ^ (-1);
            //cout << "Index " << i << ": " << endl;
            //bitset<32>bis(bmaskTurnOn);
            //bitset<32>bis2(bmaskTurnOff);
            //cout << "\tON: " << bis << endl;
            //cout << "\tOF: " << bis2 << endl;
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

        //Metoda koja iscrtava loptu na sceni
        void drawSelf();
        void updateSelf(bool *, unsigned int *);

        //Metoda koja regulise koliziju sa drugom loptom
        void collideWith(Ball &);    

        string toString(); 

        static int counter;

        bool isActive();

        bool getMoving();

        void cushionCollide(double, double, double, double);

        void setMoving(bool);

        void printBitMask();
        
};

#endif