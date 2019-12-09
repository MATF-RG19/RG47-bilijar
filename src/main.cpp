#include <iostream>
#include <vector>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include <GL/glut.h>

#include "Vec2.h"
#include "Ball.h"
#include "putils.h"
#include "shconsts.h"

using namespace std;

// =================================================== DEKLARACIJE ===================================================

//Vektor koji sadrzi kugle
vector<Ball>balls;         

// =================== EVENT FUNCTIONS ===================
static void on_display(void);
static void on_reshape(int, int);
static void on_keyboard(unsigned char key,int x, int y);
void mainTimerCallBack(int);

// =================== INITIALIZING FUNCTIONS ===================
void initAll(double);
void fillCluster();

// =================== DRAW FUNCTIONS ===================
void directCamera();
void drawTable();
void drawCoord();
void drawBalls();
void drawAim();
void drawHud();
void testBalls();

// =================== MISC FUNCTIONS ===================
bool anyBallsMoving();
Vec2 getViewDirection();
void setStandardLimitsAndVals();
void setShotModeLimitsAndVals();
void toggleFineTune();

// =================== TEXT DRAW FUNCTIONS ===================
void output(double x, double y, float r, float g, float b, void * font, string s);
string getShotStrengthString();
string getFineTuneString();
string getShotModeString();











// =================================================== IMPLEMENTACIJE ===================================================

int main(int argc, char ** argv){

    cout << getpid() << endl;    

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    //glutInitWindowSize(1000, 1000);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(200, 200);
    glutCreateWindow(argv[0]);

    if(GAMEMODE == 0){
        initAll(233);
    }else{
        initAll(350);
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    glutDisplayFunc(on_display);
    glutReshapeFunc(on_reshape);
    glutKeyboardFunc(on_keyboard);

    glClearColor(0.75, 0.75, 0.75, 0);
    glEnable(GL_DEPTH_TEST);
    
    //glEnable(GL_COLOR_MATERIAL);

    glutMainLoop();
    return 0;
}

// =================== EVENT FUNCTIONS ==================

static void on_display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, wwidth/wheight, 1, 800); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    directCamera();
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    drawTable();      
    drawBalls();
    if(inShotMode) drawAim();

    drawHud();
    glutSwapBuffers();
}
static void on_reshape(int x, int y){
    wwidth = x;
    wheight = y;
    glViewport(0, 0, x, y);
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
            }else{
            }
            glutPostRedisplay();
            break;
        case 'S':
        case 's':
            if(withinBounds(&camRho, RhoLimit)){
                camRho += .02;
            }else{
            }
            glutPostRedisplay();
            break;
        case 'I':
        case 'i':
            if(withinBounds(&camR, Rlimit)){
                camR -= ballRadius;
            }
            glutPostRedisplay();
            break;
        case 'K':
        case 'k':
            if(withinBounds(&camR, Rlimit)){
                camR += ballRadius;
            }
            glutPostRedisplay();
            
            break;
        case '+':
            if(shotStrength < Slimit[1]){
                shotStrength += 0.1;
                glutPostRedisplay();
            }
            break;
        case '-':
            if(shotStrength > Slimit[0]){
                shotStrength -= 0.1;
                glutPostRedisplay();
            }
            break;
        case 'N':
        case 'n':
            if(anyBallsMoving()) break;
            fillCluster();
            inShotMode = false;
            break;

        case 'x':
        case 'X':
            if(anyBallsMoving()) break;
            if(!inShotMode){
                inShotMode = true;
                setShotModeLimitsAndVals();
            }else{
                inShotMode = false;
                setStandardLimitsAndVals();
                if(fineTune){
                    toggleFineTune();
                }
            }
            glutPostRedisplay();
            break;
        case 'f':
        case 'F':
            if(!inShotMode && !fineTune) break;
            toggleFineTune();
            glutPostRedisplay();
            break;
        case 'H':
        case 'h':
            if(!inShotMode) break;
            if(anyBallsMoving()) break;
            
            inShotMode = false;

            setStandardLimitsAndVals();

            if(fineTune) toggleFineTune();

            Vec2 shotDir = getViewDirection();
            shotDir.mult(shotStrength*ballRadius);

            balls[0].setVelocity(shotDir);
            glutTimerFunc(REDRRAW_BALLS_INTERVAL, mainTimerCallBack, REDRRAW_BALLS);
            break;
        
    }
}
void mainTimerCallBack(int arg){
    switch (arg){
        case REDRRAW_BALLS:
            for (int i = 0; i < balls.size(); i++){
                if(balls[i].getOnTable() && balls[i].getMoving()){
                    if (balls[i].pocketCollide(tableEdgeUp, tableEdgeDown, tableEdgeLeft, tableEdgeRight, pocketRadius)){
                        balls[i].dieCompletely(&activityCheck);
                        continue;
                    }
                    balls[i].cushionCollide(ballLimUp, ballLimDown, ballLimLeft, ballLimRight);
                    for(int j = 0; j < balls.size(); j++){
                        if(i == j || !balls[j].getOnTable()){
                            continue;
                        }
                        balls[i].collideWith(balls[j]);
                    }
                    balls[i].updateSelf(&activityCheck);
                }
            }
            
            glutPostRedisplay();
            
            if(activityCheck != 0){
                glutTimerFunc(REDRRAW_BALLS_INTERVAL, mainTimerCallBack, arg);
            }else{
                cout << "======= DONE =======" << endl;
                shotStrength = 1.9;
                if(!balls[0].getOnTable()){
                    balls[0] = Ball(Vec2(0, tableEdgeDown/2), Vec2(0, 0), ballRadius, 1, 1, 1, 0);
                }
                
                for(int i = 1; i < balls.size(); i++){
                    if (balls[i].getOnTable()) return;
                }
                fillCluster();
            }
            break;
    }
}


