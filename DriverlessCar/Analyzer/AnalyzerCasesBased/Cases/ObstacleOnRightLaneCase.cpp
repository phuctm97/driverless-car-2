#include "../CaseRepository.h"
#include "ObstacleOnRightLaneCase.h"
#include "RightLaneSolidCase.h"
#include "BothLaneSolidCase.h"

sb::AnalyzeParams* const& sb::ObstacleOnRightLaneCase::getParams() const { return _params; }

sb::ObstacleFinder* const& sb::ObstacleOnRightLaneCase::getObstacleFinder() const { return _obstacleFinder; }

const cv::Point& sb::ObstacleOnRightLaneCase::getObstaclePosition() const { return _obstaclePosition; }

const cv::Point& sb::ObstacleOnRightLaneCase::getLeftLaneOrigin() const { return _leftLaneOrigin; }

const int& sb::ObstacleOnRightLaneCase::getLeftLaneSize() const { return _leftLaneSize; }

const int& sb::ObstacleOnRightLaneCase::getLeftLaneHeight() const { return _leftLaneHeight; }

const std::vector<sb::BlobRow>& sb::ObstacleOnRightLaneCase::getLeftRows() const { return _leftRows; }

const std::vector<std::pair<int, int>>& sb::ObstacleOnRightLaneCase::getLeftGoodSections() const { return _leftGoodSections; }

const std::vector<std::pair<int, int>>& sb::ObstacleOnRightLaneCase::getLeftBadSections() const { return _leftBadSections; }

int sb::ObstacleOnRightLaneCase::getType()
{
	return CaseType::OBSTACLE_ON_RIGHT_LANE_CASE;
}

int sb::ObstacleOnRightLaneCase::analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	return trackAnalyze( caseRepository, collectData, calculateData, analyzeData );
}

int sb::ObstacleOnRightLaneCase::trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData )
{
	// track obstacle
	int center = _obstaclePosition.x;
	int bottom = _obstaclePosition.y;

	cv::Rect checkArea( MAX( 0, center - 100 + _params->CROP_OFFSET.x ), MAX( 0, bottom - 100 + _params->CROP_OFFSET.y ), 100, 100 );
	cv::Mat checkImage = collectData->colorImage( checkArea );

	// case to save
	ObstacleOnRightLaneCase* caseToSave = new ObstacleOnRightLaneCase( _params, _obstacleFinder );
	caseToSave->_obstaclePosition = _obstaclePosition;

	// obstacle disappear, find right lane and redirect right lane
	if ( _obstacleFinder->checkObstacle( checkImage ) < 0 ) {
		// find left lane
		Blob* leftBlob = findLeftBlob( caseRepository, collectData, calculateData );

		// check for right row lane width
		if ( leftBlob != nullptr ) {
			caseToSave->_leftLaneOrigin = leftBlob->origin;
			caseToSave->_leftLaneSize = static_cast<int>(leftBlob->size);
			caseToSave->_leftLaneHeight = leftBlob->box.height;
			caseToSave->_leftGoodSections.clear();
			caseToSave->_leftBadSections.clear(); {
				auto it_row = leftBlob->rows.begin();
				while ( it_row != leftBlob->rows.end() ) {
					auto pre_row = it_row;

					if ( (it_row->row >= calculateData->bgrImage.rows / 2 && (it_row->width < _params->MIN_LANE_WIDTH_1 || it_row->width > _params->MAX_LANE_WIDTH_1))
						|| (it_row->row < calculateData->bgrImage.rows / 2 && (it_row->width < _params->MIN_LANE_WIDTH_2 || it_row->width > _params->MAX_LANE_WIDTH_2)) ) {
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

		// redirect left lane
		if ( leftBlob != nullptr ) {
			LeftLaneSolidCase* leftLaneSolidCase = new LeftLaneSolidCase( _params, _obstacleFinder );
			int res = leftLaneSolidCase->onRedirect( caseRepository, collectData, calculateData, analyzeData, caseToSave );
			delete leftLaneSolidCase;
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

int sb::ObstacleOnRightLaneCase::onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender )
{
	switch ( sender->getType() ) {
	case CaseType::LEFT_LANE_SOLID_CASE: {}
		break;
	case CaseType::RIGHT_LANE_SOLID_CASE: {}
		break;
	case CaseType::BOTH_LANE_SOLID_CASE: {
		BothLaneSolidCase* bothLaneSolidCase = static_cast<BothLaneSolidCase*>(sender);

		auto caseToSave = new ObstacleOnRightLaneCase( _params, _obstacleFinder );
		caseToSave->_obstaclePosition.x = bothLaneSolidCase->getObstaclePosition();
		caseRepository->push( caseToSave );

		// use lane calculated in BOTH_LANE and width in repository calculate target
		auto cit_rrow = bothLaneSolidCase->getRightRows().crbegin();

		for ( ; cit_rrow != bothLaneSolidCase->getRightRows().crend(); ++cit_rrow ) {
			auto roadWidth = caseRepository->findRoadWidth( cit_rrow->row );
			if ( roadWidth < 0 ) continue;

			caseToSave->_obstaclePosition.y = cit_rrow->row;

			int xr = cit_rrow->minX;
			analyzeData->target = cv::Point( xr - roadWidth * 3 / 4, cit_rrow->row );
			break;
		}
	}
		break;
	}
	return 0;
}

sb::Blob* sb::ObstacleOnRightLaneCase::findLeftBlob( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData )
{
	for ( auto cit_blob = calculateData->blobs.cbegin(); cit_blob != calculateData->blobs.cend(); ++cit_blob ) {
		auto blob = *cit_blob;

		// check blob size
		if ( blob->size < _params->MIN_LANE_BLOB_SIZE ) continue;

		// check blob height
		if ( blob->box.height < _params->MIN_LANE_BLOB_HEIGHT ) continue;

		if ( blob->origin.x > _obstaclePosition.x - caseRepository->findRoadWidth( _obstaclePosition.y ) / 2 ) continue;

		// TODO: check shape (rows pos and width)
		return blob;
	}

	return nullptr;
}

void sb::ObstacleOnRightLaneCase::beginBadSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const
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

void sb::ObstacleOnRightLaneCase::beginCorrectSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const
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
