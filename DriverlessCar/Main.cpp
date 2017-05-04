#include "Collector/ICollector.h"
#include "Collector/CollectorWithVideo/CollectorWithVideo.h"
#include "Calculator/ICalculator.h"
#include "Calculator/CalculatorBlobsBased/CalculatorBlobsBased.h"
#include "Calculator/CropTool.h"
#include "Calculator/FlipTool.h"
#include "Calculator/BinarizeTool.h"
#include "Calculator/BlobTool.h"
#include "Analyzer/IAnalyzer.h"
#include "Application/IApplication.h"
#include "Application/Keyboard/WindowsKeyboard.h"
#include "Application/ApplicationTestCollector/ApplicationTestCollector.h"
#include "Application/ApplicationTestCalculator/ApplicationTestCalculator.h"

sb::IApplication* application = nullptr;

void composeApplication();

void releaseApplication();

void onKeyPressed( int key );

int main( const int argc, const char** argv )
{
	composeApplication();

	if ( application == nullptr ) return -1;

	application->addKeyboardCallback( &onKeyPressed );

	application->run();

	releaseApplication();

	return 0;
}

void composeApplication()
{
	sb::ICollector* collector = nullptr;

	sb::ICalculator* calculator = nullptr;

	collector = new sb::CollectorWithVideo( "..\\Debug\\sample-3.avi" );

	calculator = new sb::CalculatorBlobsBased( new sb::CropTool( cv::Rect( 0, 332, 640, 100 ) ),
	                                           new sb::FlipTool(),
	                                           new sb::BinarizeTool( 200 ),
	                                           new sb::BlobTool() );

	application = new sb::ApplicationTestCalculator( collector, calculator, new sb::WindowsKeyboard( 33 ) );
}

void releaseApplication()
{
	application->release();
}

void onKeyPressed( int key )
{
	if ( key == 27 ) application->exit();
}
