#ifndef ITREE_H
#define ITREE_H
#include "data.h"
#include "treenode.h"
#include <boost/serialization/list.hpp>
#include <vector>

class itree
{
    public:
        itree(const data &);									//used for: create an empty tree to load the itree from file and insert new points in data object
        itree(const data &,int, int, int, int);					//used for: create a tree using data object and initialize
        virtual ~itree();
        void constructiTree();									//used for: construct itree split the nodes and grow the itree as per definition
		long double computePathLength(int, const data &);						//compute PL = height of point + adjaustment for unbuilt tree below max.height
		long double avgPathLengthComputationOfBST();			//compute avg PL for actual structure of iTree the sample in hand (Not estimation)
		long double pathLengthEstimationForUnbuiltTree(int );	//avg PL estimation for given size iTree(samples size)
		treenode * rootNode;									//address of root node of the tree
    protected:

    private:
	int _sampleSize;    										//|S|, S is set of points for which iTree is created
	int _maxTreeHeight;											//log base 2 (|S|), max possible height of the iTree
    int _maxNumOfNodes;											//2|S|-1
	int _avgPLEstOfBST;											//estimation of avg height of a iTree
	int _avgPLCompOfBST;										//actual avg height of iTree
	const data & _dataObject;									//reference of the input dataObject, only a container not responsible for deletion of the object.
    

    /*friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & maxTreeHeight;
        ar & maxNumOfNodes;
        ar & BOOST_SERIALIZATION_NVP(pointToNode);
    }*/
};

#endif // ITREE_H
