#ifndef __SB_I_BLOB_TOOL_H__
#define __SB_I_BLOB_TOOL_H__

#include <opencv2/opencv.hpp>

namespace sb
{
class IBlobTool
{
public:
	virtual ~IBlobTool() = default;

	virtual int findBlobs( const cv::Mat& binImage ) = 0;
};
}

#endif //!__SB_I_BLOB_TOOL_H__
