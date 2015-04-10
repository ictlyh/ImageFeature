#include "Image.h"

Image::Image()
{
	image = NULL;
}

Image::Image(char * file)
{
	image = file;
}

Image::~Image()
{
	image = NULL;
}

char * Image::getImagePath()
{
	return image;
}

void Image::setImagePath(char * file)
{
	image = file;
}

void Image::showImage()
{
	IplImage * img = cvLoadImage(image);
	if(img == NULL)
	{
		fprintf(stderr, "Error:load image failed.\n Image path:%s", image);
		return ;
	}
	cvNamedWindow("ShowImage", CV_WINDOW_AUTOSIZE );
	cvShowImage("ShowImage", img );
	cvWaitKey(0);
	cvReleaseImage( &img );
	cvDestroyWindow("ShowImage");
}

void Image::showGrayHist()
{
	//Load image in gray mode
	IplImage * src = cvLoadImage(image, CV_LOAD_IMAGE_GRAYSCALE);
	if(src == NULL)
	{
		fprintf(stderr, "Error:load image failed.\n Image path:%s", image);
		return ;
	}
	int width = src->width;
	int height = src->height;
	int step = src->widthStep;
	uchar * data = (uchar * )src->imageData;
	int hist[256] = {0};
	for(int i = 0; i < height; i ++ )
	{
		int tmp = i * step;
		for(int j = 0; j < width; j ++ )
		{
			hist[data[tmp + j]] ++;
		}
	}
	int max = 0;
	for(int i = 0; i < 256; i ++ )
	{
		if(hist[i] > max)
		{
			max = hist[i];
		}
	}
	IplImage * dst = cvCreateImage(cvSize(400,300), 8, 3);
	//Set background color to white
	cvSet(dst, cvScalarAll(255), 0);
	double bin_width = (double)dst->width / 256;
	double bin_unith = (double)dst->height / max;
	for(int i = 0; i < 256; i ++ )
	{
		//Left top of the window is (0,0)
		CvPoint p0 = cvPoint(i * bin_width, dst->height);
		CvPoint p1 = cvPoint((i + 1) * bin_width, dst->height - hist[i] * bin_unith);
		//Set hiso color to black
		cvRectangle(dst, p0, p1, cvScalarAll(0), -1, 8, 0);
	}
	cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image", src);
	cvNamedWindow("Gray Histogram", CV_WINDOW_AUTOSIZE);
	cvShowImage("Gray Histogram", dst);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
}

