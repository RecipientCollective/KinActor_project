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
        if(!currentRecorder.getXnUserGenerator().GetSkeletonCap().IsTracking(i)) 
        {
#ifdef DEBUG        
            cerr << "OSC_SEND_SKELETONS: Not tracking this user: " << i << endl;
#endif
            return;
        }
        
        ofxTrackedUser * user = currentRecorder.getTrackedUser(i);
        oscSendSkeleton(user, i, currentRecorder);
    }
}

void oniActorApp::oscSendLimb(ofxLimb &limb, string addr, ofxOscMessage &m)
{
    m.setAddress(addr);
	m.addFloatArg(limb.position[0].X);
    m.addFloatArg(limb.position[0].Y);
    m.addFloatArg(limb.position[0].Z);
    m.addFloatArg(limb.position[1].X);
    m.addFloatArg(limb.position[1].Y);
    m.addFloatArg(limb.position[1].Z);
    sender.sendMessage(m);
    m.clear();
}

void oniActorApp::oscSendCenter(XnPoint3D center, string addr, ofxOscMessage &m)
{
    m.setAddress(addr);
    m.addFloatArg(center.X);
    m.addFloatArg(center.Y);
    m.addFloatArg(center.Z);
    sender.sendMessage(m);
    m.clear();
}

void oniActorApp::oscSendSkeleton(ofxTrackedUser *us, int idUser, ofxUserGenerator &recorder)
{
    ofxOscMessage m;
    ofxLimb rLimb;
    
    string baseAddr = "/skeleton/" + ofToString(idUser);

    // center
    string addr = baseAddr + "/center";
    oscSendCenter(us->center, addr, m);
    
    // torso
    addr = baseAddr + "/left_upper_torso/projection";
    oscSendLimb(us->left_upper_torso, addr, m);
    rLimb = getWorldLimb(us->left_upper_torso, idUser, recorder);
    addr = baseAddr + "/left_upper_torso/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    addr = baseAddr + "/right_upper_torso/projection";
    oscSendLimb(us->right_upper_torso, addr, m);
    rLimb = getWorldLimb(us->right_upper_torso, idUser, recorder);
    addr = baseAddr + "/right_upper_torso/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    // left lower torso + leg
    addr = baseAddr + "/left_lower_torso/projection";
    oscSendLimb(us->left_lower_torso, addr, m);
    rLimb = getWorldLimb(us->left_lower_torso, idUser, recorder);
    addr = baseAddr + "/left_lower_torso/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    addr = baseAddr + "/left_upper_leg/projection";
    oscSendLimb(us->left_upper_leg, addr, m);
    rLimb = getWorldLimb(us->left_upper_leg, idUser, recorder);
    addr = baseAddr + "/left_upper_leg/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    addr = baseAddr + "/left_lower_leg/projection";
    oscSendLimb(us->left_lower_leg, addr, m);
    rLimb = getWorldLimb(us->left_lower_leg, idUser, recorder);
    addr = baseAddr + "/left_lower_leg/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    // right lower torso + leg
    addr = baseAddr + "/right_lower_torso/projection";
    oscSendLimb(us->right_lower_torso, addr, m);
    rLimb = getWorldLimb(us->right_lower_torso, idUser, recorder);
    addr = baseAddr + "/right_lower_torso/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    addr = baseAddr + "/right_upper_leg/projection";
    oscSendLimb(us->right_upper_leg, addr, m);
    rLimb = getWorldLimb(us->right_upper_leg, idUser, recorder);
    addr = baseAddr + "/right_upper_leg/realWorld";
    oscSendLimb(rLimb, addr, m);

    addr = baseAddr + "/right_lower_leg/projection";
    oscSendLimb(us->right_lower_leg, addr, m);
    rLimb = getWorldLimb(us->right_lower_leg, idUser, recorder);
    addr = baseAddr + "/right_lower_leg/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    // hip
    addr = baseAddr + "/hip/projection";
    oscSendLimb(us->hip, addr, m);
    rLimb = getWorldLimb(us->hip, idUser, recorder);
    addr = baseAddr + "/hip/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    // right arm + shoulder
    addr = baseAddr + "/right_shoulder/projection";
    oscSendLimb(us->right_shoulder, addr, m);
    rLimb = getWorldLimb(us->right_shoulder, idUser, recorder);
    addr = baseAddr + "/right_shoulder/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    addr = baseAddr + "/right_upper_arm/projection";
    oscSendLimb(us->right_upper_arm, addr, m);
    rLimb = getWorldLimb(us->right_upper_arm, idUser, recorder);
    addr = baseAddr + "/right_upper_arm/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    addr = baseAddr + "/right_lower_arm/projection";
    oscSendLimb(us->right_lower_arm, addr, m);
    rLimb = getWorldLimb(us->right_lower_arm, idUser, recorder);
    addr = baseAddr + "/right_lower_arm/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    // left arm + shoulder
    addr = baseAddr + "/left_shoulder/projection";
    oscSendLimb(us->left_shoulder, addr, m);
    rLimb = getWorldLimb(us->left_shoulder, idUser, recorder);
    addr = baseAddr + "/left_shoulder/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    addr = baseAddr + "/left_upper_arm/projection";
    oscSendLimb(us->left_upper_arm, addr, m);
    rLimb = getWorldLimb(us->left_upper_arm, idUser, recorder);
    addr = baseAddr + "/left_upper_arm/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    addr = baseAddr + "/left_lower_arm/projection";
    oscSendLimb(us->left_lower_arm, addr, m);
    rLimb = getWorldLimb(us->left_lower_arm, idUser, recorder);
    addr = baseAddr + "/left_lower_arm/realWorld";
    oscSendLimb(rLimb, addr, m);
    
    // neck
    addr = baseAddr + "/neck/projection";
    oscSendLimb(us->neck, addr, m);
    rLimb = getWorldLimb(us->neck, idUser, recorder);
    addr = baseAddr + "/neck/realWorld";
    oscSendLimb(rLimb, addr, m);
    
}