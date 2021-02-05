#include "Chromakey.hpp"
#include <iostream>

void chromakey::background_delete()
{
	for (int y = 0; y < hsv.cols; y++)
	{
		for (int x = 0; x < hsv.rows; x++)
		{
			// получаем HSV-компоненты пикселя
			int color = static_cast<int>(splitedHsv[0].at<uchar>(x, y));        // Тон
			int intens = static_cast<int>(splitedHsv[1].at<uchar>(x, y));       // Интенсивность
			int light = static_cast<int>(splitedHsv[2].at<uchar>(x, y));        // Яркость

			//Уничтожаем все, что попадет в диапазон
			if ((light > LIGTH - LIGTH_RANGE && light < LIGTH + LIGTH_RANGE) ||
				(color > COLOR - COLOR_RANGE && color < COLOR + COLOR_RANGE) ||
				(intens > INTENS - INTENS_RANGE && intens < INTENS + INTENS_RANGE))
			{
				temp.at<cv::Vec3b>(x, y)[0] = 255;
				temp.at<cv::Vec3b>(x, y)[1] = 255;
				temp.at<cv::Vec3b>(x, y)[2] = 255;
			}
			else
			{
				temp.at<cv::Vec3b>(x, y) = src.at<cv::Vec3b>(x, y);
			}
		}
	}
}

void chromakey::save_image(std::string file)
{
	cv::imwrite(file, out);
}

void chromakey::orig_image(std::string file)
{
	src = cv::imread(file);
	cv::resize(src, src, cv::Size(0, 0), 0.5, 0.5);
	cv::imshow("Original", src);
}

void chromakey::background_image(std::string file)
{
	bgr = cv::imread(file);
}

void chromakey::make_hsv()
{
	hsv = cv::Mat(src.cols, src.rows, 8, 3);
	splitedHsv = std::vector<cv::Mat>();
	cvtColor(src, hsv, cv::COLOR_BGR2HLS);
	split(hsv, splitedHsv);
	temp = src.clone();
}

void chromakey::show()
{
	background_delete();
	//std::cout << COLOR << " " << COLOR_RANGE << " " << INTENS << " " << INTENS_RANGE << " " << LIGTH << " " << LIGTH_RANGE << std::endl;

	//Морфологическое замыкание для удаления остаточных шумов.
	int an = 3;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(an * 2 + 1, an * 2 + 1), cv::Point(an, an));
	cv::Mat tmp;
	dilate(temp, tmp, element);
	erode(tmp, tmp, element);
	cv::Mat grayscaleMat;
	cvtColor(tmp, grayscaleMat, cv::COLOR_BGR2GRAY);

	//Бинарная маска
	cv::Mat mask(grayscaleMat.size(), grayscaleMat.type());
	threshold(grayscaleMat, mask, 245, 255, cv::THRESH_BINARY_INV);

	out = bgr.clone();
	cv::resize(out, out, cv::Size(src.cols, src.rows));

	//Копируем зашумленное изображение через маску
	temp.copyTo(out, mask);

	cv::imshow("Result", out);
}

void chromakey::s_show(int, void* user)
{
	auto self = reinterpret_cast<chromakey*>(user);
	self->show();
}

bool chromakey::init(const std::string& picture_file, const std::string& background_file)
{
	if (picture_file.empty()) 
	{
		std::cout << "input picture error" << std::endl;
		return false;
	}
	orig_image(picture_file);

	if (background_file.empty())
	{
		std::cout << "input picture error" << std::endl;
		return false;
	}
	background_image(background_file);

	make_hsv();

	namedWindow("Color regulations", cv::WINDOW_AUTOSIZE);
	//ползунки
	cv::createTrackbar("color_bar", "Color regulations", &COLOR, 359, s_show, this);
	cv::createTrackbar("color_range", "Color regulations", &COLOR_RANGE, 100, s_show, this);
	cv::createTrackbar("intens_bar", "Color regulations", &INTENS, 100, s_show, this);
	cv::createTrackbar("intens_range", "Color regulations", &INTENS_RANGE, 30, s_show, this);
	cv::createTrackbar("light_bar", "Color regulations", &LIGTH, 100, s_show, this);
	cv::createTrackbar("light_range", "Color regulations", &LIGTH_RANGE, 30, s_show, this);

	show();

	return true;
}

void chromakey::main_loop()
{
	bool end = false;
	while (!end)
	{
		std::cout << "Save frame (1) , exit(2): ";
		int key = cv::waitKey();

		if (key == '1')
		{
			std::cout << "File: ";
			std::string answer;
			std::getline(std::cin, answer);
			save_image(answer);
		}
		else if (key == '2')
		{
			end = true;
		}
		else
		{
			std::cout << "Incorrect comand. Try again." << std::endl;
		}
	}
}