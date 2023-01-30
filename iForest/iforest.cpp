#define AnomalyDetection false
#include "iforest.h"
#include "itree.cpp"
#include <queue>
#include <math.h>
using namespace std;

iforest::iforest(const data & dataObject): _dataObject(dataObject){}

iforest::iforest(const data & dataObject, int numiTrees, int sampleSize):_dataObject(dataObject), _numiTrees(numiTrees), _sampleSize(sampleSize){
	_maxTreeHeight = (int)log2(_sampleSize);
	_maxNumOfNodes = (int)pow(2.0,_maxTreeHeight+1)-1;
	_iTrees.resize(_numiTrees);	
}

iforest::~iforest(){}

//***************************************************Training: iForest creation*********************************************************//
void iforest::constructiForest(){
    for(int treeId = 0; treeId < _numiTrees; treeId++){
		_iTrees[treeId] = new itree(_dataObject, _sampleSize, _maxTreeHeight, _maxNumOfNodes, _avgPLEstimationOfBST);
		_iTrees[treeId]->constructiTree();
	}
	//_avgPLComputationOfBST = this->avgPathLengthComputationOfBST();
	_avgPLEstimationOfBST = this->avgPathLengthEstimationOfBST(_sampleSize);
}



//***************************************Evaluation: Anoamly detection AD: anomaly score computation *************************************//

void iforest::computeAnomalyScore(int pointX, const data & testDataObject){
	long double avgPathLength = computeAvgPathLength(pointX, testDataObject);
	//long double AscoreComputed = pow(2,-(avgPathLength/_avgPLComputationOfBST));       
	long double AscoreEstimated = pow(2,-(avgPathLength/_avgPLEstimationOfBST));

	anomalyScore.push_back(AscoreEstimated);
}

long double iforest::avgPathLengthEstimationOfBST(int size){
	long double avgPathLengthEstimationOfBST = 0;
	if(size == 2){avgPathLengthEstimationOfBST=1;}
	else if(size > 2){avgPathLengthEstimationOfBST = (2 * (log(size-1) + 0.5772156649)) - (2 * (size - 1) / size);}	
	return (avgPathLengthEstimationOfBST);
}

long double iforest::avgPathLengthComputationOfBST(){
	long double avgPathLengthComputationOfBST = 0;
	for(int treeId = 0; treeId < _numiTrees; treeId++){
		avgPathLengthComputationOfBST += _iTrees[treeId]->avgPathLengthComputationOfBST();
	}
	avgPathLengthComputationOfBST /=_numiTrees;
	return avgPathLengthComputationOfBST;
}


long double iforest::computeAvgPathLength(int pointX, const data & testDataObject){
	long double avgPathLength = 0;
	for(int treeId = 0; treeId < _numiTrees; treeId++){
		avgPathLength += _iTrees[treeId]->computePathLength(pointX, testDataObject);
	}
	avgPathLength /=_numiTrees;
	return avgPathLength;
}




