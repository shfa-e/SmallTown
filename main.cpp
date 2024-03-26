#include <iostream>
#include <GLUT/GLUT.h>
#include <OpenGL/glu.h>
#include <GLFW/glfw3.h>

#include <AudioToolbox/AudioToolbox.h> // sound on mac


//#include <windows.h> // sound for winodws
//#include <MMSystem.h>

#include <math.h>
#include <iostream>

using namespace std;

// window size
GLint win_width = 900, win_hight = 900;

// camera movement
GLfloat cameraPosition[] = {1.2, 1.0, 1.8};


// texture buffers
GLuint myTexture1;
GLuint myTexture2;
GLuint myTexture3;
GLuint myTexture4;
GLuint myTexture5;
GLuint myTexture6;
GLuint myTexture7;


char image1Path[] = "/Users/Shefaa/Desktop/Uni/CS/Year 3/Project/building1.bmp"; // building
char image2Path[] = "/Users/Shefaa/Desktop/Uni/CS/Year 3/Project/ground.bmp";  // ground
char image3Path[] = "/Users/Shefaa/Desktop/Uni/CS/Year 3/Project/sun.bmp";  // sun
char image4Path[] = "/Users/Shefaa/Desktop/Uni/CS/Year 3/Project/grass.bmp";  // grass
char image5Path[] = "/Users/Shefaa/Desktop/Uni/CS/Year 3/Project/roof1.bmp";  // roof
char image6Path[] = "/Users/Shefaa/Desktop/Uni/CS/Year 3/Project/wall.bmp"; // wall
char image7Path[] = "/Users/Shefaa/Desktop/Uni/CS/Year 3/Project/road.bmp";// road

GLfloat light_ambient[] = { 1, 1, 1, 1.0 };
GLfloat light_diffuse[] = { 0.8, 1, 1, 1.0 };
GLfloat light_specular[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light_position[] = { -8, 5, 1, 1.0 }; //-0.4, 1.1, -0.4

GLfloat light_cloudamb[] = { 0.7, 0.7, 0.7, 1 };
GLfloat light_clouddif[] = { 0.9, 1, 1, 1 };
GLfloat light_cloudposition[] = { -0.8, 3.2, 1.0, 1 };

//GLfloat mat_amb[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_dif[] = { 1, 1, 1, 1.0 };
GLfloat mat_spec[] = { 0.7, 0.7, 0.7, 1.0};
GLfloat mat_shin[] = { 128 };


GLuint LoadTexture(const char* filename, int width, int height) {

    //The following code will read in our RAW file
    FILE* file = fopen(filename, "rb");

    if (file == NULL) {
        cout << "Unable to open the image file" << endl << "Program will exit :(" << endl;
        exit(0);
    }

    unsigned char* data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    // reorder the image colors to RGB not BGR
    for (int i = 0; i < width * height; ++i) {
        int index = i * 3;
        unsigned char B = data[index], R = data[index + 2];
        data[index] = R, data[index + 2] = B;
    }

    GLuint texture;
    glGenTextures(1, &texture);  //generate the texture with the loaded data
    glBindTexture(GL_TEXTURE_2D, texture); //bind the texture to it's array

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //set texture environment parameters

    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // sphere
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    free(data); //free the texture array

    if (glGetError() != GL_NO_ERROR)
        printf("GLError in genTexture()\n");

    if (texture == -1)
        cout << "Error in loading the texture" << endl;

    return texture; //return whether it was successfull
}

void init() {
    // backgroud colour
    glClearColor(0.53, 0.78, 0.89, 0);

    // transparent around the sun
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65, ((double)win_width / win_hight), 0.01f, 50);

    // Texture
    myTexture1 = LoadTexture(image1Path, 1003, 1200); // building
    if (myTexture1 == -1){
        cout << "Error in loading the texture" << endl;
    }else
        cout << "The texture value is: " << myTexture1 << endl;
    
    myTexture2 = LoadTexture(image2Path, 2000, 600); // ground
    if (myTexture2 == -1){
        cout << "Error in loading the texture" << endl;
    }else
        cout << "The texture value is: " << myTexture2 << endl;
    
    myTexture3 = LoadTexture(image3Path, 500, 500); // sun
    if (myTexture3 == -1){
        cout << "Error in loading the texture" << endl;
    }else
        cout << "The texture value is: " << myTexture3 << endl;
    
    myTexture4 = LoadTexture(image4Path, 600, 400); // grass
    if (myTexture4 == -1){
        cout << "Error in loading the texture" << endl;
    }else
        cout << "The texture value is: " << myTexture4 << endl;
    
    myTexture5 = LoadTexture(image5Path, 735, 770); // roof
    if (myTexture5 == -1){
        cout << "Error in loading the texture" << endl;
    }else
        cout << "The texture value is: " << myTexture5 << endl;

    myTexture6 = LoadTexture(image6Path, 612, 408); // wall
    if (myTexture6 == -1){
        cout << "Error in loading the texture" << endl;
    }else
        cout << "The texture value is: " << myTexture6 << endl;
    
    myTexture7 = LoadTexture(image7Path, 626, 417); // road
    if (myTexture7 == -1){
        cout << "Error in loading the texture" << endl;
    }else
        cout << "The texture value is: " << myTexture7 << endl;

    
}


