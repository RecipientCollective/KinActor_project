#include "oniActorApp.h"

//--------------------------------------------------------------
// GUI FUNCTIONS
// LEFT 400 x WINDOWHEIGHT
//--------------------------------------------------------------

void oniActorApp::setupWindowOptions()
{
    ofSetFrameRate(30);
    ofEnableSmoothing();
    // DIFFERENT BG COLOR FOR DEBUG AND RELEASE
#ifdef DEBUG
    ofBackground(100, 100, 100);
#else
    ofBackground(0, 0, 0);
#endif
}

void oniActorApp::setupGUI()
{
    setupGUIleft();
    setupGUIright();
}

void oniActorApp::updateGUI()
{
#ifdef TARGET_OSX // only working on Mac at the moment    
    string          statusHardware;
    stringstream	statusHardwareStream;    
    ofPoint statusAccelerometers = hardware.getAccelerometers();
    
    statusHardwareStream
	<< " TILT: " << hardware.getTiltAngle() << "/" << hardware.tilt_angle
	<< " x - " << statusAccelerometers.x
	<< " y - " << statusAccelerometers.y
	<< " z - " << statusAccelerometers.z;
    
	statusHardware = statusHardwareStream.str();
    accelerationLabel->setLabel(statusHardware);
#endif
    
    string statusPlay = STATUS_PLAY_DEFAULT + (string)(isLive ? "LIVE" : "PLAY");
    string statusRecord = STATUS_RECORD_DEFAULT + (string)(!isRecording ? "READY" : "RECORDING");
    string statusSkeleton = STATUS_SKELETON_DEFAULT + (string)(isTracking ? "TRACKING USERS: " + (string)(isLive ? ofToString(recordUser.getNumberOfTrackedUsers()) : ofToString(playUser.getNumberOfTrackedUsers())) + "" : "NOT TRACKING USERS");
    string statusSkeletonSmooth = STATUS_SMOOTH_SKELETON_DEFAULT + (string)(isLive ? ofToString(recordUser.getSmoothing()) : ofToString(playUser.getSmoothing()));
    string statusFileName = RECORDING_FILENAME_STR + oniRecorder.getCurrentFileName();
    string statusHands = STATUS_HANDS_DEFAULT + (string)(isTrackingHands ? "TRACKING HANDS: " + (string)(isLive ? ofToString(recordHandTracker.getNumTrackedHands()) : ofToString(playHandTracker.getNumTrackedHands())) + ""  : "NOT TRACKING HANDS");
    string statusFilter = STATUS_FILTER_HANDS_DEFAULT + (string)(isFiltering ? "FILTERING. Level: " + ofToString(filterFactor) : "NOT FILTERING");
    string statusHandSmooth = STATUS_SMOOTH_HANDS_DEFAULT + (string)(isLive ? ofToString(recordHandTracker.getSmoothing()) : ofToString(playHandTracker.getSmoothing()));
    string statusMasks = STATUS_DRAW_MASKS_DEFAULT + (string)(!isMasking ? "HIDE" : (isTracking ? "SHOW" : "YOU NEED TO TURN ON TRACKING!!"));
    string statusCloud = STATUS_CLOUD_DEFAULT + (string)(isCloud ? "ON" : "OFF");
    string statusCloudData = STATUS_CLOUD_DATA_DEFAULT + (string)(isCPBkgnd ? "SHOW BACKGROUND" : (isTracking ? "SHOW USER" : "YOU NEED TO TURN ON TRACKING!!"));
    
    filenameLabel             -> setLabel(statusFileName);
    statusPlayLabel           -> setLabel(statusPlay);
    statusRecordLabel         -> setLabel(statusRecord);
    statusSkeletonLabel       -> setLabel(statusSkeleton);
    statusSkeletonSmoothLabel -> setLabel(statusSkeletonSmooth);
    statusHandLabel           -> setLabel(statusHands);
    statusFilterLabel         -> setLabel(statusFilter);
    statusSmoothHandsLabel    -> setLabel(statusHandSmooth);
    statusDrawMasksLabel      -> setLabel(statusMasks);
    statusCloudLabel          -> setLabel(statusCloud);
    statusCloudDataLabel      -> setLabel(statusCloudData);
}

