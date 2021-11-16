#include "graphics.h"
#include "circle.h"
#include "rect.h"
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

GLdouble width, height;
int wd;
const color white(1, 1, 1);
const color black(0, 0, 0);
const color yellow(1, 1, 0);
const color magenta(1, 0, 1);
const color cyan (0, 1, 1);
const color silver(191/255.0,193/255.0,194/255.0);
const color glass(240/255.0, 255/255.0, 255/255.0);
const color asphalt(82/255.0,77/255.0,80/255.0);
const color skyBlue(135/255.0, 206/255.0, 235/255.0);
const color raspberry(227/255.0, 11/255.0, 93/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color orangeYellow(255/255.0, 174/255.0, 66/255.0);

Rect carBase;
Rect carRoof;
Rect windshield;
Rect headlight;
Circle rearWindshield;
Circle rearTyre;
Circle rearInnerWheel;
Circle frontTyre;
Circle frontInnerWheel;
Rect road;
vector<unique_ptr<Shape>> clouds;
vector<Rect> roadLines;
Rect user;
Rect whiteBlock;

void initCarBase() {
    carBase.setCenter(200, 390);
    carBase.setSize(240, 65);
    carBase.setColor(raspberry);

    headlight.setCenter(310, 367.5);
    headlight.setSize(10,10);
    headlight.setColor(yellow);
}

void initCarRoof() {
    carRoof.setCenter(180, 360);
    carRoof.setSize(100, 80);
    carRoof.setColor(raspberry);
}

void initWindshield() {
    windshield.setCenter(230, 360);
    windshield.setColor(glass);

    rearWindshield.setCenter(140, 360);
    rearWindshield.setColor(glass);
    rearWindshield.setRadius(35);
}

void initWheel() {
    rearTyre.setCenter(125, 425);
    rearTyre.setRadius(30);
    rearTyre.setColor(black);
    rearInnerWheel.setCenter(125, 425);
    rearInnerWheel.setRadius(15);
    rearInnerWheel.setColor(silver);

    frontTyre.setCenter(275, 425);
    frontTyre.setRadius(30);
    frontTyre.setColor(black);
    frontInnerWheel.setCenter(275, 425);
    frontInnerWheel.setRadius(15);
    frontInnerWheel.setColor(silver);
}

void initRoad() {
    road.setCenter(250, 500);
    road.setSize(width, height/3);
    road.setColor(asphalt);
}

void initClouds() {
    // Note: the Rect objects that make up the flat bottom of the clouds
    // won't appear until you implement the Rect::draw method.
    clouds.clear();
    dimensions cloudBottom(30, 30);
    // First cloud
    clouds.push_back(make_unique<Circle>(white, 300, 100, 20));
    clouds.push_back(make_unique<Circle>(white, 330, 100, 20));
    clouds.push_back(make_unique<Circle>(white, 320, 90, 20));
    clouds.push_back(make_unique<Rect>(white, 315, 105, cloudBottom));
    // Second cloud
    clouds.push_back(make_unique<Circle>(white, 100, 80, 20));
    clouds.push_back(make_unique<Circle>(white, 130, 80, 20));
    clouds.push_back(make_unique<Circle>(white, 120, 70, 20));
    clouds.push_back(make_unique<Rect>(white, 115, 85, cloudBottom));
    // Third cloud
    clouds.push_back(make_unique<Circle>(white, 450, 50, 20));
    clouds.push_back(make_unique<Circle>(white, 480, 50, 20));
    clouds.push_back(make_unique<Circle>(white, 470, 40, 20));
    clouds.push_back(make_unique<Rect>(white, 465, 55, cloudBottom));
    // Fourth cloud
    clouds.push_back(make_unique<Circle>(white, 350, 200, 20));
    clouds.push_back(make_unique<Circle>(white, 380, 200, 20));
    clouds.push_back(make_unique<Circle>(white, 370, 190, 20));
    clouds.push_back(make_unique<Rect>(white, 365, 205, cloudBottom));
    // Fifth cloud
    clouds.push_back(make_unique<Circle>(white, 250, 180, 20));
    clouds.push_back(make_unique<Circle>(white, 280, 180, 20));
    clouds.push_back(make_unique<Circle>(white, 270, 170, 20));
    clouds.push_back(make_unique<Rect>(white, 265, 185, cloudBottom));
    // Sixth cloud
    clouds.push_back(make_unique<Circle>(white, 20, 150, 20));
    clouds.push_back(make_unique<Circle>(white, 50, 150, 20));
    clouds.push_back(make_unique<Circle>(white, 40, 140, 20));
    clouds.push_back(make_unique<Rect>(white, 35, 155, cloudBottom));

}

void initRoadLines() {
    int totalLineWidth = 0;
    while (totalLineWidth < width + 100) {
        roadLines.push_back(Rect(orangeYellow, totalLineWidth, 450));
        totalLineWidth += 45;
    }
}

void initUser() {
    // Initialize the user to be a 20x20 white block
    // centered in the top left corner of the graphics window
    whiteBlock.setCenter(0,0);
    whiteBlock.setWidth(10);
    whiteBlock.setHeight(10);
    whiteBlock.setColor(white);
}

void init() {
    width = 500;
    height = 500;
    srand(time(0));
    initWindshield();
    initCarBase();
    initCarRoof();
    initWheel();
    initRoad();
    initClouds();
    initRoadLines();
    initUser();
}

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(skyBlue.red, skyBlue.green, skyBlue.blue, 1.0f);
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // Tell OpenGL to use the whole window for drawing
    glViewport(0, 0, width, height); // DO NOT CHANGE THIS LINE (unless you are running Catalina on Mac)
    
    // Do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION); // DO NOT CHANGE THIS LINE
    glLoadIdentity(); // DO NOT CHANGE THIS LINE
    glOrtho(0.0, width, height, 0.0, -1.f, 1.f); // DO NOT CHANGE THIS LINE

    // Clear the color buffer with current clearing color
    glClear(GL_COLOR_BUFFER_BIT); // DO NOT CHANGE THIS LINE
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // DO NOT CHANGE THIS LINE
    
    /*
     * Draw here
     */

    for (unique_ptr<Shape> &s : clouds) {
        // #polymorphism
        s->draw();
    }

    road.setColor(asphalt);
    road.draw();

    windshield.setColor(glass);
    windshield.drawDiamond();

    for (Rect &r : roadLines) {
        r.drawTrapezoid();
    }

    rearWindshield.setColor(glass);
    rearWindshield.draw();

    carBase.setColor(raspberry);
    carBase.draw();

    carRoof.setColor(raspberry);
    carRoof.draw();

    headlight.setColor(yellow);
    headlight.draw();

    rearTyre.setColor(black);
    rearTyre.draw();
    frontTyre.setColor(black);
    frontTyre.draw();
    rearInnerWheel.setColor(silver);
    rearInnerWheel.draw();
    frontInnerWheel.setColor(silver);
    frontInnerWheel.draw();

    whiteBlock.draw();
    
    glFlush();  // Render now
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y) {
    // escape
    if (key == 27) {
        glutDestroyWindow(wd);
        exit(0);
    }
    
    glutPostRedisplay();
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            
            break;
        case GLUT_KEY_LEFT:
            
            break;
        case GLUT_KEY_RIGHT:
            
            break;
        case GLUT_KEY_UP:
            
            break;
    }
    
    glutPostRedisplay();
}

