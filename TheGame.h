#include "Vec2.h"
#include "Ball.h"

#include <vector>
#include <math.h>

#ifndef THEGAME_H
#define THEGAME_H

/***
 * Glavna upravljacka klasa cele igrice.
 * Sadrzi podatke o trenutnom polozaju kamere, osetljivosti kamere, rezimu udarca.
 * Takodje zaduzena za iscrtavanje stola, kugli, detekciju kolizija, ... (videti komentare dole)
 * 
 * Jedna od glavnih uloga je racunanje dimenzija svega SAMO na osnovu duzine stola. Koriste se proporcije pravih bilijarskih stolova, na osnovu toga se racuna i poluprecnik kugle.
 * To je razlog zasto konstruktor prima samo jedan parametar (tableLength). Moguce je da ce biti jos parametera ukolko bude implentacije samog gameplay-a.
 * 
 * 
 * */

class TheGame{
    private:
        vector<Ball> balls;         //Niz koji ce sadrzati kugle
        double camR;                //Sferne koordinate (r, teta, ro) koje opisuju poziciju kamere (uglavnom je 'Ro' konvencionalna oznaka za poluprecnik, ovde oznacava drugi ugao)
        double camTheta;
        double camRho;

        double deltaR;              //Predstavljaju promenu svake od sfernih koordinata iliti "osetljivost" kamere.
        double deltaTheta;          //Bice izracunato na osnovu dimenzija stola. Za sad se koriste fiksne vrednosti.
        double deltaRho;

        double tableLength;         //Dimenzije stola
        double tableWidth;
        double ballRadius;          

        //Visina stola kao i odnos duzina/sirina su konstantne vrednosti
        static constexpr double tableRatio = 0.5;
        static const int tableHeight = 20;

        //Racuna relevantne koordinate ivice stola. Koristice se u detekciji kolizija sa ivicama.
        double tableEdgeUp;
        double tableEdgeDown;
        double tableEdgeLeft;
        double tableEdgeRight;


        //Oznacava da li se igrac trenutno nalazi u rezimu udarca kugle ili ne.
        //Ukoliko se nalazi u rezimu udarca, koordinate "oka" kamere bice sferne koordinate bele kugle a ne sfere oko stola
        bool inShotMode;


        //Za sada se ne koriste, vec se manuelno racunaju u TheGame::directCamera()
        double lookingAtX;  
        double lookingAtY;
        double lookingAtZ;
        
        double lookingFromX;
        double lookingFromY;
        double lookingFromZ;


        //Jacina udarca, bice implementirano kada pocnem animaciju
        double shotStrength;

        

    public:
        //Osnovni konstruktor, prima samo duzinu stola 
        TheGame(double);
        TheGame();

        //Utility metoda za konverziju sfernih koordinata u dekartove
        inline void sphereToDecartes(double r, double theta, double rho, double *x, double *y, double *z);
        
        //Metoda koja reaguje na pristike tastera na tastaturi
        void handleKeyboard(char);

        //Metoda koja iscrtava sto za bilijar
        void drawTable();

        //Metoda koja usmerava kameru
        void directCamera();
        
        //Vrlo privremena metoda. Cisto radi estetske provere.
        void testBalls();
        
        

        
};

#endif