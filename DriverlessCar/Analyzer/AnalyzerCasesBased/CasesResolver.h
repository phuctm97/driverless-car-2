#ifndef __SB_CASES_RESOLVER_H__
#define __SB_CASES_RESOLVER_H__
#include "../../Collector/Data/CollectData.h"
#include "../../Calculator/Data/CalculateData.h"
#include "CaseRepository.h"

namespace sb
{
class CasesResolver
{
public:
	void predictCases( CaseRepository* caseRepository,
	                   CollectData* collectData,
	                   CalculateData* calculateData,
	                   std::vector<ICase*>& possibleCases );
};
}

#endif //!__SB_CASES_RESOLVER_H__
