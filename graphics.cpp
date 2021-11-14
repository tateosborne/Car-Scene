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
const color asphalt(82/255.0,77/255.0,80/255.0);
const color skyBlue(77/255.0, 213/255.0, 240/255.0);
const color darkBlue(1/255.0, 110/255.0, 214/255.0);
const color purple(119/255.0, 11/255.0, 224/255.0);
const color orangeYellow(255/255.0, 174/255.0, 66/255.0);

Rect carBase;
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
    carBase.setCenter(250, 500);
    carBase.setSize(width, height/5);
    carBase.setColor(darkBlue);
}

void initWheel() {
    rearTyre.setCenter(150, 500);
    rearTyre.setRadius(20);
    rearTyre.setColor(black);
    rearInnerWheel.setCenter(150, 500);
    rearInnerWheel.setRadius(10);
    rearInnerWheel.setColor(silver);

    frontTyre.setCenter(250, 500);
    frontTyre.setRadius(20);
    frontTyre.setColor(black);
    frontInnerWheel.setCenter(250, 500);
    frontInnerWheel.setRadius(10);
    frontInnerWheel.setColor(silver);

}

void initRoad() {
    road.setCenter(250, 500);
    road.setSize(width, height/5);
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

}

void initRoadLines() {
    int totalLineWidth = 0;
    dimensions lineSize = {5, 50};
    while (totalLineWidth < width + 50) {
        roadLines.push_back(Rect(orangeYellow,
                                  totalLineWidth+(lineSize.width/2)+25,
                                  height-((lineSize.height/2)),
                                  lineSize));
        totalLineWidth += lineSize.width + 25;
    }
}

void initUser() {
    // TODO: Initialize the user to be a 20x20 white block
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
    initCarBase();
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

    // TODO: Add logic to draw the grass, the user, and the
    // rest of the buildings. Note that the order of drawing
    // matters because whatever is drawn last appears on top.
    // Note that darkBlue buildings turn cyan when overlapping
    // with the user, and purple buildings turn magenta.

    for (unique_ptr<Shape> &s : clouds) {
        // #polymorphism
        s->draw();
    }

    for (Rect &r : buildings3) {
        if (r.isOverlapping(user)) {
            r.setColor(magenta);
        } else {
            r.setColor(purple);
        }
        r.draw();
    }

    for (Rect &r : buildings2) {
        if (r.isOverlapping(user)) {
            r.setColor(cyan);
        } else {
            r.setColor(darkBlue);
        }
        r.draw();
    }

    grass.setColor(grassGreen);
    grass.draw();

    for (Rect &r : buildings1) {
        if (r.isOverlapping(user)) {
            r.setColor(orange);
        } else {
            r.setColor(brickRed);
        }
        r.draw();
    }

    whiteBlock.draw();

    // Check if the user is overlapping with the cloud
    // Only check the Rect object within the cloud
    for (int i = 3; i < clouds.size(); i += 4) {
        if (dynamic_cast<Rect&>(*clouds[i]).isOverlapping(user)){
            glutDestroyWindow(wd);
            exit(0);
        }
    }
    
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

void buildingTimer(int dummy) {
    // TODO: Make the other two vectors of buildings move.
    // The larger the buildings, the slower they should move.

    for (int i = 0; i < buildings1.size(); ++i) {
        // Move all the red buildings to the left
        buildings1[i].moveX(-3);
        // If a shape has moved off the screen
        if (buildings1[i].getCenterX() < -(buildings1[i].getWidth()/2)) {
            // Set it to the right of the screen so that it passes through again
            int buildingOnLeft = (i == 0) ? buildings1.size()-1 : i - 1;
            buildings1[i].setCenterX(buildings1[buildingOnLeft].getCenterX() + buildings1[buildingOnLeft].getWidth()/2 + buildings1[i].getWidth()/2 + 5);
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, buildingTimer, dummy);
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    
    init();
    
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitDisplayMode(GLUT_RGBA);
    
    glutInitWindowSize((int)width, (int)height);
    glutInitWindowPosition(100, 200); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Runner" /* title */ );
    
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
    glutTimerFunc(0, buildingTimer, 0);
    
    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
