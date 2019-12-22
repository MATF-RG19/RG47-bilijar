#include "Game.h"
#include "shconsts.h"
Game::Game(){

}

Game::Game(double tl){
    this->tableLength = tl;
    initAll();
}

void Game::initAll(){
    tableWidth = tableLength * tableRatio;
    
    tableEdgeUp = tableLength/2;
    tableEdgeDown = - tableEdgeUp;
    tableEdgeRight = tableWidth/2;
    tableEdgeLeft = -tableEdgeRight;
    
    ballRadius = tableLength * 0.02182285/2;
    pocketRadius = 2.3*ballRadius;
    
    ballLimUp = tableEdgeUp - ballRadius;
    ballLimDown = tableEdgeDown + ballRadius;
    ballLimLeft = tableEdgeLeft + ballRadius;
    ballLimRight = tableEdgeRight - ballRadius;

    cam = CamCord(tableLength, -M_PI_2, 1);

    sLims.lo = 0.1;
    sLims.hi = 3.9;
    setStandardLimitsAndVals();

    ambientLength = rLims.hi*1.2;
    ambientWidth =  rLims.hi*1.2;
    ambientHeight = tableHeight + rLims.hi;

    persp = 3*ambientWidth;
    shotStrength = 1.9;
    

    deltaTheta = .02;
    fineTune = false;
    
    shotModeCamR = 16*ballRadius;

    pillarConst1 = 1.5*ballRadius;
    pillarConstA1 = tableEdgeLeft*0.8 + pillarConst1;
    pillarConstA2 = tableEdgeRight*0.8 - pillarConst1;
    pillarConstB1 = tableEdgeUp*0.75 - pillarConst1;
    pillarConstB2 = tableEdgeDown*0.75 + pillarConst1;

    tableBasisScaleX = 0.8*tableWidth;
    tableBasisScaleY = 0.8*tableLength;
    tableBasisScaleZ = (tableHeight + TABLEOFF2)/4;

    tableBasisShiftZ = tableHeight*7/8 - TABLEOFF2;

    toCtlModePlaceCueball();

    controlModeDelta = ballRadius/2;
    fillCluster();
}

void Game::setStandardLimitsAndVals(){
    cam.camRho = FOURTY_FIVE_DEGREES;
    cam.camR = tableLength;
    
    rLims.lo = ballRadius;
    rLims.hi = 1.2*tableLength;
    rhoLims.lo = TWENTY_DEGREES;
    rhoLims.hi = NINETY_DEGREES;

}

void Game::setShotModeLimitsAndVals(){
    cam.camRho = NINETY_DEGREES - TWENTY_DEGREES;
    cam.camR= 16*ballRadius;

    rLims.lo] = 3*ballRadius;
    rLims.hi = 25*ballRadius;
    rhoLims.lo = FOURTY_FIVE_DEGREES;
    rhoLims.hi = NINETY_DEGREES - FIVE_DEGREES;
}

void Game::fillCluster(){
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