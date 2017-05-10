#include "ObstacleOnLeftLaneCase.h"

int sb::ObstacleOnLeftLaneCase::getType()
{
	return CaseType::OBSTACLE_ON_LEFT_LANE_CASE;
}

int sb::ObstacleOnLeftLaneCase::analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	return trackAnalyze( caseRepository, collectData, calculateData, analyzeData );
}

int sb::ObstacleOnLeftLaneCase::trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	
	return 0;
}

int sb::ObstacleOnLeftLaneCase::onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender )
{
	return 0;
}