void Image::showRGBGrayHist()
{
	//Load image in color mode
	IplImage * src = cvLoadImage(image, CV_LOAD_IMAGE_COLOR);
	if(src == NULL)
	{
		fprintf(stderr, "Error:load image failed.\n Image path:%s", image);
		return ;
	}
	IplImage * r = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage * g = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	IplImage * b = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
	//Split origin image to B G R images
	cvSplit(src, b, g, r, NULL);
	//Turn color image to gray image
	IplImage * gray = cvCreateImage(cvGetSize(src), 8, 1);
	cvCvtColor(src, gray, CV_BGR2GRAY);

	int size = 256;
	float range[] = {0,255};
	float * ranges[] = {range};
	CvHistogram * r_hist = cvCreateHist(1, &size, CV_HIST_ARRAY, ranges, 1);
	CvHistogram * g_hist = cvCreateHist(1, &size, CV_HIST_ARRAY, ranges, 1);
	CvHistogram * b_hist = cvCreateHist(1, &size, CV_HIST_ARRAY, ranges, 1);
	CvHistogram * gray_hist  = cvCreateHist(1, &size, CV_HIST_ARRAY, ranges, 1);

	//Draw histogram of color red
	cvCalcHist(&r, r_hist, 0, NULL);
	IplImage * r_dst = cvCreateImage(cvSize(400, 300), 8, 3);
	//Set background color to white
	cvSet(r_dst, cvScalarAll(255), 0);
	float r_max = 0;
	cvGetMinMaxHistValue(r_hist, NULL, &r_max, NULL, NULL);
	double r_bin_width = (double)r_dst->width / size;
	double r_bin_unith = (double)r_dst->height / r_max;
	for(int i = 0; i < size; i ++ )
	{
		CvPoint p0 = cvPoint(i * r_bin_width, r_dst->height);
		CvPoint p1 = cvPoint((i + 1) * r_bin_width, r_dst->height - cvGetReal1D(r_hist->bins, i) * r_bin_unith);
		//Show rectangle in color red
		cvRectangle(r_dst, p0, p1, cvScalar(255, 0, 0), -1, 8, 0);
	}

	//Draw histogram of color green
	cvCalcHist(&g, g_hist, 0, NULL);
	IplImage * g_dst = cvCreateImage(cvSize(400, 300), 8, 3);
	cvSet(g_dst, cvScalarAll(255), 0);
	float g_max = 0;
	cvGetMinMaxHistValue(g_hist, NULL, &g_max, NULL, NULL);
	double g_bin_width = (double)g_dst->width / size;
	double g_bin_unith = (double)g_dst->height / g_max;
	for(int i = 0; i < size; i ++ )
	{
		CvPoint p0 = cvPoint(i * g_bin_width, g_dst->height);
		CvPoint p1 = cvPoint((i + 1) * g_bin_width, g_dst->height - cvGetReal1D(g_hist->bins, i) * g_bin_unith);
		//Show rectangle in color green
		cvRectangle(g_dst, p0, p1, cvScalar(0, 255, 0), -1, 8, 0);
	}

	//Draw histogram of color blue
	cvCalcHist(&b, b_hist, 0, NULL);
	IplImage * b_dst = cvCreateImage(cvSize(400, 300), 8, 3);
	cvSet(b_dst, cvScalarAll(255), 0);
	float b_max = 0;
	cvGetMinMaxHistValue(b_hist, NULL, &b_max, NULL, NULL);
	double b_bin_width = (double)b_dst->width / size;
	double b_bin_unith = (double)b_dst->height / b_max;
	for(int i = 0; i < size; i ++ )
	{
		CvPoint p0 = cvPoint(i * b_bin_width, b_dst->height);
		CvPoint p1 = cvPoint((i + 1) * b_bin_width, b_dst->height - cvGetReal1D(b_hist->bins, i) * b_bin_unith);
		//Show rectangle in color blue
		cvRectangle(b_dst, p0, p1, cvScalar(0, 0, 255), -1, 8, 0);
	}

	//Draw gray histogram
	cvCalcHist(&gray, gray_hist, 0, NULL);
	IplImage * gray_dst = cvCreateImage(cvSize(400, 300), 8, 3);
	cvSet(gray_dst, cvScalarAll(255), 0);
	float max = 0;
	cvGetMinMaxHistValue(gray_hist, NULL, &max, NULL, NULL);
	double bin_width = (double)gray_dst->width / size;
	double bin_unith = (double)gray_dst->height / max;
	for(int i = 0; i < size; i ++ )
	{
		CvPoint p0 = cvPoint(i * bin_width, gray_dst->height);
		CvPoint p1 = cvPoint((i + 1) * bin_width, gray_dst->height - cvGetReal1D(gray_hist->bins, i) * bin_unith);
		//Show rectangle in color black
		cvRectangle(gray_dst, p0, p1, cvScalar(0), -1, 8, 0);
	}
	IplImage * dst = cvCreateImage(cvSize(800, 600), 8, 3);
	cvSetZero(dst);
	CvRect rect = cvRect(0, 0, 400, 300); 
	cvSetImageROI(dst, rect); 
	cvCopy(r_dst, dst); 
	rect = cvRect(400, 0, 400, 300);
	cvSetImageROI(dst, rect); 
	cvCopy(g_dst, dst);
	rect = cvRect(0, 300, 400, 300);
	cvSetImageROI(dst, rect);
	cvCopy(b_dst, dst);
	rect = cvRect(400, 300, 400, 300);
	cvSetImageROI(dst, rect); 
	cvCopy(gray_dst, dst);
	cvResetImageROI(dst);
	cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image", src);
	cvNamedWindow("RGBGrayHist", CV_WINDOW_AUTOSIZE);
	cvShowImage("RGBGrayHist", dst);
	//cvSaveImage("RGBGrayHist.jpg",dst);
	cvWaitKey(0); 
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	cvReleaseImage(&r);
	cvReleaseImage(&g);
	cvReleaseImage(&b);
	cvReleaseImage(&gray);
	cvReleaseImage(&r_dst);
	cvReleaseImage(&g_dst);
	cvReleaseImage(&b_dst);
	cvReleaseImage(&gray_dst);
	cvDestroyAllWindows();
}

