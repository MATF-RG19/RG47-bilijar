#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include <unistd.h>
#include <sys/types.h>


#include "Vec2.h"
#include "Ball.h"
#include "putils.h"

#include <chrono>

#define REDRRAW_BALLS (0)
#define REDRRAW_BALLS_INTERVAL (5)

#define BALLS_MODE (1)

#define TABLEOFF1 (2)
#define TABLEOFF2 (4)

#define GAMEMODE (1)

#define NINETY_DEGREES (1.5708)
#define FOURTY_FIVE_DEGREES (0.785398)
#define TWENTY_DEGREES (0.349066)
#define FIVE_DEGREES (0.0872665)

using namespace std;

vector<Ball> balls;         //Niz koji ce sadrzati kugle
double camR;                //Sferne koordinate (r, teta, ro) koje opisuju poziciju kamere (uglavnom je 'Ro' konvencionalna oznaka za poluprecnik, ovde oznacava drugi ugao)
double camTheta;
double camRho;

double Rlimit[2];
double ThetaLimit[2];
double RhoLimit[2];

double deltaR;              //Predstavljaju promenu svake od sfernih koordinata iliti "osetljivost" kamere.
double deltaTheta;          //Bice izracunato na osnovu dimenzija stola. Za sad se koriste fiksne vrednosti.
double deltaRho;

bool hasCamMoved = false;   //budalastina, vraticu se na ovo

double tableLength;         //Dimenzije stola
double tableWidth;
double ballRadius;     
double holeRadius;     

double elA;
double elB;

//Visina stola kao i odnos duzina/sirina su konstantne vrednosti
static constexpr double tableRatio = 0.5;
static const int tableHeight = 73;

//Racuna relevantne koordinate ivice stola. Koristice se u detekciji kolizija sa ivicama.
double tableEdgeUp;
double tableEdgeDown;
double tableEdgeLeft;
double tableEdgeRight;

double ballLimUp, ballLimDown, ballLimLeft, ballLimRight;

double tableTopHeight;
double tableBottomHeight;

double c_centerX = 0;
double c_centerY = 0;


int testMod = 0;
bool enableSafe = true;
//Oznacava da li se igrac trenutno nalazi u rezimu udarca kugle ili ne.
//Ukoliko se nalazi u rezimu udarca, koordinate "oka" kamere bice sferne koordinate bele kugle a ne sfere oko stola
bool inShotMode = false;

double lookingAtX;  
double lookingAtY;
double lookingAtZ;

double lookingFromX;
double lookingFromY;
double lookingFromZ;


//Jacina udarca, bice implementirano kada pocnem animaciju
double shotStrength;
bool fineTune;

//Standardne funkcije
static void on_display(void);
static void on_reshape(int, int);
static void on_keyboard(unsigned char key,int x, int y);

//Neke od glavnih funkcija
void initAll(double);
void drawTable();
void directCamera();
void testBalls();
void drawCoord();
void mainTimerCallBack(int);
bool anyBallsMoving();
void initBalls();
void drawBalls();
void fillCluster();
void drawAim();
Vec2 getViewDirection();

void setStandardLimitsAndVals();
void setShotModeLimitsAndVals();


GLfloat table_ambient_material[] = { 0, 1, 0, 1 };
GLfloat table_diffuse_material[] = { 1,1,1 };
GLfloat table_specular_material[] = { 0, 0, 0, 1 };
GLfloat table_shininess = 2;

GLfloat hole_ambient_material[] = { 0, 0, 0, 1 };
GLfloat hole_diffuse_material[] = { 1,1,1 };
GLfloat hole_specular_material[] = { 1, 1, 1, 1 };

GLfloat base_ambient_material[] = { 0.48,0.46,0.4,1 };
GLfloat base_diffuse_material[] = { 0.48,0.46,0.4,1};
GLfloat base_specular_material[] = { 1, 0, 0, 1 };
GLfloat base_shininess = 10;

