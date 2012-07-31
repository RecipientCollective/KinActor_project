//
//  actor.cpp
//  KinectTor
//
//  Created by drambald on 7/31/12.
//  Copyright (c) 2012 reicipient.cc All rights reserved.
//

#include "actor.h"

// COSTRUTTORE
actor::actor()
{
    // an actor have a CODE, a FLAG and a BLOB
	code = randomString(10, true, false, false);
    updated = true;
    blob = actorBlob();
}

// COSTRUTTORE CON UN BLOB
actor::actor(ofxCvBlob mb)
{
    // take the blob, cast in an actorBlob and store
    blob = mb;
    code = blob.code; // the actor code is the blob code
    updated = true;
}

/////////////////////////////////////////////////////////////////
// copiatore da ofxCvBlob (a = ofCvBlob)
// Questo metodo viene chiamata ad ogni update con contourFinder
// quindi ogni frame ho un actorBlob nuovo.
/////////////////////////////////////////////////////////////////
actor::actor(const actor &other)
{    
    // it's a copy ...
	code = other.code;
	updated = other.updated;
    blob = other.blob;
}

// CLEAN
void actor::clean()
{
    
}