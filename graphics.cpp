#include "graphics.h"
#include "circle.h"
#include "rect.h"
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

enum roadLane {
    top,
    bottom
};

enum direction {
    east,
    west
};

GLdouble width, height;
int wd;
const color white(1, 1, 1);
const color black(0, 0, 0);
const color red(1, 0, 0);
const color green(0, 1, 0);
const color blue(0, 0, 1);
const color yellow(1, 1, 0);
const color magenta(1, 0, 1);
const color cyan (0, 1, 1);
const color silver(191/255.0,193/255.0,194/255.0);
const color asphalt(82/255.0,77/255.0,80/255.0);
const color steel(57/255.0, 60/255.0, 85/255.0);
const color glass(240/255.0, 255/255.0, 255/255.0);
const color skyBlue(135/255.0, 206/255.0, 235/255.0);
const color orangeYellow(255/255.0, 174/255.0, 66/255.0);
const color paleYellow(255/255.0, 255/255.0, 191/255.0);
const color palePurple(145/255.0, 128/255.0, 191/255.0);
const color paleRed(221/255.0, 68/255.0, 17/255.0);

Rect carBase;
Rect carRoof;
Rect windshield;
Rect rearWindow;
Rect frontWindow;
Rect road;
Circle headlight;
Circle tailLight;
Circle rearWindshield;
Circle rearTyre;
Circle rearInnerWheel;
Circle frontTyre;
Circle frontInnerWheel;
Circle sun;
vector<unique_ptr<Shape>> clouds;
vector<unique_ptr<Shape>> roadSign;
vector<Rect> roadLines;
Rect user;
roadLane lane = top;
direction bearing = east;
int shiftX = 500;
int shiftY = 40;

void initSun() {
    sun.setCenter(500, 0);
    sun.setRadius(60);
    sun.setColor(yellow);
}

void initWindshield() {
    windshield.setCenter(230, 365);
    windshield.setColor(glass);

    rearWindshield.setCenter(140, 360);
    rearWindshield.setRadius(35);
    rearWindshield.setColor(glass);
}

void initCarBase() {

    carBase.setCenter(200, 390);
    carBase.setSize(240, 65);
    carBase.setColor(palePurple);

    tailLight.setCenter(80, 370);
    tailLight.setRadius(10);
    tailLight.setColor(paleRed);

    headlight.setCenter(320, 370);
    headlight.setRadius(10);
    headlight.setColor(paleYellow);
}

void initCarRoof() {
    carRoof.setCenter(180, 360);
    carRoof.setSize(100, 80);
    carRoof.setColor(palePurple);

    rearWindow.setCenter(160, 340);
    rearWindow.setSize(35, 30);
    rearWindow.setColor(glass);

    frontWindow.setCenter(200, 340);
    frontWindow.setSize(35, 30);
    frontWindow.setColor(glass);
}

void initWheel() {
    rearTyre.setCenter(125, 420);
    rearTyre.setRadius(30);
    rearTyre.setColor(black);

    rearInnerWheel.setCenter(125, 420);
    rearInnerWheel.setRadius(15);
    rearInnerWheel.setColor(silver);

    frontTyre.setCenter(275, 420);
    frontTyre.setRadius(30);
    frontTyre.setColor(black);

    frontInnerWheel.setCenter(275, 420);
    frontInnerWheel.setRadius(15);
    frontInnerWheel.setColor(silver);
}

void initRoad() {
    road.setCenter(250, 500);
    road.setSize(width, height/3);
    road.setColor(asphalt);
}

