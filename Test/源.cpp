#include<Windows.h>
#include<string>
//#include<sdl.h>
//#include<SDL_image.h>
#include<opencv2/core.hpp>
#include<opencv/cv.h>
#include<opencv/highgui.h>

//#pragma comment(lib,"sdl2.lib")
//#pragma comment(lib,"sdl2_image.lib")
using namespace std;
using namespace cv;
//bool InitWindow()
//{
//	WNDCLASSEX wnd;
//	wnd.cbClsExtra = 0;
//	wnd.cbSize = sizeof(WNDCLASSEX);
//	wnd.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
//	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wnd.hIcon = NULL;
//	wnd.hIconSm = 0;
//}
int main()
{
	/*WORD lpramp[3][256];
	GetDeviceGammaRamp(GetDC(NULL),&lpramp);
	int a = 0;
	MessageBox(0,0,0,0);*/

	/*ifstream fin("F:\\game\\NieRAutomata\\data\\data018.cpk", std::ios::binary);

	vector<char> buf(fin.seekg(0, std::ios::end).tellg());
	fin.seekg(0, std::ios::beg).read(&buf[0], static_cast<std::streamsize>(buf.size()));

	fin.close();*/
	//53 54 55 56
	//i i+1 i+2 i+3
	//IplImage* Image = cvLoadImage("C:\\a.jpg",CV_LOAD_IMAGE_GRAYSCALE);

	//SetSysColors();


	/*Mat Image = imread("C:\\a.jpg", 1);

	Mat dimage;
	resize(Image, dimage, Size(), 1.0,1.0);
	Mat afterimage;
	GaussianBlur(dimage,afterimage,Size(1023,601),24,24);
	Mat Img_out(dimage.size(), CV_32FC3);
	dimage.convertTo(Img_out, CV_32FC3);

	int width = dimage.cols;
	int height = dimage.rows;

	float R;
	float angle;

	Point Center(width / 2, height / 2);
	float t1, t2, t3;
	int new_x, new_y;

	int Num = 40;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			t1 = 0; t2 = 0; t3 = 0;
			R = sqrt((y - Center.y) * (y - Center.y) + (x - Center.x) * (x - Center.x));
			angle = atan2((float)(y - Center.y), (float)(x - Center.x));
			for (int mm = 0; mm < Num; mm++)
			{
				float tmR = R - mm > 0 ? R - mm : 0.0;

				new_x = tmR * cos(angle) + Center.x;
				new_y = tmR * sin(angle) + Center.y;

				if (new_x < 0)       new_x = 0;
				if (new_x > width - 1) new_x = width - 1;
				if (new_y < 0)       new_y = 0;
				if (new_y > height - 1)new_y = height - 1;

				t1 = t1 + dimage.at<Vec3b>(new_y, new_x)[0];
				t2 = t2 + dimage.at<Vec3b>(new_y, new_x)[1];
				t3 = t3 + dimage.at<Vec3b>(new_y, new_x)[2];

			}

			Img_out.at<Vec3f>(y, x)[0] = t1 / Num;
			Img_out.at<Vec3f>(y, x)[1] = t2 / Num;
			Img_out.at<Vec3f>(y, x)[2] = t3 / Num;

		}
	}
	Img_out = Img_out / 255.0;
	blur(dimage,dimage,Size(51,51),Point(24,24));

	namedWindow("window0");
	namedWindow("window1");
	namedWindow("window2");
	imshow("window0",dimage);
	imshow("window1",afterimage);
	imshow("window2",Img_out);
	cvWaitKey(0);
	cvDestroyAllWindows();*/

	char b[8]="\0";
	cin >> b;
	FILE* fo; fopen_s(&fo, "C:\\test.txt", "w");
	
	fwrite(b,1,sizeof(b),fo);
	fclose(fo);
	FILE* fi; fopen_s(&fi, "C:\\test.txt", "rb");
	memset(b,0,8);
	fread(b,1,8,fi);
	cout << "read:"<<b<<endl;
	return 0;
}