// camera movements
void key(unsigned char keyPressed, int x, int y) {
    
    switch(keyPressed){
            
        case '+':
            cameraPosition[2] -= 0.1;
            printf("camera position: (%0.1f, %0.1f, %0.1f)\n", cameraPosition[0], cameraPosition[1], cameraPosition[2]);
            break;
            
        case '-':
            cameraPosition[2] += 0.1;
            printf("camera position: (%0.1f, %0.1f, %0.1f)\n", cameraPosition[0], cameraPosition[1], cameraPosition[2]);
            break;
            
        case 'q':
        case 27:
        exit(0);
            break;
        
        default:
        fprintf(stderr, "\nKeyboard commands:\n\n"
        "q, <esc> - Quit\n"); break;
            
    }
}


void specialKey(int key, int x, int y) {

    if (key == GLUT_KEY_RIGHT)
        cameraPosition[0] = cameraPosition[0] += 0.1;

    else if (key == GLUT_KEY_LEFT)
        cameraPosition[0] = cameraPosition[0] -= 0.1;

    else if (key == GLUT_KEY_UP)
        cameraPosition[1] = cameraPosition[1] += 0.1;

    else if (key == GLUT_KEY_DOWN)
        cameraPosition[1] = cameraPosition[1] -= 0.1;

    // print camera posion
    printf("camera position: (%0.1f, %0.1f, %0.1f)\n",
        cameraPosition[0], cameraPosition[1], cameraPosition[2]);
}


// texture on quad
void quadTexture(float v1[], float v2[], float v3[], float v4[], GLuint texture) {

    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(1, 1); glVertex3fv(v1);
    glTexCoord2f(0, 1); glVertex3fv(v2);
    glTexCoord2f(0, 0); glVertex3fv(v3);
    glTexCoord2f(1, 0); glVertex3fv(v4);
    glEnd();
}

// translate then scale
void translateAndScale(GLfloat tx, GLfloat ty, GLfloat tz, GLfloat s, void(*shapeFunction)()) {
    glPushMatrix();
    glTranslatef(tx, ty, tz);
    glScalef(s, s, s);
    shapeFunction();
    glPopMatrix();
}

// translate the function
void translate(GLfloat x, GLfloat y, GLfloat z, void(*shapeFunction)()) {
    glPushMatrix();
    glTranslatef(x, y, z);
    shapeFunction();
    glPopMatrix();
}

// camera
void camera(void) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], 0, 0, 0, 0, 1, 0);
}

void reshapeFunc(GLint new_width, GLint new_hight) {
    glViewport(0, 0, new_width, new_hight);
    win_width = new_width;
    win_hight = new_hight;
}

