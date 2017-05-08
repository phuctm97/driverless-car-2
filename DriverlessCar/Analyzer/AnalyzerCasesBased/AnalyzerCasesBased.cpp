#include "AnalyzerCasesBased.h"
#include "Cases/BothLaneSolidCase.h"

int sb::AnalyzerCasesBased::init()
{
	_caseRepository = new CaseRepository( 10 );
	return 0;
}

int sb::AnalyzerCasesBased::analyze( CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	ICase* lastCase = nullptr;
	if ( _caseRepository->empty() ) {
		lastCase = new BothLaneSolidCase( _params );
	}
	else {
		lastCase = _caseRepository->last();
	}

	return lastCase->analyze( _caseRepository, collectData, calculateData, analyzeData );
}

void sb::AnalyzerCasesBased::release()
{
	if ( _caseRepository != nullptr ) {
		_caseRepository->release();
		delete _caseRepository;
		_caseRepository = nullptr;
	}

	if ( _params != nullptr ) {
		delete _params;
	}
}
