#include "RightLaneSolidCase.h"

int sb::RightLaneSolidCase::getType()
{
	return CaseType::RIGHT_LANE_SOLID_CASE;
}

int sb::RightLaneSolidCase::analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	return 0;
}

int sb::RightLaneSolidCase::onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender )
{
	
	return 0;
}
