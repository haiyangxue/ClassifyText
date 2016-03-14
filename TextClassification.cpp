#include "PreProcess.h"


int main(int argc, char **argv) {
    int beginIndex=1;
    int endIndex=1351;
    PreProcess p(beginIndex,endIndex);
    clock_t start;
    start = clock();
    vector<string>labels;
    labels.push_back("transport");//交通
    labels.push_back("sports");//体育
    labels.push_back("military");//军事
    labels.push_back("medicine");//医药
    labels.push_back("economic");//经济
    labels.push_back("art");//艺术
    vector<string> news;
    vector<string> words;
    string result;
    map<string,vector<int> > articleIdinEachClass;
    map<int,string> idToLable;
    p.preProcessedText(labels,TRAININGNEWS_PATH,"*~*",news,articleIdinEachClass,idToLable);


    unordered_set<string> stopwords = p.MakeStopSet();
    DICTIONARY mymap;
    p.CutWords(news,stopwords,mymap);

    CONTINGENCY contigencyTable;
    p.GetContingencyTable(mymap,contigencyTable,articleIdinEachClass);
    vector<string> keywords= p.ChiSquareFeatureSelection(labels,mymap,contigencyTable,articleIdinEachClass,1000);
    //为训练集建立VSM模型
    DOCMATRIX trainingSetVSM;
    p.VSMConstruction(mymap,trainingSetVSM,keywords);
    //为测试集建立VSM模型
    map<int,string> idToLable2;//for testing
    p.preProcessedText(labels,TESTINGNEWS_PATH,"*~*",news,articleIdinEachClass,idToLable2);
    DOCMATRIX testingSetVSM;
    p.VSMConstructionForTest(news,stopwords,mymap,testingSetVSM,keywords);
    RESULTINFO  classifyResults;
    p.KNNclassifier(trainingSetVSM,testingSetVSM,labels,100,idToLable,classifyResults);
    cout<<classifyResults<<endl;
    clock_t finish = clock();
    double totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "对篇章分词的时间为" << totaltime << endl;
    return EXIT_SUCCESS;
}