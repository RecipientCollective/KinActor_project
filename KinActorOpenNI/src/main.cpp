#include "kinactorApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
//  OPENFRAMEWORKS - KINACTOR
//  
//  DEBUG FLAGS: DEBUG=1 (in build settings Prerocessor MACROS)
//
//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, OUTPUT_WIDTH, OUTPUT_HEIGHT, OF_WINDOW);
	ofRunApp(new kinactorApp()); // start the app
}
