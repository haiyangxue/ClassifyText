#ifndef _PreProcess_H
#define _PreProcess_H

#include "cppjieba/KeywordExtractor.hpp"

using namespace std;

typedef map<string, vector<pair<int, int> > > DICTIONARY;
//定义字典数据结构
typedef map<pair<string, string>, pair<int, int> > CONTINGENCY;
//定义关联表数据结构
typedef map<int, vector<double> > DOCMATRIX;
//文档向量矩阵
typedef vector<pair<int, string> > RESULTINFO;//最后的分类和聚类结果信息

const char *const DICT_PATH = "dict/jieba.dict.utf8";
const char *const TRAININGNEWS_PATH = "dict/corpus.txt";
const char *const TESTINGNEWS_PATH = "dict/test.txt";
const char *const NEWSOUT_PATH = "dict/10train.txt";
const char *const HMM_PATH = "dict/hmm_model.utf8";
const char *const USER_DICT_PATH = "dict/user.dict.utf8";
const char *const IDF_PATH = "dict/idf.utf8";
const char *const STOP_WORD_PATH = "dict/stop_words.utf8";
const char *const KEYWORDS_PATH = "dict/keywords";


class PreProcess {
private:
    int beginIndex;
    //开始聚类的文章id
    int endIndex;//结束聚类的文章id
public:
    PreProcess() {

    }

    PreProcess(int beginIndex, int endIndex) {
        this->beginIndex = beginIndex;
        this->endIndex = endIndex;
    }

    ~PreProcess() {

    }

    unordered_set<string> MakeStopSet();

    //构造停用词表
    void ConstructDictionary(DICTIONARY &mymap, vector<string> wordcollection, int articleid);

    //分词
    void CutWords(vector<string> news, unordered_set<string> stopwords, DICTIONARY &mymap);

    //建立词袋子模型
    void SaveDictionary(DICTIONARY &mymap, const char *address);

    //保存词袋子模型到硬盘
    void LoadContingencyTable(CONTINGENCY &contingencyTable, char *address);

    //将关联表信息从硬盘加载到内存
    void LoadDictionary(DICTIONARY &mymap, const char *address);

    //加载词典信息到内存
    int GetContingencyTable(DICTIONARY &mymap, CONTINGENCY &contigencyTable,
                            map<string, vector<int> > &articleIdsEachClass);

    //获得每个词的ContingencyTable
    void SaveContingencyTable(CONTINGENCY &contingencyTable, char *address);

    //将关联表保存到本地硬盘
//    int GetArticleIdinEachClass(vector<string> labels, map<string, vector<int> > &articleIdinEachClass);

    //文本预处理:得到词在哪些文章中..
    void preProcessedText(vector<string> lables, const char *corpusPath, const string &connector,
                          vector<string> &news, map<string, vector<int> > &articleIdinEachClass,map<int,string> &idToLable);

    //计算CHI-square 值
    double CalChiSquareValue(double N11, double N10, double N01, double N00);

    //计算词袋子中的每一个词对某一类别的卡方值
    vector<pair<string, double> > ChiSquareFeatureSelectionForPerclass(DICTIONARY &mymap,
                                                                       CONTINGENCY &contingencyTable,
                                                                       vector<string> &tempvector, string classLabel);

    //卡方特征词选择算法
    vector<string> ChiSquareFeatureSelection(vector<string> classLabels, DICTIONARY &mymap,
                                             CONTINGENCY &contingencyTable,
                                             map<string, vector<int> > &articleIdinEachClass, int N);


    // 获得特征词的maxTF,DF
    vector<pair<int, int> > GetfinalKeysMaxTFDF(DICTIONARY &mymap, vector<string> &myKeys);

    // 文档向量模型归一化
    vector<pair<int, double> > NormalizationVSM(vector<pair<int, double> > tempVSM);

    //建立文档向量模型
    int VSMConstruction(DICTIONARY &mymap, DOCMATRIX &trainingsetVSM, vector<string> &myKeys);

    //获得待分类文档集合的VSM模型
    int VSMConstructionForTest(vector<string> news, unordered_set<string> stopwords, DICTIONARY &mymap,
                               DOCMATRIX &testingsetVSM, vector<string> keywords);

    //计算向量内积
    double CalDotProductOfVectors(const vector<double> &vector1, const vector<double> &vector2);

    //计算向量点积
    double CalCosineofVectors(const vector<double> &vector1, const vector<double> &vector2);

    // 对一篇文章分类获取其类别标签   N为KNN中的N的取值
    string KNNClassificationCell(int N, map<int,string> &idToLable,vector<double> vsm, vector<string> categorization,
                                 DOCMATRIX &trainingsetVSM);

    // KNN分类器
    void KNNclassifier(DOCMATRIX &trainingsetVSM, DOCMATRIX &testingsetVSM,
                       vector<string> catigorization,int N,map<int,string> &idToLable, RESULTINFO &classifyResults);
};

#endif