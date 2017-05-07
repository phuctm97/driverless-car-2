#ifndef __SB_ANALYZE_DATA_H__
#define __SB_ANALYZE_DATA_H__

#include <opencv2/opencv.hpp>

namespace sb
{

struct AnalyzeData
{
	int currentCase;

	cv::Point target;

	std::vector<cv::Point> leftKnots;

	std::vector<cv::Point> rightKnots;
};

void release( AnalyzeData* data );

}

#endif //!__SB_ANALYZE_DATA_H__
