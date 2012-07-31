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
#ifdef DEBUG		
    std::cerr << "DEBUG MODE" << std::endl;
#endif
    
    // INIT KINECT
    kinect.init();
    //kinect.init(true);  // shows infrared instead of RGB video image
	//kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
	kinect.setVerbose(true);
	kinect.open();
    
    // start with the live kinect source
	kinectSource = &kinect;

#ifdef DEBUG		
    std::cerr << "Input size: width =" << kinect.width << " height = " << kinect.height << endl;
#endif
    
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
    bShowInterface = true;
    bBox = false;
	kinectAngle = 0;
	kinect.setCameraTiltAngle(kinectAngle);
    blobMax=2;
	contour_min = 350;
    
	// DRAW CONTROL
    currentFormat = kinector;
	pointCloudRotationY = 180;
	scaleFactor = 1.0;
    
    // In the default draw I have an image on the left of the interface: 410 x, 10 y
	mtrx = 410;
	mtry = 10;
    
    // GUIS STUFFS
    xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    dim = 24;
    length = 400-xInit;
    ofEnableSmoothing(); 
    ofBackground(0);
    setupGUIleft();
    setupGUIbottom();
    
    // INITIAL BACKGROUND IS GREY
    ofBackground(100,100,100);
}

//--------------------------------------------------------------
void kinectorApp::setupGUIleft()
{
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
    guileft->addWidgetDown(new ofxUILabel("KINECT ANGLE [< >]:", OFX_UI_FONT_MEDIUM));
    guileft->addWidgetDown(new ofxUILabelButton(false, "UP", OFX_UI_FONT_MEDIUM)); 
    guileft->addWidgetDown(new ofxUILabelButton(false, "DOWN", OFX_UI_FONT_MEDIUM)); 
    guileft->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    guileft->addWidgetDown(new ofxUILabel("KINECT CONNECTION:", OFX_UI_FONT_MEDIUM));
    guileft->addWidgetDown(new ofxUILabelButton(false, "CONNECT [o]", OFX_UI_FONT_MEDIUM));
    guileft->addWidgetDown(new ofxUILabelButton(false, "DISCONNECT [c]", OFX_UI_FONT_MEDIUM));
    guileft->addWidgetDown(new ofxUIToggle( dim, dim, false, "DEPTH NEAR VALUE WHITE"));
    guileft->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    guileft->addWidgetDown(new ofxUILabel("PLAYBACK/RECORD", OFX_UI_FONT_MEDIUM));
    playbackToggle = new ofxUIToggle( dim, dim, false, "PLAYBACK [p]");
    guileft->addWidgetDown(playbackToggle);
    recordToggle = new ofxUIToggle( dim, dim, false, "RECORD [r]");
    guileft->addWidgetDown(recordToggle);
    guileft->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    guileft->addWidgetDown(new ofxUILabel("WINDOW OPTIONS:", OFX_UI_FONT_MEDIUM));
    
    // trPad for the interface, convert mtrx as RATIO and then to the PAD sizes
    int padHeight = (length - xInit) / 2;
    trPad = ofPoint((mtrx/ofGetScreenWidth())*(length-xInit),(mtry/ofGetScreenHeight())*padHeight);
    guileft->addWidgetDown(new ofxUI2DPad(length-xInit,padHeight, trPad, "TRANSLATE"));
    guileft->addWidgetDown(new ofxUISlider(length-xInit,dim, 0.0, 10.0, scaleFactor, "SCALE")); 
    guileft->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    ofAddListener(guileft->newGUIEvent, this, &kinectorApp::guiEvent);
    guileft->loadSettings("GUI/guileftSettings.xml");
}

//--------------------------------------------------------------
void kinectorApp::setupGUIbottom()
{
    // INIT A GUI OBJECT: ofxUICanvas(float x, float y, float width, float height)
    loggerH = (ofGetWindowHeight() / 3);
    loggerW = (ofGetWindowWidth() -(length+(xInit*2)));
    loggerP = ofPoint(length+(xInit*2),(ofGetWindowHeight() - loggerH));
    guibottom = new ofxUICanvas(loggerP.x,loggerP.y,loggerW,loggerH);
    guibottom->setDrawWidgetPadding(true);
    guibottom->addWidgetDown(new ofxUILabel("LOGGER", OFX_UI_FONT_LARGE));
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
    
    // show interface?
    bShowInterface == true ? showInterface() : hideInterface();
}

