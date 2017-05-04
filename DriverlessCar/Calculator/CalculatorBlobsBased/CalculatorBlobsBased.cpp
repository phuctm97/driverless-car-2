#include "CalculatorBlobsBased.h"

int sb::CalculatorBlobsBased::init()
{
	if ( _cropTool == nullptr || _flipTool == nullptr || _binarizeTool == nullptr || _blobTool == nullptr ) return -1;

	return 0;
}

int sb::CalculatorBlobsBased::calculate( CollectData* collectData, CalculateData* calculateData )
{
	if ( _cropTool->crop( collectData->colorImage, calculateData->bgrImage ) < 0 ) {
		std::cerr << "CalculatorBlobsBased: crop image fail" << std::endl;
		return -1;
	}

	if ( _flipTool->flip( calculateData->bgrImage, calculateData->bgrImage ) < 0 ) {
		std::cerr << "CalculatorBlobsBased: flip image fail" << std::endl;
		return -1;
	}

	if ( _binarizeTool->binarize( calculateData->bgrImage, calculateData->binImage ) < 0 ) {
		std::cerr << "CalculatorBlobsBased: binarize image fail" << std::endl;
		return -1;
	}

	if ( _blobTool->findBlobs( calculateData->binImage ) < 0 ) {
		std::cerr << "CalculatorBlobsBased: find blobs fail" << std::endl;
		return -1;
	}

	return 0;
}

void sb::CalculatorBlobsBased::release()
{
	if ( _cropTool != nullptr ) delete _cropTool;
	if ( _flipTool != nullptr ) delete _flipTool;
	if ( _binarizeTool != nullptr ) delete _binarizeTool;
	if ( _blobTool != nullptr ) delete _blobTool;
}
