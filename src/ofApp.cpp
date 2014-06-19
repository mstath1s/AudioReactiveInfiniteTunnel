#include "ofApp.h"
#include "ofMain.h"

void ofApp::enableFog(float near, float far)
{
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	GLfloat fogColor[4]= {0, 0, 0, 1};
	glFogfv(GL_FOG_COLOR, fogColor);
	glHint(GL_FOG_HINT, GL_FASTEST);
	glFogf(GL_FOG_START, near);
	glFogf(GL_FOG_END, far);
}

void ofApp::disableFog()
{
	glDisable(GL_FOG);
}



ofVec2f ofApp::getOrientation(float t)
{

    int choice = ofNoise(t, 0) * 5;
    
    switch(choice) {
        case 0: return ofVec2f(-xRotation, 0);
        case 1: return ofVec2f(+xRotation, 0);
        case 2: return ofVec2f(0, -yRotation);
        case 3: return ofVec2f(0, +yRotation);
    }
    return ofVec2f();
}

void ofApp::soundSetup()
{
    // 0 outputs channels
    // 2 input channels
    //256 samples per buffer
    // 4 num buffers(latency)
    soundStream.listDevices();
    
    int bufferSize = 256;
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(400, 0.0);
    
    bufferCounter = 0;
    drawCounter = 0;
    smoothedVol = 0.0;
    scaledVol = 0.0;
    
    soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    glEnable(GL_DEPTH_TEST);
    gui = new ofxUICanvas();
    
        soundSetup();
    
    
    
    
    gui->addSlider("fogNear", 0, 1000, &fogNear);
    gui->addSlider("fogFar", 0, 10000, &fogFar);
    gui->addSlider("tunnelLength",  0, 1000, &tunnelLength);
    gui->addSlider("tunnelSeparation", 0, 1000, &tunnelSeparation);
    gui->addSlider("tunnelRadius", 0, 1000, &tunnelRadius);
    gui->addSlider("rotationChange", 0, 1, &rotationChange);
    gui->addSlider("xRotation", 0, 45, &xRotation);
    gui->addSlider("yRotation", 0, 45, &yRotation);
    gui->addSlider("moveSpeed", 0, 5000, &moveSpeed);
    gui->addSlider("circleResolution", 1, 30, &circleResolution);
    
    gui->loadSettings("settings.xml");
    
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent, this, &ofApp::guiEvent);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
        scaledVol = ofMap(smoothedVol, 0, 0.17, 0, 255.0, true);
    
}

void ofApp::audioIn(float *input, int bufferSize, int nChannels)
{
    float curVol = 0.0;
    
    int numCounted = 0;
    
    for (int i = 0; i<bufferSize; i++) {
        left[i] = input[i*2] * 0.5;
        left[i] = input[i*2] * 0.5;
        
        curVol += left[i] * left[i];
        curVol += right[i] * right[i];
        
        numCounted +=2;
    }
    
    curVol /= (float)numCounted;
    curVol = sqrt(curVol);
    
    smoothedVol *= 0.92;
    smoothedVol += 0.07* curVol;
    
    bufferCounter++;
}

void ofApp::exit()
{
    gui->saveSettings("settings.xml");
    delete gui;
}

void ofApp::guiEvent(ofxUIEventArgs &e)
{
//    if(e.getName() == "Background")
//    {
//        ofxUISlider *slider = new ofxUISlider();
//        ofBackground(slider->getScaledValue());
//    }
    if(e.getName() == "fogNear")
    {
        ofxUISlider *slider = new ofxUISlider();
        
    }
//    else if(e.getName() == "tunnelLength")
//    {
//        ofxUISlider *slider = new ofxUISlider();
//        
//    }
//    else if(e.getName() == "tunnelSeparation")
//    {
//        
//    }
//    else if(e.getName() == "tunnelRadius")
//    {
//        
//    }
//    else if(e.getName() == "rotationChange")
//    {
//        
//    }
//    else if(e.getName() == "xRotation")
//    {
//        
//    }
//    else if(e.getName() == "yRotation")
//    {
//        
//    }
//    else if(e.getName() == "moveSpeed")
//    {
//        
//    }
//    else if(e.getName() == "circleResolution")
//    {
//        
//    }
    
}



//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(scaledVol);
    ofNoFill();
    
    
    ofSetColor(scaledVol);
    
    cam.begin();
    ofScale(1, 1, -1);
    enableFog(fogNear, fogFar);
    

    
    float moveOffset = moveSpeed * ofGetElapsedTimef() / tunnelSeparation;
    
    int moveWhole = floor(moveOffset);
    
    float moveFraction = fmodf(moveOffset, 1);
    
    ofVboMesh tunnelSegment;
    tunnelSegment.setMode(OF_PRIMITIVE_LINES);
    ofVec3f segmentOffset(0, 0, tunnelSeparation);
    

    
    for(int i = 0; i < circleResolution; i++) {
        
        float theta0 = ofMap(i + 0, 0, circleResolution - 1, 0, 360);
        float theta1 = ofMap(i + 1, 0, circleResolution - 1, 0, 360);
        
        ofVec2f v0(tunnelRadius, 0);
        ofVec2f v1(tunnelRadius, 0);
        
        v0.rotate(theta0);
        v1.rotate(theta1);
        
        tunnelSegment.addVertex(v0);
        tunnelSegment.addVertex(v1);
        
        tunnelSegment.addVertex(v0);
        tunnelSegment.addVertex(segmentOffset + v0);
        
        tunnelSegment.addVertex(segmentOffset + v0);
        tunnelSegment.addVertex(segmentOffset + v1);
    }
    
    ofPushMatrix();
    
    ofVec2f lerpOrientation = ofVec2f().interpolate(getOrientation(rotationChange * (1 + moveWhole)), moveFraction);
    
    ofRotateX(-lerpOrientation.x);
    ofRotateY(-lerpOrientation.y);
    
    ofTranslate(0, 0, -moveFraction * tunnelSeparation);
    
    for(int i = 0; i < tunnelLength; i++)
    {
        if(i > 0)
        {
            ofVec2f orientation = getOrientation(rotationChange * (i + moveWhole));
            ofRotateX(orientation.x);
            ofRotateY(orientation.y);
        }
        
        ofSetColor(255);
        
        tunnelSegment.draw();
        
        ofTranslate(0, 0, tunnelSeparation);
    }
    ofPopMatrix();
    disableFog();
    cam.end();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
