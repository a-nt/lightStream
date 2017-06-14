#include "ofMain.h"

class Text {
public:
    
    void calculate(ofTrueTypeFont font, string myString);
    void draw(int x, int y);
    
    Text();
    
    ofTrueTypeFont _font;
    string _myString;
    
    int width;
    int height;
    
};
