#ifndef graphics_h
#define graphics_h

#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Program initialization NOT OpenGL/GLUT dependent,
// as we haven't created a GLUT window yet
void initSun();
void initWindshield();
void initCarBase();
void initCarRoof();
void initWheel();
void initRoad();
void initClouds();
void initSign();
void initRoadLines();
void init();

// Initialize OpenGL Graphics
void InitGL();

// Callback functions for GLUT

// Draw the window - this is where all the GL actions are
void display();

// Trap and process alphanumeric keyboard events
void kbd(unsigned char key, int x, int y);

// Trap and process special keyboard events
void kbdS(int key, int x, int y);

// Handle "mouse cursor moved" events 
void cursor(int x, int y);

// Moves car according to keyboard input
void relocateCar();

// Calls itself after a specified time
void cloudTimer(int dummy);
void signTimer(int dummy);
void roadLineTimer(int dummy);

// Handle mouse button pressed and released events
void mouse(int button, int state, int x, int y);

#endif /* graphics_h */