// ground
void ground() {

    glColor3f(0.6, 0.72, 0.45);

    float A[] = { -0.75, 0.25, -0.5 };
    float B[] = { 0.75, 0.25, -0.5 };
    float C[] = { -0.75, 0.25,  0.5 };
    float D[] = { 0.75, 0.25,  0.5 };

    float E[] = { -0.75, 0.2, -0.5 };
    float F[] = { 0.75, 0.2, -0.5 };
    float G[] = { -0.75, 0.2,  0.5 };
    float H[] = { 0.75, 0.2,  0.5 };

    glEnable(GL_TEXTURE_2D);
    quadTexture(C, A, E, G, myTexture2);
    quadTexture(A, B, F, E, myTexture2);
    quadTexture(D, B, F, H, myTexture2);
    quadTexture(C, D, H, G, myTexture2);
    quadTexture(A, B, D, C, myTexture4); // top
    quadTexture(E, F, H, G, myTexture2);
    glDisable(GL_TEXTURE_2D);
}


// sun
void sun(void) {

    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, myTexture3);
    
    glColor3f(1, 1, 0); // yellow

    GLUquadric* quadratic = gluNewQuadric();
    gluQuadricTexture(quadratic, GL_TRUE);        // Texture
    gluQuadricNormals(quadratic, GLU_SMOOTH);    // Smooth
    gluSphere(quadratic, 0.15, 20, 20);
    
    glDisable(GL_TEXTURE_2D);

    // transparent sphere around dun
    glEnable(GL_BLEND);
    glColor4f(1, 1, 0, 0.3);
    glutSolidSphere(0.165, 20, 20);
    glDisable(GL_BLEND);
}

void building() {

    glColor3f(0.54, 0.56, 0.57); // grey

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture1);
    
    //front face
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    
    glTexCoord2f(1, 0);
    glVertex3f(0.5, 0, 0);
    
    glTexCoord2f(1, 1);
    glVertex3f(0.5, 1, 0);
    
    glTexCoord2f(0, 1);
    glVertex3f(0, 1, 0);
    glEnd();
    

    //back face
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, -0.45);
    
    glTexCoord2f(1, 0);
    glVertex3f(0.5, 0, -0.45);
    
    glTexCoord2f(1, 1);
    glVertex3f(0.5, 1, -0.45);
    
    glTexCoord2f(0, 1);
    glVertex3f(0, 1, -0.45);
    glEnd();

    //left face
    glBegin(GL_POLYGON);
    glTexCoord2f(1, 0);
    glVertex3f(0.5, 0, 0);
    
    glTexCoord2f(1, 0);
    glVertex3f(0.5, 0, -0.45);
    
    glTexCoord2f(1, 1);
    glVertex3f(0.5, 1, -0.45);
    
    glTexCoord2f(1, 1);
    glVertex3f(0.5, 1, 0);
    glEnd();

    //right face
    glBegin(GL_POLYGON);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);
    
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, -0.45);
    
    glTexCoord2f(0, 1);
    glVertex3f(0, 1, -0.45);
    
    glTexCoord2f(0, 1);
    glVertex3f(0, 1, 0);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}

// group of buildings
void buildings() {
    
    // big middle
    glPushMatrix();
    glScalef(1.2, 1.2, 1.2);
    glTranslatef(0, 0, 0.4);
    building();
    glPopMatrix();
    
    // right back
    glPushMatrix();
    glTranslatef(-0.6, 0, -0.05);
    building();
    glPopMatrix();
    
    // right front
    glPushMatrix();
    glScalef(0.95, 0.95, 0.95);
    glTranslatef(-0.75, 0, 0.9);
    building();
    glPopMatrix();
    
    // middle left small
    glPushMatrix();
    glTranslatef(1.2, 0, 0.6);
    building();
    glPopMatrix();
    
    //front left small
    glPushMatrix();
    glScalef(0.9, 0.9, 0.9);
    glTranslatef(1, 0, 1.3);
    building();
    glPopMatrix();
    
    // back left
    glPushMatrix();
    glTranslatef(0.7, 0, -0.1);
    building();
    glPopMatrix();
}


void cloud(void) {

    // left
    glPushMatrix();
    glTranslated(-0.5, -0.1, 0);
    glutSolidSphere(0.35, 20, 20);
    glPopMatrix();

    // above right
    glPushMatrix();
    glTranslated(0.4, 0.2, 0);
    glutSolidSphere(0.25, 20, 20);
    glPopMatrix();

    // above left
    glPushMatrix();
    glTranslated(-0.4, 0.2, 0);
    glutSolidSphere(0.25, 20, 20);
    glPopMatrix();

    // middle
    glutSolidSphere(0.5, 20, 20);

    // right
    glPushMatrix();
    glTranslated(0.5, -0.1, 0);
    glutSolidSphere(0.35, 20, 20);
    glPopMatrix();
}


