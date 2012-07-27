//
//  kinectorApp.h
//  KinectTor
//
//  Created by drambald on 7/24/12.
//  Copyright (c) 2012 recipient.cc. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxUI.h"

#define OUTPUT_HEIGHT 800
#define OUTPUT_WIDTH 1280

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
    ofxUICanvas *guileft;
    
    void drawPointCloud();
    void debugDraw();
    void kinectorDraw();
    void setFullScreen();
    
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

    /// KINECT FUNCTIONS
    void downKinectAngle();
    void upKinectAngle();
    
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
    
    // Parameters
public:
    bool                bToogleFullScreen;
    bool                bFullscreen;
    int 				nearThreshold;
    int					farThreshold;    
    int					kinectAngle;    
    int 				pointCloudRotationY;
    int                 contour_min;
    int                 blobMax;
    bool 				bRecord;
    bool 				bPlayback;
    
    enum DrawFormat 
    {
        debug,
        cloud,
        kinector
    };
    DrawFormat currentFormat;
};
