//
//  kinectorApp.cpp
//  KinectTor
//
//  Created by drambald on 7/24/12.
//  Copyright (c) 2012 recipient.cc. All rights reserved.
//

#include <kinectorApp.h>

//--------------------------------------------------------------
void kinectorApp::setup()
{
    // INIT KINECT
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
    
    // PARAMETERS SETUP
    nearThreshold = 250;
	farThreshold  = 100;
    ofSetFrameRate(30);
	bRecord = false;
	bPlayback = false;
    bToogleFullScreen = false;
    bFullscreen = false;
	kinectAngle = 0;
	kinect.setCameraTiltAngle(kinectAngle);
    blobMax=2;
	contour_min = 350;
    
	// DRAW CONTROL
    currentFormat = kinector;
	pointCloudRotationY = 180;
    
    setupGUIleft();
    
    // INITIAL BACKGROUND IS GREY
    ofBackground(100,100,100);
}

//--------------------------------------------------------------
void kinectorApp::setupGUIleft()
{
    // SMOOTHING GUI
    ofEnableSmoothing(); 
    ofBackground(0);
    
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float dim = 32;
    float length = 400-xInit; 
    
    // INIT A GUI OBJECT: ofxUICanvas(float x, float y, float width, float height)		
    guileft = new ofxUICanvas(0,0,length+xInit,ofGetHeight());
    guileft->setDrawWidgetPadding(true);
    guileft->addWidgetDown(new ofxUILabel("KINECTOR CONTROLS", OFX_UI_FONT_LARGE));
    guileft->addWidgetDown(new ofxUILabel("Press [h] to hide. [f] for fullscreen", OFX_UI_FONT_LARGE));
    guileft->addWidgetDown(new ofxUILabel("KINECTOR VIEWS: [1] - kinector, [2] - debug, [3] - pointCloud", OFX_UI_FONT_SMALL));
    guileft->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    guileft->addWidgetDown(new ofxUILabel("THRESHOLDS:", OFX_UI_FONT_MEDIUM)); 
    guileft->addWidgetDown(new ofxUISlider(length-xInit,dim, 0.0, 255.0, nearThreshold, "NEAR THRESHOLD")); 
    guileft->addWidgetDown(new ofxUISlider(length-xInit,dim, 0.0, 255.0, farThreshold, "FAR THRESHOLD"));
    guileft->addWidgetDown(new ofxUILabel("Kinect convert the distance in grey pixels [0-255]", OFX_UI_FONT_SMALL));
    guileft->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    guileft->addWidgetDown(new ofxUILabel("KINECT ANGLE [arrows keys]:", OFX_UI_FONT_MEDIUM));
    guileft->addWidgetDown(new ofxUILabelButton(false, "UP", OFX_UI_FONT_MEDIUM)); 
    guileft->addWidgetDown(new ofxUILabelButton(false, "DOWN", OFX_UI_FONT_MEDIUM)); 
    guileft->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    guileft->addWidgetDown(new ofxUILabel("KINECT OPTIONS:", OFX_UI_FONT_MEDIUM));
    guileft->addWidgetDown(new ofxUIToggle( dim, dim, false, "DEPTH NEAR VALUE WHITE"));
    guileft->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    ofAddListener(guileft->newGUIEvent, this, &kinectorApp::guiEvent);
    guileft->loadSettings("GUI/guileftSettings.xml");
    
    // SHOW GUI ON START
    guileft->setVisible(true);
}

//--------------------------------------------------------------
void kinectorApp::update()
{
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
        grayThreshNear = grayImage;
        grayThreshFar = grayImage;
        grayThreshNear.threshold(nearThreshold, true);
        grayThreshFar.threshold(farThreshold);
        cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImage.getCvImage(), NULL);
        
		// update the cv images
		grayImage.flagImageChanged();
        
        /*********************************
         * COUNTOUR FINDER FUNCTION      *
         *********************************
         ofxCvContourFinder::findContours( 
         ofxCvGrayscaleImage&  input,
         int minArea,
         int maxArea,
         int nConsidered,
         bool bFindHoles,
         bool bUseApproximation)         
         find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
         also, find holes is set to true so we will get interior contours as well...  */
        contourFinder.findContours(grayImage, contour_min, (kinect.width*kinect.height)/2, blobMax, false, true);
    }

}

//--------------------------------------------------------------
void kinectorApp::draw()
{
    setFullScreen();
    
    switch (currentFormat) 
    {
        case kinector:
            kinectorDraw();
            break;
        case debug:
            debugDraw();
            break;
        case cloud:
            ofPushMatrix();
            ofTranslate(420, 320);
            // we need a proper camera class
            drawPointCloud();
            ofPopMatrix();
        default:
            break;
    }
}

void kinectorApp::kinectorDraw()
{
    // JUST CONTOUR
    grayImage.draw(400,10);
    for(int i = 0; i < contourFinder.blobs.size(); i++) 
    {
        contourFinder.blobs[i].draw(400,10);
    }
}

