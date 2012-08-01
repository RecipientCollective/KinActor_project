//
//  kinectorApp.h
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
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxUI.h"
#include "actor.h"

#define OUTPUT_HEIGHT 1040
#define OUTPUT_WIDTH 1280

// BITMASK blobUpdate
#define ACTORSEMPTY        1  // 2^0, bit 0
#define BLOBSEMPTY        2  // 2^1, bit 1
#define MOREACTORS         4  // 2^2, bit 2
#define MOREBLOBS         8  // 2^3, bit 3
#define EQUALSIZE        16  // 2^4, bit 4

class kinectorApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    // ofxUI methods and gui objects
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    void setupGUIleft();
    void setupGUIbottom();
    
    // GUI STUFFS    
    ofxUICanvas *guileft;
    ofxUICanvas *guibottom;
    ofxUIToggle *recordToggle;
    ofxUIToggle *playbackToggle;
    ofPoint      trPad;
    float xInit;
    float dim;
    float length;
    ofPoint loggerP;
    float loggerW;
    float loggerH;
    
    void showInterface();
    void hideInterface();
    
    void drawPointCloud();
    void debugDraw();
    void kinectorDraw();
    void setFullScreen();
    void loggerDraw();
    void drawPlayIcons();
    void drawBox();
    void drawBlobs();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    /// start/stop recording and playback,
    /// make sure you don't record and playback simultaneously 
    /// using the same file!!!
    void startRecording();
    void stopRecording();
    void startPlayback();
    void stopPlayback();
    void toggleRecord();
    void togglePlayback();
    
    /// KINECT FUNCTIONS
    void downKinectAngle();
    void upKinectAngle();
    void kinectConnect();
    void kinectDisconnect();
    
private:
    // KINECT VARS
    ofxKinect 			kinect;
    ofxKinectRecorder 	kinectRecorder;
    ofxKinectPlayer 	kinectPlayer;
    ofxBase3DVideo* 	kinectSource;       // used to switch between the live kinect and the recording player
    
    // Images
    ofxCvColorImage		colorImg;
    ofxCvGrayscaleImage grayImage;			// grayscale depth image
    ofxCvGrayscaleImage grayThreshNear;		// the near thresholded image
    ofxCvGrayscaleImage grayThreshFar;		// the far thresholded image
    
    // ContourFinder openCV
    ofxCvContourFinder 	contourFinder;
    
    // BLOBS FUNCTIONS & VARS
    void blobsInsert();
    void blobsUpdate();
    void checkStatus();
    
    // containers for actors identified by code
	map<string ,actor>  actors;
    
    // Parameters
public:
    bool                bToogleFullScreen;
    bool                bFullscreen;
    bool                bShowInterface;
    bool                bBox;
    int 				nearThreshold;
    int					farThreshold;    
    int					kinectAngle;    
    int 				pointCloudRotationY;
    int                 contour_min;
    int                 blobMax;
    bool 				bRecord;
    bool 				bPlayback;
    float               scaleFactor;
    float               mtrx;
    float               mtry;

    // vars to store info on multiple inputs
    float inputWidth;
    float inputHeight;
    
    // STATUS UPDATE FLAGS
	unsigned char flags;    // 8-bit MAX flag
	unsigned char pflags;   // 8-bit flags for previuous state

    enum DrawFormat 
    {
        debug,
        cloud,
        kinector
    };
    DrawFormat currentFormat;
};