//--------------------------------------------------------------
void kinectorApp::loggerDraw()
{
    ofSetColor(255, 255, 255);
    stringstream reportStream;
    reportStream << "Accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
    << ofToString(kinect.getMksAccel().y, 2) << " / "
    << ofToString(kinect.getMksAccel().z, 2) << endl
    << "Near threshold " << nearThreshold  << endl
    << "Far threshold " << farThreshold << endl
    << "Num blobs found " << contourFinder.nBlobs
    << ", fps: " << ofGetFrameRate() << endl
    << "Kinect connection is: " << kinect.isConnected() << endl
    << "Tilt angle: " << kinectAngle << " degrees" << endl
    << "Record is: " << bRecord << ", playback is: " << bPlayback;
    ofDrawBitmapString(reportStream.str(),(loggerP.x + xInit),(loggerP.y + (xInit * 6)));
}

//--------------------------------------------------------------
void kinectorApp::kinectorDraw()
{
    // JUST CONTOUR FOR NOW
    // ORIGINAL IMAGE SIZE IS 640x480
    
    ofPushMatrix();
	ofScale(scaleFactor, scaleFactor, 1.0);
	ofTranslate(mtrx, mtry, 1.0);
    
    if (bBox) {
		ofPushStyle();
		ofSetColor(255, 255, 255);
		ofNoFill();
		ofRect(0, 0, kinect.width, kinect.height);
		ofPopStyle();
	}
    
    grayImage.draw(0,0);
    for(int i = 0; i < contourFinder.blobs.size(); i++) 
    {
        contourFinder.blobs[i].draw(0,0);
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
    
    ofPopMatrix();
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
void kinectorApp::showInterface()
{
    guileft->setVisible(true);
    guibottom->setVisible(true);
    loggerDraw();
    bBox = true;
}

//--------------------------------------------------------------
void kinectorApp::hideInterface()
{
    guileft->setVisible(false);
    guibottom->setVisible(false);
    bBox = false;
}

//--------------------------------------------------------------
void kinectorApp::keyPressed(int key)
{
    switch (key) {
        case '1':
            currentFormat = kinector;
            bShowInterface = true;
            break;
        case '2':
            currentFormat = debug;
            bShowInterface = false;
            break;
        case '3':
            currentFormat = cloud;
            bShowInterface = false;
            break;            
        case 'f':
			bToogleFullScreen = true;
			break;
        case 'h':
            bShowInterface = !bShowInterface;
            break;
		case '>':
        case '.':
            upKinectAngle();
			break;
        case '<':
        case ',':
            downKinectAngle();
			break;
		case 'o':
            kinectConnect();
			break;           
		case 'c':
            kinectDisconnect();
			break;
		case 'r':
            toggleRecord();
			break;           
		case 'p':
            togglePlayback();
			break;
		case OF_KEY_UP:
            mtry--;
#ifdef DEBUG
            std::cerr << "Translate y: " << mtry << std::endl;
#endif
			break;
		case OF_KEY_DOWN:
			mtry++;
#ifdef DEBUG
            std::cerr << "Translate y: " << mtry << std::endl;
#endif            
			break;
		case OF_KEY_LEFT:
			mtrx--;
#ifdef DEBUG
            std::cerr << "Translate x: " << mtrx << std::endl;
#endif            
			break;
		case OF_KEY_RIGHT:
			mtrx++;
#ifdef DEBUG
            std::cerr << "Translate x: " << mtrx << std::endl;
#endif
			break;
        case '-':
            scaleFactor-=0.01;
            break;
        case '+':
        case '=':
            scaleFactor+=0.01;
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

#ifdef DEBUG
    std::cerr << "END OF EXIT ROUTINE ...." << std::endl;
#endif
    
    OF_EXIT_APP(0);
}

//--------------------------------------------------------------
void kinectorApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "NEAR THRESHOLD")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        nearThreshold = (int) slider->getScaledValue();
#ifdef DEBUG
        std::cerr << "slider event: " << slider -> getScaledValue() << " , threshold: " << nearThreshold << std::endl;
#endif
        
    } 
    else if(e.widget->getName() == "FAR THRESHOLD") 
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        farThreshold = (int) slider->getScaledValue();
    }
    else if(e.widget->getName() == "UP")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        upKinectAngle();
    }
    else if(e.widget->getName() == "DOWN")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        downKinectAngle();
    }
    else if(e.widget->getName() == "DEPTH NEAR VALUE WHITE")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget; 
        kinect.enableDepthNearValueWhite(toggle->getValue());
