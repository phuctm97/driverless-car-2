#include "BothLaneSolidCase.h"
#include "../CaseRepository.h"
#include "RightLaneSolidCase.h"
#include "LeftLaneSolidCase.h"

const int sb::BothLaneSolidCase::MIN_LANE_BLOB_SIZE = 2500;
const int sb::BothLaneSolidCase::MIN_LANE_WIDTH_1 = 30;
const int sb::BothLaneSolidCase::MAX_LANE_WIDTH_1 = 55;
const int sb::BothLaneSolidCase::MIN_LANE_WIDTH_2 = 15;
const int sb::BothLaneSolidCase::MAX_LANE_WIDTH_2 = 40;
const int sb::BothLaneSolidCase::MAX_ROW_WIDTH_DIFF = 7;
const int sb::BothLaneSolidCase::SECTION_HOPS_TO_LIVE = 4;

int sb::BothLaneSolidCase::getType()
{
	return CaseType::BOTH_LANE_SOLID_CASE;
}

int sb::BothLaneSolidCase::analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	// first time run
	if ( caseRepository->empty() ) {
		return firstAnalyze( caseRepository, collectData, calculateData, analyzeData );
	}

	return trackAnalyze( caseRepository, collectData, calculateData, analyzeData );
}

int sb::BothLaneSolidCase::firstAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	// find 2 largest blob
	Blob* largestBlobs[2] = { nullptr };
	if ( findTwoLargestBlobs( collectData, calculateData, largestBlobs ) < 0 )
		return -1;

	// set left blob and right blob
	Blob* leftBlob = largestBlobs[0]->origin.x < largestBlobs[1]->origin.x ? largestBlobs[0] : largestBlobs[1];
	Blob* rightBlob = largestBlobs[0]->origin.x > largestBlobs[1]->origin.x ? largestBlobs[0] : largestBlobs[1];

	// check for size
	if ( leftBlob->size < MIN_LANE_BLOB_SIZE || rightBlob->size < MIN_LANE_BLOB_SIZE )
		return -1;

	// check for height
	if ( leftBlob->box.height < calculateData->bgrImage.rows * 3 / 4 || rightBlob->box.height < calculateData->bgrImage.rows * 3 / 4 )
		return -1;

	auto caseToSave = new BothLaneSolidCase();
	caseRepository->push( caseToSave );

	// check for left lane width
	caseToSave->_leftGoodSections.clear();
	caseToSave->_leftBadSections.clear(); {
		auto it_row = leftBlob->rows.begin();
		while ( it_row != leftBlob->rows.end() ) {
			auto pre_row = it_row;

			if ( (it_row->row >= calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_1 || it_row->width > MAX_LANE_WIDTH_1))
				|| (it_row->row < calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_2 || it_row->width > MAX_LANE_WIDTH_2)) ) {
				beginBadSection( it_row, leftBlob, calculateData );
				caseToSave->_leftBadSections.push_back( std::make_pair( static_cast<int>(std::distance( leftBlob->rows.begin(), pre_row )), static_cast<int>(std::distance( pre_row, it_row )) ) );
			}
			else {
				beginCorrectSection( it_row, leftBlob, calculateData );
				caseToSave->_leftGoodSections.push_back( std::make_pair( static_cast<int>(std::distance( leftBlob->rows.begin(), pre_row )), static_cast<int>(std::distance( pre_row, it_row )) ) );
			}
		}
	}

	// check for right lane width
	caseToSave->_rightGoodSections.clear();
	caseToSave->_rightBadSections.clear(); {
		auto it_row = rightBlob->rows.begin();
		while ( it_row != rightBlob->rows.end() ) {
			auto pre_row = it_row;

			if ( (it_row->row >= calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_1 || it_row->width > MAX_LANE_WIDTH_1))
				|| (it_row->row < calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_2 || it_row->width > MAX_LANE_WIDTH_2)) ) {
				beginBadSection( it_row, rightBlob, calculateData );
				caseToSave->_rightBadSections.push_back( std::make_pair( static_cast<int>(std::distance( leftBlob->rows.begin(), pre_row )), static_cast<int>(std::distance( pre_row, it_row )) ) );
			}
			else {
				beginCorrectSection( it_row, rightBlob, calculateData );
				caseToSave->_rightGoodSections.push_back( std::make_pair( static_cast<int>(std::distance( rightBlob->rows.begin(), pre_row )), static_cast<int>(std::distance( pre_row, it_row )) ) );
			}
		}
	}

	// check good section percentage
	{
		int sum;

		sum = 0;
		for ( auto sectionInfo : caseToSave->_leftGoodSections ) sum += sectionInfo.second;
		if ( (1.0f * sum / calculateData->bgrImage.rows) < 0.6f ) return -1;

		sum = 0;
		for ( auto sectionInfo : caseToSave->_rightGoodSections ) sum += sectionInfo.second;
		if ( (1.0f * sum / calculateData->bgrImage.rows) < 0.6f ) return -1;
	}

	// save size, height, shape: save to this, and push this to repository
	caseToSave->_leftLaneOrigin = leftBlob->origin;
	caseToSave->_rightLaneOrigin = rightBlob->origin;
	caseToSave->_leftLaneSize = static_cast<int>(leftBlob->size);
	caseToSave->_rightLaneSize = static_cast<int>(rightBlob->size);
	caseToSave->_leftLaneHeight = leftBlob->box.height;
	caseToSave->_rightLaneHeight = rightBlob->box.height;
	caseToSave->_leftRows = leftBlob->rows;
	caseToSave->_rightRows = rightBlob->rows;

	// calculate road width: save to repository in a map (from row index to road width)
	{
		auto cit_lrow = leftBlob->rows.cbegin();
		auto cit_rrow = rightBlob->rows.cbegin();
		while ( cit_lrow->row > cit_rrow->row && cit_lrow != leftBlob->rows.cend() ) ++cit_lrow;
		if ( cit_lrow == leftBlob->rows.cend() ) return -1;

		while ( cit_rrow->row > cit_lrow->row && cit_rrow != rightBlob->rows.cend() ) ++cit_rrow;
		if ( cit_rrow == rightBlob->rows.cend() ) return -1;

		while ( cit_lrow != leftBlob->rows.cend() && cit_rrow != rightBlob->rows.cend() ) {
			if ( cit_lrow->tag >= 0 && cit_rrow->tag >= 0 )
				caseRepository->mapRoadWidth( cit_lrow->row, cit_rrow->minX - cit_lrow->maxX + 1 );
			++cit_lrow;
			++cit_rrow;
		}
	}

	// debug
	{
		cv::Mat img = calculateData->bgrImage.clone();
		for ( auto row : caseToSave->_leftRows ) {
			cv::Scalar color;
			if ( row.tag == 0 ) color = cv::Scalar( 0, 255, 0 );
			else if ( row.tag == 1 ) color = cv::Scalar( 0, 255, 255 );
			else if ( row.tag == -1 ) color = cv::Scalar( 0, 0, 255 );

			cv::line( img, cv::Point( row.minX, row.row ), cv::Point( row.maxX, row.row ), color, 1 );
		}
		for ( auto row : caseToSave->_rightRows ) {
			cv::Scalar color;
			if ( row.tag == 0 ) color = cv::Scalar( 0, 255, 0 );
			else if ( row.tag == 1 ) color = cv::Scalar( 0, 255, 255 );
			else if ( row.tag == -1 ) color = cv::Scalar( 0, 0, 255 );

			cv::line( img, cv::Point( row.minX, row.row ), cv::Point( row.maxX, row.row ), color, 1 );
		}
		cv::imshow( "Analyzer", img );
		cv::waitKey();
	}

	// calculate target: select the most reasonable row to calculate
	{
		auto cit_lrow = leftBlob->rows.crbegin();
		auto cit_rrow = rightBlob->rows.crbegin();

		for ( ; cit_lrow != leftBlob->rows.crend() && cit_rrow != rightBlob->rows.crend(); ++cit_lrow , ++cit_rrow ) {
			if ( cit_lrow->tag != 0 || cit_rrow->tag != 0 ) continue;

			int x1 = cit_lrow->maxX;
			int x2 = cit_rrow->minX;
			analyzeData->target = cv::Point( (x1 + x2) / 2, cit_lrow->row );
			break;
		}
	}

	return 0;
}

