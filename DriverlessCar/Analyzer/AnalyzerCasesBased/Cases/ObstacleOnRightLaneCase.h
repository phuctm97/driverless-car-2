#ifndef __SB_OBSTACLE_ON_RIGHT_LANE_CASE_H__
#define __SB_OBSTACLE_ON_RIGHT_LANE_CASE_H__
#include "ICase.h"
#include "../ObstacleFinder.h"
#include "../../Data/Params/AnalyzeParams.h"

namespace sb
{
class ObstacleOnRightLaneCase : public ICase
{
private:
	AnalyzeParams* _params;
	ObstacleFinder* _obstacleFinder;

	cv::Point _obstaclePosition;
	cv::Point _leftLaneOrigin;
	int _leftLaneSize;
	int _leftLaneHeight;
	std::vector<BlobRow> _leftRows;
	std::vector<std::pair<int, int>> _leftGoodSections, _leftBadSections;

public:
	// ReSharper disable CppPossiblyUninitializedMember
	ObstacleOnRightLaneCase( AnalyzeParams* params, ObstacleFinder* obstacleFinder )
		// ReSharper restore CppPossiblyUninitializedMember
		: _params( params ), _obstacleFinder( obstacleFinder )
	{}

	AnalyzeParams* const& getParams() const;

	ObstacleFinder* const& getObstacleFinder() const;

	const cv::Point& getObstaclePosition() const;

	const cv::Point& getLeftLaneOrigin() const;

	const int& getLeftLaneSize() const;

	const int& getLeftLaneHeight() const;

	const std::vector<BlobRow>& getLeftRows() const;

	const std::vector<std::pair<int, int>>& getLeftGoodSections() const;

	const std::vector<std::pair<int, int>>& getLeftBadSections() const;

	int getType() override;

	int analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;

	int trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData );

	int onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender );

	Blob* findLeftBlob( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData );

	void beginBadSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const;

	void beginCorrectSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const;

};
}

#endif //!__SB_OBSTACLE_ON_RIGHT_LANE_CASE_H__