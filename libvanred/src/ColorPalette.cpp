#include "ColorPalette.h"

ColorPalette::ColorPalette() {
	for (int i=0; i < 16; i++)
		colorValues[i] = 0;
	colorValues[15] = '\0';
}

void ColorPalette::print(int colorPaletteBase) {
     switch(colorPaletteBase) 
     {
        case ColorPalette::RAW :
        	std::cout << "  Color Palette: " << std::endl;
             	for (int i = 0; i < 16; i++){
             		std::cout << "    " << std::setw(8) << std::setfill('0') << std::hex;
		        	std::cout << std::right << getColor (i) << std::endl;
		        	std::cout << std::setw(0) << std::dec;
             	}
             	std::cout << std::endl;
		break;
       default:
             std::cout << "unknown Base" << std::endl;
     }    
}

void ColorPalette::print() {
	print(ColorPalette::RAW);
}
void ColorPalette::printXml() {
	for (int i = 0; i < 16; i++) {
		std::cout << "         <color index=\"" << i << "\" value=\"" << std::setw(8) << std::setfill('0') << std::hex;
		std::cout << std::right << getColor (i) << "\"/>" << std::endl;
		std::cout << std::setw(0) << std::dec;
	}
}

void ColorPalette::setColor(int pos, int value) {
	colorValues[pos] = value;
}
int ColorPalette::getColor(int pos) {
	return colorValues[pos];
}
