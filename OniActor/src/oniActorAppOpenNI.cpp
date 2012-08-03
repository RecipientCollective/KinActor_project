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
#if defined (TARGET_OSX) //|| defined(TARGET_LINUX) 
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
	recordUser.setUseCloudPoints(isCloud);
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
	playUser.setUseCloudPoints(isCloud);    
	playHandTracker.setup(&playContext, 4);
	playHandTracker.setSmoothing(filterFactor);			
	playHandTracker.setFilterFactors(filterFactor);
	playContext.toggleRegisterViewport();
	playContext.toggleMirror();   
}

void oniActorApp::openniUpdate()
{
#ifdef TARGET_OSX // only working on Mac at the moment
	hardware.update();
#endif
    
    if (isLive) {
        
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

void oniActorApp::openniClose()
{
    // no shutDown, is called on object delete (otherwise...malloc fredddddd!)
    
}