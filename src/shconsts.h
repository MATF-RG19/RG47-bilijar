#ifndef SHCONSTS_H
#define SHCONSTS_H

    #include <GL/glut.h>
    #include <vector>

    #define REDRRAW_BALLS (0)
    #define REDRRAW_BALLS_INTERVAL (15)

    #define TABLEOFF1 (2)
    #define TABLEOFF2 (4)

    #define GAMEMODE (0)

    #define NINETY_DEGREES (1.5708)
    #define FOURTY_FIVE_DEGREES (0.785398)
    #define TWENTY_DEGREES (0.349066)
    #define FIVE_DEGREES (0.0872665)

    #define POCKET_TOLERANCE (2)
    
    #define CLOTH_TEXTURE_PATH "cloth_text.bmp"
    double wwidth;
    double wheight;

    static const double tableRatio = 0.5;
    static const int tableHeight = 73;

    

    /**
     * SFERNE KOORDINATE KAMERE
     * R - poluprecnik
     * Theta - polarni ugao
     * Rho - ugao u odnosu na pozitivan deo z-ose
     * 
     * **/
    double camR;                
    double camTheta;
    double camRho;
    //Ogranicenja sfernih koordinata
    double Rlimit[2];
    double RhoLimit[2];
    double Slimit[2];
    double deltaTheta;          


    /**
     * Dimenzije stola (duzina, sirina)
     * Ivice stola, koristi se u detekciji kolizija
     * 
     * **/
    double tableLength;         
    double tableWidth;
    double tableEdgeUp;
    double tableEdgeDown;
    double tableEdgeLeft;
    double tableEdgeRight;


    double ambientWidth;
    double ambientLength;
    double ambientHeight;
    
    /**
     * Poluprecnik kugle i poluprecnik rupe
     * ballLim__ predstavljaju granice (gore dole levo desno) dokle kugla sme da ide
     * u principu ballLim__ = tableEdge__ - ballRadius
     * */
    double ballRadius;     
    double pocketRadius;
    double ballLimUp, ballLimDown, ballLimLeft, ballLimRight;
    /**
     * 
     * lookingFromXYZ - koordinate tacke iz koje se trenutno gleda
     * lookingAtXYZ - koordinate tacke u koju se gleda
     * **/
    double lookingAtX;  
    double lookingAtY;
    double lookingAtZ;

    double lookingFromX;
    double lookingFromY;
    double lookingFromZ;


    //Predstavlja meru jacine udarca. Meri se u "poluprecnicima kugle"
    double shotStrength;

    //inShotMode - rezim udarca, od njega zavisi pogled i mogucnost izvodjenja udarca
    bool inShotMode = false;
    bool fineTune = false;
    bool fullScreen = true;

    /**
     * 
     * Koristi se za proveru da li se bilo koja kugla trenutno krece da bi se znalo
     * da li ponovo treba pozvati tajmer
     * i-ti bit s leve strane odgovoara i-toj kugli u vektoru 'balls' i on je 0 ukoliko 
     * kugla stoji i 1 ukoliko se krece. Svaka kugla sama ima svoje bitmaske koje koristi
     * za manipulaciju ovog broja.
     * 
     * Kada je activityCheck == 0 tada se nijedna kugla ne krece
     * **/
    unsigned int activityCheck = 0;


    //Konstante osvetljenja
    GLfloat light_position[] = { 0, 0, 2*tableHeight, 0};
    GLfloat ambient_light[] = { 0.3, 0.3, 0.3, 1 };
    GLfloat diffuse_light[] = { 0.95, 0.95, 0.95 ,1 };
    GLfloat specular_light[] = { 1, 1, 1, 1 };

    GLfloat table_ambient_material[] = { 0, 1, 0, 1 };
    GLfloat table_diffuse_material[] = { 0,1,0 };
    GLfloat table_specular_material[] = { 0, 0, 0, 1 };
    GLfloat table_shininess = 2;

    GLfloat pocket_ambient_material[] = { 0, 0, 0, 1 };
    GLfloat pocket_diffuse_material[] = { 1,1,1 };
    GLfloat pocket_specular_material[] = { 1, 1, 1, 1 };
    GLfloat pocket_shininess = 10;

    //glColor3f(0.30859375,0.140625,0.0703125);
    GLfloat base_ambient_material[] = { 0.30859375,0.140625,0.0703125,1 };
    GLfloat base_diffuse_material[] = { 0.30859375,0.140625,0.0703125, 1};
    GLfloat base_specular_material[] = {0.7, 0.5, 0.7, 1  };
    GLfloat base_shininess = 10;

    GLfloat trans_ambient_material[] = { 0.48,0.46,0.4,0.2 };
    GLfloat trans_diffuse_material[] = { 0.48,0.46,0.4,0.2};
    GLfloat trans_specular_material[] = { 1, 0, 0, 0.2 };

    GLfloat cyl_ambient_material[] = { 0.396078431,0.258823529,0.168627451, 1 };
    GLfloat cyl_diffuse_material[] = {  0.396078431,0.258823529,0.168627451, 1};
    GLfloat cyl_specular_material[] = { 1, 1, 1, 1 };
    GLfloat cyl_shininess = 10;
#endif