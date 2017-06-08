#pragma once

#include "ofMain.h"
#include "drop.hpp"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void windowResized(int w, int h);
		void keyPressed(int key);
    
    ofImage setColorMode();
    ofImage getModeOneImage();
    ofImage getModeTwoImage();
    ofImage getModeThreeImage();
    ofImage getModeFourImage();

    
    ofFbo screen;
    ofFbo output;
    ofFbo simulation;
    ofFbo preview;

    vector<ofFbo> fbos;
    
    int pixelCount;
    int pixelPitch;
    int gapSize;
    int gapHeight;
    int startX;
    int lessTiles;
    ofPixels p;
    
    int numTilesX;
    int numTilesY;
    
    
    // drops
    vector<Drop> drops;
    
    // textures
    ofImage white;
    ofImage blue;
    ofImage purple;
    ofImage orange;
    
    // interface
    ofxPanel gui;
    ofParameter<int> numParticles;
    ofParameter<int> lengthMin;
    ofParameter<int> lengthMax;
    ofParameter<float> particleSpeedMin;
    ofParameter<float> particleSpeedMax;
    ofParameter<int> obstructionSize;
    ofParameter<float> obstructionForce;
    ofParameter<float> pointRadius;
    ofParameter<int> colorMode;
    ofxLabel screenSize;
    ofxLabel frameRate;

    
    
    ofEasyCam cam;
    bool drawDebug;
    
    
    

		
};
