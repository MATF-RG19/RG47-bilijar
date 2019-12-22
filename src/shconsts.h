#ifndef SHCONSTS_H
#define SHCONSTS_H

    #include <GL/glut.h>
    #include <vector>

    #define REDRRAW_BALLS (0)
    #define REDRRAW_BALLS_INTERVAL (15)

    #define ANIMATE_STARTING_SHOT (1)
    #define ANIMATE_STARTING_SHOT_INTERVAL (16)

    #define TABLEOFF1 (2)
    #define TABLEOFF2 (4)

    #define GAMEMODE (0)

    #define NINETY_DEGREES (1.5708)
    #define FOURTY_FIVE_DEGREES (0.785398)
    #define TWENTY_DEGREES (0.349066)
    #define FIVE_DEGREES (0.0872665)


    #define PI (3.14159265359)
    #define MINPI (-3.14159265359)

    #define TWOPI (6.28318530718)
    #define MINTWOPI (-6.28318530718)

    #define PITWO (1.57079632679)
    #define MINPITWO (-1.57079632679)

    #define PIFOUR (0.78539816339)
    #define FIVEPIFOUR (3.92699081699)

    #define TRHEEPIFOUR (2.35619449019)
    #define SEVENPIFOUR (5.49778714378)

    #define POCKET_TOLERANCE (2)
    
    #define CLOTH_TEXTURE_PATH "cloth_text.bmp"


    #define CTL_MODE_REGULAR (0)
    #define CTL_MODE_PLACE_CUEBALL (1)

    


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