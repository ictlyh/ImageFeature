#ifndef __IMAGE_H__
#define __IMAGE_H__ 1

#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <queue>
#define Num_ColorBin 32 //* quantized to 32 colors */
#define Distance_Range 10 //* distance range to calculate in correlogram *//
using namespace std;

class Image
{
public:
	Image();
	Image(char* file);
	char* getImagePath();				// 获取图片文件路径
	void setImagePath(char* file);		// 设置图片文件路径
	void showImage();					// 显示图片
	void showGrayHist();				// 显示图片灰度直方图
	void showRGBGrayHist();				// 显示颜色直方图
	void showHSHist();					// 显示HS直方图
	void showColorMoment();				// 显示颜色矩
	// 颜色量化,<<基于色彩量化及索引的图像检索>>,汪华章，何小海，宰文姣，王炜
	void colorQuantization(IplImage** planes, IplImage* quantized);
	void showCCV();						// 显示颜色聚合向量
	void showColorCorrelogram();		// 显示颜色相关图
	~Image();

private:
	char* image;						// 图片文件路径
};

#endif