#ifdef DEBUG
        std::cerr << "Depth Near Value is now: " << kinect.isDepthNearValueWhite() << std::endl;
#endif
    }
    else if(e.widget->getName() == "CONNECT")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        kinectConnect();
    }
    else if(e.widget->getName() == "DISCONNECT")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        kinectDisconnect();
    }
    else if(e.widget->getName() == "RECORD")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        if (toggle->getValue()) {
            startRecording();
        } else {
            stopRecording();
        }
    }
    else if(e.widget->getName() == "PLAYBACK")
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        if (toggle->getValue()) {
            startPlayback();
        } else {
            stopPlayback();
        }
    }
    else if(e.widget->getName() == "TRANSLATE")
    {
        ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
#ifdef DEBUG
        std::cerr << "PAD % VALUES x:" << pad->getPercentValue().x << " y:" << pad->getPercentValue().y << std::endl;
        std::cerr << "Conversion x:" << pad->getPercentValue().x * ofGetWindowWidth() << " y:" << pad->getPercentValue().y * ofGetWindowHeight() << std::endl;
#endif
        mtrx = pad->getPercentValue().x * ofGetWindowWidth();
        mtry = pad->getPercentValue().y * ofGetWindowWidth();
    }
    else if(e.widget->getName() == "SCALE")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        scaleFactor = slider->getScaledValue();
    }
    
}

//--------------------------------------------------------------
void kinectorApp::startRecording()
{
    
	// stop playback if running
	stopPlayback();
    recordToggle->setValue(true);
    playbackToggle->setValue(false);
	kinectRecorder.init(ofToDataPath("recording.dat"));
	bRecord = true;
}

//--------------------------------------------------------------
void kinectorApp::stopRecording()
{
	kinectRecorder.close();
	bRecord = false;
    recordToggle->setValue(false);
}

//--------------------------------------------------------------
void kinectorApp::startPlayback() 
{    
	stopRecording();
	kinect.close();
    recordToggle->setValue(false);
    playbackToggle->setValue(true);
	// set record file and source
	kinectPlayer.setup(ofToDataPath("recording.dat"), true);
	kinectPlayer.loop();
	kinectSource = &kinectPlayer;
	bPlayback = true;
}

//--------------------------------------------------------------
void kinectorApp::stopPlayback() 
{
    playbackToggle->setValue(false);
	kinectPlayer.close();
	kinect.open();
	kinectSource = &kinect;
	bPlayback = false;
}

//--------------------------------------------------------------
void kinectorApp::toggleRecord()
{
    bRecord = !bRecord;
    if(bRecord) {
        startRecording();
    } else {
        stopRecording();
    }
}

//--------------------------------------------------------------
void kinectorApp::togglePlayback()
{
    bPlayback = !bPlayback;
    if(bPlayback) {
        startPlayback();
    } else {
        stopPlayback();
    }    
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
#ifdef DEBUG
    std::cerr << "DOWN" << std::endl;
#endif
}

//--------------------------------------------------------------
void kinectorApp::upKinectAngle()
{
    kinectAngle++;
    if(kinectAngle>30) kinectAngle=30;
    kinect.setCameraTiltAngle(kinectAngle);
#ifdef DEBUG
    std::cerr << "UP" << std::endl;
#endif
}

//--------------------------------------------------------------
void kinectorApp::kinectConnect()
{
    kinect.setCameraTiltAngle(kinectAngle);	// go back to prev tilt
    kinect.open();    
#ifdef DEBUG
    std::cerr << "KINECT: CONNECTED" << std::endl;
#endif
}

//--------------------------------------------------------------
void kinectorApp::kinectDisconnect()
{
    kinect.setCameraTiltAngle(0);		// zero the tilt
    kinect.close();    
#ifdef DEBUG
    std::cerr << "KINECT: DISCONNECTED" << std::endl;
#endif
}