// =================== DRAW FUNCTIONS ===================
//Usmeravanje pogleda
void directCamera(){
    /**
     * Usmeravanje pogleda, zavisi od rezima igre.
     * Ukoliko igrac nije u rezimu udarca tada se gleda iz neke tacke na sferi sa centrom u centru povrsine stola u centar stola
     * Ukoliko igrac jeste u rezimu udarca tada se gleda iz neke tacke na sferi sa centrom u centru bele kugle u centar bele kugle
     * 
     * 
     * **/

    lookingFromZ = camR*cos(camRho) + tableHeight;
    lookingFromX = camR*cos(camTheta)*sin(camRho);
    lookingFromY = camR*sin(camTheta)*sin(camRho);
    lookingAtX = 0;
    lookingAtY = 0;
    lookingAtZ = tableHeight;
    if(inShotMode){
        Vec2 ps = balls[0].getPosition();
        lookingFromX += ps.x;
        lookingFromY += ps.y;

        lookingAtX = ps.x;
        lookingAtY = ps.y;
    }
    gluLookAt (
        lookingFromX, lookingFromY, lookingFromZ,
        lookingAtX, lookingAtY, lookingAtZ,
        0, 0, 1
    );   
}
//Iscrtava sto
void drawTable(){

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, table_ambient_material);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, table_diffuse_material);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, table_specular_material);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, table_shininess);
    //Povrsina stola
    glPushMatrix();
        glTranslated(0, 0, tableHeight - TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(tableWidth, tableLength, TABLEOFF2); //Skaliranje po dimenzijama
        glutSolidCube(1);     
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, pocket_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, pocket_diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, pocket_specular_material);

    //Rupe
    glPushMatrix();
        glTranslated(0, 0, tableHeight+0.1);
        glPushMatrix();
            glTranslated(tableEdgeLeft, tableEdgeUp, 0);
            drawCircle(pocketRadius);
        glPopMatrix();
        glPushMatrix();
            glTranslated(tableEdgeLeft, tableEdgeDown, 0);
            glColor3f(0, 0, 0);
            drawCircle(pocketRadius);
        glPopMatrix();
        glPushMatrix();
            glTranslated(tableEdgeRight, tableEdgeUp, 0);
            glColor3f(0, 0, 0);
            drawCircle(pocketRadius);
        glPopMatrix();
        glPushMatrix();
            glTranslated(tableEdgeRight, tableEdgeDown, 0);
            glColor3f(0, 0, 0);
            drawCircle(pocketRadius);
        glPopMatrix();
        glPushMatrix();
            glTranslated(tableEdgeLeft - pocketRadius/2, 0, 0);
            glColor3f(0, 0, 0);
            drawCircle(pocketRadius);
        glPopMatrix();

        glPushMatrix();
            glTranslated(tableEdgeRight + pocketRadius/2, 0, 0);
            glColor3f(0, 0, 0);
            drawCircle(pocketRadius);
        glPopMatrix();
    glPopMatrix();


    //Cilindri oko rupa

    glMaterialfv(GL_FRONT, GL_AMBIENT, cyl_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cyl_diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cyl_specular_material);
    glMaterialf(GL_FRONT, GL_SHININESS, cyl_shininess);

    glPushMatrix();
        glTranslated(tableEdgeLeft - pocketRadius/2, 0, tableHeight); 
        glRotated(180, 0, 0, 1);
        glScaled(pocketRadius, pocketRadius, 1);
        draw_object(TABLEOFF2);
    glPopMatrix();

    glPushMatrix();
        glTranslated(tableEdgeRight + pocketRadius/2, 0, tableHeight); 
        //glRotated(180, 0, 0, 1);
        glScaled(pocketRadius, pocketRadius, 1);
        draw_object(TABLEOFF2);
    glPopMatrix();

    glPushMatrix();
        glTranslated(tableEdgeLeft, tableEdgeUp, tableHeight); 
        glRotated(135, 0, 0, 1);
        glScaled(pocketRadius, pocketRadius, 1);
        draw_object(TABLEOFF2);
    glPopMatrix();

    glPushMatrix();
        glTranslated(tableEdgeRight, tableEdgeUp, tableHeight); 
        glRotated(45, 0, 0, 1);
        glScaled(pocketRadius, pocketRadius, 1);
        draw_object(TABLEOFF2);
    glPopMatrix();

    glPushMatrix();
        glTranslated(tableEdgeRight, tableEdgeDown, tableHeight); 
        glRotated(315, 0, 0, 1);
        glScaled(pocketRadius, pocketRadius, 1);
        draw_object(TABLEOFF2);
    glPopMatrix();

    glPushMatrix();
        glTranslated(tableEdgeLeft, tableEdgeDown, tableHeight); 
        glRotated(225, 0, 0, 1);
        glScaled(pocketRadius, pocketRadius, 1);
        draw_object(TABLEOFF2);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_AMBIENT, pocket_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, pocket_diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, pocket_specular_material);
    //Mantinele
    glPushMatrix();
        glTranslated(0, tableEdgeUp + TABLEOFF1, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(tableWidth - POCKET_TOLERANCE*pocketRadius, TABLEOFF2, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(0, tableEdgeDown - TABLEOFF1, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(tableWidth - POCKET_TOLERANCE*pocketRadius, TABLEOFF2, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(tableEdgeRight + TABLEOFF1, tableEdgeUp/2, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(TABLEOFF2, tableLength/2 - POCKET_TOLERANCE*pocketRadius, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(tableEdgeRight + TABLEOFF1, -tableEdgeUp/2, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(TABLEOFF2, tableLength/2 - POCKET_TOLERANCE*pocketRadius, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(tableEdgeLeft - TABLEOFF1, tableEdgeUp/2, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(TABLEOFF2, tableLength/2 - POCKET_TOLERANCE*pocketRadius, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
        glTranslated(tableEdgeLeft - TABLEOFF1, -tableEdgeUp/2, tableHeight + TABLEOFF1); //Translacija na odgovarajucu visinu
        glScaled(TABLEOFF2, tableLength/2 - POCKET_TOLERANCE*pocketRadius, TABLEOFF2); 
        glutSolidCube(1);
    glPopMatrix();
    

	glMaterialfv(GL_FRONT, GL_AMBIENT, base_ambient_material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, base_diffuse_material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, base_specular_material);
	glMaterialf(GL_FRONT, GL_SHININESS, base_shininess);
    //Osnova stola
    glPushMatrix();
        glTranslated(0, 0, (tableHeight - TABLEOFF2)/2);
        glScaled(0.8*tableWidth, 0.8*tableLength, tableHeight - TABLEOFF2);
        glColor3f(0.48,0.46,0.4);
        glutSolidCube(1);
        glColor3f(0, 0, 0);
        glutWireCube(1);
    glPopMatrix(); 
}
//Iscrtava one kugle koje su trenutno aktivne, tj nisu ubacene u rupu
void drawBalls(){
    glTranslated(0, 0,tableHeight + ballRadius);
    for(Ball b: balls){   
        if(b.getOnTable()) b.drawSelf();
    }
    glTranslated(0, 0, - tableHeight - ballRadius);

}
//Iscrtavanje nisana
void drawAim(){
    /**
     * Nisam realizovan kao dve paralelene tangente na belu kuglu u pravcu pogleda
     * 
     * **/
    glDisable(GL_LIGHTING);
    //glEnable(GL_COLOR_MATERIAL);


    Vec2 vv = getViewDirection();
    vv = Vec2(vv.y, - vv.x);
    vv.mult(ballRadius);

    Vec2 cbp = balls[0].getPosition();
    glLineWidth(2);
    glColor3f(0.839215686, 0.28627451, 1);
    glPushMatrix();
        glTranslated(vv.x, vv.y, 0);
        glBegin(GL_LINES);
            glVertex3f(cbp.x, cbp.y, tableHeight+1);
            glVertex3f(cbp.x + (lookingAtX - lookingFromX)*5,cbp.y +  (lookingAtY - lookingFromY)*5, tableHeight + 1);
        glEnd();
        vv.mult(-2);
        glTranslated(vv.x, vv.y, 0);
        glBegin(GL_LINES);
            glVertex3f(cbp.x, cbp.y, tableHeight+1);
            glVertex3f(cbp.x + (lookingAtX - lookingFromX)*5,cbp.y +  (lookingAtY - lookingFromY)*5, tableHeight + 1);
        glEnd();
    glPopMatrix();


    //glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
}
//Pomocna funkcija za crtanje koordinatnog sistema.
//Verovatno ce biti izbrisana do finalne verzije
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
//Ispisivanje podataka relevantnih za korisnika
void drawHud(){
    glPushMatrix();
    glMatrixMode (GL_PROJECTION); 
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor3f(1, 1, 1);
    
    //glDisable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
        output(-0.99, 0.97, 0, 0, 0, GLUT_BITMAP_HELVETICA_18, getShotStrengthString());
        output(-0.99, 0.92, 0, 0, 0, GLUT_BITMAP_HELVETICA_18, getShotModeString());

        if(inShotMode){
            output(-0.99, 0.87, 0, 0, 0, GLUT_BITMAP_HELVETICA_18, "============================");
            output(-0.99, 0.82, 0, 0, 0, GLUT_BITMAP_HELVETICA_18, "H to hit, X again to go back");
            output(-0.99, 0.77, 0, 0, 0, GLUT_BITMAP_HELVETICA_18, getFineTuneString());
        }
    glDisable(GL_COLOR_MATERIAL);
    //glEnable(GL_LIGHTING);
    glPopMatrix();
}

// =================== INITIALIZING FUNCTIONS ===================

//Samo ime kaze, inicijalizacija svega
void initAll(double tl){--
    
    tableLength = tl;
    tableWidth = tableLength * tableRatio;
    
    tableEdgeUp = tableLength/2;
    tableEdgeDown = - tableEdgeUp;
    tableEdgeRight = tableWidth/2;
    tableEdgeLeft = -tableEdgeRight;

    if(GAMEMODE == 1){
        ballRadius = tableLength * 0.007293693;
        pocketRadius = 2*ballRadius;
    }else{
        ballRadius = tableLength * 0.02182285/2;
        pocketRadius = 2.1*ballRadius;
    }    


    ballLimUp = tableEdgeUp - ballRadius;
    ballLimDown = tableEdgeDown + ballRadius;
    ballLimLeft = tableEdgeLeft + ballRadius;
    ballLimRight = tableEdgeRight - ballRadius;


    camR = tableLength;
    camTheta = 1;
    camRho = 1;

    Rlimit[0] = ballRadius;
    Rlimit[1] = 2*tableLength;
    RhoLimit[0] = TWENTY_DEGREES; 
    RhoLimit[1] = NINETY_DEGREES; 

    shotStrength = 0.1;
    Slimit[0] = 0.1;
    Slimit[1] = 3.9;

    deltaTheta = .02;
    fineTune = false;
    
    fillCluster();
    
}
//Popunjavanje vektora balls kuglama sa pozicijama na pocetku bilijarske igre (trougao)
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
            if(ballId == 5){
                r = 0;
                g = 0;
                b = 0;
            }else{
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
            }
            
            
            balls.push_back(Ball(Vec2(currentX, currentY + ballRadius/5), Vec2(0, 0), ballRadius, r, g, b, ballId++));
            currentX += 2*ballRadius;
        }
        numInRow++;
        currentY += dy;

    }
    glutPostRedisplay();

}

// =================== MISC FUNCTIONS ===================
bool anyBallsMoving(){
    return activityCheck != 0;
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
    Rlimit[0] = 3*ballRadius;
    Rlimit[1] = 25*ballRadius;
    RhoLimit[0] = FOURTY_FIVE_DEGREES;
    RhoLimit[1] = NINETY_DEGREES - FIVE_DEGREES;
}
void toggleFineTune(){
    if(fineTune){
        fineTune = false;
        deltaTheta = .02;
    }else{
        fineTune = true;
        deltaTheta = .001;
    }
}

// =================== TEXT DRAW FUNCTIONS ===================
void output(double x, double y, float r, float g, float b, void * font, string s){
    glColor3f( r, g, b );
    glRasterPos2f(x, y);
    int len, i;
    len = s.size();
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(font, s[i]);
    }
}
string getShotStrengthString(){
    return "(  +-  ) Shot strength: " + to_string((int)round(shotStrength/3.9*100)) + "%";
}
string getFineTuneString(){
    if(fineTune) return "(   F   ) Fine tune: ON";
    return "(   F   ) Fine tune: OFF";
}
string getShotModeString(){
    if(inShotMode) return "(   X   ) Shot mode: ON";
    return "(   X   ) Shot mode: OFF";
}