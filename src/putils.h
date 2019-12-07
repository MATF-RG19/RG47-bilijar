#ifndef PUTILS_H
#define PUTILS_H

bool withinBounds(double *, double *);
bool withinBoundsSimple(double x, double * bounds);
void drawCircle(double);
void drawEllipse(double, double, double, double);

bool circleDrop(double bigX, double bigY, double bigRadius, double smallX, double smallY);
#endif