#include "Vec2.h"
#include <string>

Vec2::Vec2(){

}

Vec2::Vec2(double x, double y){
    this->x = x;
    this->y = y;
}

void Vec2::add(Vec2 v){
    this->x += v.x;
    this->y += v.y;
}


Vec2 Vec2::r_add(Vec2 v){
    return Vec2(this->x + v.x, this->y + v.y);
}

void Vec2::sub(Vec2 v){
    this->x -= v.x;
    this->y -= v.y;
}

Vec2 Vec2::r_sub(Vec2 v){
    return Vec2(this->x - v.x, this->y - v.y);
}

void Vec2::mult(double scalar){
    this->x *= scalar;
    this->y *= scalar;
}

Vec2 Vec2::r_mult(double scalar){
    return Vec2(this->x * scalar, this->y * scalar);
}

double Vec2::dot(Vec2 v){
    return this->x*v.x + this->y*v.y;
}

double Vec2::squaredMag(){
    return this->x * this->x + this->y * this->y;
}

double Vec2::mag(){
    return sqrt(squaredMag());
}

void Vec2::anull(){
    mult(0);
}

string Vec2::toString(){
    return "(" + to_string(x) + ", " + to_string(y) + ")";
}

void Vec2::normalize(){
    double mg = mag();
    if (mg != 0){
        this->mult(1/mg);
    }
}

Vec2 Vec2::r_normalize(){
    Vec2 v = Vec2(x, y);
    v.normalize();

    return v;
    
}



