#ifndef DATA_H
#define DATA_H
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

typedef struct point{
    double* attributes;
    bool isPresent;
    int label;
}point;

class data
{
    public:
        data();
        virtual ~data();
        void createDataVector(const string &);
        vector<int> & getSample(int) const ;
        vector<point*>  getDataVector() const ;
        int getnumInstances() const;
        void setnumInstances(int);
        int getnumAttributes() const;
        void setnumAttributes(int);
        
        vector<point*> dataVector;
        
    protected:

    private:
    int numInstances;
    int numAttributes;
};

#endif // DATA_H
