//
//  actorBlob.h
//  KinectTor
//
//  Created by drambald on 7/31/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef KinectTor_actorBlob_h
#define KinectTor_actorBlob_h

#include "ofxOpenCv.h"
#include "functions.h"

class actorBlob : public ofxCvBlob 
{
        
public:
    // ATTRIBUTES
    string  code;
    int	    mycolor;
    
    // costruttori
	actorBlob();
	actorBlob(const ofxCvBlob &other);
    
    // metodi
    void update(const ofxCvBlob blob);
    void drawPixels();
};


#endif
