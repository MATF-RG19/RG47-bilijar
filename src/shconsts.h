#ifndef SHCONSTS_H
#define SHCONSTS_H

    #include <GL/glut.h>

    #define REDRRAW_BALLS (0)
    #define REDRRAW_BALLS_INTERVAL (5)

    #define TABLEOFF1 (2)
    #define TABLEOFF2 (4)

    #define GAMEMODE (1)

    #define NINETY_DEGREES (1.5708)
    #define FOURTY_FIVE_DEGREES (0.785398)
    #define TWENTY_DEGREES (0.349066)
    #define FIVE_DEGREES (0.0872665)

    #define HOLE_TOLERANCE (3.5)

    static const double tableRatio = 0.5;
    static const int tableHeight = 73;

    double tableEdgeUp;
    double tableEdgeDown;
    double tableEdgeLeft;
    double tableEdgeRight;

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

#endif