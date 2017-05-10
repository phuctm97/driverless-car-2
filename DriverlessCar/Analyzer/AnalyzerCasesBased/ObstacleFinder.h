#ifndef __SB_OBSTACLE_FINDER_H__
#define __SB_OBSTACLE_FINDER_H__

#include <opencv2/opencv.hpp>

namespace sb
{
class ObstacleFinder
{
public:
	int init();

	int checkObstacle( const cv::Mat& image );

	void release();

	int CountObstaclePixel( const cv::Mat& bgrImage );

	bool IsObstacleOnFrame( const cv::Mat &img, int minNumberOfPixel = 3000 );
};
}

#endif //!__SB_OBSTACLE_FINDER_H__