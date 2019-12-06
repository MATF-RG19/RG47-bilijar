#include "Ball.h"
#include <GL/glut.h>
#include <bitset>

using namespace std;

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

Ball::Ball(Vec2 position, Vec2 velocity, double radius, GLfloat r, GLfloat g, GLfloat b, int i){
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
    glutSolidSphere(radius, 50, 50);
    glTranslated(-position.x, -position.y, 0);

}

void Ball::collideWith(Ball & b){
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


bool Ball::getMoving(){
    return (this->velocity.squaredMag() != 0);
}

void Ball::updateSelf(bool * issafe, unsigned int * activity){
    double ckmag = this->velocity.squaredMag();
    if (ckmag == 0) return;
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
    
    *issafe = (ckmag < 2*this->radius);
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

void Ball::printBitMask(){
    /* unsigned int n = bmaskTurnOn;
    string bis = "";
    while(n != 0){
        bis += (n % 2 == 0)?"0":"1";
        n /= 2;
    }

    for(int i = 0, j = bis.size() - 1; i < j; i++, j--){
        char tmp = bis[i];
        bis[i] = bis[j];
        bis[j] = tmp;
    }

    cout << "Ball " << stupidIndex << ": " << bis << endl; */
    
}