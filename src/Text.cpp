//
//  Text.cpp
//  EvryStockholmStairs
//
//  Created by Anders Nærø Tangen on 12/06/2017.
//
//

#include "Text.hpp"

Text::Text(){}

void Text::calculate(ofTrueTypeFont font, string myString) {
    
    _font = font;
    _myString = myString;
    
    width = _font.stringWidth(_myString);
    height = _font.stringHeight(_myString);
    
}

void Text::draw(int x, int y) {
    
    _font.drawString(_myString, x - width/2, y + height/2);
    
}
