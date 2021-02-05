#pragma once

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

class chromakey
{
private:

	cv::Mat bgr;
	//��������
	cv::Mat src;
	//�������� � HVS
	cv::Mat hsv;
	//��������� ��������� ����
	cv::Mat temp;
	//���������
	cv::Mat out;
	//������ ���������
	std::vector<cv::Mat> splitedHsv;

	static void s_show(int, void* user);

public:

	//��������� �������� ���������� ��� �������� ���� (������������ HVS)
	//���� 
	int COLOR = 100;
	int COLOR_RANGE = 45;
	//�������������
	int INTENS = 0;
	int INTENS_RANGE = 1;
	//�������
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