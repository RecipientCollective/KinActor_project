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

void oniActorApp::oscSendSkeletons()
{
    ofxUserGenerator currentRecorder = isLive ? recordUser : playUser;
    for(int i = 1; i <= currentRecorder.getNumberOfTrackedUsers(); i++)
    {
        ofxTrackedUser * user = currentRecorder.getTrackedUser(i);
        sendSkeleton(user, i);
    }
}

void oniActorApp::sendSkeleton(ofxTrackedUser *us, int idUser)
{
    ofxOscMessage m;
    string baseAddr = "/skeleton/" + ofToString(idUser);
    // torso
    string addr = baseAddr + "/left_upper_torso";
    m.setAddress(addr);
	m.addFloatArg(us->left_upper_torso.position[0].X);
    m.addFloatArg(us->left_upper_torso.position[0].Y);
    m.addFloatArg(us->left_upper_torso.position[1].X);
    m.addFloatArg(us->left_upper_torso.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    addr = baseAddr + "/right_upper_torso";
    m.setAddress(addr);
	m.addFloatArg(us->right_upper_torso.position[0].X);
    m.addFloatArg(us->right_upper_torso.position[0].Y);
    m.addFloatArg(us->right_upper_torso.position[1].X);
    m.addFloatArg(us->right_upper_torso.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    // left lower torso + leg
    addr = baseAddr + "/left_lower_torso";
    m.setAddress(addr);
	m.addFloatArg(us->left_lower_torso.position[0].X);
    m.addFloatArg(us->left_lower_torso.position[0].Y);
    m.addFloatArg(us->left_lower_torso.position[1].X);
    m.addFloatArg(us->left_lower_torso.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    addr = baseAddr + "/left_upper_leg";
    m.setAddress(addr);
	m.addFloatArg(us->left_upper_leg.position[0].X);
    m.addFloatArg(us->left_upper_leg.position[0].Y);
    m.addFloatArg(us->left_upper_leg.position[1].X);
    m.addFloatArg(us->left_upper_leg.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    addr = baseAddr + "/left_lower_leg";
    m.setAddress(addr);
	m.addFloatArg(us->left_lower_leg.position[0].X);
    m.addFloatArg(us->left_lower_leg.position[0].Y);
    m.addFloatArg(us->left_lower_leg.position[1].X);
    m.addFloatArg(us->left_lower_leg.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    // right lower torso + leg
    addr = baseAddr + "/right_lower_torso";
    m.setAddress(addr);
	m.addFloatArg(us->right_lower_torso.position[0].X);
    m.addFloatArg(us->right_lower_torso.position[0].Y);
    m.addFloatArg(us->right_lower_torso.position[1].X);
    m.addFloatArg(us->right_lower_torso.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    addr = baseAddr + "/right_upper_leg";
    m.setAddress(addr);
	m.addFloatArg(us->right_upper_leg.position[0].X);
    m.addFloatArg(us->right_upper_leg.position[0].Y);
    m.addFloatArg(us->right_upper_leg.position[1].X);
    m.addFloatArg(us->right_upper_leg.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    addr = baseAddr + "/right_lower_leg";
    m.setAddress(addr);
	m.addFloatArg(us->right_lower_leg.position[0].X);
    m.addFloatArg(us->right_lower_leg.position[0].Y);
    m.addFloatArg(us->right_lower_leg.position[1].X);
    m.addFloatArg(us->right_lower_leg.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    // hip
    addr = baseAddr + "/hip";
    m.setAddress(addr);
	m.addFloatArg(us->hip.position[0].X);
    m.addFloatArg(us->hip.position[0].Y);
    m.addFloatArg(us->hip.position[1].X);
    m.addFloatArg(us->hip.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    // right arm + shoulder
    addr = baseAddr + "/right_shoulder";
    m.setAddress(addr);
	m.addFloatArg(us->right_shoulder.position[0].X);
    m.addFloatArg(us->right_shoulder.position[0].Y);
    m.addFloatArg(us->right_shoulder.position[1].X);
    m.addFloatArg(us->right_shoulder.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    addr = baseAddr + "/right_upper_arm";
    m.setAddress(addr);
	m.addFloatArg(us->right_upper_arm.position[0].X);
    m.addFloatArg(us->right_upper_arm.position[0].Y);
    m.addFloatArg(us->right_upper_arm.position[1].X);
    m.addFloatArg(us->right_upper_arm.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    addr = baseAddr + "/right_lower_arm";
    m.setAddress(addr);
	m.addFloatArg(us->right_lower_arm.position[0].X);
    m.addFloatArg(us->right_lower_arm.position[0].Y);
    m.addFloatArg(us->right_lower_arm.position[1].X);
    m.addFloatArg(us->right_lower_arm.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    // left arm + shoulder
    addr = baseAddr + "/left_shoulder";
    m.setAddress(addr);
	m.addFloatArg(us->left_shoulder.position[0].X);
    m.addFloatArg(us->left_shoulder.position[0].Y);
    m.addFloatArg(us->left_shoulder.position[1].X);
    m.addFloatArg(us->left_shoulder.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    addr = baseAddr + "/left_upper_arm";
    m.setAddress(addr);
	m.addFloatArg(us->left_upper_arm.position[0].X);
    m.addFloatArg(us->left_upper_arm.position[0].Y);
    m.addFloatArg(us->left_upper_arm.position[1].X);
    m.addFloatArg(us->left_upper_arm.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    addr = baseAddr + "/left_lower_arm";
    m.setAddress(addr);
	m.addFloatArg(us->left_lower_arm.position[0].X);
    m.addFloatArg(us->left_lower_arm.position[0].Y);
    m.addFloatArg(us->left_lower_arm.position[1].X);
    m.addFloatArg(us->left_lower_arm.position[1].Y);
    sender.sendMessage(m);
    m.clear();
    
    // neck
    addr = baseAddr + "/neck";
    m.setAddress(addr);
	m.addFloatArg(us->neck.position[0].X);
    m.addFloatArg(us->neck.position[0].Y);
    m.addFloatArg(us->neck.position[1].X);
    m.addFloatArg(us->neck.position[1].Y);
    sender.sendMessage(m);
    m.clear(); 
}