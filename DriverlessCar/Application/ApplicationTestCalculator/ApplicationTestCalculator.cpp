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

		for( auto cit_blob = calculateData->blobs.cbegin(); cit_blob != calculateData->blobs.cend(); ++cit_blob ) {
			cv::Mat img = calculateData->bgrImage.clone();

			sb::Blob* blob = *cit_blob;
			for( auto cit_childblob = blob->childBlobs.cbegin(); cit_childblob != blob->childBlobs.cend(); ++cit_childblob ) {
				sb::Blob* childBlob = *cit_childblob;

				if( childBlob->size == 0 ) continue;

				cv::rectangle( img, childBlob->box.tl(), childBlob->box.br(), cv::Scalar( 0, 0, 255 ), 1 );
				cv::circle( img, childBlob->origin, 3, cv::Scalar( 0, 255, 0 ), 2 );
			}

			cv::putText( img, std::to_string( blob->size ), cv::Point( calculateData->bgrImage.cols / 2, 30 ),
									 cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar( 0, 255, 255 ), 2 );
			cv::imshow( "Calculator", img );
			cv::waitKey();
		}

		// keyboard event
		if ( _keyboard != nullptr ) {
			_keyboard->checkBufferedKey();
		}
	}

	// release
	if ( collectData != nullptr ) {
		sb::release( collectData );
		delete collectData;
	}

	if ( calculateData != nullptr ) {
		sb::release( calculateData );
		delete calculateData;
	}
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
