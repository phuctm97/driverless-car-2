#include "BothLaneCase.h"
#include "../CaseRepository.h"

const int sb::BothLaneCase::MIN_ACCEPTABLE_LANE_BLOB_SIZE = 1500;
const int sb::BothLaneCase::MIN_LANE_WIDTH_1 = 20;
const int sb::BothLaneCase::MAX_LANE_WIDTH_1 = 40;
const int sb::BothLaneCase::MIN_LANE_WIDTH_2 = 12;
const int sb::BothLaneCase::MAX_LANE_WIDTH_2 = 30;
const int sb::BothLaneCase::MAX_LANE_WIDTH_DIFF = 7;
const int sb::BothLaneCase::SECTION_HOPS_TO_LIVE = 4;

int sb::BothLaneCase::getType()
{
	return CaseType::BOTH_LANE_CASE;
}

int sb::BothLaneCase::analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	// first time run
	if ( caseRepository->empty() ) {
		return firstAnalyze( caseRepository, collectData, calculateData, analyzeData );
	}

	return trackAnalyze( caseRepository, collectData, calculateData, analyzeData );
}

int sb::BothLaneCase::firstAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	// find 2 largest blob
	Blob* largestBlobs[2] = { nullptr };
	if ( findTwoLargestBlobs( collectData, calculateData, largestBlobs ) < 0 )
		return -1;

	// set left blob and right blob
	Blob* leftBlob = largestBlobs[0]->origin.x < largestBlobs[1]->origin.x ? largestBlobs[0] : largestBlobs[1];
	Blob* rightBlob = largestBlobs[0]->origin.x > largestBlobs[1]->origin.x ? largestBlobs[0] : largestBlobs[1];

	// check for size
	if ( leftBlob->size < MIN_ACCEPTABLE_LANE_BLOB_SIZE || rightBlob->size < MIN_ACCEPTABLE_LANE_BLOB_SIZE )
		return -1;

	// check for height
	if ( leftBlob->box.height < calculateData->bgrImage.rows * 3 / 4 || rightBlob->box.height < calculateData->bgrImage.rows * 3 / 4 )
		return -1;

	// check for left lane width
	std::vector<std::pair<int, int>> leftSections; {
		auto it_row = leftBlob->rows.begin();
		while ( it_row != leftBlob->rows.end() ) {
			auto pre_row = it_row;

			if ( (it_row->row >= calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_1 || it_row->width > MAX_LANE_WIDTH_1))
				|| (it_row->row < calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_2 || it_row->width > MAX_LANE_WIDTH_2)) ) {
				beginErrorSection( it_row, leftBlob, calculateData );
				leftSections.push_back( std::make_pair( static_cast<int>(std::distance( pre_row, it_row )), -1 ) );
			}
			else {
				beginCorrectSection( it_row, leftBlob, calculateData );
				leftSections.push_back( std::make_pair( static_cast<int>(std::distance( pre_row, it_row )), 0 ) );
			}
		}
	}

	// check for right lane width
	std::vector<std::pair<int, int>> rightSections; {
		auto it_row = rightBlob->rows.begin();
		while ( it_row != rightBlob->rows.end() ) {
			auto pre_row = it_row;

			if ( (it_row->row >= calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_1 || it_row->width > MAX_LANE_WIDTH_1))
				|| (it_row->row < calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_2 || it_row->width > MAX_LANE_WIDTH_2)) ) {
				beginErrorSection( it_row, rightBlob, calculateData );
				rightSections.push_back( std::make_pair( static_cast<int>(std::distance( pre_row, it_row )), -1 ) );
			}
			else {
				beginCorrectSection( it_row, rightBlob, calculateData );
				rightSections.push_back( std::make_pair( static_cast<int>(std::distance( pre_row, it_row )), 0 ) );
			}
		}
	}

	return 0;
}

int sb::BothLaneCase::trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	
	return 0;
}

int sb::BothLaneCase::findTwoLargestBlobs( CollectData* collectData, CalculateData* calculateData, Blob* largestBlobs[2] ) const
{
	if ( calculateData->blobs.empty() ) return -1;

	for ( auto cit_blob = calculateData->blobs.cbegin(); cit_blob != calculateData->blobs.cend(); ++cit_blob ) {
		sb::Blob* blob = *cit_blob;

		if ( largestBlobs[0] == nullptr || blob->size > largestBlobs[0]->size ) {
			largestBlobs[1] = largestBlobs[0];
			largestBlobs[0] = blob;
		}
		else if ( largestBlobs[1] == nullptr || blob->size > largestBlobs[1]->size ) {
			largestBlobs[1] = blob;
		}
	}

	return 0;
}

void sb::BothLaneCase::beginErrorSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData )
{
	int correctRow = 0;
	int correctWidth = -1;

	while ( it_row != blob->rows.cend() ) {

		if ( (it_row->row >= calculateData->bgrImage.rows / 2 && it_row->width >= MIN_LANE_WIDTH_1 && it_row->width <= MAX_LANE_WIDTH_1)
			|| (it_row->row < calculateData->bgrImage.rows / 2 && it_row->width >= MIN_LANE_WIDTH_2 && it_row->width <= MAX_LANE_WIDTH_2) ) {
			if ( correctRow == 0 ) {
				correctWidth = it_row->width;
				correctRow++;
			}
			else if ( correctRow > SECTION_HOPS_TO_LIVE ) {
				break;
			}
			else {
				if ( abs( it_row->width - correctWidth ) < MAX_LANE_WIDTH_DIFF ) {
					correctRow++;
				}
				else {
					correctRow = 0;
				}
			}
		}
		it_row->tag = -1;
		++it_row;
	}
}

void sb::BothLaneCase::beginCorrectSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData )
{
	auto rowToCompare = it_row;
	int failTimes = 0;

	cv::Mat img = calculateData->bgrImage.clone();

	while ( it_row != blob->rows.cend() ) {

		if ( abs( it_row->width - rowToCompare->width ) < MAX_LANE_WIDTH_DIFF ) {
			it_row->tag = 0;
			failTimes = 0;
			cv::line( img, cv::Point( it_row->minX, it_row->row ), cv::Point( it_row->maxX, it_row->row ), cv::Scalar( 0, 255, 0 ), 1 );
		}
		else if ( it_row->width < MIN_LANE_WIDTH_1 ) {
			it_row->tag = 1;
			failTimes = 0;
			cv::line( img, cv::Point( it_row->minX, it_row->row ), cv::Point( it_row->maxX, it_row->row ), cv::Scalar( 0, 255, 255 ), 1 );
		}
		else {
			if ( failTimes <= SECTION_HOPS_TO_LIVE ) {
				failTimes++;
			}
			else {
				break;
			}
			it_row->tag = 1;

			cv::line( img, cv::Point( it_row->minX, it_row->row ), cv::Point( it_row->maxX, it_row->row ), cv::Scalar( 0, 0, 255 ), 1 );
		}
		cv::imshow( "Analyzer", img );
		cv::waitKey();

		++it_row;
		if ( std::distance( rowToCompare, it_row ) > 10 ) {
			rowToCompare = it_row - 10;
			while ( rowToCompare->tag != 0 && rowToCompare != it_row ) ++rowToCompare;

			if ( rowToCompare->tag != 0 ) break;
		}
	}
}
