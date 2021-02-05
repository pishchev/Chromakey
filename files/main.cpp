
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include "Chromakey.hpp"

int main(int argc, const char* argv[])
{
	chromakey ch;

	if (argc == 3)
	{
		if (!ch.init(argv[1], argv[2]))
		{
			return -1;
		}
	}
	else
	{
		std::cout << "Usage: prog_name main_image background_image" << std::endl;
		system("pause");
		return 1;
	}

	ch.main_loop();

	return 0;
}