// group of clouds
void clouds(void) {

    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);

    glColor3f(1, 1, 1); // white
    translateAndScale(0.25, 0.9, -0.2, 0.1, cloud);  // cloud 1
    translateAndScale(-0.6, 1.0, -0.3, 0.2, cloud); // cloud 2
    translateAndScale(0.45, 0.95, -0.25, 0.15, cloud); // cloud 3
    translateAndScale(-0.4, 1.1, -0.4, 0.15, cloud); // cloud 4
    translateAndScale(-0.06, 1, -0.5, 0.2, cloud); // cloud 5
    translateAndScale(0.25, 1, -0.3, 0.12, cloud); // cloud 6

    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHTING);
}


void tree() {

    // trunk
    GLUquadric* trunk = gluNewQuadric();
    glColor3f(0.5, 0.3, 0.1);
    glPushMatrix();
    glTranslatef(0.1, 0.49, 0.13);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(trunk, 0.04, 0.04, 0.4, 20, 20);
    glPopMatrix();

    // upper cone
    glColor4f(0.2, 0.4, 0.1, 1);
    glPushMatrix();
    glTranslatef(0.1, 1.25, 0.14);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glutSolidCone(0.15, 0.3, 10, 10);
    glPopMatrix();

    // middle cone
    glColor4f(0.2, 0.3, 0.1, 1);
    glPushMatrix();
    glTranslatef(0.1, 1.07, 0.14);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glutSolidCone(0.18, 0.3, 10, 10);
    glPopMatrix();

    // lower cone
    glColor4f(0.2, 0.4, 0.1, 1);
    glPushMatrix();
    glTranslatef(0.1, 0.9, 0.14);
    glRotatef(-90, 1.0, 0.0, 0.0);
    glutSolidCone(0.19, 0.3, 10, 10);
    glPopMatrix();
    
    
}

