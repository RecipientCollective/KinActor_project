#include "oniActorApp.h"

//--------------------------------------------------------------
// DRAW FUNCTIONS
//--------------------------------------------------------------
void oniActorApp::oniactorDraw()
{
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    ofScale(scaleFactor, scaleFactor, 1.0);
	ofTranslate(mtrx, mtry, 1.0);
       
    if (toggleDrawBox)
        drawBox(0,0,inputWidth,inputHeight);
    
    
    ofPopMatrix();
}

void oniActorApp::debugDraw()
{
    ofSetColor(255, 255, 255);
    ofPushMatrix();
    ofScale(0.75, 0.75);
    
    // RECORD DEPTH DRAW
    drawBox(0,0,inputWidth,inputHeight);
    isLive ? recordDepth.draw(0,0,inputWidth,inputHeight) : playDepth.draw(0,0,inputWidth,inputHeight);
    
    // RECORD IMAGE DRAW
    drawBox(inputWidth,0,inputWidth,inputHeight);
    isLive ? recordImage.draw(inputWidth, 0, inputWidth,inputHeight) : playImage.draw(inputWidth, 0, inputWidth,inputHeight);
    
    // DETPTH RANGE MASK DRAW
    drawBox(inputWidth*2,0,inputWidth,inputHeight);
    depthRangeMask.draw(inputWidth*2, 0, inputWidth, inputHeight);
    
    ofPushMatrix();
    ofTranslate(0, inputHeight);
    drawBox(0,0,inputWidth,inputHeight);
    if (isTracking) {        
        isLive ? recordImage.draw(0, 0, inputWidth,inputHeight) : playImage.draw(0, 0, inputWidth,inputHeight);
        isLive ? recordUser.draw() : playUser.draw();
    }
    ofPopMatrix();
    
    if (isMasking) {
        ofPushMatrix();
        ofTranslate(inputWidth, inputHeight);
        drawMasks();
        ofPopMatrix();
    }
    
    if (isTrackingHands) {
        isLive ? recordHandTracker.drawHands() : playHandTracker.drawHands();
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
	allUserMasks.draw(0, 0, inputWidth, inputHeight);
	glDisable(GL_BLEND);
    glPopMatrix();
	user1Mask.draw(inputWidth, 0, inputWidth / 2.0f, inputHeight / 2.0f);
	user2Mask.draw(inputWidth, inputHeight / 2.0f, inputWidth / 2.0f, inputHeight / 2.0f);
}

void oniActorApp::cloudDraw()
{
    glPushMatrix();
    
    ofxUserGenerator currentUser = isLive ? recordUser : playUser;
    int w = currentUser.getWidth();
    int h = currentUser.getHeight();
    glTranslatef(w, h, -400);
    ofRotateY(pointCloudRotationY);
    
    glBegin(GL_POINTS);
    
    int step = 1;
    
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
            ofPoint pos = currentUser.getWorldCoordinateAt(x, y, 1);
            ofColor color = currentUser.getWorldColorAt(x,y, 1);
            if (pos.z == 0) continue;
            glColor4ub((unsigned char)color.r,(unsigned char)color.g,(unsigned char)color.b, (unsigned char)color.a);
            glVertex3f(pos.x, pos.y, pos.z);
        }
    }
    
    glEnd();
    
	glColor3f(1.0f, 1.0f, 1.0f);
    
	glPopMatrix();

}

void oniActorApp::drawBox(int x, int y, int w, int h)
{
    ofPushStyle();
    ofSetColor(255, 255, 255);
    ofNoFill();
    ofRect(x, y, w, h);
    ofPopStyle();
}