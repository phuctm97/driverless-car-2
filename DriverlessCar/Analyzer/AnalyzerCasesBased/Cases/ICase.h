#ifndef __SB_I_CASE_H__
#define __SB_I_CASE_H__

#include "../../../Collector/Data/CollectData.h"
#include "../../../Calculator/Data/CalculateData.h"
#include "../../Data/AnalyzeData.h"

namespace sb
{

enum CaseType
{
	BOTH_LANE_CASE = 0
};

class ICase
{
public:
	virtual ~ICase() = default;

	virtual int getType() = 0;

	virtual int analyze( CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) = 0;
};
}

#endif //!__SB_I_CASE_H__