// home
void home() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture6);
    
    // main rec
    glColor3f(0.5, 0.75, 0.35);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.2, 0.1);
    glVertex3f(0.2, 0.1, 0.0);
    
    glTexCoord2f(0.9, 0.1);
    glVertex3f(0.9, 0.1, 0.0);
   
    glTexCoord2f(0.9, 0.6);
    glVertex3f(0.9, 0.575, 0.0);
    
    glTexCoord2f(0.2, 0.6);
    glVertex3f(0.2, 0.575, 0.0);
    glEnd();

    //back rec
    glColor3f(0.5, 0.75, 0.35);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.2, 0.1);
    glVertex3f(0.2, 0.1, 0.5);
    
    glTexCoord2f(0.9, 0.1);
    glVertex3f(0.9, 0.1, 0.5);
    
    glTexCoord2f(0.9, 0.6);
    glVertex3f(0.9, 0.575, 0.5);
    
    glTexCoord2f(0.2, 0.6);
    glVertex3f(0.2, 0.575, 0.5);
    glEnd();

    // left rec
    glColor3f(0.75, 0.75, 0.25);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.2, 0.1);
    glVertex3f(0.2, 0.1, 0.5);
    
    glTexCoord2f(0.2, 0.1);
    glVertex3f(0.2, 0.1, 0.0);
    
    glTexCoord2f(0.2, 0.6);
    glVertex3f(0.2, 0.575, 0.0);
    
    glTexCoord2f(0.2, 0.6);
    glVertex3f(0.2, 0.575, 0.5);
    glEnd();

    // right rec
    glColor3f(0.75, 0.75, 0.25);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.9, 0.1);
    glVertex3f(0.9, 0.1, 0.5);
    
    glTexCoord2f(0.9, 0.1);
    glVertex3f(0.9, 0.1, 0.0);
    
    glTexCoord2f(0.9, 0.6);
    glVertex3f(0.9, 0.575, 0.0);
    
    glTexCoord2f(0.9, 0.6);
    glVertex3f(0.9, 0.575, 0.5);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);

    // left tri
    glColor3f(0.5, 0.5, 0.25);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.9, 0.575, 0);
    glVertex3f(0.9, 0.575, 0.5);
    glVertex3f(0.9, 0.8, 0.25);
    glEnd();

    // right tri
    glColor3f(0.5, 0.5, 0.25);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.2, 0.575, 0);
    glVertex3f(0.2, 0.575, 0.5);
    glVertex3f(0.2, 0.8, 0.25);
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture5);

    // roof
    glColor3f(0.55, 0.35, 0.2);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.2, 0.6);
    glVertex3f(0.2, 0.575, 0);
    
    glTexCoord2f(0.9, 0.6);
    glVertex3f(0.9, 0.575, 0);
    
    glTexCoord2f(0.9, 0.8);
    glVertex3f(0.9, 0.8, 0.25);
    
    glTexCoord2f(0.2, 0.8);
    glVertex3f(0.2, 0.8, 0.25);
    glEnd();

    //back roof
    glColor3f(.55, 0.35, .2);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.2, 0.6);
    glVertex3f(0.2, 0.575, 0.5);
   
    glTexCoord2f(0.9, 0.6);
    glVertex3f(0.9, 0.575, 0.5);
    
    glTexCoord2f(0.9, 0.8);
    glVertex3f(0.9, 0.8, 0.25);
    
    glTexCoord2f(0.2, 0.8);
    glVertex3f(0.2, 0.8, 0.25);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // door
    glColor3f(0.15, 0.2, 0.3);
    glBegin(GL_POLYGON);
    glVertex3f(0.47, 0.105, -0.01);
    glVertex3f(0.65, 0.105, -0.01);
    glVertex3f(0.65, 0.46, -0.01);
    glVertex3f(0.47, 0.46, -0.01);
    glEnd();

    // window 1
    glColor3f(0.3, 0.2, 0.1);
    glBegin(GL_POLYGON);
    glVertex3f(0.72, 0.25, -0.01);
    glVertex3f(0.83, 0.25, -0.01);
    glVertex3f(0.83, 0.4, -0.01);
    glVertex3f(0.72, 0.4, -0.01);
    glEnd();

    // window 2
    glColor3f(.3, 0.2, .1);
    glBegin(GL_POLYGON);
    glVertex3f(0.27, 0.25, -0.01);
    glVertex3f(0.38, 0.25, -0.01);
    glVertex3f(0.38, 0.4, -0.01);
    glVertex3f(0.27, 0.4, -0.01);
    glEnd();
}

void houses(void) {
    
    // house 1
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glTranslatef(3.1, 1, -1.5);
    glRotated(180, 0, 1, 0);
    home();
    glPopMatrix();

    // house 2
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glTranslatef(2.1, 1, -1.5);
    glRotated(180, 0, 1, 0);
    home();
    glPopMatrix();

    // house 3
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glTranslatef(1.1, 1, -1.5);
    glRotated(180, 0, 1, 0);
    home();
    glPopMatrix();

    // house 4
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glTranslatef(3.7, 1, 0);
    glRotated(180, 0, 1, 0);
    home();
    glPopMatrix();

    // house 5
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glTranslatef(2.7, 1, 0);
    glRotated(180, 0, 1, 0);
    home();
    glPopMatrix();

    // house 6
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glTranslatef(1.7, 1, 0);
    glRotated(180, 0, 1, 0);
    home();
    glPopMatrix();
    
    // house 7
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glTranslatef(3.4, 1, 1.5);
    glRotated(180, 0, 1, 0);
    home();
    glPopMatrix();

    // house 8
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glTranslatef(2.4, 1, 1.5);
    glRotated(180, 0, 1, 0);
    home();
    glPopMatrix();

    // house 9
    glPushMatrix();
    glScalef(0.2, 0.2, 0.2);
    glTranslatef(1.4, 1, 1.5);
    glRotated(180, 0, 1, 0);
    home();
    glPopMatrix();

}

