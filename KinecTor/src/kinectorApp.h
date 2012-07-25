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

#define OUTPUT_HEIGHT 768.0
#define OUTPUT_WIDTH 1024.0

class kinectorApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();
    
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

private:
    // VARS
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
    
public:
    // Parameters
    bool				bDrawPointCloud;
    bool                bDebugDraw;
    bool                bToogleFullScreen;
    bool                bFullscreen;
    int 				nearThreshold;
    int					farThreshold;    
    int					angle;    
    int 				pointCloudRotationY;    
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