void Image::showHSHist()
{
	IplImage * src;
	// 载入图像
	src = cvLoadImage(image, CV_LOAD_IMAGE_UNCHANGED);
	if(!src)
	{
		printf("Could not load image file: %s\n", image);
    exit(0);
	}

  IplImage * hsv = cvCreateImage( cvGetSize(src), 8, 3 );
  IplImage * h_plane = cvCreateImage( cvGetSize(src), 8, 1 ); 
  IplImage * s_plane = cvCreateImage( cvGetSize(src), 8, 1 ); 
  IplImage * v_plane = cvCreateImage( cvGetSize(src), 8, 1 ); 
  IplImage * planes[] = { h_plane, s_plane,v_plane }; 
  
  //* * H 分量划分为16个等级，S分量划分为8个等级 */  
  int h_bins = 16 , s_bins = 8, v_bins = 8; 
  int hist_size[] = {h_bins, s_bins, v_bins}; 
  
  //* * H 分量的变化范围 */  
  float h_ranges[] = { 0, 180 };  
  
  //* * S 分量的变化范围 */  
  float s_ranges[] = { 0, 255 }; 
  float v_ranges[] = { 0, 255 }; 
 
  float * ranges[] = { h_ranges, s_ranges,v_ranges}; 
  
  //* * 输入图像转换到HSV颜色空间 */  
  cvCvtColor( src, hsv, CV_BGR2HSV ); 
  cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 ); 
  
  //* * 创建直方图，二维, 每个维度上均分 */  
  CvHistogram * hist = cvCreateHist( 3, hist_size, CV_HIST_ARRAY, ranges, 1 ); 
  //* * 根据H,S两个平面数据统计直方图 */  
  cvCalcHist( planes, hist, 0, 0 ); 
  
  //* * 获取直方图统计的最大值，用于动态显示直方图 */  
  float max_value; 
  cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 ); 
  
  
  //* * 设置直方图显示图像 */  
  int height = 100; 
  int width = (h_bins * s_bins * v_bins * 5); 
  IplImage * hist_img = cvCreateImage( cvSize(width,height), 8, 3 ); 
  cvZero( hist_img ); 
  
  //* * 用来进行HSV到RGB颜色转换的临时单位图像 */  
  IplImage * hsv_color = cvCreateImage(cvSize(1,1), 8, 3); 
  IplImage * rgb_color = cvCreateImage(cvSize(1,1), 8, 3); 
  int bin_w = width / (h_bins * s_bins); 
  for(int h = 0; h < h_bins; h ++ ) 
  { 
    for(int s = 0; s < s_bins; s ++ ) 
    { 
      for(int v = 0; v < v_bins; v ++ ) 
      { 
				int i = h * s_bins + s * v_bins + v; 
				/* * 获得直方图中的统计次数，计算显示在图像中的高度 */ 
				float bin_val = cvQueryHistValue_3D( hist, h, s,v ); 
				int intensity = cvRound(bin_val * height / max_value); 
  
				/* * 获得当前直方图代表的颜色，转换成RGB用于绘制 */ 
				cvSet2D(hsv_color,0,0,cvScalar(h * 180.f / h_bins,s * 255.f/s_bins,v * 255.f/v_bins,0)); 
				cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR); 
				CvScalar color = cvGet2D(rgb_color,0,0); 
  
				cvRectangle( hist_img, cvPoint(i * bin_w,height), 
					cvPoint((i + 1) * bin_w,height - intensity), 
					color, -1, 8, 0 ); 
      } 
    } 
  } 
 
  cvNamedWindow( "Source", 1 ); 
  cvShowImage( "Source", src ); 
  cvNamedWindow( "H-S-V Histogram",1); 
  cvShowImage( "H-S-V Histogram", hist_img );
  cvWaitKey(0);
  cvReleaseImage(&src);
  cvReleaseImage(&hist_img);
  cvDestroyWindow("Source");
  cvDestroyWindow("H-S-V Histogram");
}

