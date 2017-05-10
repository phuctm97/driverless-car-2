#include "../CaseRepository.h"
#include "ObstacleOnLeftLaneCase.h"
#include "LeftLaneSolidCase.h"
#include "BothLaneSolidCase.h"

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

	// obstacle disappear, find right lane and redirect right lane
	if( _obstacleFinder->checkObstacle( checkImage ) < 0 ) {
		// find right lane
		// redirect right lane
		int res;
		return res;
	}

	// obstacle still, update obstacle position, calculate target
	ObstacleOnLeftLaneCase* caseToSave = new ObstacleOnLeftLaneCase( _params, _obstacleFinder );
	caseToSave->_obstaclePosition = _obstaclePosition;
	caseRepository->push( caseToSave );

	return 0;
}

int sb::ObstacleOnLeftLaneCase::onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender )
{
	switch ( sender->getType() ) {
	case CaseType::LEFT_LANE_SOLID_CASE: { }
		break;
	case CaseType::RIGHT_LANE_SOLID_CASE: {
		
	}
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
		}
		break;
	}

	}
	return 0;
}
