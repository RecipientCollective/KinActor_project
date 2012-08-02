//
//  kinactorApp.cpp
//  KinectTor
//
//  Created by drambald on 7/24/12.
//  Copyright (c) 2012 recipient.cc. All rights reserved.
//

#include <kinactorApp.h>

//--------------------------------------------------------------
// OF MAIN FUNCTIONS
//--------------------------------------------------------------
void kinactorApp::setup()
{
    
#ifdef DEBUG		
    cerr << ".... KINACTOR DEBUG MODE ...." << endl 
    << "Entering setup:" << endl;
#endif

    // GENERAL SETUP
    ofSetFrameRate(30);
    bToogleFullScreen = false;
    bFullscreen = false;
    bShowInterface = true;
    bBox = false;

    // STORE INPUT SIZE
    inputWidth = 640;
    inputHeight = 480;
    
#ifdef DEBUG		
    std::cerr << "Input size: width =" << inputWidth << " height = " << inputHeight << endl;
#endif
        	     
    // GUIS STUFFS
    xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    dim = 24;
    guiPanelLength = 400-xInit;
    loggerH = (ofGetWindowHeight() / 3.5);
    loggerW = guiPanelLength+xInit;
    setupGUIleft();
    setupGUIright();
    setupGUIlogger();

    // OF DRAW STUFFS
    currentFormat = kinactor;
	scaleFactor = 1.0;    
    // In the default draw I have an image on the left of the interface: 410 x, 10 y
	mtrx = TRANSLATE_GUIVIEW_X;
	mtry = TRANSLATE_GUIVIEW_Y;
    ofEnableSmoothing();
    ofBackground(100, 100, 100);
#ifdef DEBUG		
    cerr << "END setup" << endl;
#endif

}

void kinactorApp::update()
{
    
}

void kinactorApp::draw()
{
    setFullScreen();
    
    switch (currentFormat) 
    {
        case kinactor:
            kinactorDraw();
            break;
        case debug:
            debugDraw();
            break;
        case cloud:
            drawPointCloud();
        default:
            break;
    }
    
    // show interface?
    bShowInterface == true ? showInterface() : hideInterface();
}

