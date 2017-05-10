#include "ObstacleFinder.h"

int sb::ObstacleFinder::init()
{
	return 0;
}

int sb::ObstacleFinder::checkObstacle( const cv::Mat& image )
{
	if( IsObstacleOnFrame( image ) )
		return 1;
	return -1;
}

void sb::ObstacleFinder::release() {}

int sb::ObstacleFinder::CountObstaclePixel( const cv::Mat& bgrImage )
{
	cv::Mat imgHSV;
	cv::cvtColor( bgrImage, imgHSV, cv::COLOR_BGR2HSV );

	int count = 0;

	for( int i = 0; i < bgrImage.rows; i++ ) {
		for( int j = 0; j < bgrImage.cols; j++ ) {
			cv::Vec3b hsvPixel = imgHSV.at<cv::Vec3b>( cv::Point( j, i ) );
			int temp = hsvPixel[0];
			if( (temp >= 160 && temp <= 179) || (temp >= 0 && temp <= 10) || (temp >= 32 && temp <= 82) ) {
				temp = hsvPixel[1];
				if( temp > 50 ) {
					temp = hsvPixel[2];

					if( (temp > 50) ) {
						count++;

					}
				}
			}
		}
	}

	return count;
}

bool sb::ObstacleFinder::IsObstacleOnFrame( const cv::Mat &img, int minNumberOfPixel )
{
	int count = CountObstaclePixel( img );

	if( count > minNumberOfPixel ) {
		return true;
	}
	return false;
}
