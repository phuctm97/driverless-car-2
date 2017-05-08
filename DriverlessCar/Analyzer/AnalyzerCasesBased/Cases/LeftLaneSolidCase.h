#ifndef __SB_LEFT_LANE_SOLID_CASE_H__
#define __SB_LEFT_LANE_SOLID_CASE_H__
#include "ICase.h"
#include "../../Data/Params/AnalyzeParams.h"

namespace sb
{
class LeftLaneSolidCase : public ICase
{
private:
	AnalyzeParams* _params;

	cv::Point _leftLaneOrigin;
	int _leftLaneSize;
	int _leftLaneHeight;
	std::vector<BlobRow> _leftRows;
	std::vector<std::pair<int, int>> _leftGoodSections, _leftBadSections;

public:
	// ReSharper disable CppPossiblyUninitializedMember
	LeftLaneSolidCase( AnalyzeParams* params )
	// ReSharper restore CppPossiblyUninitializedMember
		: _params( params ) {}

	int getType() override;

	int analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;

	int onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender );

private:
	int trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData );

	void beginBadSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const;

	void beginCorrectSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const;

	Blob* trackLeftBlob( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData );
};
}

#endif //!__SB_LEFT_LANE_SOLID_CASE_H__
