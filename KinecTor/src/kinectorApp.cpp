//
//  kinectorApp.cpp
//  KinectTor
//
//  Created by drambald on 7/24/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <kinectorApp.h>

//--------------------------------------------------------------
void kinectorApp::setup()
{
    kinect.init();
    //kinect.init(true);  // shows infrared instead of RGB video image
	//kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
	kinect.setVerbose(true);
	kinect.open();
    
    // start with the live kinect source
	kinectSource = &kinect;
    
    // Allocate image
    colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
    
    nearThreshold = 230;
	farThreshold  = 70;
	bThreshWithOpenCV = true;
    
    ofSetFrameRate(60);
    
    // VIDEO CONTROLS
	bRecord = false;
	bPlayback = false;
    
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
    
	// start from the front
	pointCloudRotationY = 180;
	bDrawPointCloud = false;
}

//--------------------------------------------------------------
void kinectorApp::update()
{
    // background
    ofBackground(100, 100, 100);
    // update kinect source
	kinectSource->update();
    
    // there is a new frame and we are connected
	if(kinectSource->isFrameNew()) {
        // record ?
		if(bRecord && kinectRecorder.isOpened()) {
			kinectRecorder.newFrame(kinect.getRawDepthPixels(), kinect.getPixels());
		}
        
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinectSource->getDepthPixels(), kinect.width, kinect.height);
        
        // we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayImage;
			grayThreshFar = grayImage;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
		} else {
            
			// or we do it ourselves - show people how they can work with the pixels
			unsigned char * pix = grayImage.getPixels();
            
			int numPixels = grayImage.getWidth() * grayImage.getHeight();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
        
		// update the cv images
		grayImage.flagImageChanged();
        
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    	// also, find holes is set to true so we will get interior contours as well....
    	contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
    }

}

//--------------------------------------------------------------
void kinectorApp::draw()
{
    ofSetColor(255, 255, 255);
    
	if(bDrawPointCloud) {
		ofPushMatrix();
		ofTranslate(420, 320);
		// we need a proper camera class
		drawPointCloud();
		ofPopMatrix();
	} else {
		if(!bPlayback) {
			// draw from the live kinect
			kinect.drawDepth(10, 10, 400, 300);
			kinect.draw(420, 10, 400, 300);
		} else {
			// draw from the player
			kinectPlayer.drawDepth(10, 10, 400, 300);
			kinectPlayer.draw(420, 10, 400, 300);
		}
        
		grayImage.draw(10, 320, 400, 300);
		contourFinder.draw(10, 320, 400, 300);
	}
    
	// draw recording/playback indicators
	ofPushMatrix();
	ofTranslate(25, 25);
	ofFill();
	if(bRecord) {
		ofSetColor(255, 0, 0);
		ofCircle(0, 0, 10);
	}
	if(bPlayback) {
		ofSetColor(0, 255, 0);
		ofTriangle(-10, -10, -10, 10, 10, 0);
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void kinectorApp::drawPointCloud() {
	ofScale(400, 400, 400);
	int w = 640;
	int h = 480;
	ofRotateY(pointCloudRotationY);
	glBegin(GL_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofVec3f cur = kinect.getWorldCoordinateFor(x, y);
			ofColor color = kinect.getCalibratedColorAt(x,y);
			glColor3ub((unsigned char)color.r,(unsigned char)color.g,(unsigned char)color.b);
			glVertex3f(cur.x, cur.y, cur.z);
		}
	}
	glEnd();
}

//--------------------------------------------------------------
void kinectorApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void kinectorApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void kinectorApp::mouseMoved(int x, int y)
{
    pointCloudRotationY = x;
}

//--------------------------------------------------------------
void kinectorApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void kinectorApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void kinectorApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void kinectorApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void kinectorApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void kinectorApp::dragEvent(ofDragInfo dragInfo){ 
    
}

//--------------------------------------------------------------
void kinectorApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	kinectPlayer.close();
	kinectRecorder.close();
}

//--------------------------------------------------------------
void kinectorApp::startRecording() {
    
	// stop playback if running
	stopPlayback();
    
	kinectRecorder.init(ofToDataPath("recording.dat"));
	bRecord = true;
}

//--------------------------------------------------------------
void kinectorApp::stopRecording() {
	kinectRecorder.close();
	bRecord = false;
}

//--------------------------------------------------------------
void kinectorApp::startPlayback() {
    
	stopRecording();
	kinect.close();
    
	// set record file and source
	kinectPlayer.setup(ofToDataPath("recording.dat"), true);
	kinectPlayer.loop();
	kinectSource = &kinectPlayer;
	bPlayback = true;
}

//--------------------------------------------------------------
void kinectorApp::stopPlayback() {
	kinectPlayer.close();
	kinect.open();
	kinectSource = &kinect;
	bPlayback = false;
}