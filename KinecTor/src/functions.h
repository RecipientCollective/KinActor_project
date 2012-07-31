//
//  functions.h
//  KinectTor
//
//  Created by drambald on 7/31/12.
//  Copyright (c) 2012 recipient.cc. All rights reserved.
//

#ifndef KinectTor_functions_h
#define KinectTor_functions_h

#include "ofMain.h"
#include "ofxCvBlob.h"
#include <cstdlib> // rand()
#include <string>

//--------------------------------------------------------------
// Random Functions
vector<float> randomScatter(float x, float y, int range);
vector<int> randomRgb();
std::string randomString(int length, bool letters, bool numbers, bool symbols);
ofColor randOfColor();

//--------------------------------------------------------------
// sort helper functions
bool sortByCentroid(const ofxCvBlob& d1, const ofxCvBlob& d2);


#endif