int sb::BothLaneSolidCase::trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	Blob *leftBlob = nullptr, *rightBlob = nullptr;

	// find blob similar the most to this old left and right
	leftBlob = trackLeftBlob( caseRepository, collectData, calculateData );

	rightBlob = trackRightBlob( caseRepository, collectData, calculateData );

	if ( leftBlob == nullptr && rightBlob == nullptr ) return -1; // TODO: dashed lane

	auto caseToSave = new BothLaneSolidCase();

	// check for left lane width
	if ( leftBlob != nullptr ) {
		caseToSave->_leftLaneOrigin = leftBlob->origin;
		caseToSave->_leftLaneSize = static_cast<int>(leftBlob->size);
		caseToSave->_leftLaneHeight = leftBlob->box.height;

		caseToSave->_leftGoodSections.clear();
		caseToSave->_leftBadSections.clear(); {
			auto it_row = leftBlob->rows.begin();
			while ( it_row != leftBlob->rows.end() ) {
				auto pre_row = it_row;

				if ( (it_row->row >= calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_1 || it_row->width > MAX_LANE_WIDTH_1))
					|| (it_row->row < calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_2 || it_row->width > MAX_LANE_WIDTH_2)) ) {
					beginBadSection( it_row, leftBlob, calculateData );
					caseToSave->_leftBadSections.push_back( std::make_pair( static_cast<int>(std::distance( leftBlob->rows.begin(), pre_row )), static_cast<int>(std::distance( pre_row, it_row )) ) );
				}
				else {
					beginCorrectSection( it_row, leftBlob, calculateData );
					caseToSave->_leftGoodSections.push_back( std::make_pair( static_cast<int>(std::distance( leftBlob->rows.begin(), pre_row )), static_cast<int>(std::distance( pre_row, it_row )) ) );
				}
			}
		}

		caseToSave->_leftRows = leftBlob->rows;
	}

	// check for right lane width
	if ( rightBlob != nullptr ) {
		caseToSave->_rightLaneOrigin = rightBlob->origin;
		caseToSave->_rightLaneSize = static_cast<int>(rightBlob->size);
		caseToSave->_rightLaneHeight = rightBlob->box.height;	caseToSave->_rightGoodSections.clear();

		caseToSave->_rightGoodSections.clear();
		caseToSave->_rightBadSections.clear(); {
			auto it_row = rightBlob->rows.begin();
			while ( it_row != rightBlob->rows.end() ) {
				auto pre_row = it_row;

				if ( (it_row->row >= calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_1 || it_row->width > MAX_LANE_WIDTH_1))
					|| (it_row->row < calculateData->bgrImage.rows / 2 && (it_row->width < MIN_LANE_WIDTH_2 || it_row->width > MAX_LANE_WIDTH_2)) ) {
					beginBadSection( it_row, rightBlob, calculateData );
					caseToSave->_rightBadSections.push_back( std::make_pair( static_cast<int>(std::distance( leftBlob->rows.begin(), pre_row )), static_cast<int>(std::distance( pre_row, it_row )) ) );
				}
				else {
					beginCorrectSection( it_row, rightBlob, calculateData );
					caseToSave->_rightGoodSections.push_back( std::make_pair( static_cast<int>(std::distance( rightBlob->rows.begin(), pre_row )), static_cast<int>(std::distance( pre_row, it_row )) ) );
				}
			}
		}

		caseToSave->_rightRows = rightBlob->rows;
	}

	// redirect LEFT_LANE_SOLID
	if ( leftBlob == nullptr ) {
		auto rightLaneSolidCase = new RightLaneSolidCase();
		int res = rightLaneSolidCase->onRedirect( caseRepository, collectData, calculateData, analyzeData, caseToSave );
		delete caseToSave;
		return res;
	}

	// redirect RIGHT_LANE_SOLID
	if ( rightBlob == nullptr ) {

		auto leftLaneSolidCase = new LeftLaneSolidCase();
		int res = leftLaneSolidCase->onRedirect( caseRepository, collectData, calculateData, analyzeData, caseToSave );
		delete caseToSave;
		return res;
	}

	caseRepository->push( caseToSave );

	// debug
	{
		cv::Mat img = calculateData->bgrImage.clone();
		for ( auto row : caseToSave->_leftRows ) {
			cv::Scalar color;
			if ( row.tag == 0 ) color = cv::Scalar( 0, 255, 0 );
			else if ( row.tag == 1 ) color = cv::Scalar( 0, 255, 255 );
			else if ( row.tag == -1 ) color = cv::Scalar( 0, 0, 255 );

			cv::line( img, cv::Point( row.minX, row.row ), cv::Point( row.maxX, row.row ), color, 1 );
		}
		for ( auto row : caseToSave->_rightRows ) {
			cv::Scalar color;
			if ( row.tag == 0 ) color = cv::Scalar( 0, 255, 0 );
			else if ( row.tag == 1 ) color = cv::Scalar( 0, 255, 255 );
			else if ( row.tag == -1 ) color = cv::Scalar( 0, 0, 255 );

			cv::line( img, cv::Point( row.minX, row.row ), cv::Point( row.maxX, row.row ), color, 1 );
		}
		cv::imshow( "Analyzer", img );
		cv::waitKey();
	}

	// check good section percentage
	float leftGoodRatio = 0;
	float rightGoodRatio = 0; {
		int sum;

		sum = 0;
		for ( auto sectionInfo : caseToSave->_leftGoodSections ) sum += sectionInfo.second;
		leftGoodRatio = 1.0f * sum / calculateData->bgrImage.rows;

		sum = 0;
		for ( auto sectionInfo : caseToSave->_rightGoodSections ) sum += sectionInfo.second;
		rightGoodRatio = 1.0f * sum / calculateData->bgrImage.rows;
	}

	if ( leftGoodRatio < 0.5f && rightGoodRatio < 0.5f ) return -1; // TODO: dashed lane

	// calculate target base on left lane
	if ( leftGoodRatio < 0.5f ) {
		auto cit_lrow = leftBlob->rows.crbegin();

		for ( ; cit_lrow != leftBlob->rows.crend(); ++cit_lrow ) {
			if ( cit_lrow->tag != 0 ) continue;

			auto roadWidth = caseRepository->findRoadWidth( cit_lrow->row );
			if ( roadWidth < 0 ) continue;

			int xl = cit_lrow->maxX;
			analyzeData->target = cv::Point( xl + roadWidth / 2, cit_lrow->row );
			break;
		}
	}
	// calculate target base on right lane
	else if ( rightGoodRatio < 0.5f ) {
		auto cit_rrow = rightBlob->rows.crbegin();

		for ( ; cit_rrow != rightBlob->rows.crend(); ++cit_rrow ) {
			if ( cit_rrow->tag != 0 ) continue;

			auto roadWidth = caseRepository->findRoadWidth( cit_rrow->row );
			if ( roadWidth < 0 ) continue;

			int xr = cit_rrow->maxX;
			analyzeData->target = cv::Point( xr - roadWidth / 2, cit_rrow->row );
			break;
		}
	}
	// calculate target with both lanes
	else {
		auto cit_lrow = leftBlob->rows.crbegin();
		auto cit_rrow = rightBlob->rows.crbegin();

		for ( ; cit_lrow != leftBlob->rows.crend() && cit_rrow != rightBlob->rows.crend(); ++cit_lrow , ++cit_rrow ) {
			if ( cit_lrow->tag != 0 || cit_rrow->tag != 0 ) continue;

			int x1 = cit_lrow->maxX;
			int x2 = cit_rrow->minX;
			analyzeData->target = cv::Point( (x1 + x2) / 2, cit_lrow->row );
			break;
		}
	}

	return 0;
}

