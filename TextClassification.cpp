#include "PreProcess.h"

int main(int argc, char **argv) {
    int beginIndex=1;
    PropUtil prop;
    string value1;
    prop.getConfig("utils/test.prop","articleNum",value1);
    int endIndex=atoi(value1.c_str());
    string value2;
    prop.getConfig("utils/test.prop","testArticleNum",value2);
    int testArticle=atoi(value2.c_str());
    PreProcess p(beginIndex,endIndex);
    clock_t start;
    start = clock();
    vector<string>labels;

    vector<string> news;
    map<string,vector<int> > articleIdinEachClass;
    map<int,string> idToLable;
    map<string,string> newsList;
    getFiles("dict/corpus_wangyi",newsList, 0);
    p.preProcessedText(labels,newsList,news,articleIdinEachClass,idToLable);


    unordered_set<string> stopwords = p.MakeStopSet();
    DICTIONARY mymap;
    p.CutWords(news,stopwords,mymap);
    news.clear();
    CONTINGENCY contigencyTable;
    p.GetContingencyTable(mymap,contigencyTable,articleIdinEachClass);
    cout<<labels<<endl;
    vector<string> keywords= p.ChiSquareFeatureSelection(labels,mymap,contigencyTable,articleIdinEachClass,2000);
    contigencyTable.clear();
//    //为训练集建立VSM模型
    DOCMATRIX trainingSetVSM;
    p.VSMConstruction(mymap,trainingSetVSM,keywords);
//cout<<trainingSetVSM<<endl;
    p.SaveVSM(trainingSetVSM,"dict/vsm",idToLable,labels);
//    //为测试集建立VSM模型

    getFiles("dict/corpus_wangyi_test",newsList, 0);
    p.preProcessedText(newsList,news,articleIdinEachClass,idToLable);
    DOCMATRIX testingSetVSM;
    p.VSMConstructionForTest(testArticle,news,stopwords,mymap,testingSetVSM,keywords);
    p.SaveVSMForTest(testingSetVSM,"dict/vsm_test",idToLable,labels);
    mymap.clear();
    RESULTINFO  classifyResults;
    p.KNNclassifier(trainingSetVSM,testingSetVSM,labels,200,idToLable,classifyResults);
    clock_t finish = clock();
    double totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "对篇章分词的时间为" << totaltime << endl;
    return EXIT_SUCCESS;
}