#ifndef __SB_BOTH_LANE_CASE_H__
#define __SB_BOTH_LANE_CASE_H__

#include "ICase.h"

namespace sb
{
class BothLaneCase : public ICase {
public:
	int getType() override;

	int analyze( CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;
};
}

#endif //!__SB_BOTH_LANE_CASE_H__
