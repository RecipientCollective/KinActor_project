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
#include "ofxUI.h"

//#define USE_IR // Uncomment this to use infra red instead of RGB cam...

// WINDOWS SIZES
#define OUTPUT_HEIGHT 852
#define OUTPUT_WIDTH 1480
#define GUIPANEL_BASE_LENGTH 400
#define LOGGER_PANEL_HEIGHT 300

// OPENNI CUSTOM DEFINES
#define MAX_ACTORS 2
#define MAX_HANDS 4

// GUI DEFINES
#define GUILEFT_FILE "GUI/guileftSettings.xml"
#define GUIRIGHT_FILE "GUI/guirightSettings.xml"
#define GUIDEFAULT_LEFT_FILE "GUI/guileftDefaultSettings.xml"
#define GUIDEFAULT_RIGHT_FILE "GUI/guirightDefaultSettings.xml"
#define TRANSLATE_PAD "TRANSLATE"
#define SCALE_SLIDER  "SCALE"
#define NEAR_THRESHOLD_SLIDER "NEAR THRESHOLD"
#define FAR_THRESHOLD_SLIDER "FAR THRESHOLD"
#define SMOOTH_SKELETON_SLIDER "SMOOTH SKELETON"
#define FILTER_HANDS_SLIDER "FILTER HANDS FACTOR"
#define SMOOTH_HANDS_SLIDER "SMOOTH HANDS"
#define ACCEL_STRING_DEFAULT "TILT: 0/0 x - 0 y - 0 z - 0"
#define RECORDING_FILENAME_STR "RECORDING FILENAME:"
#define STATUS_PLAY_DEFAULT "STATUS STREAM: "
#define STATUS_RECORD_DEFAULT "PLAYBACK/LIVE STREAM: "
#define STATUS_SKELETON_DEFAULT "SKELETON TRACKING: "
#define STATUS_SMOOTH_SKELETON_DEFAULT "SMOOTH SKELY (openni): "
#define STATUS_HANDS_DEFAULT "HAND TRACKING: "
#define STATUS_FILTER_HANDS_DEFAULT "HAND FILTER: "
#define STATUS_SMOOTH_HANDS_DEFAULT "HAND SMOOTH: "
#define STATUS_DRAW_MASKS_DEFAULT "DRAWING MASKS: "
#define STATUS_CLOUD_DEFAULT "DRAWING CLOUD POINTS: "
#define STATUS_CLOUD_DATA_DEFAULT "CLOUD USER DATA: "
#define RECORDING_TOGGLE "RECORD"
#define PLAYBACK_TOGGLE  "PLAYBACK"
#define SKELETON_TRACKING_TOGGLE  "SKELY TRACKING"
#define HANDS_TRACKING_TOGGLE  "HANDS TRACKING"
#define HANDS_FILTER_TOGGLE  "HANDS FILTER"
#define MASKING_TOGGLE "DRAW MASK"
#define DRAW_BOX_TOGGLE "DRAW BOX"
#define DEFAULT_SETTINGS_BUTTON "LOAD DEFAULTS"

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
    bool				isLive, isTracking, isRecording, isMasking;
	bool				isTrackingHands, isFiltering;
    int					nearThreshold, farThreshold;
	int					pointCloudRotationY;
    float				filterFactor;
    
    // OF IMAGES
    ofImage				allUserMasks, user1Mask, user2Mask, depthRangeMask;
    
    // GUI OBJECTS
    ofxUICanvas *guileft;
    ofxUICanvas *guiright;
    ofxUICanvas *guilogger;
    ofPoint      trPad;
    ofxUILabel  *accelerationLabel;
    ofxUILabel  *filenameLabel;
    ofxUILabel  *statusPlayLabel;
    ofxUILabel  *statusRecordLabel;
    ofxUILabel  *statusSkeletonLabel;
    ofxUILabel  *statusSkeletonSmoothLabel;
    ofxUILabel  *statusHandLabel;
    ofxUILabel  *statusFilterLabel;
    ofxUILabel  *statusSmoothHandsLabel;
    ofxUILabel  *statusDrawMasksLabel;
    ofxUILabelToggle *recordToggle;
    ofxUILabelToggle *playbackToggle;
    
    // GUI PARAMETERS
    float xInit;
    float dim;
    float guiPanelLength;
    float inputWidth;
    float inputHeight;
    bool  isFullScreen;
    bool  toggleFullScreen;
    bool  toggleShowInterface;
    bool  toggleShowLogger;
    
    // DRAW PARAMETERS
    enum DrawFormat 
    {
        oniactor = 1,
        debug = 2,
        cloud = 3
    };
    DrawFormat currentFormat;
    bool toggleDrawBox;
    
    // DRAW PARAMETERS
    float scaleFactor;
    float mtrx;
    float mtry;
    
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
    void setupWindowOptions();
    void setupGUI();
    void setupGUIleft();
    void setupGUIright();
    void setupGUIlogger();
    void closeGUI();
    void guiEvent(ofxUIEventArgs &e);
    void setFullScreen();
    void showInterface();
    void hideInterface();
    void updateGUI();
    void showLogger();
    void hideLogger();
    
    // oniActorAppOpenNI.cpp METHODS
    void setupRecording(string _filename = "");
    void setupPlayback(string _filename);
    void openniUpdate();
    void openniClose();
    
    // oniActorAppDraw.cpp METHODS
    void debugDraw();
    void logDraw();
    void drawAllUserMasks();
    void drawUserMasks();
    void drawPointCloud(ofxUserGenerator * user_generator, int userID);
    void oniactorDraw();
    void cloudDraw();
    void drawBox(int x, int y, int w, int h);
    
    // oniActorAppUtils.cpp METHODS
    string	generateFileName();
};




#endif