GLfloat light_position[] = { 0, 0, 2*tableHeight, 0};
GLfloat ambient_light[] = { 1, 1, 1, 1 };
GLfloat diffuse_light[] = { 0.7, 0.7, 0.7, 1 };
GLfloat specular_light[] = { 0.9, 0.9, 0.9, 1 };

GLfloat trans_ambient_material[] = { 0.48,0.46,0.4,0.2 };
GLfloat trans_diffuse_material[] = { 0.48,0.46,0.4,0.2};
GLfloat trans_specular_material[] = { 1, 0, 0, 0.2 };

int cnt = 0;

time_t thetime;
time_t displayTime;

Vec2 cbp;

int main(int argc, char ** argv){

    cout << "PID: " << getpid() << endl;    

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(200, 200);
    glutCreateWindow(argv[0]);

    
    //BILIJAR:
    //initAll(233);
    //SNUKER:
    initAll(233);
    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);

    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    displayTime = time(NULL);
    glutMainLoop();
    

    return 0;
}

//EVENT FUNCTIONS:
static void on_display(){
    time_t ctime = time(NULL);
    
    displayTime = ctime;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 800); //Dalji clipping-plane je za sada vrlo daleko zbog cestih promena dimenzija objekata radi testiranja. Bice racunat na osnovu dimenzija

    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    glLoadIdentity();
    directCamera();
    drawTable();      
    drawBalls();
    //drawCoord();
    if(inShotMode) drawAim();

    glutSwapBuffers();
}
void mainTimerCallBack(int arg){
    switch (arg){
        
        case REDRRAW_BALLS:
            bool allSafe = true;
            unsigned int activityCheck = 0;
            int ind = 0;
            for (Ball & b: balls){
                if(b.getMoving()){
                    b.cushionCollide(ballLimUp, ballLimDown, ballLimLeft, ballLimRight);
                    int jnd = 0;
                    for(Ball & g: balls){
                        if(ind == jnd){
                            jnd++;
                            continue;
                        }
                        b.collideWith(g);
                        jnd++;
                    }
                    bool currentsafe;
                    b.updateSelf(&currentsafe, &activityCheck);
                    allSafe = allSafe & currentsafe;
                }
                ind++;
                
            }
            
            glutPostRedisplay();
            
            //cout << "ACTIVITY: " << activityCheck << endl;
            if(activityCheck != 0){
                glutTimerFunc(REDRRAW_BALLS_INTERVAL, mainTimerCallBack, arg);
            }else{
                cout << "DONEEEEEEEEEEEEEEEEEEEEEEEE" << endl;
            }
            break;
    }
}
static void on_reshape(int x, int y){
    glutPostRedisplay();
}
static void on_keyboard(unsigned char c,int x, int y){
    switch (c){
        case 27:
            exit(1);
            break;
        case 'A':
        case 'a':
            camTheta -= deltaTheta;
            glutPostRedisplay();
            break;
        case 'D':
        case 'd':
            camTheta += deltaTheta;
            glutPostRedisplay();
            break;
        case 'W':
        case 'w':
            if(withinBounds(&camRho, RhoLimit)){
                camRho -= .02;
                //cout << "RHO: " << camRho << endl;
            }else{
                //cout << "Cant go past " << camRho << endl;
            }
            glutPostRedisplay();
            break;
        case 'S':
        case 's':
            if(withinBounds(&camRho, RhoLimit)){
                camRho += .02;
                //cout << "RHO: " << camRho << endl;
            }else{
                //cout << "Cant go past " << camRho << endl;
            }
            glutPostRedisplay();
            break;
        case 'I':
        case 'i':
            //camR -= .5;
            if(withinBounds(&camR, Rlimit)){
                camR -= ballRadius;
            }
            glutPostRedisplay();
            break;
        case 'K':
        case 'k':
            //camR += .5;
            if(withinBounds(&camR, Rlimit)){
                camR += ballRadius;
                //cout << "(r = " << camR << ", theta = " << camTheta << ", rho = " << camRho << ")" << endl; 
            }
            glutPostRedisplay();
            
            break;
        case '+':
            shotStrength += 0.1;
            cout << "S: " << shotStrength << endl;
            break;
        case '-':
            shotStrength += -.1;
            cout << "S: " << shotStrength << endl;
            break;
        case 'N':
        case 'n':
            fillCluster();
            break;

        case 'x':
        case 'X':
            if(!inShotMode){
                inShotMode = true;
                setShotModeLimitsAndVals();
            }else{
                inShotMode = false;
                setStandardLimitsAndVals();
            }
            glutPostRedisplay();
            break;
        case 'f':
        case 'F':
            if(fineTune){
                fineTune = false;
                deltaTheta = .02;
                cout << "FINE TUNE OFF" << endl;
            }else{
                fineTune = true;
                deltaTheta = .002;
                cout << "FINE TUNE ON" << endl;
            }
            break;
        case 'H':
        case 'h':
            if(!inShotMode) break;
            if(anyBallsMoving()) break;
            
            inShotMode = false;

            setStandardLimitsAndVals();


            if (shotStrength > 2){
                enableSafe = true;
            }

            thetime = time(NULL);

            Vec2 shotDir = getViewDirection();
            shotDir.normalize();
            shotDir.mult(shotStrength*ballRadius);

            balls[0].setVelocity(shotDir);
            glutTimerFunc(REDRRAW_BALLS_INTERVAL, mainTimerCallBack, REDRRAW_BALLS);
            break;
        
    }
}

