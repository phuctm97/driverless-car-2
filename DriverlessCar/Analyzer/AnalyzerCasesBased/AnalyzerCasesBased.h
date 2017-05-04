#ifndef __SB_ANALYZER_CASES_BASED_H__
#define __SB_ANALYZER_CASES_BASED_H__

#include "../IAnalyzer.h"
#include "CaseRepository.h"
#include "CasesResolver.h"

namespace sb
{
class AnalyzerCasesBased : public IAnalyzer
{
private:
	CaseRepository* _caseRepository;
	CasesResolver* _casesResolver;

public:
	int init() override;

	int analyze( CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;

	void release() override;
};
}

#endif //!__SB_ANALYZER_CASES_BASED_H__