void initClouds() {
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

void initSign() {
    roadSign.clear();
    dimensions post(10, 200);
    roadSign.push_back(make_unique<Rect>(steel, 500, 400, post));
    roadSign.push_back(make_unique<Circle>(red, 500, 300, 20));
    roadSign.push_back(make_unique<Circle>(white, 500, 300, 15));

    roadSign.push_back(make_unique<Rect>(steel, -500, 400, post));
    roadSign.push_back(make_unique<Circle>(red, -500, 300, 20));
    roadSign.push_back(make_unique<Circle>(white, -500, 300, 15));
}

void initRoadLines() {
    roadLines.clear();
    int totalLineWidth = 0;
    while (totalLineWidth < width + 40) {
        roadLines.push_back(Rect(orangeYellow, totalLineWidth, 450));
        totalLineWidth += 50;
    }
}

void init() {
    width = 500;
    height = 500;
    srand(time(0));
    initSun();
    initWindshield();
    initCarBase();
    initCarRoof();
    initWheel();
    initRoad();
    initClouds();
    initSign();
    initRoadLines();
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

    sun.setColor(yellow);
    sun.draw();

    for (unique_ptr<Shape> &cloud : clouds) {
        // #polymorphism
        cloud->draw();
    }

    for (unique_ptr<Shape> &sign : roadSign) {
        // #polymorphism
        sign->draw();
    }

    road.setColor(asphalt);
    road.draw();

    for (Rect &line : roadLines) {
        line.drawTrapezoid();
    }

    windshield.setColor(glass);
    windshield.drawDiamond();

    rearWindshield.setColor(glass);
    rearWindshield.draw();

    tailLight.setColor(paleRed);
    tailLight.draw();

    headlight.setColor(paleYellow);
    headlight.draw();

    carBase.setColor(palePurple);
    carBase.draw();

    carRoof.setColor(palePurple);
    carRoof.draw();

    rearWindow.setColor(glass);
    rearWindow.draw();

    frontWindow.setColor(glass);
    frontWindow.draw();

    rearTyre.setColor(black);
    rearTyre.draw();

    frontTyre.setColor(black);
    frontTyre.draw();

    rearInnerWheel.setColor(silver);
    rearInnerWheel.draw();

    frontInnerWheel.setColor(silver);
    frontInnerWheel.draw();

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
            lane = bottom;
            relocateCar();
            break;
        case GLUT_KEY_LEFT:
            bearing = west;
            relocateCar();
            break;
        case GLUT_KEY_RIGHT:
            bearing = east;
            relocateCar();
            break;
        case GLUT_KEY_UP:
            lane = top;
            relocateCar();
            break;
    }
    
    glutPostRedisplay();
}

