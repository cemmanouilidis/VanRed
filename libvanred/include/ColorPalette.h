#ifndef __COLOR_PALETTE_H_
#define	__COLOR_PALETTE_H_

#include <iostream>
#include <iomanip>

class ColorPalette {
	private:
		int colorValues[16];
	public:
		static int const YUV = 128;
		static int const RGB = 129;
		static int const RAW = 130;

		ColorPalette();

		void setColor(int pos, int value);
		int getColor(int pos);

		void print(int colorPaletteBase);
		void print();
		void printXml();

};
#endif
