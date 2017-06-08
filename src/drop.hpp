#include "ofMain.h"

class Drop {
public:
    void init(float min, float max, ofImage tex, int horiz, int vertic, int lmin, int lmax);
    void update(int size, float force);
    void reset(ofImage tex);
    bool isComplete();
    void display();
    
    Drop();
    
    float xPos;
    float yPos;
    float ySpeed;
    float gravity;
    float maxSpeed;
    float minSpeed;
    
    int verticalPixels;
    int horizontalPixels;
    
    bool obstruction;
    
    float maxLength;
    float length;
    
    ofImage texture;
    
};
