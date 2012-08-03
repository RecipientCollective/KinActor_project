#ifndef OniActor_oniActorApp_h
#define OniActor_oniActorApp_h

/*********************************************************
 * ONIACTOR APP FILES:
 * - oniActorApp.h 
 * - oniActorApp.cpp MAIN
 * - oniActorAppGUI.h GUI FUNCTIONS
 * - oniActorAppDraw.h DRAW FUNCTIONS
 * - oniActorAppUtils.h UTILITY FUNCTIONS
 *********************************************************/

#include "ofxOpenNI.h"
#include "ofMain.h"

//#define USE_IR // Uncomment this to use infra red instead of RGB cam...

// WINDOWS SIZES
#define OUTPUT_HEIGHT 1040
#define OUTPUT_WIDTH 1280
#define TRANSLATE_GUIVIEW_X 410
#define TRANSLATE_GUIVIEW_Y 10

class oniActorApp : public ofBaseApp
{
private:
    // INSTANCE VARIABLES
    
    
public:
    // OpenFrameworks METHODS
    void setup();
	void update();
	void draw();
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
};




#endif
