#include <iostream>
#include <string>
#include <math.h>
using namespace std;

/*
    Klasa koja predstavlja vektor u dve dimenzije.
    Obezbedjuje osnovne operacije sa vektorima (sabiranje, oduzimanje, mnozenje skalarom, skalarni proizvod).
    Kod operacija ciji je rezultat vektor date su dve verzije, verzija koja menja trenutni vektor i verzija koja vraca novi.
    Npr .add(Vec2 v) dodaje v na postojeci vektor dok .r_add(Vec2 v) vraca novi vektor koji predstavlja zbir trenutnog vektora i vektora v.
*/
#ifndef VEC2_H
#define VEC2_H
class Vec2{
    public:
        //Komponente vektora su javne radi lakseg koriscenja u glavnom programu.
        double x;
        double y;

        Vec2();
        Vec2(double, double);

        void add(Vec2 v);
        Vec2 r_add(Vec2 v);

        void sub(Vec2 v);
        Vec2 r_sub(Vec2 v);

        void mult(double);
        Vec2 r_mult(double);

        void normalize();
        Vec2 r_normalize();

        void flipX();
        void flipY();

        double dot(Vec2 v);

        /* Vraca kvadriranu normu (magnitudu) vektora. Pogodno za koriscenje ukoliko je potrebno uporedjivati neke udaljenosti. U tim slucajevima nije potrebno trositi vreme na racunanje korena */
        double squaredMag();

        //Vraca normu vektora
        double mag();

        //Anulira vektor (ekvivalentno sa .mult(0))
        void anull();

        string toString();

        

};
#endif