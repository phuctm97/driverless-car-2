#include "../CaseRepository.h"
#include "ObstacleOnLeftLaneCase.h"
#include "LeftLaneSolidCase.h"
#include "BothLaneSolidCase.h"

sb::AnalyzeParams* const& sb::ObstacleOnLeftLaneCase::getParams() const { return _params; }

sb::ObstacleFinder* const& sb::ObstacleOnLeftLaneCase::getObstacleFinder() const { return _obstacleFinder; }

const cv::Point& sb::ObstacleOnLeftLaneCase::getObstaclePosition() const { return _obstaclePosition; }

const cv::Point& sb::ObstacleOnLeftLaneCase::getRightLaneOrigin() const { return _rightLaneOrigin; }

const int& sb::ObstacleOnLeftLaneCase::getRightLaneSize() const { return _rightLaneSize; }

const int& sb::ObstacleOnLeftLaneCase::getRightLaneHeight() const { return _rightLaneHeight; }

const std::vector<sb::BlobRow>& sb::ObstacleOnLeftLaneCase::getRightRows() const { return _rightRows; }

const std::vector<std::pair<int, int>>& sb::ObstacleOnLeftLaneCase::getRightGoodSections() const { return _rightGoodSections; }

const std::vector<std::pair<int, int>>& sb::ObstacleOnLeftLaneCase::getRightBadSections() const { return _rightBadSections; }

int sb::ObstacleOnLeftLaneCase::getType()
{
	return CaseType::OBSTACLE_ON_LEFT_LANE_CASE;
}

int sb::ObstacleOnLeftLaneCase::analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	return trackAnalyze( caseRepository, collectData, calculateData, analyzeData );
}

int sb::ObstacleOnLeftLaneCase::trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	// track obstacle
	int center = _obstaclePosition.x;
	int bottom = _obstaclePosition.y;

	cv::Rect checkArea( MAX( 0, center - 100 + _params->CROP_OFFSET.x ), MAX( 0, bottom - 100 + _params->CROP_OFFSET.y ), 100, 100 );
	cv::Mat checkImage = collectData->colorImage( checkArea );

	// case to save
	ObstacleOnLeftLaneCase* caseToSave = new ObstacleOnLeftLaneCase( _params, _obstacleFinder );
	caseToSave->_obstaclePosition = _obstaclePosition;

	// obstacle disappear, find right lane and redirect right lane
	if ( _obstacleFinder->checkObstacle( checkImage ) < 0 ) {
		// find right lane
		Blob* rightBlob = findRightBlob( caseRepository, collectData, calculateData );

		// check for left row lane width
		if ( rightBlob != nullptr ) {
			caseToSave->_rightLaneOrigin = rightBlob->origin;
			caseToSave->_rightLaneSize = static_cast<int>(rightBlob->size);
			caseToSave->_rightLaneHeight = rightBlob->box.height;
			caseToSave->_rightGoodSections.clear();
			caseToSave->_rightBadSections.clear(); {
				auto it_row = rightBlob->rows.begin();
				while ( it_row != rightBlob->rows.end() ) {
					auto pre_row = it_row;

					if ( (it_row->row >= calculateData->bgrImage.rows / 2 && (it_row->width < _params->MIN_LANE_WIDTH_1 || it_row->width > _params->MAX_LANE_WIDTH_1))
						|| (it_row->row < calculateData->bgrImage.rows / 2 && (it_row->width < _params->MIN_LANE_WIDTH_2 || it_row->width > _params->MAX_LANE_WIDTH_2)) ) {
						beginBadSection( it_row, rightBlob, calculateData );
						caseToSave->_rightBadSections.push_back( std::make_pair( static_cast<int>(std::distance( rightBlob->rows.begin(), pre_row )), static_cast<int>(std::distance( pre_row, it_row )) ) );
					}
					else {
						beginCorrectSection( it_row, rightBlob, calculateData );
						caseToSave->_rightGoodSections.push_back( std::make_pair( static_cast<int>(std::distance( rightBlob->rows.begin(), pre_row )), static_cast<int>(std::distance( pre_row, it_row )) ) );
					}
				}
			}
			caseToSave->_rightRows = rightBlob->rows;
		}

		// redirect right lane
		if ( rightBlob != nullptr ) {
			RightLaneSolidCase* rightLaneSolidCase = new RightLaneSolidCase( _params, _obstacleFinder );
			int res = rightLaneSolidCase->onRedirect( caseRepository, collectData, calculateData, analyzeData, caseToSave );
			delete rightLaneSolidCase;
			delete caseToSave;
			return res;
		}
	}

	// obstacle still, update obstacle position, calculate target
	caseRepository->push( caseToSave );

	auto roadWidth = caseRepository->findRoadWidth( _obstaclePosition.y );

	int xl = _obstaclePosition.x;
	analyzeData->target = cv::Point( xl + roadWidth * 3 / 4, _obstaclePosition.y );

	return 0;
}

