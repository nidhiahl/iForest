# iForest
Training an isolation forest (iforest), and evalation for anomaly detection task.

Our c++ implementation of iforest takes input data file (format of input file is described below) and returns list of anomlay scores for all the points in input file.

**Input file format:**
First line has two values separated by a blank space, first is "number of instances=N" and second is "number of dimensions=d". Each of the following row corresponds to a data instance (or data point or data object). There are total d+2 columns in the input file. First Column is the index associated with the data instance and last column is the actual label/class of the data instance.
**Output file format:**
There are N rown and three columns in the outfile. First column is the index associated with the data point (same as first column in input file). Second column is the anomlay score calculted for each point using the iforest implementation. Third column is the actual label of the data point (smae as last column of the input file). Our implementation automatically stores the outputfile at location "anomalyScore/"inputfile_name"

Runner file:
"iForest/run_iF_for_AD.cpp" is the runner file to get anomlay score associated the the given dataset. It takes 4 inputs, i) datafile path ii) numbre of iTrees in Iforest (input parameter) iii) sampling factor (input parameter) and minimum sample size.

Compilation: 
> g++ run_iF_for_AD.cpp
Run:
> ./a.out dataFiles/"inputfile_name" 100 0.1 256
