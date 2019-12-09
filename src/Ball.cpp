#include "Ball.h"
#include "putils.h"
#include <GL/glut.h>
#include <bitset>

using namespace std;

//Ovi makroi predstavljaju cistu ljudsku lenjost
#define X (position.x)
#define Y (position.y)

// === KONSTRUKTORI ===
Ball::Ball(){}

Ball::Ball(Vec2 position, Vec2 velocity, double radius, GLfloat r, GLfloat g, GLfloat b, int i){
    this->position = position;
    this->velocity = velocity;
    this->r = r;
    this->g = g;
    this->b = b;
    this->onTable = true;
    this->radius = radius;
    
    bmaskTurnOn = 1 << (i);
    bmaskTurnOff = bmaskTurnOn ^ (-1);
}

// === GETERI, SETERI I SL ===
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
bool Ball::getMoving(){
    return (this->velocity.squaredMag() != 0);
}
bool Ball::getOnTable(){
    return onTable;
}
bool Ball::setOnTable(bool b){
    this->onTable = b;
}
unsigned int Ball::getBitMaskTurnOn(){
    return bmaskTurnOn;
}
void Ball::dieCompletely(unsigned int * act){
    this->velocity.anull();
    onTable = false;
    *act = *act & bmaskTurnOff;
}

// === PAMETNE METODE ===
void Ball::drawSelf(){
    //Prilikom crtanja lopte pretpostavljamo da je koordinati sistem vec transliran uvis.
    //Razlog je izbegavanje velikog broja pozivanja funkcije glPushMatrix() zato sto ce se lopte crtati u petlji
    //TheGame ce eventualno imati metodu za crtanje svih kugli u kojoj ce pozivati upravo ovu metodu za svaku kuglu pa ce
    //translacija biti uradjena tu.

    //Jos jedan razlog je neko postovanje OOP principa (ne treba lopta da zna koliko je sto visok)
    
    GLfloat ambient_material[] = { r, g, b, 1 };
	GLfloat diffuse_material[] = { r, g, b, 1 };
	GLfloat specular_material[] = {0.7, 0.5, 0.7, 1 };
	GLfloat shininess = 10;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_material);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    glColor3f(r, g, b);
    glTranslated(position.x, position.y, 0);
    /* glutSolidSphere(radius, 20, 20); */
    glutWireSphere(radius, 20, 20);
    glTranslated(-position.x, -position.y, 0);

}
void Ball::updateSelf(unsigned int * activity){
    double ckmag = this->velocity.squaredMag();
    if (ckmag == 0) {
        return;
    }
    this->position.add(this->velocity);

    this->velocity.mult(0.99);
    ckmag = this->velocity.mag();
    if (this->velocity.squaredMag() < 0.001){
        this->velocity.anull();
        *activity = *activity & bmaskTurnOff;
    }else
    {
        *activity = *activity | bmaskTurnOn;
    }
    
}
void Ball::collideWith(Ball & b){
    /**
     * Zasnovano na ideji elasticnih kolizija.
     * http://www.vobarian.com/collisions/2dcollisions2.pdf
     * **/
    Vec2 NormalVector = this->position.r_sub(b.position); //NON - NORMALIZED VECTOR THAT CONNECTS THE TWO CENTERS
    double ballRadius = b.radius;
    double NormalVectorMag = NormalVector.mag();
    if(NormalVectorMag > 2*ballRadius){ 
        return;
    }

    if(NormalVectorMag < 2*ballRadius){
        //The Balls are intersecting which needs to be fixed.
        double intersectionDepth = 2*ballRadius - NormalVectorMag; //The depth of intersection
        //Ball 1 must move BACK (in the -1 direction) across the normal vector with the intensity of the intersection depth
        Vec2 correctionVector = NormalVector.r_normalize();
        correctionVector.mult(intersectionDepth/2);
        this->position.add(correctionVector);

        
        b.position.sub(correctionVector);
        
        //cout << "NVM: " << NormalVector.mag() << ", 2ballrad: " << 2*ballRadius << endl;
    }
    
    NormalVector = this->position.r_sub(b.position);
    //cout << "AFTER ALL: MAG: " << NormalVector.mag() << ", RAD: " << 2*ballRadius << endl;
    NormalVector.normalize();
    Vec2 TangentVector = Vec2(-NormalVector.y, NormalVector.x);
    
    //Project velocities onto normal and tangent vectors

    double ball1NormalIntensity = this->velocity.dot(NormalVector); //this will be the intensity in the normal direction of ball 1
    double ball1TangentIntensity = this->velocity.dot(TangentVector);

    double ball2NormalIntensity = b.velocity.dot(NormalVector); //this will be the intensity in the normal direction of ball 2
    double ball2TangentIntensity = b.velocity.dot(TangentVector);

    double ball1NormalPower = ball2NormalIntensity;
    double ball2NormalPower = ball1NormalIntensity;

    Vec2 ball1NormalComponent = NormalVector.r_mult(ball1NormalPower);
    Vec2 ball1TangentComponent = TangentVector.r_mult(ball1TangentIntensity);

    Vec2 ball2NormalComponent = NormalVector.r_mult(ball2NormalPower);
    Vec2 ball2TangentComponent = TangentVector.r_mult(ball2TangentIntensity);

    ball1NormalComponent.add(ball1TangentComponent);
    this->velocity = ball1NormalComponent;
    ball2NormalComponent.add(ball2TangentComponent);
    b.velocity = ball2NormalComponent;

            
}
void Ball::cushionCollide(double limUp, double limDown, double limLeft, double limRight){
    Vec2 transPos;
    bool b1 = Y >= limUp;
    bool b2 = Y < limDown;
    bool b3 = X > limRight;
    bool b4 = X < limLeft;

    if(b1){
        Y -= (Y - limUp);
        velocity.flipY();
    }
    if(b2){
        Y += (limDown - Y);
        velocity.flipY();
    }
    if(b3){
        X -= (X - limRight);
        velocity.flipX();
    }

    if(b4){
        X += (limLeft - X);
        velocity.flipX();
    }



}   
bool Ball::pocketCollide(double limUp, double limDown, double limLeft, double limRight, double pocketRadius){
    //KOORDINATE RUPA SU

    //GORE LEVO: (limUp, limLeft)
    //GORE DESNO: (limUp, limRight)
    //SREDNJA LEVO: (0, limLeft)
    //SREDNJA DESNO: (0, limRight)
    //DONJA LEVO: (limDown, limLeft)
    //DONJA DESNO: (limDown, limRight)

    if(circleDrop(limLeft, limUp, pocketRadius, position.x, position.y)) {
        cout << "POT top left" << endl;
        return true;
    }
    if(circleDrop(limRight, limUp, pocketRadius, position.x, position.y)){
        cout << "POT top right" << endl;
        return true;
    }

    if(circleDrop(limLeft, limDown, pocketRadius, position.x, position.y)){
        cout << "POT bottom left" << endl;
        return true;
    }
    if(circleDrop(limRight, limDown, pocketRadius, position.x, position.y)){
        cout << "POT bottom right" << endl;
        return true;
    }

    if(circleDrop(limLeft - pocketRadius/2, 0, pocketRadius*1.1, position.x, position.y)){
        cout << "POT mid left" << endl;
        return true;
    } 
    if(circleDrop(limRight + pocketRadius/2, 0, pocketRadius*1.1, position.x, position.y)) {
        cout << "POT mid right" << endl;
        return true;
    }

    return false;
}

// === DEBUG METODE ===
string Ball::toString(){
    return this->position.toString();
}