int sb::ObstacleOnLeftLaneCase::onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender )
{
	switch ( sender->getType() ) {
	case CaseType::LEFT_LANE_SOLID_CASE: { }
		break;
	case CaseType::RIGHT_LANE_SOLID_CASE: { }
		break;
	case CaseType::BOTH_LANE_SOLID_CASE: {
		BothLaneSolidCase* bothLaneSolidCase = static_cast<BothLaneSolidCase*>(sender);

		auto caseToSave = new ObstacleOnLeftLaneCase( _params, _obstacleFinder );
		caseToSave->_obstaclePosition.x = bothLaneSolidCase->getObstaclePosition();
		caseRepository->push( caseToSave );

		// use lane calculated in BOTH_LANE and width in repository calculate target
		auto cit_lrow = bothLaneSolidCase->getLeftRows().crbegin();

		for ( ; cit_lrow != bothLaneSolidCase->getLeftRows().crend(); ++cit_lrow ) {
			auto roadWidth = caseRepository->findRoadWidth( cit_lrow->row );
			if ( roadWidth < 0 ) continue;

			caseToSave->_obstaclePosition.y = cit_lrow->row;

			int xl = cit_lrow->maxX;
			analyzeData->target = cv::Point( xl + roadWidth * 3 / 4, cit_lrow->row );
			break;
		}
	}
		break;
	}
	return 0;
}

sb::Blob* sb::ObstacleOnLeftLaneCase::findRightBlob( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData )
{
	for ( auto cit_blob = calculateData->blobs.cbegin(); cit_blob != calculateData->blobs.cend(); ++cit_blob ) {
		auto blob = *cit_blob;

		// check blob size
		if ( blob->size < _params->MIN_LANE_BLOB_SIZE ) continue;

		// check blob height
		if ( blob->box.height < _params->MIN_LANE_BLOB_HEIGHT ) continue;

		if ( blob->origin.x < _obstaclePosition.x + caseRepository->findRoadWidth( _obstaclePosition.y ) / 2 ) continue;

		// TODO: check shape (rows pos and width)
		return blob;
	}

	return nullptr;
}

void sb::ObstacleOnLeftLaneCase::beginBadSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const
{
	int correctRow = 0;
	int correctWidth = -1;

	while ( it_row != blob->rows.cend() ) {

		if ( (it_row->row >= calculateData->bgrImage.rows / 2 && it_row->width >= _params->MIN_LANE_WIDTH_1 && it_row->width <= _params->MAX_LANE_WIDTH_1)
			|| (it_row->row < calculateData->bgrImage.rows / 2 && it_row->width >= _params->MIN_LANE_WIDTH_2 && it_row->width <= _params->MAX_LANE_WIDTH_2) ) {
			if ( correctRow == 0 ) {
				correctWidth = it_row->width;
				correctRow++;
			}
			else if ( correctRow > _params->SECTION_HOPS_TO_LIVE ) {
				break;
			}
			else {
				if ( abs( it_row->width - correctWidth ) < _params->MAX_ROW_WIDTH_DIFF ) {
					correctRow++;
				}
				else {
					correctRow = 0;
				}
			}
		}
		else {
			correctRow = 0;
		}
		it_row->tag = -1;
		++it_row;
	}
}

void sb::ObstacleOnLeftLaneCase::beginCorrectSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const
{
	auto rowToCompare = it_row;
	int failTimes = 0;

	while ( it_row != blob->rows.cend() ) {

		if ( abs( it_row->width - rowToCompare->width ) < _params->MAX_ROW_WIDTH_DIFF ) {
			it_row->tag = 0;
			failTimes = 0;
		}
		else if ( it_row->width < _params->MIN_LANE_WIDTH_1 ) {
			it_row->tag = 1;
			failTimes = 0;
		}
		else {
			if ( failTimes <= _params->SECTION_HOPS_TO_LIVE ) {
				failTimes++;
			}
			else {
				break;
			}
			it_row->tag = 1;
		}

		if ( std::distance( rowToCompare, it_row ) >= 10 ) {
			rowToCompare = it_row - 10;
			while ( rowToCompare->tag != 0 && rowToCompare != it_row ) ++rowToCompare;

			if ( rowToCompare->tag != 0 ) break;
		}
		++it_row;
	}
}