void road(void){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, myTexture7);
    
    glBegin(GL_QUADS);
    glColor3f(0.2, 0.2, 0.2);
    
    glTexCoord2f(-0.17, 0.225);
    glVertex3f(-0.17, 0.255, 0.51); // top left
    
    glTexCoord2f(-0.17, 0.225);
    glVertex3f(0, 0.255, 0.51); // top right
   
    glTexCoord2f(-0.17, 0.225);
    glVertex3f(0, 0.255, -0.51); // bottom right
    
    glTexCoord2f(-0.17, 0.225);
    glVertex3f(-0.17, 0.255, -0.51); // bottom left
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
}


static void play(void) {
    glutPostRedisplay();
}

//
//void drawAxes(GLfloat xNeg, GLfloat xPos, GLfloat yNeg, GLfloat yPos, GLfloat zNeg, GLfloat zPos) {
//
//    glLineWidth(1.5);
//
//    glBegin(GL_LINES);
//
//    // x axis
//    glColor3f(1, 0, 0);
//    glVertex3f(xNeg, 0.0f, 0.0f);
//    glVertex3f(xPos, 0.0f, 0.0f);
//
//    // y axis
//    glColor3f(0, 1, 0);
//    glVertex3f(0.0f, yNeg, 0.0f);
//    glVertex3f(0.0f, yPos, 0.0f);
//
//    // z axis
//    glColor3f(0, 0, 1);
//    glVertex3f(0.0f, 0.0f, zNeg);
//    glVertex3f(0.0f, 0.0f, zPos);
//
//    glEnd();
//}


void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera();
    
    // lighting
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_cloudamb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_clouddif);
    glLightfv(GL_LIGHT1, GL_POSITION, light_cloudposition);
    
//    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shin);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glEnable(GL_COLOR_MATERIAL);

    // ground
    ground();

    // buildings
    glPushMatrix();
    glTranslatef(-0.57, 0.2, 0.1);
    glRotatef(90, 0, 1, 0);
    glScalef(0.35, 0.6, 0.3);
    buildings();
    glPopMatrix();

    // trees
    translateAndScale(0.05, 0.1, -0.1, 0.2, tree);
    translateAndScale(0.03, 0.07, 0.39, 0.25, tree);
    translateAndScale(-0.25, 0.1, -0.1, 0.2, tree);
    translateAndScale(0.65, 0.05, -0.4, 0.3, tree);
    translateAndScale(-0.65, 0.1, 0.4, 0.2, tree);

    // houses
    houses();

    // sun
    translate(0.6, 1.05, -0.4, sun);
    
    // road
    road();
    
    glDisable(GL_COLOR_MATERIAL);
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    
    glEnable(GL_COLOR_MATERIAL);
    
    // clouds
    clouds();
    
    glDisable(GL_COLOR_MATERIAL);
    
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

//    drawAxes(-1, 1, -1, 1, -1, 1);
    glFlush();
    glutSwapBuffers();
    
    
}

int main(int argc, char** argv) {

    // Initialize GLUT
    glutInit(&argc, argv);

    // number of buffers and depth for 3D shapes
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    // Set the window's initial width & height
    glutInitWindowSize(win_width, win_hight);

    // Position the window's initial top-left corner
    glutInitWindowPosition(0, 0);

    // title
    glutCreateWindow("Small Town");

    //texture, lighting
    init();

    // display
    glutIdleFunc(play);
    glutDisplayFunc(display);

    // resize the window
    glutReshapeFunc(reshapeFunc);

    // keyboard input from user
    glutKeyboardFunc(key);
    glutSpecialFunc(specialKey);
    
    // sound
    // ------------ FOR MAC -----------------//
    CFURLRef soundFileURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, CFSTR("/Users/Shefaa/Desktop/Uni/CS/Year 3/Project/citySound.wav"), kCFURLPOSIXPathStyle, false);
    SystemSoundID soundID;
    AudioServicesCreateSystemSoundID(soundFileURL, &soundID);
    AudioServicesPlaySystemSound(soundID);
    CFRelease(soundFileURL);
    
    // ------------ FOR WINDOWS -----------------//
    // PlaySound(TEXT("/Users/Shefaa/Desktop/Uni/CS/Year 3/Project/citySound.wav"), NULL, SND_ASYNC);
   
    
    glutMainLoop();
    
    return 0;
}
