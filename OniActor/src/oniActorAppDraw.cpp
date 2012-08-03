#include "oniActorApp.h"

//--------------------------------------------------------------
// DRAW FUNCTIONS
//--------------------------------------------------------------

void oniActorApp::debugDraw()
{
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    ofTranslate(mtrx,mtry);
    ofScale(0.75, 0.75);
    
    if (isLive) {
        
        recordDepth.draw(0,0,640,480);
		recordImage.draw(640, 0, 640, 480);
        
        // can use this with openCV to make masks, 
        // find contours etc when not dealing 
        // with openNI 'User' like objects
        depthRangeMask.draw(0, 480, 320, 240);

        if (isTracking) {
			recordUser.draw();
			if (isMasking) drawMasks();
            // 0 gives you all point clouds; 
            // use userID to see point clouds for specific users
			if (isCloud) drawPointCloud(&recordUser, 1);	            
		}
        
		if (isTrackingHands)
			recordHandTracker.drawHands();
        
        
    } else {
        
        playDepth.draw(0,0,640,480);
		playImage.draw(640, 0, 640, 480);        
		depthRangeMask.draw(0, 480, 320, 240);
        
		if (isTracking) {
			playUser.draw();            
			if (isMasking) drawMasks();
			if (isCloud) drawPointCloud(&playUser, 0);            
		}
        
		if (isTrackingHands)
			playHandTracker.drawHands();
        
    }
    
    ofPopMatrix();
}

void oniActorApp::logDraw()
{
    
}

void oniActorApp::drawMasks()
{
    glPushMatrix();
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
	allUserMasks.draw(640, 0, 640, 480);
	glDisable(GL_BLEND);
    glPopMatrix();
	user1Mask.draw(320, 480, 320, 240);
	user2Mask.draw(640, 480, 320, 240);
}

void oniActorApp::drawPointCloud(ofxUserGenerator *user_generator, int userID)
{
    glPushMatrix();
    
	int w = user_generator->getWidth();
	int h = user_generator->getHeight();
    
	glTranslatef(w, h/2, -500);
	ofRotateY(pointCloudRotationY);
    
	glBegin(GL_POINTS);
    
	int step = 1;
    
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofPoint pos = user_generator->getWorldCoordinateAt(x, y, userID);
			if (pos.z == 0 && isCPBkgnd) continue;	// gets rid of background -> still a bit weird if userID > 0...
			ofColor color = user_generator->getWorldColorAt(x,y, userID);
			glColor4ub((unsigned char)color.r, (unsigned char)color.g, (unsigned char)color.b, (unsigned char)color.a);
			glVertex3f(pos.x, pos.y, pos.z);
		}
	}
    
	glEnd();
    
	glColor3f(1.0f, 1.0f, 1.0f);
    
	glPopMatrix();
}

void oniActorApp::oniactorDraw()
{
    
}

void oniActorApp::cloudDraw()
{
    
}