void oniActorApp::setupGUIleft()
{
    // INIT A GUI OBJECT: 
    // ofxUICanvas(float x, float y, float width, float height)
    guileft = new ofxUICanvas(0,0,guiPanelLength+xInit,ofGetHeight());
    guileft -> setDrawWidgetPadding(true);
    
    guileft->addWidgetDown(new ofxUILabel("ONIACTOR - alpha",OFX_UI_FONT_LARGE));
    guileft->addWidgetDown(new ofxUISpacer(guiPanelLength-xInit, 2));
    
    // WINDOW CONTROLS
    guileft->addWidgetDown(new ofxUILabel("WINDOW",OFX_UI_FONT_MEDIUM));    
    // trPad for the interface, convert mtrx as RATIO and then to the PAD sizes
    int padHeight = (guiPanelLength - xInit) / 3;
    trPad = ofPoint((mtrx/ofGetScreenWidth())*(guiPanelLength-xInit),(mtry/ofGetScreenHeight())*padHeight);
    guileft->addWidgetDown(new ofxUI2DPad(guiPanelLength-xInit, padHeight, trPad, TRANSLATE_PAD));
    guileft->addWidgetDown(new ofxUISlider(guiPanelLength-xInit,dim, 0.0, 10.0, scaleFactor, SCALE_SLIDER));
    guileft->addWidgetDown(new ofxUIFPSSlider(guiPanelLength-xInit, dim, 0, 400, 0, "CURRENT FPS"));
    guileft->addWidgetDown(new ofxUISpacer(guiPanelLength-xInit, 2));
    
    // KINECT LOGS
    guileft->addWidgetDown(new ofxUILabel("KINECT", OFX_UI_FONT_MEDIUM));
    guileft->addWidgetDown(new ofxUILabel("ACCELEROMETERS:", OFX_UI_FONT_SMALL));
    accelerationLabel = new ofxUILabel(ACCEL_STRING_DEFAULT,OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(accelerationLabel);
    statusPlayLabel = new ofxUILabel(STATUS_PLAY_DEFAULT, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(statusPlayLabel);
    statusRecordLabel = new ofxUILabel(STATUS_RECORD_DEFAULT, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(statusRecordLabel);
    filenameLabel = new ofxUILabel(RECORDING_FILENAME_STR, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(filenameLabel);
    statusSkeletonLabel = new ofxUILabel(STATUS_SKELETON_DEFAULT, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(statusSkeletonLabel);
    statusSkeletonSmoothLabel = new ofxUILabel(STATUS_SMOOTH_SKELETON_DEFAULT, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(statusSkeletonSmoothLabel);
    statusHandLabel = new ofxUILabel(STATUS_HANDS_DEFAULT, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(statusHandLabel);
    statusFilterLabel = new ofxUILabel(STATUS_FILTER_HANDS_DEFAULT, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(statusFilterLabel);
    statusSmoothHandsLabel = new ofxUILabel(STATUS_SMOOTH_HANDS_DEFAULT, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(statusSmoothHandsLabel);
    statusDrawMasksLabel = new ofxUILabel(STATUS_DRAW_MASKS_DEFAULT, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(statusDrawMasksLabel);
    statusCloudLabel = new ofxUILabel(STATUS_CLOUD_DEFAULT, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(statusCloudLabel);
    statusCloudDataLabel = new ofxUILabel(STATUS_CLOUD_DATA_DEFAULT, OFX_UI_FONT_SMALL);
    guileft->addWidgetDown(statusCloudDataLabel);
    guileft->addWidgetDown(new ofxUISpacer(guiPanelLength-xInit, 2));    
    
    
    // DRAW HELP AT END
    stringstream _help_str1, _help_str2, _help_str3;
    _help_str1 << "[h]:hide/show interface" << " [f]:fullscreen" << endl;
    _help_str2 << "[ARROWS]:translate view" << " [+/-]:scale" << endl;
    _help_str3 << "[</>]:tilt kinect" << endl;
    guileft->addWidgetDown(new ofxUILabel("HELP",OFX_UI_FONT_LARGE));
    guileft->addWidgetDown(new ofxUILabel(_help_str1.str(), OFX_UI_FONT_SMALL));
    guileft->addWidgetDown(new ofxUILabel(_help_str2.str(), OFX_UI_FONT_SMALL));
    guileft->addWidgetDown(new ofxUILabel(_help_str3.str(), OFX_UI_FONT_SMALL));
    // LISTENER AND LOAD SETTINGS
    ofAddListener(guileft->newGUIEvent, this, &oniActorApp::guiEvent);
    guileft->loadSettings(GUILEFT_FILE);
}

void oniActorApp::setupGUIright()
{
    guiright = new ofxUICanvas(ofGetWindowWidth()-(guiPanelLength+xInit), 0, guiPanelLength+xInit,ofGetHeight());
    guiright -> setDrawWidgetPadding(true);
    
    guiright->addWidgetDown(new ofxUILabel("OpenNI - CONTROLS",OFX_UI_FONT_LARGE));
    guiright->addWidgetDown(new ofxUISpacer(guiPanelLength-xInit, 2));
    guiright->addWidgetDown(new ofxUILabel("SOURCE",OFX_UI_FONT_MEDIUM));
    float half_panel = ((guiPanelLength-xInit) / 2.0f) - xInit;
    recordToggle = new ofxUILabelToggle(half_panel, isRecording, RECORDING_TOGGLE, OFX_UI_FONT_MEDIUM);
    playbackToggle = new ofxUILabelToggle(half_panel, isRecording, PLAYBACK_TOGGLE, OFX_UI_FONT_MEDIUM);
    guiright->addWidgetDown(recordToggle);
    guiright->addWidgetEastOf(playbackToggle, RECORDING_TOGGLE);
    guiright->addWidgetDown(new ofxUISpacer(guiPanelLength-xInit, 2));
    guiright->addWidgetDown(new ofxUILabel("TRACKING",OFX_UI_FONT_MEDIUM));
    guiright->addWidgetDown(new ofxUILabelToggle(guiPanelLength - xInit, isTracking, SKELETON_TRACKING_TOGGLE, OFX_UI_FONT_MEDIUM));
    guiright->addWidgetDown(new ofxUILabelToggle(half_panel, isTrackingHands, HANDS_TRACKING_TOGGLE, OFX_UI_FONT_MEDIUM));
    guiright->addWidgetEastOf(new ofxUILabelToggle(half_panel, isFiltering, HANDS_FILTER_TOGGLE, OFX_UI_FONT_MEDIUM), HANDS_TRACKING_TOGGLE);
    guiright->addWidgetDown(new ofxUISlider(guiPanelLength-xInit,dim, 0.0, recordDepth.getMaxDepth(), nearThreshold, NEAR_THRESHOLD_SLIDER)); 
    guiright->addWidgetDown(new ofxUISlider(guiPanelLength-xInit,dim, 0.0, recordDepth.getMaxDepth(), farThreshold, FAR_THRESHOLD_SLIDER));
    guiright->addWidgetDown(new ofxUISlider(guiPanelLength-xInit,dim, 0.0,1.0, recordUser.getSmoothing(), SMOOTH_SKELETON_SLIDER));
    guiright->addWidgetDown(new ofxUISlider(guiPanelLength-xInit,dim, 0.0,1.0, filterFactor, FILTER_HANDS_SLIDER));
    guiright->addWidgetDown(new ofxUISlider(guiPanelLength-xInit,dim, 0.0,1.0, recordHandTracker.getSmoothing(), SMOOTH_HANDS_SLIDER));

    guiright->addWidgetDown(new ofxUISpacer(guiPanelLength-xInit, 2));
    guiright->addWidgetDown(new ofxUILabel("VIEW",OFX_UI_FONT_MEDIUM));
    guiright->addWidgetDown(new ofxUILabelToggle(guiPanelLength-xInit, isMasking, MASKING_TOGGLE, OFX_UI_FONT_MEDIUM));
    
    // REMOVE THIS FIXME
    guiright->addWidgetDown(new ofxUILabelToggle(half_panel, isFiltering, DRAWCLOUD_TOGGLE, OFX_UI_FONT_MEDIUM));
    guiright->addWidgetEastOf(new ofxUILabelToggle(half_panel, isFiltering, DRAWCPB_TOGGLE, OFX_UI_FONT_MEDIUM), DRAWCLOUD_TOGGLE);
    
        
    // LISTENER AND LOAD SETTINGS
    ofAddListener(guiright->newGUIEvent, this, &oniActorApp::guiEvent);
    guiright->loadSettings(GUIRIGHT_FILE);
}

void oniActorApp::closeGUI()
{
    // ON EXIT SAVE GUI and DELETE
    guileft->saveSettings(GUILEFT_FILE); 
    delete guileft;
    guiright->saveSettings(GUIRIGHT_FILE);
    delete guiright;
}

void oniActorApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == TRANSLATE_PAD)
    {
        ofxUI2DPad *pad = (ofxUI2DPad *) e.widget;
#ifdef DEBUG
        std::cerr << "PAD % VALUES x:" << pad->getPercentValue().x << " y:" << pad->getPercentValue().y << std::endl;
        std::cerr << "Conversion x:" << pad->getPercentValue().x * ofGetWindowWidth() << " y:" << pad->getPercentValue().y * ofGetWindowHeight() << std::endl;
#endif
        mtrx = pad->getPercentValue().x * ofGetWindowWidth();
        mtry = pad->getPercentValue().y * ofGetWindowWidth();
    }
    else if(e.widget->getName() == SCALE_SLIDER)
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        scaleFactor = slider->getScaledValue();
    }
    else if(e.widget->getName() == NEAR_THRESHOLD_SLIDER)
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        nearThreshold = (int) slider->getScaledValue();
    }
    else if(e.widget->getName() == FAR_THRESHOLD_SLIDER)
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        farThreshold = (int) slider->getScaledValue();
    }
    else if(e.widget->getName() == SMOOTH_SKELETON_SLIDER)
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        recordUser.setSmoothing(slider->getScaledValue());
        playUser.setSmoothing(slider->getScaledValue());
    }
    else if(e.widget->getName() == FILTER_HANDS_SLIDER)
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        filterFactor = slider->getScaledValue();
    }
    else if(e.widget->getName() == SMOOTH_HANDS_SLIDER)
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        recordHandTracker.setSmoothing(slider->getScaledValue());
        playHandTracker.setSmoothing(slider->getScaledValue());
    }
    else if(e.widget->getName() == RECORDING_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        // CHECK FOR STATE CHANGE
        if (toggle->getValue() != isRecording)
        {     
#ifdef DEBUG		
            cerr << endl << "RECORD STATE CHANGE: " << toggle->getValue() << "," << isRecording << endl;
#endif                
            isRecording = toggle->getValue();
            if (isRecording) {
                if (playbackToggle->getValue()) {
                    // IS IN PLAYBACK ... STOP
                    playbackToggle->setValue(false);
                    isLive = true;
                }
                oniRecorder.startRecord(generateFileName());
            } else {
                oniRecorder.stopRecord();
            }
        }
    }
    else if(e.widget->getName() == PLAYBACK_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        // CHECK FOR STATE CHANGE
        bool isPlayBack = !isLive;
        if (toggle->getValue() != isPlayBack)
        {
#ifdef DEBUG		
            cerr << endl << "PLAYBACK STATE CHANGE: " << toggle->getValue() << "," << isPlayBack << endl;
#endif      
            isPlayBack = toggle -> getValue();
            if (isPlayBack)
            {
                if (oniRecorder.getCurrentFileName() != "" && !isRecording && isLive) {
                    setupPlayback(oniRecorder.getCurrentFileName());
                    isLive = false;
                } else {
                    toggle->setValue(false);
                }
            } else {
#ifdef DEBUG		
                cerr << endl << "PLAYBACK STATE CHANGE CRASH HERE? isLive: " << isLive << endl;
#endif
        
                // FIXME CRASH HERE ON CLOSE PLAYBACK
                isLive = true;
            }
        }
    }
    else if(e.widget->getName() == SKELETON_TRACKING_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        isTracking = toggle->getValue();
    }
    else if(e.widget->getName() == HANDS_TRACKING_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
#ifdef DEBUG		
        cerr << endl << "HANDS STATE CHANGE: " << toggle->getValue() << "," << isTrackingHands << endl;
#endif  
        isTrackingHands = toggle->getValue();
        if (isTrackingHands) {
            isLive ? recordHandTracker.startTrackHands() : playHandTracker.startTrackHands();
        } else {
            isLive ? recordHandTracker.stopTrackHands() : playHandTracker.stopTrackHands();            
        }
        
    }
    else if(e.widget->getName() == HANDS_FILTER_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
#ifdef DEBUG		
        cerr << endl << "HANDS FILTER TOGGLE CHANGE: " << toggle->getValue() << "," << isFiltering << endl;
#endif
        isFiltering = toggle -> getValue();
        recordHandTracker.isFiltering = isFiltering;
        playHandTracker.isFiltering = isFiltering;
    }
    else if(e.widget->getName() == MASKING_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        isMasking = toggle -> getValue();
        recordUser.setUseMaskPixels(isMasking);
        playUser.setUseMaskPixels(isMasking);
    }
    else if(e.widget->getName() == DRAWCLOUD_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        isCloud = toggle -> getValue();
        recordUser.setUseCloudPoints(isCloud);
        playUser.setUseCloudPoints(isCloud);
    }
    else if(e.widget->getName() == DRAWCPB_TOGGLE)
    {
        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        isCPBkgnd = toggle -> getValue();
    }
}

void oniActorApp::showInterface()
{
    guileft->setVisible(true);
    guiright->setVisible(true);
}

void oniActorApp::hideInterface()
{
    guileft->setVisible(false);
    guiright->setVisible(false);
}

void oniActorApp::setFullScreen()
{
    if (toggleFullScreen && !isFullScreen) {
        isFullScreen = true;
        toggleFullScreen = false;        
        ofSetFullscreen(true);        
    } else if (toggleFullScreen && isFullScreen) {
        isFullScreen = false;
        toggleFullScreen = false;        
        ofSetWindowShape(OUTPUT_WIDTH,OUTPUT_HEIGHT);
        ofSetFullscreen(false);
    }
}