int sb::BothLaneSolidCase::onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender )
{
	/* 1 hour

	case ONE_LANE:

	find opposite lane based on one lane calculated and road width in repository

	if found and check error pass, do like firstAnalyze and finish

	if not found/error, redirect ONE_LANE

	*/

	return 0;
}

int sb::BothLaneSolidCase::findTwoLargestBlobs( CollectData* collectData, CalculateData* calculateData, Blob* largestBlobs[2] ) const
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

void sb::BothLaneSolidCase::beginBadSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const
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
				if ( abs( it_row->width - correctWidth ) < MAX_ROW_WIDTH_DIFF ) {
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

void sb::BothLaneSolidCase::beginCorrectSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const
{
	auto rowToCompare = it_row;
	int failTimes = 0;

	while ( it_row != blob->rows.cend() ) {

		if ( abs( it_row->width - rowToCompare->width ) < MAX_ROW_WIDTH_DIFF ) {
			it_row->tag = 0;
			failTimes = 0;
		}
		else if ( it_row->width < MIN_LANE_WIDTH_1 ) {
			it_row->tag = 1;
			failTimes = 0;
		}
		else {
			if ( failTimes <= SECTION_HOPS_TO_LIVE ) {
				failTimes++;
			}
			else {
				break;
			}
			it_row->tag = 1;
		}

		++it_row;
		if ( std::distance( rowToCompare, it_row ) > 10 ) {
			rowToCompare = it_row - 10;
			while ( rowToCompare->tag != 0 && rowToCompare != it_row ) ++rowToCompare;

			if ( rowToCompare->tag != 0 ) break;
		}
	}
}

sb::Blob* sb::BothLaneSolidCase::trackLeftBlob( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData )
{
	for ( auto cit_blob = calculateData->blobs.cbegin(); cit_blob != calculateData->blobs.cend(); ++cit_blob ) {
		Blob* blob = *cit_blob;

		// debug
		{
			cv::Mat img = calculateData->bgrImage.clone();
			cv::circle( img, blob->origin, 5, cv::Scalar( 0, 255, 0 ), -1 );
			cv::imshow( "Analyzer", img );
			cv::waitKey();
		}

		// check blob size
		if ( blob->size < MIN_LANE_BLOB_SIZE ) continue;

		// check blob height
		if ( blob->box.height < calculateData->bgrImage.rows * 3 / 4 ) continue;

		// compare position
		if ( abs( blob->origin.x - _leftLaneOrigin.x ) > 100 ) continue;

		// compare size
		if ( abs( static_cast<int>(blob->size) - _leftLaneSize ) > 700 ) continue;

		// compare height
		if ( abs( blob->box.height - _leftLaneHeight ) > 10 ) continue;

		// TODO: compare shape (row width, angle)

		// TODO: save and compare blob knot (row width, angle)

		return blob;
	}

	return nullptr;
}

sb::Blob* sb::BothLaneSolidCase::trackRightBlob( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData )
{
	for ( auto cit_blob = calculateData->blobs.cbegin(); cit_blob != calculateData->blobs.cend(); ++cit_blob ) {
		Blob* blob = *cit_blob;

		// debug
		{
			cv::Mat img = calculateData->bgrImage.clone();
			cv::circle( img, blob->origin, 5, cv::Scalar( 0, 255, 0 ), -1 );
			cv::imshow( "Analyzer", img );
			cv::waitKey();
		}
		
		// check blob size
		if ( blob->size < MIN_LANE_BLOB_SIZE ) continue;

		// check blob height
		if ( blob->box.height < calculateData->bgrImage.rows * 3 / 4 ) continue;

		// compare position
		if ( abs( blob->origin.x - _rightLaneOrigin.x ) > 100 ) continue;

		// compare size
		if ( abs( static_cast<int>(blob->size) - _rightLaneSize ) > 700 ) continue;

		// compare height
		if ( abs( blob->box.height - _rightLaneHeight ) > 10 ) continue;

		// TODO: compare shape (row width, angle)

		// TODO: save and compare blob knot (row width, angle)
		
		return blob;
	}

	return nullptr;
}
