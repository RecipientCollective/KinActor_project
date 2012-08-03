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

    setupBackgroud(); // oniActorAppGUI.cpp

#ifdef DEBUG		
    cerr << endl << "<--End setup" << endl;
#endif

}

void oniActorApp::update()
{
    openniUpdate();
}

void oniActorApp::draw()
{
    
}

void oniActorApp::keyPressed(int key)
{
    
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
    
#ifdef DEBUG
    std::cerr << "END OF EXIT ROUTINE ...." << std::endl;
#endif
}
