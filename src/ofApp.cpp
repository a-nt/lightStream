#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // variables
    
    numTilesX = 31;
    numTilesY = 14;
    
    pixelCount = 8; //pixels per tile
    pixelPitch = 25; //millimeters center to center
    
    gapSize = 3; // pixel gap compensation
    gapHeight = gapSize * 6;
    
    
    // font
    fontBold.load("fonts/standard09_65.ttf", 6);
    fontRegular.load("fonts/standard07_53.ttf", 6);
    
    
    
    // interface panel
    drawDebug = false; //start condition
    
    //ofxGui
    gui.setup();
    gui.setHeaderBackgroundColor(0);
    gui.add(numParticles.set("PARTICLE NUM", 500, 0, 5000));
    gui.add(lengthMin.set("Min length", 4, 1, 20));
    gui.add(lengthMax.set("Max length", 12, 1, 20));
    gui.add(particleSpeedMin.set("Min speed", 1.0, 0.0, 5.0));
    gui.add(particleSpeedMax.set("Max speed", 2.0, 0.0, 5.0));
    gui.add(obstructionSize.set("Obstruction size", 2, 0, 20));
    gui.add(obstructionForce.set("Obstruction force", 0.2, 0.0, 1.0));
    gui.add(pointRadius.set("LED size", 5, 1.0, 5.0));
    gui.add(colorMode.set("Color Mode", 0, 0, 3));
    gui.add(screenSize.setup("screen size", ofToString(ofGetWidth())+"x"+ofToString(ofGetHeight())));
    gui.add(frameRate.setup("FPS", ofToString(ofGetFrameRate())));
    
    
    
    // general settings
    ofSetFrameRate(60);
    
    
    // texture allocation
    white.load("textures/white.png");
    blue.load("textures/blue.png");
    purple.load("textures/purple.png");
    orange.load("textures/orange.png");
    
    
    
    // allocate virtual FBOs
    
    screen.allocate(numTilesX * pixelCount, numTilesY * pixelCount + gapHeight); // 6 gaps
    screen.begin();
    ofClear(0,0,0,255);
    screen.end();
    
    // allocate part FBOs (stairs)

    lessTiles = 6; // reset
    for (int i = 0; i < 7; i++) {
        ofFbo fbo;
        fbo.allocate(screen.getWidth(), (screen.getHeight() - gapHeight) / 7);
        fbos.push_back(fbo);
        fbos[i].begin();
        ofClear(0, 0, 0, 255);
        fbos[i].end();
    }
    
    // allocate output FBO
    output.allocate(numTilesX * pixelCount, numTilesY * pixelCount); // no gaps
    output.begin();
    ofClear(0,0,0,255);
    output.end();
    
    // allocate preview FBOs
    simulation.allocate(numTilesX * pixelCount, numTilesY * pixelCount + gapHeight); // 6 gaps
    simulation.begin();
    ofClear(0,0,0,255);
    simulation.end();
    
    preview.allocate(screen.getWidth() * pixelPitch, screen.getHeight() * pixelPitch);
    preview.begin();
    ofClear(0, 0, 0, 255);
    preview.end();
    
    // camera settings
    cam.setDistance(1000);

}

