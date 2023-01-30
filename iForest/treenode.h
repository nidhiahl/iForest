#ifndef TREENODE_H
#define TREENODE_H
# include "data.h"
#include <vector>
#include <boost/serialization/list.hpp>


class treenode
{
    public:

        int nodeId;
        int parentId;
        int lChildId;
        int rChildId;
        treenode *parentAdd;
        treenode *lChildAdd;
        treenode *rChildAdd;
        vector<int> dataPointIndices;
        int splitAttribute;
        double splitValue;
        double minimumVal;
        double maximumVal;
        int nodeSize;   
        int nodeHeight;
		long double pathLengthEst;
        bool isLeaf;
        
		
        treenode();
        treenode(int);
        virtual ~treenode();
        double splitInfoSelection(const data &);
        void createLeftChild();
		void createRightChild();

};

#endif // TREENODE_H
