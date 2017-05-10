#ifndef __SB_OBSTACLE_ON_LEFT_LANE_CASE_H__
#define __SB_OBSTACLE_ON_LEFT_LANE_CASE_H__
#include "ICase.h"
#include "../ObstacleFinder.h"
#include "../../Data/Params/AnalyzeParams.h"

namespace sb
{
class ObstacleOnLeftLaneCase : public ICase
{
private:
	AnalyzeParams* _params;
	ObstacleFinder* _obstacleFinder;

	cv::Point _obstaclePosition;
	cv::Point _rightLaneOrigin;
	int _rightLaneSize;
	int _rightLaneHeight;
	std::vector<BlobRow> _rightRows;
	std::vector<std::pair<int, int>> _rightGoodSections, _rightBadSections;

public:
	// ReSharper disable CppPossiblyUninitializedMember
	ObstacleOnLeftLaneCase( AnalyzeParams* params, ObstacleFinder* obstacleFinder )
		// ReSharper restore CppPossiblyUninitializedMember
		: _params( params ), _obstacleFinder( obstacleFinder )
	{}

	AnalyzeParams* const& getParams() const;

	ObstacleFinder* const& getObstacleFinder() const;

	const cv::Point& getObstaclePosition() const;

	const cv::Point& getRightLaneOrigin() const;

	const int& getRightLaneSize() const;

	const int& getRightLaneHeight() const;

	const std::vector<BlobRow>& getRightRows() const;

	const std::vector<std::pair<int, int>>& getRightGoodSections() const;

	const std::vector<std::pair<int, int>>& getRightBadSections() const;

	int getType() override;

	int analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;
	
	int trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData );

	int onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender );

	Blob* findRightBlob( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData );

	void beginBadSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const;

	void beginCorrectSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const;

};
}

#endif //!__SB_OBSTACLE_ON_LEFT_LANE_CASE_H__