//DRAW FUNCTIONS:
void directCamera(){
    //Usmeravanje kamere na osnovu sfernih koordinata 
    //Centar sfere nije centar koordinatnog sistema vec centar stola, koji ima koordinate (0, 0, tableHeight). Zbog toga se tableHeight dodaje na poslednju koordinatu.
    glLoadIdentity();
    Vec2 ps = balls[0].getPosition();
    //glTranslated(0, 0, tableHeight);
    if(inShotMode){
        lookingFromX = camR*cos(camTheta)*sin(camRho) + ps.x;
        lookingFromY = camR*sin(camTheta)*sin(camRho) + ps.y;
        lookingFromZ = camR*cos(camRho) + tableHeight;
        lookingAtX = ps.x;
        lookingAtY = ps.y;
        lookingAtZ = tableHeight;
        gluLookAt(
            lookingFromX, lookingFromY, lookingFromZ,
            lookingAtX, lookingAtY, lookingAtZ,
            0, 0, 1
        );
        
    }else{
        lookingFromX = camR*cos(camTheta)*sin(camRho);
        lookingFromY = camR*sin(camTheta)*sin(camRho);
        lookingFromZ = camR*cos(camRho) + tableHeight;
        lookingAtX = 0;
        lookingAtY = 0;
        lookingAtZ = tableHeight;
        gluLookAt (
            lookingFromX, lookingFromY, lookingFromZ,
            lookingAtX, lookingAtY, lookingAtZ,
            0, 0, 1
        );   
    }
    

  
    //glTranslated(0, 0, -tableHeight);
}
void drawTable(){
    
	glMaterialfv(GL_FRONT, GL_AMBIENT, table_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, table_diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, table_specular_material);
	glMaterialf(GL_FRONT, GL_SHININESS, table_shininess);
    //Drawing table top
    glPushMatrix();
        glTranslated(0, 0, tableHeight - TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(tableWidth, tableLength, TABLEOFF2); //Skaliranje po dimenzijama
        glutSolidCube(1);     
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, hole_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, hole_diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, hole_specular_material);

    //Drawing holes
    glPushMatrix();
        glTranslated(0, 0, tableHeight+0.1);
        glPushMatrix();
            glTranslated(tableEdgeLeft, tableEdgeUp, 0);
            //glRotated(45, 0, 0, 1);
            drawCircle(holeRadius);
            //drawEllipse(elA, elB, 0, M_PI);
        glPopMatrix();
        glPushMatrix();
            glTranslated(tableEdgeLeft, tableEdgeDown, 0);
            glColor3f(0, 0, 0);
            drawCircle(holeRadius);
        glPopMatrix();
        glPushMatrix();
            glTranslated(tableEdgeRight, tableEdgeUp, 0);
            glColor3f(0, 0, 0);
            drawCircle(holeRadius);
        glPopMatrix();
        glPushMatrix();
            glTranslated(tableEdgeRight, tableEdgeDown, 0);
            glColor3f(0, 0, 0);
            drawCircle(holeRadius);
        glPopMatrix();
        glPushMatrix();
            glTranslated(tableEdgeLeft, 0, 0);
            glColor3f(0, 0, 0);
            drawCircle(holeRadius);
        glPopMatrix();
        glPushMatrix();
            glTranslated(tableEdgeRight, 0, 0);
            glColor3f(0, 0, 0);
            drawCircle(holeRadius);
        glPopMatrix();
    glPopMatrix();

    //Drawing rails...
    glPushMatrix();
        glTranslated(0, tableEdgeUp + TABLEOFF1, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(tableWidth - 2*holeRadius, TABLEOFF2, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0, tableEdgeDown - TABLEOFF1, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(tableWidth - 2*holeRadius, TABLEOFF2, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(tableEdgeRight + TABLEOFF1, tableEdgeUp/2, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(TABLEOFF2, tableLength/2 - 2*holeRadius, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(tableEdgeRight + TABLEOFF1, -tableEdgeUp/2, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(TABLEOFF2, tableLength/2 - 2*holeRadius, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(tableEdgeLeft - TABLEOFF1, tableEdgeUp/2, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(TABLEOFF2, tableLength/2 - 2*holeRadius, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(tableEdgeLeft - TABLEOFF1, -tableEdgeUp/2, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(TABLEOFF2, tableLength/2 - 2*holeRadius, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, base_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, base_diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, base_specular_material);
	glMaterialf(GL_FRONT, GL_SHININESS, base_shininess);
    //Drawing table base
    glPushMatrix();
        glTranslated(0, 0, (tableHeight - TABLEOFF2)/2);
        glScaled(0.8*tableWidth, 0.8*tableLength, tableHeight - TABLEOFF2);
        glColor3f(0.48,0.46,0.4);
        glutSolidCube(1);
        glColor3f(0, 0, 0);
        glutWireCube(1);
    glPopMatrix(); 
}
void drawBalls(){
    glTranslated(0, 0,tableHeight + ballRadius);
    for(Ball b: balls){   
        b.drawSelf();
    }
    glTranslated(0, 0, - tableHeight - ballRadius);
}
void drawAim(){
    cbp = balls[0].getPosition();
    glLineWidth(5);
    glColor3f(0.839215686, 0.28627451, 1);
    glBegin(GL_LINES);
        glVertex3f(cbp.x, cbp.y, tableHeight+1);
        glVertex3f(cbp.x + (lookingAtX - lookingFromX)*5,cbp.y +  (lookingAtY - lookingFromY)*5, tableHeight + 1);
    glEnd();
}
void drawCoord(){
    glLineWidth(1);
    glColor3f(0, 0, 1.0);
    //X AXIS IS BLUE
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(1000.0, 0, 0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-1000.0, 0, 0);
    glEnd();
    glColor3f(0, 1.0, 0);
    //Y AXIS IS GREEN
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1000.0, 0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -1000.0, 0);
    glEnd();

    glColor3f(1.0, 0, 0);
    //Z AXIS IS RED
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1000.0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -1000.0);
    glEnd();
}

//INITIALIZING FUNCTIONS: 
void initAll(double tl){--
    
    tableLength = tl;
    tableWidth = tableLength * tableRatio;
    
    tableEdgeUp = tableLength/2;
    tableEdgeDown = - tableEdgeUp;
    tableEdgeRight = tableWidth/2;
    tableEdgeLeft = -tableEdgeRight;

    //BILIJAR:
    ballRadius = tableLength * 0.02182285/2;
    //SNUKER:
    /* ballRadius = tableLength * 0.007293693;
    */holeRadius = 2*ballRadius; 

    elA = 3*ballRadius;
    elB = 4*ballRadius;

    ballLimUp = tableEdgeUp - ballRadius;
    ballLimDown = tableEdgeDown + ballRadius;
    ballLimLeft = tableEdgeLeft + ballRadius;
    ballLimRight = tableEdgeRight - ballRadius;


    camR = tableLength;
    camTheta = 1;
    camRho = 1;

    Rlimit[0] = ballRadius;
    Rlimit[1] = 2*tableLength;
    RhoLimit[0] = TWENTY_DEGREES; //20-ak stepeni
    RhoLimit[1] = NINETY_DEGREES; //90 stepeni
    //Theta je neogranicen, moze da se okrece kolko oces

    //SKACI VAMO
    deltaTheta = .02;
    fineTune = false;
    
    shotStrength = 0;

    
    if (BALLS_MODE == 0){
        initBalls();
    }else{
        fillCluster();
    }
    
}
void initBalls(){
    balls.empty();
    balls.push_back(Ball(Vec2(0, 2*tableEdgeUp/3), Vec2(0, 0),ballRadius, 1, 0, 0, 0));
    balls.push_back(Ball(Vec2(0, 0), Vec2(0, 0), ballRadius, 0, 0, 1, 1));
    balls.push_back(Ball(Vec2(0, 2*tableEdgeDown/3), Vec2(0, 0), ballRadius, 1, 0, 1, 2));
}
void fillCluster(){
    balls.clear();
    balls.push_back(Ball(Vec2(0, tableEdgeDown/2), Vec2(0, 0), ballRadius, 1, 1, 1, 0));

    double currentY = tableEdgeUp/3;
    double dy = 2*ballRadius;
    int numInRow = 1;
    int ballId = 1;
    for(int row = 0; row < 5; row++){
        
        double currentX = -row * ballRadius;
        for(int i = 0; i < numInRow; i++){
            GLfloat r, g, b;
            if (ballId % 2 == 0){
                r = 1;
                g = 0;
                b = 0;
            }else
            {
                r = 0;
                g = 0;
                b = 1;
            }
            
            balls.push_back(Ball(Vec2(currentX, currentY + ballRadius/5), Vec2(0, 0), ballRadius, r, g, b, ballId++));
            currentX += 2*ballRadius;
        }
        numInRow++;
        currentY += dy;

    }

    for(Ball b: balls){
        b.printBitMask();
    }

    glutPostRedisplay();

}

//MISC FUNCTIONS:
bool anyBallsMoving(){
    for(Ball b: balls){
        if (b.getMoving()) return true;
    }
    return false;
}
Vec2 getViewDirection(){
    Vec2 v(lookingAtX - lookingFromX, lookingAtY - lookingFromY);
    v.normalize();


    return v;
}
void setStandardLimitsAndVals(){
    camRho = FOURTY_FIVE_DEGREES;
    camR = tableLength;
    Rlimit[0] = ballRadius;
    Rlimit[1] = 2*tableLength;
    RhoLimit[0] = TWENTY_DEGREES;
    RhoLimit[1] = NINETY_DEGREES;
}
void setShotModeLimitsAndVals(){
    camRho = NINETY_DEGREES - TWENTY_DEGREES;
    camR = 16*ballRadius;
    Rlimit[0] = ballRadius;
    Rlimit[1] = 16*ballRadius;
    RhoLimit[0] = FOURTY_FIVE_DEGREES;
    RhoLimit[1] = NINETY_DEGREES - FIVE_DEGREES;
}



