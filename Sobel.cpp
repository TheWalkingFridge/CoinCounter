#include "Sobel.hpp"

using namespace cv;

// int main( int argc, char** argv )
// {
//     int dx[3][3] = {
//         {-1, 0, 1},
//         {-2, 0, 2},
//         {-1, 0, 1}
//     };

//     int dy[3][3] = {
//         {-1, -2, -1},
//         {0, 0, 0},
//         {1, 2, 1}
//     };

//     // LOADING THE IMAGE
// 	char* imageName = argv[1];

// 	Mat image;
// 	image = imread( imageName, 1 );

// 	if( argc != 2 || !image.data )
// 	{
// 		printf( " No image data \n " );
// 		return -1;
// 	}

// 	// CONVERT COLOUR, BLUR AND SAVE
// 	Mat gray_image;
// 	cvtColor( image, gray_image, CV_BGR2GRAY );

//     Mat dxImage = applyKernel(dx, gray_image);
//     Mat dyImage = applyKernel(dy, gray_image);

//     Mat magnitude = calculateGradientMagnitude(dxImage, dyImage);
//     Mat direction = calculateGradientDirection(dxImage, dyImage);

//     imageWrite(dxImage, "xGradient.jpg");
//     imageWrite(dyImage, "yGradient.jpg");
//     imageWrite(magnitude, "magnitudeGradient.jpg");
//     imageWrite(direction, "directionGradient.jpg");

//     return 0;
// }

Mat imageWrite(Mat &image, std::string imagename){
    Mat output = Mat(image.size(), image.type());
    normalize(image, output, 0, 255, NORM_MINMAX);

    imwrite(imagename, output);

    return output;
}

Mat calculateGradientMagnitude(Mat &dx, Mat &dy) {

    Mat mag = Mat(dx.size(), CV_32FC1);

     for(int x = 1; x < mag.rows - 1; x++) {	
		for(int y = 1; y < mag.cols - 1; y++) {
            float dxx = dx.at<float>(x,y);
            float dyy = dy.at<float>(x,y);

            mag.at<float>(x,y) =  sqrt(dxx*dxx + dyy*dyy);
        }
     }

     return mag;
}

Mat calculateGradientDirection(Mat &dx, Mat &dy) {

    Mat dir = Mat(dx.size(), CV_32FC1);

    for(int x = 1; x < dir.rows - 1; x++) {	
        for(int y = 1; y < dir.cols - 1; y++) {
            float result = atan2(dy.at<float>(x,y),  dx.at<float>(x,y));

            dir.at<float>(x,y) = result;
        }
    }

    return dir;
}

Mat applyKernel(int kernel[3][3], Mat &originalImage) {

    Mat newImage = Mat(originalImage.size(), CV_32FC1);

    for(int x = 1; x < originalImage.rows - 1; x++) {	
		for(int y = 1; y < originalImage.cols - 1; y++) {
			float result = 0.0;

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