#include "Vec2.h"
#include <GL/glut.h>
#include <bitset>
#include <math.h>
#include <pthread.h>

using namespace std;
#ifndef BALL_H
#define BALL_H

#define NUM_SLICES (20)
#define NUM_STACKS (20)
class Ball{

    private:

        //Vektori koji predstavljaju poziciju u brzinu    
        Vec2 position;
        Vec2 velocity;

        //Mozda treba i da bude izbaceno, ako se includuje shconsts.h
        double radius;

        bool onTable;

        //RGB kod boje
        GLfloat r;
        GLfloat g;
        GLfloat b;

        /**
         * bmaskTurnOn je oblika b1b2.....b_kb_{k+1}....b_n gde je k redni broj lopte i samo bk = 1, svi ostali nula. U main-u se koristi bitmaska activityCheck koja se OR-uje sa 
         * ovom bitmaskom svake kugle koja se krece.
         * Kada kugla prestane da se krece tada se AND-uje sa bmaskTurnOff koji se dobija obrtanjem bitova bmaskTurnOn-a.
         * */
        unsigned int bmaskTurnOn;
        unsigned int bmaskTurnOff;

        int idx;
        
        static double epsSpeedIntensity;

        pthread_mutex_t MYLOCK;
    public:

        // === KONSTRUKTORI ===
        Ball();
        Ball(Vec2, Vec2, double, GLfloat, GLfloat, GLfloat, int);

        // === GETERI, SETERI I SL
        Vec2 getPosition();
        void setPosition(Vec2);

        Vec2 getVelocity();
        void setVelocity(Vec2);

        bool isActive();

        bool getMoving();
        void setMoving(bool);

        unsigned int getBitMaskTurnOn();

        bool getOnTable();
        bool setOnTable(bool);

        void dieCompletely(unsigned int *);
        int getIdx();
        

        // === PAMETNE METODE ===
        void drawSelf();
        void updateSelf(unsigned int *);
        void cushionCollide(double, double, double, double);
        bool collideWith(Ball & b);
        bool pocketCollide(double limUp, double limDown, double limLeft, double limRight, double pocketRadius);
        

        // === DEBUG METODE ===
        string toString(); 
          
};

#endif