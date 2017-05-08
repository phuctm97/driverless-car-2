#ifndef __SB_RIGHT_LANE_SOLID_CASE_H__
#define __SB_RIGHT_LANE_SOLID_CASE_H__
#include "ICase.h"

namespace sb
{
class RightLaneSolidCase : public ICase
{
public:
	int getType() override;

	int analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;

	int onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender );
};
}


#endif //!__SB_RIGHT_LANE_SOLID_CASE_H__