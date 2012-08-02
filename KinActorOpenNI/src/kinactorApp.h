//
//  kinactorApp.h
//  KinectTor
//
//  Created by drambald on 7/24/12.
//  Copyright (c) 2012 recipient.cc. All rights reserved.
//


/*************************************************************** 
 *
 ***************************************************************/

#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxUI.h"
#include "actor.h"

// WINDOWS SIZES
#define OUTPUT_HEIGHT 1040
#define OUTPUT_WIDTH 1280
#define TRANSLATE_GUIVIEW_X 410
#define TRANSLATE_GUIVIEW_Y 10


class kinactorApp : public ofBaseApp
{
public:
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
    
    // ofxUI methods and gui objects
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    void setupGUIleft();
    void setupGUIlogger();
    void setupGUIright();
    
    // GUI STUFFS    
    ofxUICanvas *guileft;
    ofxUICanvas *guiright;
    ofxUICanvas *guilogger;
    ofPoint      trPad;
    float xInit;
    float dim;
    float guiPanelLength;
    ofPoint loggerP;
    float loggerW;
    float loggerH;
    void showInterface();
    void hideInterface();
    
    void drawPointCloud();
    void debugDraw();
    void kinactorDraw();
    void setFullScreen();
    void loggerDraw();
    void drawBox();
    
    /// KINECT FUNCTIONS
    void downKinectAngle();
    void upKinectAngle();
    void kinectConnect();
    void kinectDisconnect();
    
private:
        
    // containers for actors identified by code
	map<string ,actor>  actors;
    
    // Parameters
public:
    bool                bToogleFullScreen;
    bool                bFullscreen;
    bool                bShowInterface;
    bool                bBox;
    float               scaleFactor;
    float               mtrx;
    float               mtry;
    
    // vars to store info on input size
    float inputWidth;
    float inputHeight;
    
    enum DrawFormat 
    {
        kinactor = 1,
        debug = 2,
        cloud = 3
    };
    DrawFormat currentFormat;
    
};
