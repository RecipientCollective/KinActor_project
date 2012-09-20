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

void oniActorApp::processOpenFileSelection(ofFileDialogResult openFileResult)
{
	ofLogVerbose("getName(): "  + openFileResult.getName());
	ofLogVerbose("getPath(): "  + openFileResult.getPath());
	ofFile file (openFileResult.getPath());
    if (file.exists())
    {
        setupPlayback(openFileResult.getPath());
        isLive = false;
    } else {
        ofLogVerbose("Problem with openFileResult: " + openFileResult.getName());
    }
}


void oniActorApp::setupOsc()
{
    sender.setup(HOST, PORT);
}

void oniActorApp::oscSendSkeletons()
{
    ofxUserGenerator currentRecorder = isLive ? recordUser : playUser;
    for(int i = 1; i <= MAX_ACTORS; i++)
    {
        if(!currentRecorder.getXnUserGenerator().GetSkeletonCap().IsTracking(i)) 
        {
#ifdef DEBUG        
            cerr << "OSC_SEND_SKELETONS: Not tracking this user: " << i << endl;
            cerr << "Number of Tracked users: " << currentRecorder.getNumberOfTrackedUsers() << endl;
#endif
        } else {
            ofxTrackedUser * user = currentRecorder.getTrackedUser(i);
            oscSendSkeleton(user, i, currentRecorder);
        }        
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

void oniActorApp::oscSendHands()
{
    ofxHandGenerator &currentRecorder = isLive ? recordHandTracker : playHandTracker;
    vector<ofxTrackedHand *>::const_iterator cii;
    vector<ofxTrackedHand *> SS = recordHandTracker.tracked_hands;
    for(cii = SS.begin(); cii!=SS.end(); cii++)
    {
        ofxTrackedHand *hand = *cii;
        
        if (hand->isBeingTracked)
        {
            ofxOscMessage m;
            string uid = ofToString(hand->nID);
            
            // hack max hands ?
            // if (MAX_HANDS == 1) uid = "1";
            
            string baseAddr = "/hand/" + uid;
            string addr = baseAddr + "/raw";
            m.setAddress(addr);
            m.addFloatArg(hand->rawPos.X);
            m.addFloatArg(hand->rawPos.Y);
            m.addFloatArg(hand->rawPos.Z);
            sender.sendMessage(m);
            m.clear();
            
            addr = baseAddr + "/filtered";
            m.setAddress(addr);
            m.addFloatArg(hand->progPos.x);
            m.addFloatArg(hand->progPos.y);
            m.addFloatArg(hand->progPos.z);
            sender.sendMessage(m);
            m.clear();
            
            addr = baseAddr + "/projected";
            m.setAddress(addr);
            m.addFloatArg(hand->projectPos.x);
            m.addFloatArg(hand->projectPos.y);
            m.addFloatArg(hand->projectPos.z);
            sender.sendMessage(m);
            m.clear();
            
            
#ifdef DEBUG
            cerr << "SENDING HAND WITH ID " << hand->nID << endl;
            cerr << "POSITION PROJECTED: " << hand->projectPos << endl;
            cerr << "POSITION REAL FILTERED: " << hand->progPos << endl;
            cerr << "POSITION RAW X,Y,Z:" << hand->rawPos.X << ", " << hand->rawPos.Y << ", " << hand->rawPos.Z << endl;
#endif
        }
        
    }
}
