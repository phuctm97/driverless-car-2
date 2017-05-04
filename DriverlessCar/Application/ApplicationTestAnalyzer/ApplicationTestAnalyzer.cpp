#include "ApplicationTestAnalyzer.h"

sb::IKeyboard* sb::ApplicationTestAnalyzer::getKeyboard() { return _keyboard; }

void sb::ApplicationTestAnalyzer::run()
{
	// init
	_exiting = false;

	sb::CollectData* collectData = new CollectData;
	sb::CalculateData* calculateData = new CalculateData;
	sb::AnalyzeData* analyzeData = new AnalyzeData;

	if ( _collector == nullptr || _collector->init() < 0 ) _exiting = true;
	if ( _calculator == nullptr || _calculator->init() < 0 ) _exiting = true;
	if ( _analyzer == nullptr || _analyzer->init() < 0 ) _exiting = true;

	// run
	while ( !_exiting ) {
		if ( _collector->collect( collectData ) < 0 ) break;

		if ( _calculator->calculate( collectData, calculateData ) < 0 ) break;

		if ( _analyzer->analyze( collectData, calculateData, analyzeData ) < 0 ) break;

		// act

		// keyboard event
		if ( _keyboard != nullptr ) {
			_keyboard->checkBufferedKey();
		}
	}

	// release
	delete collectData;
	delete calculateData;
	delete analyzeData;

	release();
}

void sb::ApplicationTestAnalyzer::exit()
{
	_exiting = true;
}

void sb::ApplicationTestAnalyzer::release()
{
	_collector->release();
	delete _collector;
	_collector = nullptr;

	_calculator->release();
	delete _calculator;
	_calculator = nullptr;

	_analyzer->release();
	delete _analyzer;
	_analyzer = nullptr;

	delete _keyboard;
	_keyboard = nullptr;
}