void Image::showColorMoment()
{
	float h_mean = 0;
	float s_mean = 0;
	float v_mean = 0;
	float h_variance = 0;
	float s_variance = 0;
	float v_variance = 0;
	float h_skewness = 0;
	float s_skewness = 0;
	float v_skewness = 0;

	//Load image in color mode
	IplImage * src = cvLoadImage(image, CV_LOAD_IMAGE_COLOR);
	if(src == NULL)
	{
		fprintf(stderr, "Error:load image failed.\n Image path:%s", image);
		return ;
	}
	// Compute the HSV image, and decompose it into separate planes.
  IplImage * hsv = cvCreateImage( cvGetSize(src), 8, 3 ); 
  IplImage * h_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
  IplImage * s_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
  IplImage * v_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
  IplImage * planes[] = { h_plane, s_plane, v_plane };
	//输入图像转换到HSV颜色空间
	cvCvtColor( src, hsv, CV_BGR2HSV );
  cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

	int width = h_plane->width; 
	int height = h_plane->height;

	for(int i = 0; i < height; i ++ )
	{
		for(int j = 0; j < width; j ++ )
		{
			h_mean +=  * ((uchar * )(h_plane->imageData + i * h_plane->widthStep + j));
		}
	}
	h_mean = h_mean / (width * height);
	cout << "Mean of H:" << (int)h_mean << endl;

	for(int i = 0; i < height; i ++ )
	{
		for(int j = 0; j < width; j ++ )
		{
			s_mean +=  * ((uchar * )(s_plane->imageData + i * s_plane->widthStep + j));
		}
	}
	s_mean = s_mean / (width * height);
	cout << "Mean of S:" << (int)s_mean << endl;

	for(int i = 0; i < height; i ++ )
	{
		for(int j = 0; j < width; j ++ )
		{
			v_mean +=  * ((uchar * )(v_plane->imageData + i * v_plane->widthStep + j));
		}
	}
	v_mean = v_mean / (width * height);
	cout << "Mean of V:" << (int)v_mean << endl << endl;

	for(int i = 0; i < height; i ++ )
	{
		for(int j = 0; j < width; j ++ )
		{
			int temp =  * ((uchar * )(h_plane->imageData + i * h_plane->widthStep + j));
			h_variance += (temp - h_mean) * (temp - h_mean);
		}
	}
	h_variance = sqrt((float)(h_variance / (width * height)));
	cout << "Variance of H:" << (int)h_variance << endl;

	for(int i = 0; i < height; i ++ )
	{
		for(int j = 0; j < width; j ++ )
		{
			int temp =  * ((uchar * )(h_plane->imageData + i * h_plane->widthStep + j));
			s_variance += (temp-s_mean) * (temp - s_mean);
		}
	}
	s_variance = sqrt((float)(s_variance / (width * height)));
	cout << "Variance of S:" << (int)s_variance << endl;

	for(int i = 0; i < height; i ++ )
	{
		for(int j = 0; j < width; j ++ )
		{
			int temp =  * ((uchar * )(h_plane->imageData + i * h_plane->widthStep + j));
			v_variance += (temp - v_mean) * (temp - v_mean);
		}
	}
	v_variance = sqrt((float)(v_variance / (width * height)));
	cout << "Variance of V:" << (int)v_variance << endl << endl;

	for(int i = 0; i < height; i ++ )
	{
		for(int j = 0; j < width; j ++ )
		{
			int temp =  * ((uchar * )(h_plane->imageData + i * h_plane->widthStep + j));
			h_skewness += pow((float)(temp - h_mean), 3);
		}
	}
	h_skewness = (h_skewness >= 0) ? pow(h_skewness / (width * height), (float)(1.0 / 3.0)) : -pow(-h_skewness / (width * height), (float)(1.0 / 3.0));
	cout << "Skewness of H:" << (int)h_skewness << endl;

	for(int i = 0;i < height; i ++ )
	{
		for(int j = 0; j < width; j ++ )
		{
			int temp =  * ((uchar * )(h_plane->imageData + i * h_plane->widthStep + j));
			s_skewness += pow((float)(temp - s_mean), 3);
		}
	}
	s_skewness = (s_skewness >= 0) ? pow(s_skewness / (width * height), (float)(1.0 / 3.0)) : -pow(-s_skewness / (width * height), (float)(1.0 / 3.0));
	cout << "Skewness of S:" << (int)s_skewness << endl;

	for(int i = 0; i < height; i ++ )
	{
		for(int j = 0; j < width; j ++ )
		{
			int temp =  * ((uchar * )(h_plane->imageData + i * h_plane->widthStep + j));
			v_skewness += pow((float)(temp - v_mean), 3);
		}
	}
	v_skewness = (v_skewness >= 0) ? pow(v_skewness / (width * height), (float)(1.0 / 3.0)) : -pow(-v_skewness / (width * height), (float)(1.0 / 3.0));
	cout << "Skewness of V:" << (int)v_skewness << endl;

	system("pause");
}

