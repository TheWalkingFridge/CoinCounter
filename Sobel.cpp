#include <stdio.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui_c.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include <math.h>     
#define _USE_MATH_DEFINES

using namespace cv;

Mat calculateGradientMagnitude(Mat &dx, Mat &dy);
Mat calculateGradientDirection(Mat &dx, Mat &dy);

Mat applyKernel(int kernel[3][3], Mat &originalImage);
uchar normaliseUcharGray(float max, float min, int x);

int main( int argc, char** argv )
{
    int dx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int dy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    // LOADING THE IMAGE
	char* imageName = argv[1];

	Mat image;
	image = imread( imageName, 1 );

	if( argc != 2 || !image.data )
	{
		printf( " No image data \n " );
		return -1;
	}

	// CONVERT COLOUR, BLUR AND SAVE
	Mat gray_image;
	cvtColor( image, gray_image, CV_BGR2GRAY );

    Mat dxImage = applyKernel(dx, gray_image);
    Mat dyImage = applyKernel(dy, gray_image);

    Mat magnitude = calculateGradientMagnitude(dxImage, dyImage);
    Mat direction = calculateGradientDirection(dxImage, dyImage);

    Mat output = Mat(gray_image.size(), gray_image.type());

    normalize(direction, output, 0, 255, cv::NORM_MINMAX);

	// imwrite( "xGradient.jpg",  );
    // imwrite( "yGradient.jpg", dyImage );
    // imwrite( "gradient_magnitude.jpg", gradient );
    // imwrite( "gradient_direction.jpg", direction );

    imwrite( "output.jpg", output );

    return 0;
}

Mat calculateGradientMagnitude(Mat &dx, Mat &dy) {

    Mat mag = Mat(dx.size(), CV_32FC1);

     for(int x = 1; x < mag.rows - 1; x++) {	
		for(int y = 1; y < mag.cols - 1; y++) {
            float result = sqrt(pow(dx.at<float>(x,y), 2) + pow(dy.at<float>(x,y), 2));

            mag.at<float>(x,y) = result;
        }
     }

     return mag;
}

Mat calculateGradientDirection(Mat &dx, Mat &dy) {

    Mat dir = Mat(dx.size(), CV_32FC1);

    for(int x = 1; x < dir.rows - 1; x++) {	
        for(int y = 1; y < dir.cols - 1; y++) {
            int result = atan2(dy.at<float>(x,y),  dx.at<float>(x,y));

            dir.at<float>(x,y) = result;
        }
    }

    return dir;
}

Mat applyKernel(int kernel[3][3], Mat &originalImage) {

    Mat newImage = Mat(originalImage.size(), CV_32FC1);

    for(int x = 1; x < originalImage.rows - 1; x++) {	
		for(int y = 1; y < originalImage.cols - 1; y++) {
			float result = 0;

            for(int xOffset = -1; xOffset <= 1; xOffset++) {
                for(int yOffset = -1; yOffset <= 1; yOffset++) {
                    result += kernel[1 + xOffset][1 + yOffset] * originalImage.at<uchar>(x + xOffset, y + yOffset);
                }
            }

            newImage.at<float>(x, y) = result;
		}
	}

    return newImage;
}

uchar normaliseUcharGray(float max, float min, int x) {
    return (uchar) ((x + min) * 256.0 / (max-min) + min);
}