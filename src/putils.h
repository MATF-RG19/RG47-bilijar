#ifndef PUTILS_H
#define PUTILS_H

//Pripadnost granicama (specijalno)
bool withinBounds(double *, double *);
//Pripadnost granicama
bool withinBoundsSimple(double x, double * bounds);
//Crtanje kruga parametrizacijom
void drawCircle(double);
//Verovatno ce biti izbaceno
void drawEllipse(double, double, double, double);
//Proverava da li je jedan krug "unutar" drugog vise od pola (otprilike)
bool circleDrop(double bigX, double bigY, double bigRadius, double smallX, double smallY);
//Pomocna za crtanje cilindra
void set_normal_and_vertex(float u, float v);
//Crtanje cilindra
void draw_object(double height);
#endif