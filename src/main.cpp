#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glut.h>

#include "Vec2.h"
#include "Ball.h"


#define TABLEOFF1 (2)
#define TABLEOFF2 (4)

using namespace std;

vector<Ball> balls;         //Niz koji ce sadrzati kugle
double camR;                //Sferne koordinate (r, teta, ro) koje opisuju poziciju kamere (uglavnom je 'Ro' konvencionalna oznaka za poluprecnik, ovde oznacava drugi ugao)
double camTheta;
double camRho;

double deltaR;              //Predstavljaju promenu svake od sfernih koordinata iliti "osetljivost" kamere.
double deltaTheta;          //Bice izracunato na osnovu dimenzija stola. Za sad se koriste fiksne vrednosti.
double deltaRho;

double tableLength;         //Dimenzije stola
double tableWidth;
double ballRadius;          

//Visina stola kao i odnos duzina/sirina su konstantne vrednosti
static constexpr double tableRatio = 0.5;
static const int tableHeight = 73;

//Racuna relevantne koordinate ivice stola. Koristice se u detekciji kolizija sa ivicama.
double tableEdgeUp;
double tableEdgeDown;
double tableEdgeLeft;
double tableEdgeRight;

double tableTopHeight;
double tableBottomHeight;

//Oznacava da li se igrac trenutno nalazi u rezimu udarca kugle ili ne.
//Ukoliko se nalazi u rezimu udarca, koordinate "oka" kamere bice sferne koordinate bele kugle a ne sfere oko stola
bool inShotMode;


//Za sada se ne koriste, vec se manuelno racunaju u TheGame::directCamera()
double lookingAtX;  
double lookingAtY;
double lookingAtZ;

double lookingFromX;
double lookingFromY;
double lookingFromZ;


//Jacina udarca, bice implementirano kada pocnem animaciju
double shotStrength;

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
Vec2 getViewDirection();

int main(int argc, char ** argv){
    initAll(233);
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutGet(GLUT_SCREEN_HEIGHT);
    glutGet(GLUT_SCREEN_WIDTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(200, 200);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);

    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}


static void on_display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 1, 800); //Dalji clipping-plane je za sada vrlo daleko zbog cestih promena dimenzija objekata radi testiranja. Bice racunat na osnovu dimenzija

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    directCamera();   ///usmeravanje kamere
    drawTable();      //crtanje stola
    testBalls();      //test iscrtavanja kugli, privremeno

    drawCoord();        //crtanje koordinatnog sistema
    

    glutSwapBuffers();
}

void mainTimerCallBack(int arg){

}

static void on_reshape(int x, int y){
    //pass
    ;
    ;
    ;
}


static void on_keyboard(unsigned char c,int x, int y){
    switch (c){
        case 27:
            exit(1);
            break;
        case 'a':
            camTheta -= .02;
            glutPostRedisplay();
            break;
        case 'd':
            camTheta += .02;
            glutPostRedisplay();
            break;
        case 'w':
            camRho -= .02;
            glutPostRedisplay();
            break;
        case 's':
            camRho += .02;
            glutPostRedisplay();
            break;

        case 'i':
            //camR -= .5;
            camR -= ballRadius;
            glutPostRedisplay();
            break;

        case 'k':
            //camR += .5;
            camR += ballRadius;
            glutPostRedisplay();
            break;
        case '+':
            shotStrength += 0.1;
            cout << "S: " << shotStrength << endl;
            break;
        case '-':
            shotStrength -= 0.1;
            cout << "S: " << shotStrength << endl;
            break;
        case 'h':
            Vec2 shotDir = getViewDirection();
            shotDir.mult(shotStrength);

            //glutTimerFunc(20, timerCallBack, 0);

            break;



    }
}

//Iscrtavanje stola
void drawTable(){
    //Iscrtavanje stola (za sada samo kao obicni kvadar)
    //Eventualno ce biti dekorisan
    
    glPushMatrix();
        glTranslated(0, 0, tableHeight - TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(tableWidth, tableLength, TABLEOFF2); //Skaliranje po dimenzijama
            
        glColor3f(0.0, 1.0, 0);
        glutSolidCube(1);

        glColor3f(0, 0, 0);
        glutWireCube(1);        
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, 0, (tableHeight - TABLEOFF2)/2);
        glScaled(0.8*tableWidth, 0.8*tableLength, tableHeight - TABLEOFF2);
        glColor3f(0.48,0.46,0.4);
        glutSolidCube(1);
        glColor3f(0, 0, 0);
        glutWireCube(1);
    glPopMatrix();
    
}

//Usmerava kameru na osnovu sfernih parametara
void directCamera(){
    //Usmeravanje kamere na osnovu sfernih koordinata 
    //Centar sfere nije centar koordinatnog sistema vec centar stola, koji ima koordinate (0, 0, tableHeight). Zbog toga se tableHeight dodaje na poslednju koordinatu.
    gluLookAt(
        camR*cos(camTheta)*sin(camRho), camR*sin(camTheta)*sin(camRho), camR*cos(camRho) + tableHeight,
        0, 0, 0,
        0, 0, 1
    );
}

//Privremeno
void testBalls(){
    Ball b =  Ball(Vec2(0, 2*tableEdgeUp/3), Vec2(0, 0),ballRadius, 1, 0, 0, 0);
    Ball b1 = Ball(Vec2(0, 0), Vec2(0, 0), ballRadius, 0, 0, 1, 1);
    Ball b2 = Ball(Vec2(0, 2*tableEdgeDown/3), Vec2(0, 0), ballRadius, 1, 0, 1, 2);

    glPushMatrix();
        glTranslated(0, 0, tableHeight + ballRadius);
        b.drawSelf();
        b1.drawSelf();
        b2.drawSelf();
    glPopMatrix();

    glutPostRedisplay();
}

//Funkcija koja crta koordinatni sistem, sluzi kao pomoc pri debagovanju.
void drawCoord(){
    glLineWidth(1);
    glColor3f(0, 0, 1.0);
    //X AXIS IS BLUE
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(300.0, 0, 0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(-300.0, 0, 0);
    glEnd();
    glColor3f(0, 1.0, 0);
    //Y AXIS IS GREEN
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 300.0, 0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -300.0, 0);
    glEnd();

    glColor3f(1.0, 0, 0);
    //Z AXIS IS RED
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 300.0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -300.0);
    glEnd();
}

//Vraca vektor pogleda
Vec2 getViewDirection(){
    Vec2 v(lookingAtX - lookingFromX, lookingAtY - lookingFromY);
    v.normalize();

    return v;
}

//Inicijalizacija svega
void initAll(double tl){
    
    tableLength = tl;
    tableWidth = tableLength * tableRatio;

    
    tableEdgeUp = tableLength/2;
    tableEdgeDown = - tableEdgeUp;
    tableEdgeRight = tableWidth/2;
    tableEdgeLeft = -tableEdgeRight;
    
    //Pocetna pozicija kamere
    camR = tableLength;
    camTheta = 1;
    camRho = 1;
    
    shotStrength = 0;

    //Odnos izracunat na osnovu realnih mera
    ballRadius = tableLength * 0.02182285/2;
}
