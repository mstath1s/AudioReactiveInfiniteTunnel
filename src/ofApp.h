#pragma once

#include "ofMain.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void enableFog(float near, float far) ;
    void disableFog();
    void soundSetup();


    void guiEvent(ofxUIEventArgs &e);
    ofVec2f getOrientation(float t) ;
    
    ofEasyCam cam;
    
    
    ofxUICanvas *gui;
    void audioIn(float *input, int bufferSize, int nChannels);
    
        ofSoundStream soundStream;
    vector<float> left;
    vector<float> right;
    vector<float> volHistory;
    int bufferCounter;
    int drawCounter;
    float threshold;
    
    float smoothedVol;
    float scaledVol;
    
    
    float fogNear;
    float fogFar;
    float tunnelLength;
    float tunnelSeparation;
    float tunnelRadius;
    float rotationChange;
    float xRotation;
    float yRotation;
    float moveSpeed;
    float circleResolution;
};
