#ifndef __SB_OBSTACLE_ON_LEFT_LANE_CASE_H__
#define __SB_OBSTACLE_ON_LEFT_LANE_CASE_H__
#include "ICase.h"
#include "../ObstacleFinder.h"
#include "../../Data/Params/AnalyzeParams.h"

namespace sb
{
class ObstacleOnLeftLaneCase : public ICase
{
private:
	AnalyzeParams* _params;
	ObstacleFinder* _obstacleFinder;

public:
	// ReSharper disable CppPossiblyUninitializedMember
	ObstacleOnLeftLaneCase( AnalyzeParams* params, ObstacleFinder* obstacleFinder )
		// ReSharper restore CppPossiblyUninitializedMember
		: _params( params ), _obstacleFinder( obstacleFinder )
	{}

	int getType() override;

	int analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;
	
	int trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData );

	int onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender );
};
}

#endif //!__SB_OBSTACLE_ON_LEFT_LANE_CASE_H__