void Image::colorQuantization(IplImage** planes, IplImage* quantized)
{
	IplImage* h_plane = planes[0];
	IplImage* s_plane = planes[1];
	IplImage* v_plane = planes[2];

	int width = h_plane->width; 
	int height = h_plane->height;

	char* p_quantized = quantized->imageData;
	int wstep = quantized->widthStep;

	int bin[32];
	for(int m = 0;m < 32;m ++ )
	{
		bin[m] = 0;
	}

	for(int i = 0;i < height;i ++ )
	{
		for(int j = 0;j < width;j ++ )
		{
			//* h:0-179; s:0-255; v:0-255 */
			int h = * ((uchar * )(h_plane->imageData + i * h_plane->widthStep + j));
			int s = * ((uchar * )(s_plane->imageData + i * s_plane->widthStep + j));
			int v = * ((uchar * )(v_plane->imageData + i * v_plane->widthStep + j));

			if(v <= 0.1 * 255)
			{
				p_quantized[i * wstep + j] = 0;
				bin[0] ++;
			}
			else if((s <= 0.1 * 255)&&(v>0.1 * 255)&&(v <= 0.4 * 255))
			{
				p_quantized[i * wstep + j] = 1;
				bin[1] ++;
			}
			else if((s <= 0.1 * 255)&&(v>0.4 * 255)&&(v <= 0.7 * 255))
			{
				p_quantized[i * wstep + j] = 2;
				bin[2] ++;
			}
			else if((s <= 0.1 * 255)&&(v>0.7 * 255)&&(v <= 1 * 255))
			{
				p_quantized[i * wstep + j] = 3;
				bin[3] ++;
			}
			else if((h>= 0.0/360.0 * 180 && h <= 20.0/360.0 * 180) || (h>330.0/360.0 * 180 && h < 360.0/360.0 * 180))
			{
				if(s <= 0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 4;
					bin[4] ++;
				}
				else if(s>0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 5;
					bin[5] ++;
				}
				else if(s <= 0.5 * 255&&v>0.5 * 255)
				{
					p_quantized[i * wstep + j] = 6;
					bin[6] ++;
				}
				else
				{
					p_quantized[i * wstep + j] = 7;
					bin[7] ++;
				}
			}
			else if(h>20.0/360.0 * 180&&h <= 45.0/360.0 * 180)
			{
				if(s <= 0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 8;
					bin[8] ++;
				}
				else if(s>0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 9;
					bin[9] ++;
				}
				else if(s <= 0.5 * 255&&v>0.5 * 255)
				{
					p_quantized[i * wstep + j] = 10;
					bin[10] ++;
				}
				else
				{
					p_quantized[i * wstep + j] = 11;
					bin[11] ++;
				}
			}
			else if(h>45.0/360.0 * 180&&h <= 75.0/360.0 * 180)
			{
				if(s <= 0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 12;
					bin[12] ++;
				}
				else if(s>0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 13;
					bin[13] ++;
				}
				else if(s <= 0.5 * 255&&v>0.5 * 255)
				{
					p_quantized[i * wstep + j] = 14;
					bin[14] ++;
				}
				else
				{
					p_quantized[i * wstep + j] = 15;
					bin[15] ++;
				}
			}
			else if(h>75.0/360.0 * 180&&h <= 155.0/360.0 * 180)
			{
				if(s <= 0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 16;
					bin[16] ++;
				}
				else if(s>0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 17;
					bin[17] ++;
				}
				else if(s <= 0.5 * 255&&v>0.5 * 255)
				{
					p_quantized[i * wstep + j] = 18;
					bin[18] ++;
				}
				else
				{
					p_quantized[i * wstep + j] = 19;
					bin[19] ++;
				}
			}
			else if(h>155.0/360.0 * 180&&h <= 210.0/360.0 * 180)
			{
				if(s <= 0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 20;
					bin[20] ++;
				}
				else if(s>0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 21;
					bin[21] ++;
				}
				else if(s <= 0.5 * 255&&v>0.5 * 255)
				{
					p_quantized[i * wstep + j] = 22;
					bin[22] ++;
				}
				else
				{
					p_quantized[i * wstep + j] = 23;
					bin[23] ++;
				}
			}
			else if(h>210.0/360.0 * 180&&h <= 270.0/360.0 * 180)
			{
				if(s <= 0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 24;
					bin[24] ++;
				}
				else if(s>0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 25;
					bin[25] ++;
				}
				else if(s <= 0.5 * 255&&v>0.5 * 255)
				{
					p_quantized[i * wstep + j] = 26;
					bin[26] ++;
				}
				else
				{
					p_quantized[i * wstep + j] = 27;
					bin[27] ++;
				}
			}
			else
			{
				if(s <= 0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 28;
					bin[28] ++;
				}
				else if(s>0.5 * 255&&v <= 0.5 * 255)
				{
					p_quantized[i * wstep + j] = 29;
					bin[29] ++;
				}
				else if(s <= 0.5 * 255&&v>0.5 * 255)
				{
					p_quantized[i * wstep + j] = 30;
					bin[30] ++;
				}
				else
				{
					p_quantized[i * wstep + j] = 31;
					bin[31] ++;
				}
			}
		}
	}

	/*for(int m = 0;m < 32;m ++ )
	{
		cout << m << " :" << bin[m] << endl;
	}*/

	cvNamedWindow("ColorQuantization", CV_WINDOW_AUTOSIZE);
	cvShowImage("ColorQuantization", quantized);
	cvWaitKey();
	//cvReleaseImage(&quantized);
	cvDestroyWindow("ColorQuantization");
}