void cursor(int x, int y) {
    // TODO: Set the user's center point to be the coordinates
    // passed in as parameters to this function. This will make
    // the user block move with the mouse.
    user.setCenter(x, y);
    whiteBlock.setCenter(x, y);
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {

    glutPostRedisplay();
}

void cloudTimer(int dummy) {

    for (unique_ptr<Shape> &s : clouds) {
        // Move all the clouds to the left
        s->moveX(-1);
        // If a shape has moved off the screen
        if (s->getCenterX() < -20) {
            // Set it to the right of the screen so that it passes through again
            s->setCenterX(520);
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(50, cloudTimer, dummy);
}

void roadLineTimer(int dummy) {
    for (int i = 0; i < roadLines.size(); ++i) {
        roadLines[i].moveX(-2);
        // If a shape has moved off the screen
        if (roadLines[i].getCenterX() < -20) {
            roadLines[i].setCenter(520, roadLines[i].getCenterY());
        }
//        if (roadLines[i].getCenterX() < -(roadLines[i].getWidth()/2)) {
//            // Set it to the right of the screen so that it passes through again
//            int lineOnLeft = (i == 0) ? roadLines.size()-1 : i - 1;
//            roadLines[i].setCenterX(roadLines[lineOnLeft].getCenterX() + roadLines[lineOnLeft].getWidth()/2 + roadLines[i].getWidth()/2 + 5);
//        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, roadLineTimer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    
    init();
    
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitDisplayMode(GLUT_RGBA);
    
    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Car Scene" /* title */ );
    
    // Register callback handler for window re-paint event
    glutDisplayFunc(display);
    
    // Our own OpenGL initialization
    initGL();
    
    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    
    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);
    
    // handles mouse movement
    glutPassiveMotionFunc(cursor);
    
    // handles mouse click
    glutMouseFunc(mouse);
    
    // handles timer
    glutTimerFunc(0, cloudTimer, 0);
    glutTimerFunc(0, roadLineTimer, 0);
    
    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
