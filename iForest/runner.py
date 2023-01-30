import sys, os
from unittest import result
import pandas as pd
import numpy as np
import csv
import matplotlib.pyplot as plt
from sklearn import metrics

import numpy as np

class OnlineVariance(object):
    """
    Welford's algorithm computes the sample variance incrementally.
    """

    def __init__(self, iterable=None, ddof=1):
        self.ddof, self.n, self.mean, self.M2 = ddof, 0, 0.0, 0.0
        if iterable is not None:
            for datum in iterable:
                self.include(datum)

    def include(self, datum):
        self.n += 1
        self.delta = datum - self.mean
        self.mean += self.delta / self.n
        self.M2 += self.delta * (datum - self.mean)

    @property
    def variance(self):
        return self.M2 / (self.n - self.ddof)

    @property
    def std(self):
        return np.sqrt(self.variance)


runs = 10
dataset = str(sys.argv[1])
numTrees = 100
samplingFactor = 0.01
minSampleSize = 256

avg_dpTime=0.0
avg_ifTime=0.0
avg_adTime=0.0
avg_iFMemUsed=0.0


precision=0.0
recall=0.0
fpr=0.0
# fnr=0.0
accuracy=0.0
f1score=0.0

avg_anomaly_rank=0.0
auroc = 0.0
auprc = 0.0

os.system('g++ ./main.cpp ./data.cpp  ./iforest.cpp ./itree.cpp ./treenode.cpp -lboost_serialization')
os.system("rm -f "+ "results/" +dataset[10:-4]+'_result.csv')


for run in range(1, runs+1): 
    
    tot_negatives=0
    tot_positives=0
    true_positives=0
    false_positives=0   

    os.system('./a.out '+dataset+' '+str(numTrees)+' '+str(samplingFactor)+' '+str(minSampleSize)+' >> results/'+dataset[10:-4]+'_result.csv')

    data = pd.read_csv('anomalyScores/'+dataset[10:-4]+'.csv', delimiter=' ')
    point_ids = data['pointId'].to_numpy()
    ascores = data['Ascore'].to_numpy()
    actual_labels = data['actuallabel'].to_numpy()

    for label in actual_labels:
        if label == 0:
            tot_negatives+=1
        else:
            tot_positives+=1


    suspected_anomalies=np.flip(np.argsort(ascores))

    stddev1 = [0.0]
    ov1 = OnlineVariance(ddof=0)
    for i in suspected_anomalies:
        ov1.include(ascores[i])
        stddev1.append(ov1.std)
        # print(ov1.std)
        # print(ascores[suspected_anomalies[i]])

    stddev2 = [0.0]
    ov2 = OnlineVariance(ddof=0)
    for i in reversed(suspected_anomalies):
        ov2.include(ascores[i])
        stddev2.append(ov2.std)
        # print(ov2.std)

    mn=999999.0
    threshold=0.0
    anomalies = 0
    for i in range(1, len(stddev1)-1):
        if(mn>abs(stddev1[i] - stddev2[len(stddev1)-i-1])):
            mn=abs(stddev1[i] - stddev2[len(stddev1)-i-1])
            threshold = (ascores[suspected_anomalies[i-1]]+ascores[suspected_anomalies[i]])/2
            anomalies = i

    print(threshold)
    print(anomalies)

    for rank in range(0, anomalies):
        if actual_labels[suspected_anomalies[rank]] == 1:
            true_positives+=1
        else :
            false_positives+=1

    true_negatives = tot_negatives - false_positives
    false_negatives = tot_positives - true_positives  

    # print("tot positives : ", tot_positives)
    # print("true positives : ", true_positives)
    # print("false positives : ", false_positives)
    # print("tot negatives :", tot_negatives)
    # print("true negatives : ", true_negatives)
    # print("false negatives : ", false_negatives)  

    # print((true_positives/(true_positives+false_positives)))
    # print((true_positives/tot_positives))
    # print()

    precision = precision + ((true_positives/(true_positives+false_positives)) - precision)/run
    recall = recall + ((true_positives/tot_positives) - recall)/run
    fpr = fpr + ((false_positives/tot_negatives) - fpr)/run

    accuracy = accuracy + (((true_positives+true_negatives)/(tot_positives+tot_negatives)) - accuracy)/run
    f1score = f1score + (((2*true_positives)/(2*true_positives+false_positives+false_negatives)) - f1score)/run

    s=0
    for rank in range(0, len(suspected_anomalies)):
        if actual_labels[suspected_anomalies[rank]] == 1:
            s+=rank+1

    avg_anomaly_rank = avg_anomaly_rank + ((s/tot_positives) - avg_anomaly_rank)/run

    #aucroc 
    fprs, tprs, thresholds = metrics.roc_curve(actual_labels, ascores, pos_label=1)
    auroc = auroc + (metrics.auc(fprs, tprs) - auroc)/run

    # plt.plot(fprs, tprs, marker='.', label='iForest')
    # plt.xlabel('False Positive Rate')
    # plt.ylabel('True Positive Rate')
    # plt.legend()
    # plt.show()


    # aucprc
    precisions, recalls, thresholds = metrics.precision_recall_curve(actual_labels, ascores, pos_label=1)
    auprc = auprc + (metrics.auc(recalls, precisions) - auprc)/run

    # plt.plot(recalls, precisions, marker='.', label='iForest')
    # plt.xlabel('Recall')
    # plt.ylabel('Precision')
    # plt.legend()
    # plt.show()





results = pd.read_csv("results/"+dataset[10:-4]+'_result.csv', delimiter=' ')
# print(results)

run=0
with open("results/" + dataset[10:-4]+'_result.csv', "r", encoding='ascii') as file:
    read = csv.reader(file)
    for row in read :
        run+=1
        avg_dpTime = avg_dpTime + (float(row[0].split(' ')[1]) - avg_dpTime)/run
        avg_ifTime = avg_ifTime + (float(row[0].split(' ')[3]) - avg_ifTime)/run
        avg_adTime = avg_adTime + (float(row[0].split(' ')[5]) - avg_adTime)/run
        avg_iFMemUsed = avg_iFMemUsed + (float(row[0].split(' ')[7]) - avg_iFMemUsed)/run


print("dpTime : " + str(avg_dpTime) + "\tifTime : " + str(avg_ifTime) + "\tadTime : " + str(avg_adTime) + "\tiFMemUsed : " + str(avg_iFMemUsed))
print("precision : " + str(precision) + "\trecall : " + str(recall) + "\tfpr : " + str(fpr) + "\tfnr : " + str(1.0-recall))
print("accuracy : " + str(accuracy) + "\tf1_score : " + str(f1score) + "\tavg_anomaly_rank : " + str(avg_anomaly_rank))
print("auroc : " + str(auroc) + "\tauprc : " + str(auprc))

