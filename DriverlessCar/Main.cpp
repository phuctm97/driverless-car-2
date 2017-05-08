#include "Collector/ICollector.h"
#include "Collector/CollectorWithVideo/CollectorWithVideo.h"
#include "Calculator/ICalculator.h"
#include "Calculator/CalculatorBlobsBased/CalculatorBlobsBased.h"
#include "Calculator/CropTool.h"
#include "Calculator/FlipTool.h"
#include "Calculator/BinarizeTool.h"
#include "Calculator/BlobTool.h"
#include "Analyzer/IAnalyzer.h"
#include "Analyzer/AnalyzerCasesBased/AnalyzerCasesBased.h"
#include "Application/IApplication.h"
#include "Application/Keyboard/WindowsKeyboard.h"
#include "Application/ApplicationTestCollector/ApplicationTestCollector.h"
#include "Application/ApplicationTestCalculator/ApplicationTestCalculator.h"
#include "Application/ApplicationTestAnalyzer/ApplicationTestAnalyzer.h"

sb::IApplication* application = nullptr;

void composeApplicationTestCollector();

void composeApplicationTestCalculator();

void composeApplicationTestAnalyzer();

void releaseApplication();

void onKeyPressed( int key );

int main( const int argc, const char** argv )
{
	composeApplicationTestAnalyzer();

	if ( application == nullptr ) return -1;

	application->addKeyboardCallback( &onKeyPressed );

	application->run();

	releaseApplication();

	return 0;
}

void composeApplicationTestCollector()
{
	sb::ICollector* collector = nullptr;

	collector = new sb::CollectorWithVideo( "..\\Debug\\sample-1.avi" );

	application = new sb::ApplicationTestCollector( collector, new sb::WindowsKeyboard( 33 ) );
}

void composeApplicationTestCalculator()
{
	sb::ICollector* collector = nullptr;

	sb::ICalculator* calculator = nullptr;

	collector = new sb::CollectorWithVideo( "..\\Debug\\sample-1.avi" );

	calculator = new sb::CalculatorBlobsBased( new sb::CropTool( cv::Rect( 0, 332, 640, 100 ) ),
	                                           new sb::FlipTool(),
	                                           new sb::BinarizeTool( 200 ),
	                                           new sb::BlobTool( { 0.2,0.25,0.25,0.3 }, cv::Size( 640, 100 ) ) );

	application = new sb::ApplicationTestCalculator( collector, calculator, new sb::WindowsKeyboard( 33 ) );
}

void composeApplicationTestAnalyzer()
{
	sb::ICollector* collector = nullptr;

	sb::ICalculator* calculator = nullptr;

	sb::IAnalyzer* analyzer = nullptr;

	collector = new sb::CollectorWithVideo( "..\\Debug\\sample-1.avi" );

	calculator = new sb::CalculatorBlobsBased( new sb::CropTool( cv::Rect( 0, 332, 640, 100 ) ),
	                                           new sb::FlipTool(),
	                                           new sb::BinarizeTool( 200 ),
	                                           new sb::BlobTool( { 0.2,0.25,0.25,0.3 }, cv::Size( 640, 100 ) ) );

	sb::AnalyzeParams* analyzeParams = new sb::AnalyzeParams();
	analyzeParams->MIN_LANE_BLOB_SIZE = 2500;
	analyzeParams->MIN_LANE_WIDTH_1 = 30;
	analyzeParams->MAX_LANE_WIDTH_1 = 60;
	analyzeParams->MIN_LANE_WIDTH_2 = 15;
	analyzeParams->MAX_LANE_WIDTH_2 = 40;
	analyzeParams->MAX_ROW_WIDTH_DIFF = 7;
	analyzeParams->SECTION_HOPS_TO_LIVE = 4;
	analyzer = new sb::AnalyzerCasesBased( analyzeParams );

	application = new sb::ApplicationTestAnalyzer( collector, calculator, analyzer, new sb::WindowsKeyboard( 33 ) );
}

void releaseApplication()
{
	application->release();
}

void onKeyPressed( int key )
{
	if ( key == 27 ) application->exit();
}
