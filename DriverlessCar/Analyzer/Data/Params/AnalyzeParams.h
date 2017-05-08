#ifndef __SB_ANALYZE_PARAMS_H__
#define __SB_ANALYZE_PARAMS_H__

namespace sb
{
struct AnalyzeParams
{
	int MIN_LANE_BLOB_SIZE;
	int MIN_LANE_WIDTH_1;
	int MAX_LANE_WIDTH_1;
	int MIN_LANE_WIDTH_2;
	int MAX_LANE_WIDTH_2;
	int MAX_ROW_WIDTH_DIFF;
	int SECTION_HOPS_TO_LIVE;
};
}

#endif //!__SB_ANALYZE_PARAMS_H__
