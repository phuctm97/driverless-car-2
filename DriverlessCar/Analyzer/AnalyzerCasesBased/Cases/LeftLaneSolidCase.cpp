#include "LeftLaneSolidCase.h"

int sb::LeftLaneSolidCase::getType()
{
	return CaseType::LEFT_LANE_SOLID_CASE;
}

int sb::LeftLaneSolidCase::analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	return trackAnalyze( caseRepository, collectData, calculateData, analyzeData );
}

int sb::LeftLaneSolidCase::trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	/* 1 hour
	find blob the most like old lane ( size, height, shape )

	find error section (if any) based on old sections

	check width row again

	check both lane case possible ( relied on new blob position and road width in repository )
	
	if both lane possible, redirect both lane

	if lane has many error ( > 50% ), fail, return -1 ( todo redirect DASHED_LANE )
	
	save size, height, shape: save to this, and push this to repository

	calculate target and finish
	*/

	return 0;
}

int sb::LeftLaneSolidCase::onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender )
{
	/* 30 min
	case BOTH_LANE:

	use lane calculated in BOTH_LANE and width in repository calculate target

	save size, height, shape: save to this, and push this to repository
	
	calculate target and finish
	*/

	return 0;
}
