#ifndef OniActor_oniActorApp_h
#define OniActor_oniActorApp_h

/*********************************************************
 * ONIACTOR APP FILES:
 * - oniActorApp.h 
 * - oniActorApp.cpp MAIN
 * - oniActorAppOpenNI.cpp OPENNI FUNCTIONS
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

// OPENI CUSTOM DEFINES
#define MAX_ACTORS 2
#define MAX_HANDS 4

class oniActorApp : public ofBaseApp
{
private:
    // OPENNI OBJECTS
    ofxOpenNIContext	recordContext, playContext;
	ofxDepthGenerator	recordDepth, playDepth;    
#ifdef USE_IR
	ofxIRGenerator		recordImage, playImage;
#else
	ofxImageGenerator	recordImage, playImage;
#endif
	ofxHandGenerator	recordHandTracker, playHandTracker;
	ofxUserGenerator	recordUser, playUser;
	ofxOpenNIRecorder	oniRecorder;
    
#if defined (TARGET_OSX) //|| defined(TARGET_LINUX) // only working on Mac/Linux at the moment (but on Linux you need to run as sudo...)
	ofxHardwareDriver	hardware;
#endif

    // OPENNI PARAMETERS
    bool				isLive, isTracking, isRecording;
    bool                isCloud, isCPBkgnd, isMasking;
	bool				isTrackingHands, isFiltering;
    int					nearThreshold, farThreshold;
	int					pointCloudRotationY;
    float				filterFactor;
    
    // OF IMAGES
    ofImage				allUserMasks, user1Mask, user2Mask, depthRangeMask;
    
    // GUI OBJECTS
    
    // GUI PARAMETERS
    
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
    void exit();

private:
    // oniActorAppGUI.cpp METHODS
    void setupBackgroud();
    
    // oniActorAppOpenNI.cpp METHODS
    void setupRecording(string _filename = "");
    void setupPlayback(string _filename);
    void openniUpdate();
    void openniClose();
    
};




#endif
