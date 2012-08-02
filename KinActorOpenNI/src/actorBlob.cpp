//
//  actorBlob.cpp
//  KinectTor
//
//  Created by drambald on 7/31/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "actorBlob.h"

// COSTRUTTORE
actorBlob::actorBlob() : ofxCvBlob()
{
    // prove of principle object persistency
	code = randomString(10, true, false, false);
    mycolor = 0xDD00CC;
}

/////////////////////////////////////////////////////////////////
// copiatore da ofxCvBlob (a = ofCvBlob)
// Questo metodo viene chiamata ad ogni update con contourFinder
// quindi ogni frame ho un actorBlob nuovo.
/////////////////////////////////////////////////////////////////
actorBlob::actorBlob(const ofxCvBlob &other)
{
    area 		 = other.area;
	length 		 = other.length;
	hole 		 = other.hole;
	nPts         = other.nPts;
	pts          = other.pts;
	centroid     = other.centroid;
	boundingRect = other.boundingRect;
    
    // prove of principle object persistency
	code = randomString(10, true, false, false);
    mycolor = 0xDD00CC;
}

// UPDATE
void actorBlob::update(const ofxCvBlob blob)
{
    area 		 = blob.area;
	length 		 = blob.length;
	hole 		 = blob.hole;
	nPts         = blob.nPts;
	pts          = blob.pts;
	centroid     = blob.centroid;
	boundingRect = blob.boundingRect;
}

void actorBlob::drawPixels()
{
	ofPushStyle();
	ofSetHexColor(mycolor);
	ofBeginShape();
	for( int j=0; j<nPts; j++ ) {
		ofVertex( pts[j].x, pts[j].y );
	}
	ofEndShape();
	ofPopStyle();
}