void kinectorApp::debugDraw()
{
    ofSetColor(255, 255, 255);

    // DEBUG DRAW
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
    
    // draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
    << ofToString(kinect.getMksAccel().y, 2) << " / "
    << ofToString(kinect.getMksAccel().z, 2) << endl
    << "set near threshold " << nearThreshold  << endl
    << "set far threshold " << farThreshold << " num blobs found " << contourFinder.nBlobs
    << ", fps: " << ofGetFrameRate() << endl
    << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl
    << "press UP and DOWN to change the tilt angle: " << kinectAngle << " degrees" << endl
    << "press r to record and q to playback, record is: " << bRecord << ", playback is: " << bPlayback;
	ofDrawBitmapString(reportStream.str(),20,652);
}

//--------------------------------------------------------------
void kinectorApp::drawPointCloud() 
{
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
void kinectorApp::keyPressed(int key)
{
    switch (key) {
        case '1':
            currentFormat = kinector;
            if (!guileft->isVisible())
                guileft->setVisible(true);
            break;
        case '2':
            currentFormat = debug;
            if (guileft->isVisible())
                guileft->setVisible(false);
            break;
        case '3':
            currentFormat = cloud;
            if (guileft->isVisible())
                guileft->setVisible(false);
            break;            
        case 'f':
			bToogleFullScreen = true;
			break;
        case 'h':
            guileft->toggleVisible();
            break;
		case OF_KEY_UP:
            upKinectAngle();
			break;
        case OF_KEY_DOWN:
            downKinectAngle();
			break;
		case 'o':
			kinect.setCameraTiltAngle(kinectAngle);	// go back to prev tilt
			kinect.open();
			break;           
		case 'c':
			kinect.setCameraTiltAngle(0);		// zero the tilt
			kinect.close();
			break;
		case 'r':
			bRecord = !bRecord;
			if(bRecord) {
				startRecording();
			} else {
				stopRecording();
			}
			break;           
		case 'q':
			bPlayback = !bPlayback;
			if(bPlayback) {
				startPlayback();
			} else {
				stopPlayback();
			}
			break;
	}

}

//--------------------------------------------------------------
void kinectorApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void kinectorApp::mouseMoved(int x, int y)
{
    pointCloudRotationY = x;
}

//--------------------------------------------------------------
void kinectorApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void kinectorApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void kinectorApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void kinectorApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void kinectorApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void kinectorApp::dragEvent(ofDragInfo dragInfo)
{ 
    
}

//--------------------------------------------------------------
void kinectorApp::exit() 
{
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	kinectPlayer.close();
	kinectRecorder.close();
    
    // ON EXIT SAVE GUI and DELETE
    guileft->saveSettings("GUI/guiSettings.xml"); 
    delete guileft;
}

//--------------------------------------------------------------
void kinectorApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "NEAR THRESHOLD")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        nearThreshold = (int) slider->getScaledValue();
       // std::cout << "slider event: " << slider -> getScaledValue() << " , threshold: " << nearThreshold << std::endl;
    } 
    else if(e.widget->getName() == "FAR THRESHOLD") 
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        farThreshold = (int) slider->getScaledValue();
    }
    else if(e.widget->getName() == "UP")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        std::cout << "UP" << std::endl;
        upKinectAngle();
    }
    else if(e.widget->getName() == "DOWN")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        std::cout << "DOWN" << std::endl;        
        downKinectAngle();
    }
    else if(e.widget->getName() == "DEPTH NEAR VALUE WHITE")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
        kinect.enableDepthNearValueWhite(toggle->getValue());
        std::cout << "Depth Near Value is now: " << kinect.isDepthNearValueWhite() << std::endl;
    }
}

//--------------------------------------------------------------
void kinectorApp::startRecording()
{
    
	// stop playback if running
	stopPlayback();
    
	kinectRecorder.init(ofToDataPath("recording.dat"));
	bRecord = true;
}

//--------------------------------------------------------------
void kinectorApp::stopRecording()
{
	kinectRecorder.close();
	bRecord = false;
}

//--------------------------------------------------------------
void kinectorApp::startPlayback() 
{    
	stopRecording();
	kinect.close();
	// set record file and source
	kinectPlayer.setup(ofToDataPath("recording.dat"), true);
	kinectPlayer.loop();
	kinectSource = &kinectPlayer;
	bPlayback = true;
}

//--------------------------------------------------------------
void kinectorApp::stopPlayback() 
{
	kinectPlayer.close();
	kinect.open();
	kinectSource = &kinect;
	bPlayback = false;
}

//--------------------------------------------------------------
void kinectorApp::setFullScreen()
{
    if (bToogleFullScreen && !bFullscreen) {
		ofSetFullscreen(true);
		bFullscreen = true;
		bToogleFullScreen = false;
	} else if (bToogleFullScreen && bFullscreen) {
		bFullscreen = false;
		bToogleFullScreen = false;
		ofSetWindowShape(OUTPUT_WIDTH,OUTPUT_HEIGHT);
		ofSetFullscreen(false);
	}
}

//--------------------------------------------------------------
void kinectorApp::downKinectAngle()
{
    kinectAngle--;
    if(kinectAngle<-30) kinectAngle=-30;
    kinect.setCameraTiltAngle(kinectAngle);
}

//--------------------------------------------------------------
void kinectorApp::upKinectAngle()
{
    kinectAngle++;
    if(kinectAngle>30) kinectAngle=30;
    kinect.setCameraTiltAngle(kinectAngle);
}