#ifndef __SB_BOTH_LANE_CASE_H__
#define __SB_BOTH_LANE_CASE_H__

#include "ICase.h"
#include <algorithm>
#include <algorithm>
#include <algorithm>

namespace sb
{
class BothLaneCase : public ICase
{
private:
	static const int MIN_ACCEPTABLE_LANE_BLOB_SIZE;

	static const int MIN_LANE_WIDTH_1;
	static const int MAX_LANE_WIDTH_1;
	static const int MIN_LANE_WIDTH_2;
	static const int MAX_LANE_WIDTH_2;
	static const int MAX_LANE_WIDTH_DIFF;
	static const int SECTION_HOPS_TO_LIVE;

	std::vector<int> _roadWidth;

public:
	int getType() override;

	int analyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData ) override;

private:
	int firstAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData );

	int trackAnalyze( CaseRepository* caseRepository, CollectData* collectData, CalculateData* calculateData, AnalyzeData* analyzeData );

	int findTwoLargestBlobs( CollectData* collectData, CalculateData* calculateData, Blob* largestBlobs[2] ) const;

	void beginErrorSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData );

	void beginCorrectSection( std::vector<BlobRow>::iterator& it_row, Blob* blob, CalculateData* calculateData );
};
}

#endif //!__SB_BOTH_LANE_CASE_H__