void kinactorApp::keyPressed(int key)
{
    switch (key) {
        case '1':
            currentFormat = kinactor;
            bShowInterface = true;
            break;
        case '2':
            currentFormat = debug;
            bShowInterface = true;
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
		case OF_KEY_UP:
            mtry++;
#ifdef DEBUG
            std::cerr << "Translate y: " << mtry << std::endl;
#endif
			break;
		case OF_KEY_DOWN:
			mtry--;
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
// GUI FUNCTIONS
//--------------------------------------------------------------
void kinactorApp::setupGUIright()
{
    guiright = new ofxUICanvas(ofGetWindowWidth()-(guiPanelLength+xInit),inputHeight+(xInit*4),guiPanelLength+xInit,ofGetHeight()-(inputHeight+(xInit*4)));
    guiright->setDrawWidgetPadding(true);
    guiright->addWidgetDown(new ofxUILabel("OpenNI CONTROLS", OFX_UI_FONT_LARGE));
    guiright->addWidgetDown(new ofxUILabel("Kinect convert the distance in grey pixels [0-255]", OFX_UI_FONT_SMALL));    
    ofAddListener(guiright->newGUIEvent, this, &kinactorApp::guiEvent);
    guiright->loadSettings("GUI/guirightSettings.xml");
}

void kinactorApp::setupGUIleft()
{
    // INIT A GUI OBJECT: ofxUICanvas(float x, float y, float width, float height)		
    guileft = new ofxUICanvas(0,0,guiPanelLength+xInit,ofGetHeight()-(loggerH+xInit));
    guileft->setDrawWidgetPadding(true);
    guileft->addWidgetDown(new ofxUILabel("CONTROLS", OFX_UI_FONT_LARGE));
    guileft->addWidgetDown(new ofxUILabel("Press [h] to hide. [f] for fullscreen", OFX_UI_FONT_LARGE));
    guileft->addWidgetDown(new ofxUILabel("kinactor VIEWS: [1] - kinactor, [2] - debug, [3] - pointCloud", OFX_UI_FONT_SMALL));
    guileft->addWidgetDown(new ofxUISpacer(guiPanelLength-xInit, 2));
    guileft->addWidgetDown(new ofxUILabel("KINECT:", OFX_UI_FONT_MEDIUM)); 
    guileft->addWidgetDown(new ofxUILabelButton(false, "TILT UP [>]", OFX_UI_FONT_SMALL)); 
    guileft->addWidgetDown(new ofxUILabelButton(false, "TILT DOWN [<]", OFX_UI_FONT_SMALL)); 
    guileft->addWidgetDown(new ofxUILabelButton(false, "CONNECT [o]", OFX_UI_FONT_SMALL));
    guileft->addWidgetDown(new ofxUILabelButton(false, "DISCONNECT [c]", OFX_UI_FONT_SMALL));
    guileft->addWidgetDown(new ofxUISpacer(guiPanelLength-xInit, 2));
    guileft->addWidgetDown(new ofxUILabel("WINDOW OPTIONS:", OFX_UI_FONT_MEDIUM));
    
    // trPad for the interface, convert mtrx as RATIO and then to the PAD sizes
    int padHeight = (guiPanelLength - xInit) / 3;
    trPad = ofPoint((mtrx/ofGetScreenWidth())*(guiPanelLength-xInit),(mtry/ofGetScreenHeight())*padHeight);
    guileft->addWidgetDown(new ofxUI2DPad(guiPanelLength-xInit, padHeight, trPad, "TRANSLATE"));
    guileft->addWidgetDown(new ofxUISlider(guiPanelLength-xInit,dim, 0.0, 10.0,scaleFactor, "SCALE")); 
    ofAddListener(guileft->newGUIEvent, this, &kinactorApp::guiEvent);
    guileft->loadSettings("GUI/guileftSettings.xml");
}

void kinactorApp::setupGUIlogger()
{
    // INIT A GUI OBJECT: ofxUICanvas(float x, float y, float width, float height)
    loggerP = ofPoint(0,(ofGetWindowHeight() - loggerH));
    guilogger = new ofxUICanvas(loggerP.x,loggerP.y,loggerW,loggerH);
    guilogger->setDrawWidgetPadding(true);
    guilogger->addWidgetDown(new ofxUILabel("LOGGER", OFX_UI_FONT_LARGE));
    ofAddListener(guilogger->newGUIEvent, this, &kinactorApp::guiEvent);
    guilogger->loadSettings("GUI/guiloggerSettings.xml");
}

void kinactorApp::showInterface()
{
    guileft->setVisible(true);
    guilogger->setVisible(true);
    guiright->setVisible(true);
    loggerDraw();
    bBox = true;
}

void kinactorApp::hideInterface()
{
    guileft->setVisible(false);
    guilogger->setVisible(false);
    guiright->setVisible(false);
    bBox = false;
}

void kinactorApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "TILT UP [>]")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        upKinectAngle();
    }
    else if(e.widget->getName() == "TILT DOWN [<]")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        downKinectAngle();
    }
    else if(e.widget->getName() == "CONNECT [o]")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        kinectConnect();
    }
    else if(e.widget->getName() == "DISCONNECT [c]")
    {
        ofxUIButton *button = (ofxUIButton *) e.widget;
        kinectDisconnect();
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

void kinactorApp::exit() 
{

#ifdef DEBUG
    std::cerr << "START OF EXIT ROUTINE ...." << std::endl;
    std::cerr << "... Saving and closing GUI ..." << std::endl;    
#endif
    
    // ON EXIT SAVE GUI and DELETE
    guileft->saveSettings("GUI/guileftSettings.xml");
    guiright->saveSettings("GUI/guirightSettings.xml");
    guilogger->saveSettings("GUI/guiloggerSettings.xml");
    delete guileft;
    delete guiright;
    delete guilogger;
    
#ifdef DEBUG
    std::cerr << "END OF EXIT ROUTINE ...." << std::endl;
#endif
    
    OF_EXIT_APP(0);
}

void kinactorApp::setFullScreen()
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

void kinactorApp::downKinectAngle()
{
#ifdef DEBUG
    std::cerr << "DOWN" << std::endl;
#endif
}

void kinactorApp::upKinectAngle()
{
#ifdef DEBUG
    std::cerr << "UP" << std::endl;
#endif
}

void kinactorApp::kinectConnect()
{   
#ifdef DEBUG
    std::cerr << "KINECT: CONNECTED" << std::endl;
#endif
}

void kinactorApp::kinectDisconnect()
{
#ifdef DEBUG
    std::cerr << "KINECT: DISCONNECTED" << std::endl;
#endif
}

//--------------------------------------------------------------
// DRAW FUNCTIONS
//--------------------------------------------------------------
void kinactorApp::loggerDraw()
{
    ofSetColor(255, 255, 0);
    stringstream reportStream;
    reportStream << "Logger" << endl;
    ofDrawBitmapString(reportStream.str(),(loggerP.x + xInit),(loggerP.y + (xInit * 6)));
}

void kinactorApp::kinactorDraw()
{
    // JUST CONTOUR FOR NOW
    // ORIGINAL IMAGE SIZE IS 640x480
    
    ofPushMatrix();
	ofScale(scaleFactor, scaleFactor, 1.0);
	ofTranslate(mtrx, mtry, 1.0);
    
    if (bBox)
        drawBox();
       
    ofPopMatrix();
}

void kinactorApp::drawBox()
{
    ofPushStyle();
    ofSetColor(255, 255, 255);
    ofNoFill();
    ofRect(0, 0, inputWidth, inputHeight);
    ofPopStyle();    
}

void kinactorApp::debugDraw()
{
    /********************************* 
     *   DEBUG DRAW 
     *********************************/
    ofPushMatrix();
    ofSetColor(255, 255, 255);
    ofTranslate(TRANSLATE_GUIVIEW_X, TRANSLATE_GUIVIEW_Y);
    
    
    ofPopMatrix();    
}

void kinactorApp::drawPointCloud() 
{

}


//--------------------------------------------------------------
// OTHER OF FUNCTIONS
//--------------------------------------------------------------
void kinactorApp::keyReleased(int key)
{
    
}

void kinactorApp::mouseMoved(int x, int y)
{
    
}

void kinactorApp::mouseDragged(int x, int y, int button)
{
    
}

void kinactorApp::mousePressed(int x, int y, int button)
{
    
}

void kinactorApp::mouseReleased(int x, int y, int button)
{
    
}

void kinactorApp::windowResized(int w, int h)
{
    
}

void kinactorApp::gotMessage(ofMessage msg)
{
    
}

void kinactorApp::dragEvent(ofDragInfo dragInfo)
{ 
    
}