void cursor(int x, int y) {
    user.setCenter(x, y);
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {

    glutPostRedisplay();
}

void relocateCar() {
    // Car base
    if (lane == top && bearing == east) {
        carBase.setCenter(200, 390);
        carBase.setSize(240, 65);
        carBase.setColor(palePurple);

        tailLight.setCenter(80, 370);
        tailLight.setRadius(10);
        tailLight.setColor(paleRed);

        headlight.setCenter(320, 370);
        headlight.setRadius(10);
        headlight.setColor(paleYellow);
    }
    if (lane == top && bearing == west) {
        carBase.setCenter(shiftX - 200, 390);
        carBase.setSize(240, 65);
        carBase.setColor(palePurple);

        tailLight.setCenter(shiftX - 80, 370);
        tailLight.setRadius(10);
        tailLight.setColor(paleRed);

        headlight.setCenter(shiftX - 320, 367);
        headlight.setRadius(10);
        headlight.setColor(paleYellow);
    }
    if (lane == bottom && bearing == east) {
        carBase.setCenter(200, 390 + shiftY);
        carBase.setSize(240, 65);
        carBase.setColor(palePurple);

        tailLight.setCenter(80, 370 + shiftY);
        tailLight.setRadius(10);
        tailLight.setColor(paleRed);

        headlight.setCenter(320, 370 + shiftY);
        headlight.setRadius(10);
        headlight.setColor(paleYellow);
    }
    if (lane == bottom && bearing == west) {
        carBase.setCenter(shiftX - 200, 390 + shiftY);
        carBase.setSize(240, 65);
        carBase.setColor(palePurple);

        tailLight.setCenter(shiftX - 80, 370 + shiftY);
        tailLight.setRadius(10);
        tailLight.setColor(paleRed);

        headlight.setCenter(shiftX - 320, 370 + shiftY);
        headlight.setRadius(10);
        headlight.setColor(paleYellow);
    }

    // Car roof
    if (lane == top && bearing == east) {
        carRoof.setCenter(180, 360);
        carRoof.setSize(100, 80);
        carRoof.setColor(palePurple);

        rearWindow.setCenter(160, 340);
        rearWindow.setSize(35, 30);
        rearWindow.setColor(glass);

        frontWindow.setCenter(200, 340);
        frontWindow.setSize(35, 30);
        frontWindow.setColor(glass);
    }
    if (lane == top && bearing == west) {
        carRoof.setCenter(shiftX - 180, 360);
        carRoof.setSize(100, 80);
        carRoof.setColor(palePurple);

        rearWindow.setCenter(shiftX - 160, 340);
        rearWindow.setSize(35, 30);
        rearWindow.setColor(glass);

        frontWindow.setCenter(shiftX - 200, 340);
        frontWindow.setSize(35, 30);
        frontWindow.setColor(glass);
    }
    if (lane == bottom && bearing == east) {
        carRoof.setCenter(180, 360 + shiftY);
        carRoof.setSize(100, 80);
        carRoof.setColor(palePurple);

        rearWindow.setCenter(160, 340 + shiftY);
        rearWindow.setSize(35, 30);
        rearWindow.setColor(glass);

        frontWindow.setCenter(200, 340 + shiftY);
        frontWindow.setSize(35, 30);
        frontWindow.setColor(glass);
    }
    if (lane == bottom && bearing == west) {
        carRoof.setCenter(shiftX - 180, 360 + shiftY);
        carRoof.setSize(100, 80);
        carRoof.setColor(palePurple);

        rearWindow.setCenter(shiftX - 160, 340 + shiftY);
        rearWindow.setSize(35, 30);
        rearWindow.setColor(glass);

        frontWindow.setCenter(shiftX - 200, 340 + shiftY);
        frontWindow.setSize(35, 30);
        frontWindow.setColor(glass);
    }

    // Windshields
    if (lane == top && bearing == east) {
        windshield.setCenter(230, 365);
        windshield.setColor(glass);

        rearWindshield.setCenter(140, 360);
        rearWindshield.setRadius(35);
        rearWindshield.setColor(glass);
    }
    if (lane == top && bearing == west) {
        windshield.setCenter(shiftX - 230, 365);
        windshield.setColor(glass);

        rearWindshield.setCenter(shiftX - 140, 360);
        rearWindshield.setColor(glass);
        rearWindshield.setRadius(35);
    }
    if (lane == bottom && bearing == east) {
        windshield.setCenter(230, 365 + shiftY);
        windshield.setColor(glass);

        rearWindshield.setCenter(140, 360 + shiftY);
        rearWindshield.setRadius(35);
        rearWindshield.setColor(glass);
    }
    if (lane == bottom && bearing == west) {
        windshield.setCenter(shiftX - 230, 365 + shiftY);
        windshield.setColor(glass);

        rearWindshield.setCenter(shiftX - 140, 360 + shiftY);
        rearWindshield.setColor(glass);
        rearWindshield.setRadius(35);
    }

    // Wheels
    if (lane == top && bearing == east) {
        rearTyre.setCenter(125, 420);
        rearTyre.setRadius(30);
        rearTyre.setColor(black);

        rearInnerWheel.setCenter(125, 420);
        rearInnerWheel.setRadius(15);
        rearInnerWheel.setColor(silver);

        frontTyre.setCenter(275, 420);
        frontTyre.setRadius(30);
        frontTyre.setColor(black);

        frontInnerWheel.setCenter(275, 420);
        frontInnerWheel.setRadius(15);
        frontInnerWheel.setColor(silver);
    }
    if (lane == top && bearing == west) {
        rearTyre.setCenter(shiftX - 125, 420);
        rearTyre.setRadius(30);
        rearTyre.setColor(black);

        rearInnerWheel.setCenter(shiftX - 125, 420);
        rearInnerWheel.setRadius(15);
        rearInnerWheel.setColor(silver);

        frontTyre.setCenter(shiftX - 275, 420);
        frontTyre.setRadius(30);
        frontTyre.setColor(black);

        frontInnerWheel.setCenter(shiftX - 275, 420);
        frontInnerWheel.setRadius(15);
        frontInnerWheel.setColor(silver);
    }
    if (lane == bottom && bearing == east) {
        rearTyre.setCenter(125, 420 + shiftY);
        rearTyre.setRadius(30);
        rearTyre.setColor(black);

        rearInnerWheel.setCenter(125, 420 + shiftY);
        rearInnerWheel.setRadius(15);
        rearInnerWheel.setColor(silver);

        frontTyre.setCenter(275, 420 + shiftY);
        frontTyre.setRadius(30);
        frontTyre.setColor(black);

        frontInnerWheel.setCenter(275, 420 + shiftY);
        frontInnerWheel.setRadius(15);
        frontInnerWheel.setColor(silver);
    }
    if (lane == bottom && bearing == west) {
        rearTyre.setCenter(shiftX - 125, 420 + shiftY);
        rearTyre.setRadius(30);
        rearTyre.setColor(black);

        rearInnerWheel.setCenter(shiftX - 125, 420 + shiftY);
        rearInnerWheel.setRadius(15);
        rearInnerWheel.setColor(silver);

        frontTyre.setCenter(shiftX - 275, 420 + shiftY);
        frontTyre.setRadius(30);
        frontTyre.setColor(black);

        frontInnerWheel.setCenter(shiftX - 275, 420 + shiftY);
        frontInnerWheel.setRadius(15);
        frontInnerWheel.setColor(silver);
    }
}

void cloudTimer(int dummy) {

    if (bearing == east) {
        for (unique_ptr<Shape> &cloud : clouds) {
            // Move all the clouds to the left
            cloud->moveX(-0.5);
            // If a shape has moved off the screen
            if (cloud->getCenterX() < -20) {
                // Set it to the right of the screen so that it passes through again
                cloud->setCenterX(520);
            }
        }
    }
    if (bearing == west) {
        for (unique_ptr<Shape> &cloud : clouds) {
            // Move all the clouds to the left
            cloud->moveX(0.5);
            // If a shape has moved off the screen
            if (cloud->getCenterX() > 520) {
                // Set it to the right of the screen so that it passes through again
                cloud->setCenterX(-20);
            }
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(50, cloudTimer, dummy);
}

void signTimer(int dummy) {
    if (bearing == east) {
        for (unique_ptr<Shape> &sign : roadSign) {
            // Move all the clouds to the left
            sign->moveX(-7);
            // If a shape has moved off the screen
            if (sign->getCenterX() < -1000) {
                // Set it to the right of the screen so that it passes through again
                sign->setCenterX(1000);
            }
        }
    }
    if (bearing == west) {
        for (unique_ptr<Shape> &sign : roadSign) {
            // Move all the clouds to the left
            sign->moveX(7);
            // If a shape has moved off the screen
            if (sign->getCenterX() > 1000) {
                // Set it to the right of the screen so that it passes through again
                sign->setCenterX(-1000);
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(30, signTimer, dummy);
}

void roadLineTimer(int dummy) {
    if (bearing == east) {
        for (Rect &line : roadLines) {
            line.moveX(-8);
            // If a shape has moved off the screen
            if (line.getCenterX() < -20) {
                line.setCenter(520, line.getCenterY());
            }
        }
    }
    if (bearing == west) {
        for (Rect &line : roadLines) {
            line.moveX(8);
            // If a shape has moved off the screen
            if (line.getCenterX() > 520) {
                line.setCenter(-20, line.getCenterY());
            }
        }
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
    glutTimerFunc(0,signTimer, 0);
    glutTimerFunc(0, roadLineTimer, 0);
    
    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
