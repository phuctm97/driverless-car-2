#ifndef __SB_RIGHT_LANE_SOLID_CASE_H__
#define __SB_RIGHT_LANE_SOLID_CASE_H__
#include "ICase.h"
#include "../../Data/Params/AnalyzeParams.h"

namespace sb
{
class RightLaneSolidCase : public ICase
{
private:
	AnalyzeParams* _params;

	cv::Point _rightLaneOrigin;
	int _rightLaneSize;
	int _rightLaneHeight;
	std::vector<BlobRow> _rightRows;
	std::vector<std::pair<int, int>> _rightGoodSections, _rightBadSections;

public:
	// ReSharper disable CppPossiblyUninitializedMember
	RightLaneSolidCase( AnalyzeParams* params )
	// ReSharper restore CppPossiblyUninitializedMember
		: _params( params ) { }

	int getType() override;

	int analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;

	int onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender );

private:
	int trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData );

	void beginBadSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const;

	void beginCorrectSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const;

	Blob* trackRightBlob( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData );
};
}

#endif //!__SB_RIGHT_LANE_SOLID_CASE_H__
