#include "oniActorApp.h"

//--------------------------------------------------------------
// UTILS FUNCTIONS
//--------------------------------------------------------------

string oniActorApp::generateFileName()
{
    string _root = "kinectRecord";
    
	string _timestamp = ofToString(ofGetDay()) +
	ofToString(ofGetMonth()) +
	ofToString(ofGetYear()) +
	ofToString(ofGetHours()) +
	ofToString(ofGetMinutes()) +
	ofToString(ofGetSeconds());
    
	string _filename = (_root + _timestamp + ".oni");    
	return _filename;
}

void oniActorApp::setupOsc()
{
    sender.setup(HOST, PORT);
}

