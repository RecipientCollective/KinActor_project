#include "kinectorApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, OUTPUT_WIDTH, OUTPUT_HEIGHT, OF_WINDOW);
	ofRunApp(new kinectorApp()); // start the app
}
