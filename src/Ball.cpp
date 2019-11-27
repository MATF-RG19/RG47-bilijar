#include "Ball.h"
#include <GL/glut.h>

//Ovi makroi predstavljaju cistu ljudsku lenjost
#define X (position.x)
#define Y (position.y)

Ball::Ball(){}

//void setAll(Vec2 pos, Vec2 vel, double radius, double r, double g, double b, int i){

Ball::Ball(int i){
    setAll(Vec2(0, 0), Vec2(0, 0), 1, 0, 0, 0, i);
}

Ball::Ball(Vec2 position, int i){
    setAll(position, Vec2(0, 0), 1, 1, 1, 1, i);
}

Ball::Ball(Vec2 position, Vec2 velocity, int i){
    setAll(position, velocity, 1, 1, 1, 1, i);
}

Ball::Ball(Vec2 position, Vec2 velocity, double radius, double r, double g, double b, int i){
    setAll(position, velocity, radius, r, g, b, i);
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
    if (velocity.squaredMag() != 0) {
        moving = true;
    }
}

string Ball::toString(){
    return this->position.toString();
}


int Ball::counter = 0;



void Ball::drawSelf(){
    //Prilikom crtanja lopte pretpostavljamo da je koordinati sistem vec transliran uvis.
    //Razlog je izbegavanje velikog broja pozivanja funkcije glPushMatrix() zato sto ce se lopte crtati u petlji
    //TheGame ce eventualno imati metodu za crtanje svih kugli u kojoj ce pozivati upravo ovu metodu za svaku kuglu pa ce
    //translacija biti uradjena tu.

    //Jos jedan razlog je neko postovanje OOP principa (ne treba lopta da zna kolko je sto visok)

    glColor3f(r, g, b);
    glTranslated(position.x, position.y, 0);
    glutSolidSphere(radius, 50, 50);
    glTranslated(-position.x, -position.y, 0);
    glutPostRedisplay();

}

void Ball::collideWith(Ball b){

}

bool Ball::isActive(){
    return (bitMask | bitCheck != 0);
}

bool Ball::getMoving(){
    return moving;
}

void Ball::updateSelf(){
    double ckmag = this->velocity.squaredMag();
    if (ckmag == 0) return;
    this->position.add(this->velocity);
    //this->velocity.mult(0.98);

    //cout << "[" << stupidCounter << "]" << "Changing vel from " << velocity.toString();
    this->velocity.mult(0.99);
    //cout << "to " << velocity.toString();
    stupidCounter++;
    //cout << ", and setting stupidCounter to " << stupidCounter << endl;
    if (this->velocity.squaredMag() < 0.01){
        this->velocity.anull();
        this->moving = false;
    }
}


void Ball::cushionCollide(double limUp, double limDown, double limLeft, double limRight){
    
    if (Y > limUp || Y < limDown) {
        
        velocity.flipY();
    }

    if (X > limRight || X < limLeft) {
        //cout << "X OFF because pos: " << X << ", " << Y << " and v = " << velocity.toString();
        velocity.flipX();
        //cout << ", now v = " << velocity.toString() << endl;
    }

}   