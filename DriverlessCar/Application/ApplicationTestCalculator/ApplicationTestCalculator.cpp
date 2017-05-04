#include "ApplicationTestCalculator.h"

void sb::ApplicationTestCalculator::run()
{
	// init
	_exiting = false;

	sb::CollectData* collectData = new CollectData;
	sb::CalculateData* calculateData = new CalculateData;

	if ( _collector == nullptr || _collector->init() < 0 ) _exiting = true;
	if ( _calculator == nullptr || _calculator->init() < 0 ) _exiting = true;

	// run
	while ( !_exiting ) {
		if ( _collector->collect( collectData ) < 0 ) break;

		cv::imshow( "Original image", collectData->colorImage );

		if ( _calculator->calculate( collectData, calculateData ) ) break;

		cv::imshow( "BGR image", calculateData->bgrImage );

		cv::imshow( "BIN image", calculateData->binImage );

		cv::waitKey();

		// keyboard event
		if ( _keyboard != nullptr ) {
			_keyboard->checkBufferedKey();
		}
	}

	// release
	delete collectData;
	delete calculateData;
}

void sb::ApplicationTestCalculator::exit()
{
	_exiting = true;
}

void sb::ApplicationTestCalculator::release()
{
	if ( _keyboard != nullptr ) {
		_keyboard->release();
		delete _keyboard;
	}

	if ( _collector != nullptr ) {
		_collector->release();
		delete _collector;
	}

	if ( _calculator != nullptr ) {
		_calculator->release();
		delete _calculator;
	}
}

void sb::ApplicationTestCalculator::addKeyboardCallback( const std::function<void( int )>& callback )
{
	if ( _keyboard != nullptr ) _keyboard->addKeyboardCallback( callback );
}
