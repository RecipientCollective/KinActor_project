#include "oniActorApp.h"

//--------------------------------------------------------------
// OF MAIN FUNCTIONS
//--------------------------------------------------------------
void oniActorApp::setup()
{
    
#ifdef DEBUG		
    cerr << endl << ".... ONIACTOR DEBUG MODE ...." << endl 
    << "Entering setup-->" << endl;
#endif
    
    isLive			= true;
	isTracking		= false;
	isTrackingHands	= true;
	isFiltering		= false;
	isRecording		= false;
	isCloud			= false;
	isCPBkgnd		= true;
	isMasking		= true;
    
	nearThreshold = 500;
	farThreshold  = 1000;
    
	filterFactor = 0.1f;
    
    setupRecording(); // oniActorAppOpenNI.cpp
    
    // GUI SETUP
    xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    dim = 24;
    guiPanelLength = GUIPANEL_BASE_LENGTH-xInit;
    
    isFullScreen = false;
    toggleFullScreen = false;
    toggleShowInterface = true;
    setupGUI();
    
    currentFormat = oniactor;
    mtrx = GUIPANEL_BASE_LENGTH + xInit;
    mtry = xInit;
    scaleFactor = 1.0;
    
    // OF stuffs, background ...
    setupWindowOptions(); // oniActorAppGUI.cpp 

#ifdef DEBUG		
    cerr << endl << "<--End setup" << endl;
#endif

}

void oniActorApp::update()
{
    openniUpdate();
    updateGUI();
}

void oniActorApp::draw()
{
    setFullScreen();
    
    switch (currentFormat) 
    {
        case oniactor:
            oniactorDraw();            
            break;
        
        case debug:
            debugDraw();
            break;
            
        case cloud:
            cloudDraw();
            break;
            
        default:
            break;
    }
    
    // show interface?
    toggleShowInterface == true ? showInterface() : hideInterface();
}

void oniActorApp::keyPressed(int key)
{
    switch (key) 
    {
        case 'f':
			toggleFullScreen = true;
			break;
        case 'h':
            toggleShowInterface = !toggleShowInterface;
            break;
        case '1':
            currentFormat = oniactor;
            toggleShowInterface = true;
            break;
        case '2':
            currentFormat = debug;
            toggleShowInterface = true;
            break;
        case '3':
            currentFormat = cloud;
            toggleShowInterface = false;
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
#ifdef TARGET_OSX // only working on Mac at the moment
        case '>':
        case '.':
            hardware.setTiltAngle(hardware.tilt_angle++);
			break;
        case '<':
        case ',':
            hardware.setTiltAngle(hardware.tilt_angle--);
			break;
#endif
        default:
            break;
    }
}

//--------------------------------------------------------------
// OTHER OF FUNCTIONS
//--------------------------------------------------------------

void oniActorApp::keyReleased(int key)
{
    
}

void oniActorApp::mouseMoved(int x, int y)
{
    
}

void oniActorApp::mouseDragged(int x, int y, int button)
{
    
}

void oniActorApp::mousePressed(int x, int y, int button)
{
    
}

void oniActorApp::mouseReleased(int x, int y, int button)
{
    
}

void oniActorApp::windowResized(int w, int h)
{
    
}

void oniActorApp::dragEvent(ofDragInfo dragInfo)
{
    
}

void oniActorApp::gotMessage(ofMessage msg)
{
    
}

void oniActorApp::exit()
{
#ifdef DEBUG
    std::cerr << "STARTING EXIT ROUTINE ...." << std::endl;
#endif
    
    openniClose(); // oniActorAppOpenNI.cpp
    closeGUI(); // oniActorAppGUI.cpp
    
#ifdef DEBUG
    std::cerr << "END OF EXIT ROUTINE ...." << std::endl;
#endif
    
    OF_EXIT_APP(0);
}
