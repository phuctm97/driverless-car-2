#ifndef __SB_BOTH_LANE_SOLID_CASE_H__
#define __SB_BOTH_LANE_SOLID_CASE_H__

#include "ICase.h"

namespace sb
{
class BothLaneSolidCase : public ICase
{
private:
	static const int MIN_LANE_BLOB_SIZE;
	static const int MIN_LANE_WIDTH_1;
	static const int MAX_LANE_WIDTH_1;
	static const int MIN_LANE_WIDTH_2;
	static const int MAX_LANE_WIDTH_2;
	static const int MAX_ROW_WIDTH_DIFF;
	static const int SECTION_HOPS_TO_LIVE;

	cv::Point _leftLaneOrigin, _rightLaneOrigin;
	int _leftLaneSize, _rightLaneSize;
	int _leftLaneHeight, _rightLaneHeight;
	std::vector<BlobRow> _leftRows, _rightRows;
	std::vector<std::pair<int, int>> _leftGoodSections, _leftBadSections, _rightGoodSections, _rightBadSections;

public:
	int getType() override;

	int analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;

	int onRedirect( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData, ICase* sender );

private:
	int firstAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData );

	int trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData );

	int findTwoLargestBlobs( CollectData* collectData, CalculateData* calculateData, Blob* largestBlobs[2] ) const;

	void beginBadSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const;

	void beginCorrectSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData ) const;

	Blob* trackLeftBlob( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData );

	Blob* trackRightBlob( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData );
};
}

#endif //!__SB_BOTH_LANE_SOLID_CASE_H__
