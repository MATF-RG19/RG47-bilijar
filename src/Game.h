#ifndef GAME_H
#define GAME_H

#include "Ball.h"
#include "VarLims.h"
#include <vector>

using namespace std;

struct CamCord{
    double camR;
    double camRho;
    double camTheta;

    CamCord(){

    }
    CamCord(double _camR, double _camRho, double _camTheta){
        camR = _camR;
        camRho = _camRho;
        camTheta = _camTheta;
    }
};

struct Vec3{
    double x;
    double y;
    double z;
}





class Game{

    public:
        vector<Ball>balls;
        int controlMode = CTL_MODE_REGULAR;
        double controlModeDelta;

        double wwidth;
        double wheight;
        double theRatio;
        
        static const double tableRatio = 0.5;
        static const double tableHeight = 73;

        const double shotModeCamRho = NINETY_DEGREES - TWENTY_DEGREES;
        double shotModeCamR;
        

        /**
         * SFERNE KOORDINATE KAMERE
         * R - poluprecnik
         * Theta - polarni ugao
         * Rho - ugao u odnosu na pozitivan deo z-ose
         * 
         * **/

        //========================KAMERA, OGRANICENJA=============================:
        CamCord cam;
        VarLims rhoLims;
        VarLims rLims;    

        double deltaTheta;

        Vec3 lookingAt;
        Vec3 lookingFrom;

        VarLims sLims;

        //========================STO I AMBIJENT=============================:
        double tableLength;         
        double tableWidth;

        double tableEdgeUp;
        double tableEdgeDown;
        double tableEdgeLeft;
        double tableEdgeRight;


        double ambientWidth;
        double ambientLength;
        double ambientHeight;
        
        //========================KUGLE======================================:
        double ballRadius;     
        double pocketRadius;
        double ballLimUp, ballLimDown, ballLimLeft, ballLimRight;
        
        

        //=========================BOOLOVI=====================================:
        double shotStrength;
        bool inShotMode = false;
        bool fineTune = false;
        bool fullScreen = true;
        bool controlLock = false;


        //=========================KONSTANTE CRTANJA==========================:
        double pillarConst1;

        double pillarConstA1;
        double pillarConstA2;
        double pillarConstB1;
        double pillarConstB2;

        double tableBasisScaleX;
        double tableBasisScaleY;
        double tableBasisScaleZ;

        double tableBasisShiftZ;


        double persp;
        /**
         * 
         * Koristi se za proveru da li se bilo koja kugla trenutno krece da bi se znalo
         * da li ponovo treba pozvati tajmer
         * i-ti bit s leve strane odgovoara i-toj kugli u vektoru 'balls' i on je 0 ukoliko 
         * kugla stoji i 1 ukoliko se krece. Svaka kugla sama ima svoje bitmaske koje koristi
         * za manipulaciju ovog broja.
         * 
         * Kada je activityCheck == 0 tada se nijedna kugla ne krece
         * **/
        unsigned int activityCheck = 0;


        //=====================KONSTRUKTOR================================
        Game();
        Game(double);

        //=====================GETTERI I SETTERI==========================
        void setTableLength();

        void initAll();
        void fillCluster();
        void setStandardLimitsAndVals();
        void setShotModeLimitsAndVals();




};

#endif