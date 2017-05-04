#include "CasesResolver.h"

void sb::CasesResolver::predictCases( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, std::vector<ICase*>& possibleCases )
{
	// check for data in repository, see how lane position and shape change

	if ( caseRepository->getAll().empty() ) {
		
	}
}
