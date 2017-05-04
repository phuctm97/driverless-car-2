#include "AnalyzerCasesBased.h"

int sb::AnalyzerCasesBased::init()
{
	return 0;
}

int sb::AnalyzerCasesBased::analyze( CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	// predict cases
	std::vector<ICase*> possibleCases;
	_casesResolver->predictCases( _caseRepository, collectData, calculateData, possibleCases );

	// check cases
	auto it_case = possibleCases.cbegin();
	for ( ; it_case != possibleCases.cend(); ++it_case ) {
		ICase* possibleCase = *it_case;
		if ( possibleCase->analyze( collectData, calculateData, analyzeData ) >= 0 ) break;
	}

	// result, not any case passed
	if ( it_case == possibleCases.cend() ) {
		for ( auto caseToRelease : possibleCases ) delete caseToRelease; // release cases
		return -1;
	}

	// push new case to repository
	_caseRepository->push( *it_case );
	for ( auto caseToRelease : possibleCases ) if ( caseToRelease != *it_case ) delete caseToRelease; // release cases

	return 0;
}

void sb::AnalyzerCasesBased::release()
{
	if ( _caseRepository != nullptr ) {
		_caseRepository->release();
		delete _caseRepository;
		_caseRepository = nullptr;
	}

	if ( _casesResolver != nullptr ) {
		delete _casesResolver;
		_caseRepository = nullptr;
	}
}