void Image::showCCV()
{
	//Load image in color mode
	IplImage * src = cvLoadImage(image, CV_LOAD_IMAGE_COLOR);
	if(src == NULL)
	{
		fprintf(stderr, "Error:load image failed.\n Image path:%s", image);
		return ;
	}
	// Compute the HSV image, and decompose it into separate planes.
	IplImage * hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	IplImage * h_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage * s_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage * v_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage * planes[] = { h_plane, s_plane, v_plane };
	//输入图像转换到HSV颜色空间
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
	IplImage* quantized = cvCreateImage( cvGetSize(h_plane), 8, 1 );
	cvZero(quantized);
	colorQuantization(planes,quantized);

	int* coh_Lables;
	int coherence[Num_ColorBin];
	int n_coherence[Num_ColorBin];
	int threshold = hsv->height * hsv->width / 100;
	for(int i=0;i<Num_ColorBin;i++)
	{
		coherence[i]=0;
		n_coherence[i]=0;
	}
	//counter=1;
	coh_Lables=new int[h_plane->width*h_plane->height];
	for(int i=0;i<h_plane->width*h_plane->height;i++)
	{
		coh_Lables[i]=-2;
	}

	int width = quantized->width; 
	int height = quantized->height;
	int stepwidth = quantized->widthStep;

	for(int pi = 0;pi < height;pi++)
	{
		for(int pj=0;pj<width;pj++)
		{
			//* go to next pixel if this pixel has been labeled */
			if(coh_Lables[pi*width+pj]!=-2)
				continue;
	
			int color=*(quantized->imageData+pi*stepwidth+pj);

			//* BFS */
			queue<int> pos_i;
			queue<int> pos_j;
			queue<int> pos_i_forlable;
			queue<int> pos_j_forlable;

			pos_i.push(pi);
			pos_j.push(pj);
			pos_i_forlable.push(pi);
			pos_j_forlable.push(pj);
			
			coh_Lables[pi*width+pj]=-1;

			while(!pos_i.empty())
			{
				int i=pos_i.front();
				int j=pos_j.front();
				pos_i.pop();
				pos_j.pop();
				
				//* same color and not visited */
				if(i!=0&&j!=0&&(*(quantized->imageData+(i-1)*stepwidth+j-1))==color&&coh_Lables[(i-1)*width+j-1]==-2)
				{
					coh_Lables[(i-1)*width+j-1]=-1;
					pos_i.push(i-1);
					pos_j.push(j-1);
					pos_i_forlable.push(i-1);
					pos_j_forlable.push(j-1);
				}
				if(i!=0&&(*(quantized->imageData+(i-1)*stepwidth+j))==color&&coh_Lables[(i-1)*width+j]==-2)
				{
					coh_Lables[(i-1)*width+j]=-1;
					pos_i.push(i-1);
					pos_j.push(j);
					pos_i_forlable.push(i-1);
					pos_j_forlable.push(j);
				}
				if(i!=0&&j!=width-1&&(*(quantized->imageData+(i-1)*stepwidth+j+1))==color&&coh_Lables[(i-1)*width+j+1]==-2)
				{
					coh_Lables[(i-1)*width+j+1]=-1;
					pos_i.push(i-1);
					pos_j.push(j+1);
					pos_i_forlable.push(i-1);
					pos_j_forlable.push(j+1);
				}
				if(j!=0&&(*(quantized->imageData+i*stepwidth+j-1))==color&&coh_Lables[i*width+j-1]==-2)
				{
					coh_Lables[i*width+j-1]=-1;
					pos_i.push(i);
					pos_j.push(j-1);
					pos_i_forlable.push(i);
					pos_j_forlable.push(j-1);
				}
				if(j!=width-1&&(*(quantized->imageData+i*stepwidth+j+1))==color&&coh_Lables[i*width+j+1]==-2)
				{
					coh_Lables[i*width+j+1]=-1;
					pos_i.push(i);
					pos_j.push(j+1);
					pos_i_forlable.push(i);
					pos_j_forlable.push(j+1);
				}
				if(i!=height-1&&j!=0&&(*(quantized->imageData+(pi+1)*stepwidth+j-1))==color&&coh_Lables[(pi+1)*width+j-1]==-2)
				{
					coh_Lables[(i+1)*width+j-1]=-1;
					pos_i.push(i+1);
					pos_j.push(j-1);
					pos_i_forlable.push(i+1);
					pos_j_forlable.push(j-1);
				}
				if(i!=height-1&&(*(quantized->imageData+(i+1)*stepwidth+j))==color&&coh_Lables[(i+1)*quantized->width+j]==-2)
				{
					coh_Lables[(i+1)*quantized->width+j]=-1;
					pos_i.push(i+1);
					pos_j.push(j);
					pos_i_forlable.push(i+1);
					pos_j_forlable.push(j);
				}
				if(i!=height-1&&j!=width-1&&(*(quantized->imageData+(i+1)*stepwidth+j+1))==color&&coh_Lables[(i+1)*width+j+1]==-2)
				{
					coh_Lables[(i+1)*width+j+1]=-1;
					pos_i.push(i+1);
					pos_j.push(j+1);
					pos_i_forlable.push(i+1);
					pos_j_forlable.push(j+1);
				}
			}

			//* if number >= threshold, set to 1 ; if else, set to 0 */
			int num=pos_i_forlable.size();
			//cout<<num<<"-"<<color<<"\t";
			while(!pos_i_forlable.empty())
			{
				int i=pos_i_forlable.front();
				int j=pos_j_forlable.front();
				pos_i_forlable.pop();
				pos_j_forlable.pop();
				
				coh_Lables[i*width+j]=(num>=threshold)?1:0;
			}
		}
	}

	for(int i=0;i<quantized->height;i++)
	{
		for(int j=0;j<quantized->width;j++)
		{
			int colorBin=*(quantized->imageData+i*quantized->widthStep+j);
			if(coh_Lables[i*quantized->width+j]==1)
				coherence[colorBin]++;
			else
				n_coherence[colorBin]++;
		}
	}

	//* There are several small numbers (smaller after smoothing the image) in coherence[], why? */
	for(int i=0;i<Num_ColorBin;i++)
	{
		if(coherence[i]<threshold)
		{
			n_coherence[i]+=coherence[i];
			coherence[i]=0;
		}
	}
	cout << "CCV" << endl;
	cout<<"\t coh \t n_coh"<<endl;
	for(int i=0;i<32;i++)
	{
		cout<<"bin "<<i<<":\t"<<coherence[i]<<"\t"<<n_coherence[i]<<endl;
	}
}

