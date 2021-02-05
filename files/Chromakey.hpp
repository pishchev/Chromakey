#pragma once

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

class chromakey
{
private:

	cv::Mat bgr;
	//оригинал
	cv::Mat src;
	//оригинал в HVS
	cv::Mat hsv;
	//первичная обработка фона
	cv::Mat temp;
	//результат
	cv::Mat out;
	//массив оригинала
	std::vector<cv::Mat> splitedHsv;

	static void s_show(int, void* user);

public:

	//начальные значения параметров для удаления фона (пространство HVS)
	//цвет 
	int COLOR = 100;
	int COLOR_RANGE = 45;
	//интенсивность
	int INTENS = 0;
	int INTENS_RANGE = 1;
	//яркость
	int LIGTH = 0;
	int LIGTH_RANGE = 1;

	bool init(const std::string& picture_file, const std::string& background_file);

	void show();

	void main_loop();

	void make_hsv();

	void orig_image(std::string file);

	void save_image(std::string file);

	void background_delete();
	void background_image(std::string file);
};