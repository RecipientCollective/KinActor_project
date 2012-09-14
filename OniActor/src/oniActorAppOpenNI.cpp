#include "oniActorApp.h"

//--------------------------------------------------------------
// OPENNI FUNCTIONS
// only working on Mac/Linux at the moment 
// but on Linux you need to run as sudo...
// libusb direct control of motor, LED and accelerometers
// turn off the led just for yacks 
// or for live installation/performances.
// For debug turn LED_BLINK_GREEN 
//--------------------------------------------------------------

void oniActorApp::setupRecording(string _filename)
{
    

    
#if defined (TARGET_OSX) && !defined (NOKINECT)

#ifdef DEBUG
    cerr << "STARTING DEVICE INITIALIZATION..." << endl;
#endif
    
    hardware.setup();
    
#ifdef DEBUG
    hardware.setLedOption(LED_BLINK_GREEN);
#else
	hardware.setLedOption(LED_OFF); 
#endif    

#endif

    // all nodes created by code -> NOT using the xml config file at all
    recordContext.setup();
    //recordContext.setupUsingXMLFile();

    // RECORDERS SETUP
    recordDepth.setup(&recordContext);
    recordImage.setup(&recordContext);
    recordUser.setup(&recordContext);
    // built in openni skeleton smoothing:
    recordUser.setSmoothing(filterFactor);
    recordUser.setUseMaskPixels(isMasking);
    recordUser.setUseCloudPoints(true);
    // use this to set dynamic max number of users 
    // (NB: that a hard upper limit is defined by MAX_NUMBER_USERS in ofxUserGenerator) 
    // MAX_NUMBER_USERS=8    
    recordUser.setMaxNumberOfUsers(MAX_ACTORS);
    recordHandTracker.setup(&recordContext, MAX_HANDS);
    // built in openni hand track smoothing...
    recordHandTracker.setSmoothing(filterFactor);
    // custom smoothing/filtering (can also set per hand with
    // setFilterFactor)...set them all to 0.1f to begin with
    recordHandTracker.setFilterFactors(filterFactor);
    recordContext.toggleRegisterViewport();
    recordContext.toggleMirror();
    oniRecorder.setup(&recordContext, ONI_STREAMING);
    //oniRecorder.setup(&recordContext, ONI_CYCLIC, 60);
    //read the warning in ofxOpenNIRecorder about memory usage with ONI_CYCLIC recording!!!

    // getting sizes from inputUser
    inputWidth = recordUser.getWidth();
    inputHeight = recordUser.getHeight();
    
#ifdef DEBUG		
    cerr << "Input size: width =" << inputWidth << " height = " << inputHeight << endl;
    cerr << "MAX DEPTH: " << recordDepth.getMaxDepth() << "DEPTH WIDTH: " << recordDepth.getWidth() << "DEPTH HEIGHT: " << recordDepth.getHeight() << endl;
    cerr << "... END DEVICE INITIALIZATION" << endl;
#endif
}

void oniActorApp::setupPlayback(string _filename)
{
    playContext.shutdown();
    playContext.setupUsingRecording(ofToDataPath(_filename));
    playDepth.setup(&playContext);
    playImage.setup(&playContext);
    playUser.setup(&playContext);
    playUser.setSmoothing(filterFactor);				
    playUser.setUseMaskPixels(isMasking);
    playUser.setUseCloudPoints(true);    
    playHandTracker.setup(&playContext, MAX_HANDS);
    playHandTracker.setSmoothing(filterFactor);			
    playHandTracker.setFilterFactors(filterFactor);
    playContext.toggleRegisterViewport();
    playContext.toggleMirror();   
}