void Image::showColorCorrelogram()
{
	//Load image in color mode
	IplImage * src = cvLoadImage(image, CV_LOAD_IMAGE_COLOR);
	if(src == NULL)
	{
		fprintf(stderr, "Error:load image failed.\n Image path:%s", image);
		return ;
	}
	// Compute the HSV image, and decompose it into separate planes.
	IplImage * hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	IplImage * h_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage * s_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage * v_plane  = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage * planes[] = { h_plane, s_plane, v_plane };
	//输入图像转换到HSV颜色空间
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
	IplImage* quantized = cvCreateImage( cvGetSize(h_plane), 8, 1 );
	cvZero(quantized);
	colorQuantization(planes,quantized);

	double correlogram[Num_ColorBin][Distance_Range];
	int pixelNum[Num_ColorBin];
	for(int i=0;i<Num_ColorBin;i++)
	{
		for(int j=0;j<Distance_Range;j++)
		{
			correlogram[i][j] = (double)0.0;
		}
		pixelNum[i]=0;
	}

	int height = h_plane->height;
	int width = h_plane->width;
	int stepwidth = h_plane->widthStep;

	for(int pi=0;pi<height;pi++)
	{
		for(int pj=0;pj<width;pj++)
		{
			int color=*(quantized->imageData+pi*stepwidth+pj);
			pixelNum[color]++;

			for(int d=1;d<=Distance_Range;d++)
			{
				int sameColorNum=0; //* number of pixels with same color in the d-distance boundary */
				int totalBoundaryNum=0; //* total number of pixels in the d-distance boundary */

				//* up boundary */
				for(int i=pi-d,j=pj-d;j<=pj+d;j++)
				{
					if(i<0)
						break;
					if(j<0||j>=width)
						continue;
					int color2=*(quantized->imageData+i*stepwidth+j);
					if(color==color2)
					{
						sameColorNum++;
					}
					totalBoundaryNum++;
				}
				//* down boundary */
				for(int i=pi+d,j=pj-d;j<=pj+d;j++)
				{
					if(i>=height)
						break;
					if(j<0||j>=width)
						continue;
					int color2=*(quantized->imageData+i*stepwidth+j);
					if(color==color2)
					{
						sameColorNum++;
					}
					totalBoundaryNum++;
				}
				//* left boundary */
				for(int i=pi-d+1,j=pj-d;i<=pi+d-1;i++)
				{
					if(j<0)
						break;
					if(i<0||i>=height)
						continue;
					int color2=*(quantized->imageData+i*stepwidth+j);
					if(color==color2)
					{
						sameColorNum++;
					}
					totalBoundaryNum++;
				}
				//* right boundary */
				for(int i=pi-d+1,j=pj+d;i<=pi+d-1;i++)
				{
					if(j>=width)
						break;
					if(i<0||i>=height)
						continue;
					int color2=*(quantized->imageData+i*stepwidth+j);
					if(color==color2)
					{
						sameColorNum++;
					}
					totalBoundaryNum++;
				}
				if(totalBoundaryNum!=0)//* Not necessary practically, only used when there is only one pixel in the image */ 
				{
					correlogram[color][d-1]+=(double)sameColorNum/(double)totalBoundaryNum;
				}
			}
		}
	}
	for(int c=0;c<Num_ColorBin;c++)
	{
		for(int d=0;d<Distance_Range;d++)
		{
			//* if the color doesn't show up in the image, the default relative probability would be set 0 (need more study)*/
			if(pixelNum[c]!=0)
				correlogram[c][d]=correlogram[c][d]/(double)pixelNum[c];
		}
	}
	cout<<"distance";
	for(int i=0;i<Distance_Range;i++)
	{
		cout<<"\t"<<i+1;
	}
	cout<<endl<<endl<<"color"<<endl<<endl;
	for(int i=0;i<Num_ColorBin;i++)
	{
		cout<<i<<":\t";
		for(int j=0;j<Distance_Range;j++)
		{
			if(correlogram[i][j]<0)//* To avoid problem caused by : 0 is presented by a very small number in type of double */
				cout<<"\t"<<0;
			else
				printf("\t%.5lf",correlogram[i][j]);
		}
		cout<<endl<<endl;	
	}
}