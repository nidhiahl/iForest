#include "data.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>


data::data()
{
    //ctor
}

data::~data()
{
    //dtor
}



/**************Getter methods*****************/


void data::setnumInstances(int numInstances){
    this->numInstances = numInstances;
}

void data::setnumAttributes(int numAttributes){
    this->numAttributes = numAttributes;
}

/**************Setter methods*****************/


int data::getnumInstances()const{
    return this->numInstances;
}

int data::getnumAttributes() const {
    return this->numAttributes;
}

vector<point*> data::getDataVector() const {
    const vector<point*> & refDataVector = this->dataVector;
    return dataVector;
}


/*******takes complete dataset File as input************/

void data::createDataVector(const string & dataFile){
	ifstream inDataFile(dataFile.c_str(),ios::in|ios::binary) ;
	if(!inDataFile){
		cout<<"Can not open input data file: "<<dataFile<<endl;
		exit(0);
	}
	inDataFile>>numInstances>>numAttributes;
	dataVector.resize(numInstances);
	for(int instance = 0; instance < numInstances; instance++){
		point *newPoint = new point();
		newPoint->attributes = new double[numAttributes];
		int id;
		inDataFile>>id;
		for(int attribute = 0; attribute < numAttributes; attribute++){
			inDataFile>>newPoint->attributes[attribute];
		}
		
		inDataFile>>newPoint->label;
		newPoint->isPresent = bool(1);
		dataVector[id] = newPoint;
	}
	inDataFile.close();
}

//***************************************Select sample from the original dataset i.e for static run****************************************//
vector<int> & data::getSample(int sampleSize) const {
	vector<int>* sample = new vector<int>;            
	vector<int> &refSample = *sample;
	int cnt = 0;
	for(int i = 0; i < numInstances; i++){
		if(dataVector[i]->isPresent){
			(*sample).push_back(i);
			cnt++;
		}
	}
	std::random_shuffle((*sample).begin(),(*sample).end());
	(*sample).erase((*sample).begin()+sampleSize,(*sample).begin()+cnt);
	return refSample;
}