//--------------------------------------------------------------
void ofApp::update(){
    
    // monitor drops size
    if (drops.size() < numParticles) {
        for (int i = 0; i < numParticles - drops.size(); i++) {
            Drop d;
            d.init(particleSpeedMin, particleSpeedMax, setColorMode(), screen.getWidth(), screen.getHeight(), lengthMin, lengthMax);
            drops.push_back(d);
        }
    }
    if (drops.size() > numParticles) {
        for (int i = 0; i < drops.size() - numParticles; i++) {
                drops.pop_back();
        }
    }
    
    // update drops
    for (int i = 0; i < drops.size(); i++) {
        Drop &d = drops[i];
        d.update(obstructionSize, obstructionForce);
        if (d.isComplete()) {
            
            d.reset(setColorMode());
        }
    }
    
    // update framerate
    frameRate = ofToString(ofGetFrameRate());

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetBackgroundColor(30);
    
    
    // draw to virtual FBO
    
    screen.begin();
    ofBackground(0);
    
    // drops
    for (int i = 0; i < drops.size(); i++) {
        Drop &d = drops[i];
        d.display();
    }
    
    // text
    ofSetColor(255);
    string testString = "Anders Nærø Tangen, Västerös'";
    bold.calculate(fontRegular, testString);
    bold.draw(screen.getWidth()/2,screen.getHeight()/2);
    
    
//    if (z_index < 0) {
//        z_index-= z_index/50;
//    }
    
    
    
    
    
    

    if (drawDebug == true) {
        ofSetColor(255);
        ofDrawCircle(screen.getWidth()/2, screen.getHeight()/2, 5);
    }
    screen.end();
    
    
    
    
    // draw to step FBOs
    
    startX = 0; // reset X
    lessTiles = 6; // reset
    for (int i = 0; i < fbos.size(); i++) {
        
        fbos[i].begin();
        ofBackground(0);
        screen.getTexture().drawSubsection(lessTiles * pixelCount, 0, fbos[i].getWidth(), fbos[i].getHeight(), lessTiles * pixelCount, startX, fbos[i].getWidth(), fbos[i].getHeight());
        fbos[i].end();
        
        startX += fbos[i].getHeight() + gapSize;
        lessTiles--;
        
    }
    
    // draw to output FBO
    
    startX = 0; // reset X
    output.begin();
        ofBackground(0);
            for (int i = 0; i < fbos.size(); i++) {
                fbos[i].draw(0,startX, fbos[i].getWidth(), fbos[i].getHeight());
                startX += fbos[i].getHeight();
            }
    output.end();
    
    
    // draw to simulation FBO
    startX = 0; // reset X
    simulation.begin();
    ofBackground(0);
    for (int i = 0; i < fbos.size(); i++) {
        fbos[i].draw(0,startX, fbos[i].getWidth(), fbos[i].getHeight());
        startX += fbos[i].getHeight() + gapSize;
    }
    simulation.end();
    
    
    
    // draw to preview FBO
    
    preview.begin();
    
        ofBackground(0);
        
        simulation.getTexture().readToPixels(p);
        ofPushStyle();
        
            glPointSize(pointRadius);
            glBegin(GL_POINTS);
                for (int x = 0; x < p.getWidth(); x++)
                {
                    for (int y = 0; y < p.getHeight(); y++)
                    {
                        ofColor c = p.getColor(x, y);
                        ofSetColor(c);
                        glVertex2f(pixelPitch * x + (pixelCount/2), pixelPitch * y + (pixelCount/2));
                    }
                }
            glEnd();
    
        ofPopStyle();
    
    preview.end();
    
    
    
    
    // aspect ratio calculation
    
    int side = ofGetWidth();
    if (ofGetHeight() < side) {
        side = ofGetHeight();
    }
    
    
    // display preview with ofEasyCam
    cam.begin();
        ofPushMatrix();
            ofTranslate(-preview.getWidth()/2, -preview.getHeight()/2);
            preview.draw(0,0, simulation.getWidth() * pixelPitch, simulation.getHeight() * pixelPitch);
        ofPopMatrix();
    cam.end();
    
    
    //STANDARD NO CAM
    //preview.draw(0,0, screen.getWidth() * pixelCount, screen.getHeight() * pixelCount);


    if (drawDebug == true) {
        // display actual pixels preview (corner)
        output.draw(ofGetWidth()-output.getWidth(), 0, output.getWidth(), output.getHeight());
        
        // display interface
        ofPushMatrix();
        //ofTranslate(ofGetWidth()-(screen.getWidth() + gui.getWidth() + 20), 0);
        gui.draw();
        ofPopMatrix();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    if (key == 's'){
        
            for (int i = 0; i < numParticles; i++) {
                
                Drop d;
                d.init(particleSpeedMin, particleSpeedMax, white, screen.getWidth(), screen.getHeight(), lengthMin, lengthMax);
                drops.push_back(d);
                
        }
    }
    
    if (key == 'q'){
        drops.clear();
    }
    
    if (key == 'd'){
        if (drawDebug == false) {
            drawDebug = true;
        } else {
            drawDebug = false;
        }
    }

    
}

ofImage ofApp::setColorMode() {
 
    ofImage img;
    switch (colorMode) {
        case 0: {
            
            img = getModeOneImage();
            
        } break;
        case 1: {
            
            img = getModeTwoImage();
            
        } break;
        case 2: {
            
            img = getModeThreeImage();
            
        } break;
        case 3: {
            
            img = getModeFourImage();
            
        } break;
            
        default: {
            img = getModeOneImage();
        } break;
    }
    
    return img;
    
}


ofImage ofApp::getModeOneImage() {
    
    // calculate probability
    float randomNumber = ofRandom(1);
    
    ofImage color;
    if (randomNumber <= 0.7) {
        color = white;
    }
    if ((randomNumber >= 0.7) && (randomNumber <= 0.9)) {
        color = blue;
    }
    if ((randomNumber >= 0.9) && (randomNumber <= 0.975)) {
        color = purple;
    }
    if (randomNumber >= 0.975) {
        color = orange;
    }
    return color;
}

ofImage ofApp::getModeTwoImage() {
    
    // calculate probability
    float randomNumber = ofRandom(1);
    
    ofImage color;
    if (randomNumber <= 0.6) {
        color = white;
    }
    if ((randomNumber >= 0.6) && (randomNumber <= 0.75)) {
        color = blue;
    }
    if ((randomNumber >= 0.75) && (randomNumber <= 0.85)) {
        color = purple;
    }
    if (randomNumber >= 0.85) {
        color = orange;
    }
    return color;
}

ofImage ofApp::getModeThreeImage() {
    
    // calculate probability
    float randomNumber = ofRandom(1);
    
    ofImage color;
    if (randomNumber <= 0.5) {
        color = white;
    }
    if ((randomNumber >= 0.5) && (randomNumber <= 0.6)) {
        color = blue;
    }
    if ((randomNumber >= 0.6) && (randomNumber <= 0.8)) {
        color = purple;
    }
    if (randomNumber >= 0.8) {
        color = orange;
    }
    return color;
}

ofImage ofApp::getModeFourImage() {
    
    // calculate probability
    float randomNumber = ofRandom(1);
    
    ofImage color;
    if (randomNumber <= 0.4) {
        color = white;
    }
    if ((randomNumber >= 0.4) && (randomNumber <= 0.45)) {
        color = blue;
    }
    if ((randomNumber >= 0.45) && (randomNumber <= 0.7)) {
        color = purple;
    }
    if (randomNumber >= 0.7) {
        color = orange;
    }
    return color;
}

void ofApp::windowResized(int w, int h){
    screenSize = ofToString(w) + "x" + ofToString(h);
}