void oniActorApp::openniUpdate()
{
#if defined (TARGET_OSX) && !defined(NOKINECT)
	hardware.update();
#endif
    
    if (isLive) {
        
#if !defined(NOKINECT)
        // update all nodes
		recordContext.update();
		recordDepth.update();
		recordImage.update();
        
        // demo getting depth pixels directly from depth gen
		depthRangeMask.setFromPixels(recordDepth.getDepthPixels(nearThreshold,farThreshold), recordDepth.getWidth(), recordDepth.getHeight(), OF_IMAGE_GRAYSCALE);
        
        // update tracking/recording nodes
		if (isTracking) recordUser.update();
		if (isRecording) oniRecorder.update();
        
        // demo getting pixels from user gen
		if (isTracking && isMasking) 
        {
			allUserMasks.setFromPixels(recordUser.getUserPixels(), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user1Mask.setFromPixels(recordUser.getUserPixels(1), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user2Mask.setFromPixels(recordUser.getUserPixels(2), recordUser.getWidth(), recordUser.getHeight(), OF_IMAGE_GRAYSCALE);
		}
#endif
        
    } else {
        
        // update all nodes
		playContext.update();
		playDepth.update();
		playImage.update();
        
        // demo getting depth pixels directly from depth gen
		depthRangeMask.setFromPixels(playDepth.getDepthPixels(nearThreshold, farThreshold),
									 playDepth.getWidth(), playDepth.getHeight(), OF_IMAGE_GRAYSCALE);
        
		// update tracking/recording nodes
		if (isTracking) playUser.update();
        
		// demo getting pixels from user gen
		if (isTracking && isMasking) 
        {
			allUserMasks.setFromPixels(playUser.getUserPixels(), playUser.getWidth(), playUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user1Mask.setFromPixels(playUser.getUserPixels(1), playUser.getWidth(), playUser.getHeight(), OF_IMAGE_GRAYSCALE);
			user2Mask.setFromPixels(playUser.getUserPixels(2), playUser.getWidth(), playUser.getHeight(), OF_IMAGE_GRAYSCALE);
        }
    }
}

ofxLimb oniActorApp::getWorldLimb(ofxLimb &rLimb, int user_id, ofxUserGenerator &recorder)
{
    ofxLimb limb;
    XnSkeletonJointPosition a,b;
    recorder.getXnUserGenerator().GetSkeletonCap().GetSkeletonJointPosition(user_id, rLimb.start_joint, a);
    recorder.getXnUserGenerator().GetSkeletonCap().GetSkeletonJointPosition(user_id, rLimb.end_joint, b);
    limb.start_joint = rLimb.start_joint;
    limb.end_joint = rLimb.end_joint;
    limb.position[0] = a.position;
    limb.position[1] = b.position;
    return limb;
}

void oniActorApp::debugLimb(ofxLimb &limb, string name)
{
    cerr << name << ":" << endl;
    cerr << "\tSTART x,y,z: " << limb.position[0].X << ", " << limb.position[0].Y << ", " <<  limb.position[0].Z << endl;
    cerr << "\tEND x,y,z: " << limb.position[1].X << ", " << limb.position[1].Y << ", " <<  limb.position[1].Z << endl;
}

void oniActorApp::debugSkeletons()
{
    ofxUserGenerator currentRecorder = isLive ? recordUser : playUser;
    for(int i = 1; i <= currentRecorder.getNumberOfTrackedUsers(); i++)
    {
        
        if(!currentRecorder.getXnUserGenerator().GetSkeletonCap().IsTracking(i)) 
        {
#ifdef DEBUG        
            cerr << "Not tracking this user: " << i << endl;
#endif
            return;
        }
        
        ofxTrackedUser * user = currentRecorder.getTrackedUser(i);                
        cerr << "USER: " << i << " CENTER x,y,z: " << user->center.X << ", " << user->center.Y << ", " << user->center.Z << endl;
        
        // neck test
        ofxLimb rNeck = getWorldLimb(user->neck, i, currentRecorder);
        debugLimb(user->neck, "NECK PROJECTED");
        debugLimb(rNeck, "NECK REAL");
        
    }
}



void oniActorApp::openniClose()
{
    // no shutDown, is called on object delete (otherwise...malloc fredddddd!)
    
}