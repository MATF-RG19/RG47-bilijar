#include "Ball.h"

Ball::Ball(){}

Ball::Ball(Vec2 position){
    setAll(position, Vec2(0, 0), 1, 1, 1);
}

Ball::Ball(Vec2 position, Vec2 velocity){
    setAll(position, velocity, 1, 1, 1);
}

Ball::Ball(Vec2 position, Vec2 velocity, double r, double g, double b){
    setAll(position, velocity, r, g, b);
}


Vec2 Ball::getPosition(){
    return position;
}

void Ball::setPosition(Vec2 position){
    this->position = position;
}

Vec2 Ball::getVelocity(){
    return velocity;
}

void Ball::setVelocity(Vec2 velocity){
    this->velocity = velocity;
}

string Ball::toString(){
    return this->position.toString();
}






void Ball::drawSelf(){

}

void Ball::collideWith(Ball b){

}