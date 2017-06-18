#include "Drop.hpp"

Drop::Drop(){}


int Drop::returnNoiseX(){
    
    float noiseX = ofNoise(ofGetElapsedTimef() / 4.0f, ofGetElapsedTimef()/1.5f) * 300;
    float randomNoise = round(ofRandom(noiseX - 20, noiseX + 20));
    if (randomNoise <= 0 || randomNoise >= horizontalPixels) {
        randomNoise = ofRandom(horizontalPixels);
    }
    
    return randomNoise;
    
    
}


void Drop::init(float min, float max, ofImage tex, int horiz, int vertic, int lmin, int lmax){
    
    texture = tex;
    horizontalPixels = horiz;
    verticalPixels = vertic;
    
    // position
    xPos = returnNoiseX();
    
    yPos = ofRandom(0, -400);
    length = ofRandom(lmin, lmax);
    
    // physics
    ySpeed = 0.0;
    gravity = 0.01;
    minSpeed = min;
    maxSpeed = ofRandom(min, max);
    //obstruction = false;
    
}

void Drop::update(int size, float force){
    
    // detect obstructions
    for (int i = 0; i < 7; i++) {
        int breakPoint = (verticalPixels / 7) * i;
        int begin = breakPoint - size;
        
        if ((yPos >= begin) && (yPos <= breakPoint))  {
            obstruction = true;
        }
    }
    
    
    // decellerate if obstruction
    if ((obstruction == true) && (ySpeed > minSpeed)) {
        
        if (ySpeed > minSpeed) {
            ySpeed -= force;
        }
        
        // detect frozen
    }
    
    if (ySpeed < minSpeed) {
        ySpeed = minSpeed;
    }
    
    
    // accelerate if free
    else if (obstruction == false) {
        if (ySpeed <= maxSpeed) {
            ySpeed += gravity;
        }
    }
    
    
    // propulsion
    yPos = yPos += ySpeed;
    
    
    //    // reset when under screen
    //    if (yPos > verticalPixels) { //change to fbo settings
    //        xPos = ofRandom(horizontalPixels);
    //        yPos = ofRandom(-100,-200);
    //    }
    
    // start condition
    obstruction = false;
    
}

void Drop::reset(float min, float max, ofImage tex, int lmin, int lmax){
    
    texture = tex;
    
    // position
    xPos = returnNoiseX();
    yPos = ofRandom(-100, -200);
    length = ofRandom(lmin, lmax);
    
    // physics
    minSpeed = min;
    maxSpeed = ofRandom(min, max);
    
    
    
}

bool Drop::isComplete(){
    return yPos > verticalPixels;
}


void Drop::display(){
    texture.draw(xPos, yPos, 1, length);
}

