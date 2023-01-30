#include "treenode.h"
#include <math.h>
#include <random>


treenode::treenode()
{
    splitAttribute = -1;
    isLeaf = bool(0);
    nodeId = -1;
    parentId = -1;
    lChildId = -1;
    rChildId = -1;
    nodeSize = 0;
    pathLengthEst = 0;
    dataPointIndices.resize(0);
    parentAdd = nullptr;
    lChildAdd = nullptr;
    rChildAdd = nullptr;

}

treenode::treenode(int nId): nodeId(nId)
{
    splitAttribute = -1;
    isLeaf = bool(0);
    parentId = nodeId == 0 ? 0 : (nodeId-1)/2;
    lChildId = -1;
    rChildId = -1;
	nodeHeight = (int)log2(nodeId+1)+1;
    nodeSize = 0;
    pathLengthEst = 0;
    dataPointIndices.resize(0);
    parentAdd = nullptr;
    lChildAdd = nullptr;
    rChildAdd = nullptr;

	
	
}

treenode::~treenode()
{
    //dtor
}


double treenode::splitInfoSelection(const data &dataObject){

    std::random_device random_seed_generator;
    std::mt19937_64 RandomEngine(random_seed_generator());

	double maxVal = -999999.0;
	double minVal = 999999.0;
	int attempts = 0;
	while(attempts < 10){
        splitAttribute = std::uniform_int_distribution<>(0, dataObject.getnumAttributes()-1)(RandomEngine);

		for( int i = 0; i < dataPointIndices.size(); i++){
        		if(maxVal < dataObject.dataVector[(dataPointIndices[i])]->attributes[splitAttribute]){
				maxVal = dataObject.dataVector[(dataPointIndices[i])]->attributes[splitAttribute];
			}
        		if(minVal > dataObject.dataVector[(dataPointIndices[i])]->attributes[splitAttribute]){
        			minVal = dataObject.dataVector[(dataPointIndices[i])]->attributes[splitAttribute];
        		}
        }
        attempts = attempts + 1;
        double dataDiff = maxVal - minVal;
		if(dataDiff >= 0.0000000000000001){
			attempts = 10;
		}
	}
	maximumVal = maxVal;
	minimumVal = minVal;

    return std::uniform_real_distribution<double> (minVal, maxVal)(RandomEngine);
}



void treenode::createLeftChild(){
	lChildAdd = new treenode(2*nodeId+1);
	lChildAdd->parentAdd = this;
	lChildId = lChildAdd->nodeId;
}

void treenode::createRightChild(){
	rChildAdd = new treenode(2*nodeId+2);
	rChildAdd->parentAdd = this;
	rChildId = rChildAdd->nodeId;
}

	
