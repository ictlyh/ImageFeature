#include "Image.h"

void main()
{
	Image img("img.jpg");
	img.showImage();
	img.showGrayHist();
	img.showRGBGrayHist();
	img.showHSHist();
	img.showColorMoment();
	img.showCCV();
	img.showColorCorrelogram();
	system("pause");
}