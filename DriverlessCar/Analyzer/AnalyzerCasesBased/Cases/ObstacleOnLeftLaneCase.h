#ifndef __SB_OBSTACLE_ON_LEFT_LANE_CASE_H__
#define __SB_OBSTACLE_ON_LEFT_LANE_CASE_H__
#include "ICase.h"

namespace sb
{
class ObstacleOnLeftLaneCase : public ICase
{
public:
	int getType() override;

	int analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;
	
	int trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData );

	int onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender );
};
}

#endif //!__SB_OBSTACLE_ON_LEFT_LANE_CASE_H__