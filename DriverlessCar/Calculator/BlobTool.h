#ifndef __SB_BLOB_TOOL_H__
#define __SB_BLOB_TOOL_H__
#include "IBlobTool.h"

namespace sb
{
class BlobTool: public IBlobTool {
public:
	int findBlobs( const cv::Mat& binImage ) override;
};
}

#endif //!__SB_BLOB_TOOL_H__
