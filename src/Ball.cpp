#include "Ball.h"
#include "putils.h"
#include <GL/glut.h>
#include <bitset>

using namespace std;

double Ball::epsSpeedIntensity = 0;

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
    this->idx = i;

    bmaskTurnOn = 1 << (i);
    bmaskTurnOff = bmaskTurnOn ^ (-1);

    if(epsSpeedIntensity == 0){
        epsSpeedIntensity = radius/10;
    }



    
}

// === GETERI, SETERI I SL ===
Vec2 Ball::getPosition(){
    return position;
}
void Ball::setPosition(Vec2 position){
    this->position = position;
}
void Ball::setPosition(double x, double y){
    this->position.x = x;
    this->position.y = y;
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
int Ball::getIdx(){
    return idx;
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
    
    glutSolidSphere(radius, NUM_SLICES, NUM_STACKS);
    //glutWireSphere(radius, 20, 20);
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
    //HACK: promeni donji this->velocity.squaredMag() u ckmag*0.99
    if (this->velocity.squaredMag() < 0.001){
        this->velocity.anull();
        *activity = *activity & bmaskTurnOff;
    }else
    {
        *activity = *activity | bmaskTurnOn;
    }
    
    
}
bool Ball::collideWith(Ball & b){
    /**
     * Zasnovano na ideji elasticnih kolizija.
     * http://www.vobarian.com/collisions/2dcollisions2.pdf
     * **/

    //BOUNDING BOX TECHNIQUE
    double minx1, miny1, maxx1, maxy1;
    double minx2, miny2, maxx2, maxy2;

    minx1 = position.x - radius;
    miny1 = position.y - radius;
    maxx1 = position.x + radius;
    maxy1 = position.y + radius;

    minx2 = b.position.x - b.radius;
    miny2 = b.position.y - b.radius;
    maxx2 = b.position.x + b.radius;
    maxy2 = b.position.y + b.radius;
    
    
    //bool asd = true;
    //cout << "(";
    if (!(   maxx1 > minx2 && minx1 < maxx2 && maxy1 > miny2 && miny1 < maxy2        )){
        return false;
    }



    Vec2 NormalVector = this->position.r_sub(b.position); //NON - NORMALIZED VECTOR THAT CONNECTS THE TWO CENTERS
    
    double ballRadius = b.radius;
    double NormalVectorMag = NormalVector.mag();

    if(NormalVectorMag > 2*ballRadius){ 
        return false;
    }


    
    
    if(NormalVectorMag < 2*ballRadius){
        //Lopte ulaze jedna u drugu, razdvajaju se da ne bi bilo glitchovanja
        double intersectionDepth = 2*ballRadius - NormalVectorMag; //dubina preseka
        //Svaka lopta se pomera upola
        Vec2 correctionVector = NormalVector.r_normalize();
        correctionVector.mult(intersectionDepth/2);

        

        this->position.add(correctionVector);
        
        b.position.sub(correctionVector);
        
    }
    
    NormalVector = this->position.r_sub(b.position);
    NormalVector.normalize();
    Vec2 TangentVector = Vec2(-NormalVector.y, NormalVector.x);
    

    double ball1NormalIntensity = this->velocity.dot(NormalVector); 
    double ball1TangentIntensity = this->velocity.dot(TangentVector);

    double ball2NormalIntensity = b.velocity.dot(NormalVector); 
    double ball2TangentIntensity = b.velocity.dot(TangentVector);

    double ball1NormalPower = ball2NormalIntensity;
    double ball2NormalPower = ball1NormalIntensity;

    Vec2 ball1NormalComponent = NormalVector.r_mult(ball1NormalPower);
    Vec2 ball1TangentComponent = TangentVector.r_mult(ball1TangentIntensity);
    
    Vec2 ball2NormalComponent = NormalVector.r_mult(ball2NormalPower);
    Vec2 ball2TangentComponent = TangentVector.r_mult(ball2TangentIntensity);

    ball1NormalComponent.add(ball1TangentComponent);
    setVelocity(ball1NormalComponent);
    
    
    ball2NormalComponent.add(ball2TangentComponent);
    //b.setVelocity(ball2NormalComponent);
    b.setVelocity(ball2NormalComponent);
    //*b2new = Vec2(ball2NormalComponent.x, ball2NormalComponent.y);

    return true;

            
}
bool Ball::cushionCollide(double limUp, double limDown, double limLeft, double limRight){
    
    bool b1 = Y >= limUp;
    bool b2 = Y < limDown;
    bool b3 = X > limRight;
    bool b4 = X < limLeft;

    bool ret = false;
    if(b1){
        Y -= (Y - limUp);
        velocity.flipY();
        ret = true;
    }
    if(b2){
        Y += (limDown - Y);
        velocity.flipY();
        ret = true;
    }
    if(b3){
        X -= (X - limRight);
        velocity.flipX();
        ret = true;
    }

    if(b4){
        X += (limLeft - X);
        velocity.flipX();
        ret = true;
    }

    return ret;


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
        //cout << "POT top left" << endl;
        return true;
    }
    if(circleDrop(limRight, limUp, pocketRadius, position.x, position.y)){
        //cout << "POT top right" << endl;
        return true;
    }

    if(circleDrop(limLeft, limDown, pocketRadius, position.x, position.y)){
        //cout << "POT bottom left" << endl;
        return true;
    }
    if(circleDrop(limRight, limDown, pocketRadius, position.x, position.y)){
        //cout << "POT bottom right" << endl;
        return true;
    }

    if(circleDrop(limLeft - pocketRadius/2, 0, pocketRadius*1.1, position.x, position.y)){
        //cout << "POT mid left" << endl;
        return true;
    } 
    if(circleDrop(limRight + pocketRadius/2, 0, pocketRadius*1.1, position.x, position.y)) {
        //cout << "POT mid right" << endl;
        return true;
    }

    return false;
}

// === DEBUG METODE ===
string Ball::toString(){
    return this->position.toString();
}