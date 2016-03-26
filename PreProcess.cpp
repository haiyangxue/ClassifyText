#include "PreProcess.h"

using namespace std;

map<int,double>idfs;
class GT_cls {
public:
    GT_cls(string &s) : comparepart(s) { }

    bool operator()(const string &elem) {
        return elem == comparepart;

    }

private:
    string comparepart;
};

bool PredTF(const pair<int, int> &pair1, int articleId) {
    return pair1.first == articleId;

}

class PredTFclass {
private:
    const int m;
public:
    PredTFclass(int id) : m(id) { };

    bool operator()(const pair<int, int> &pair1) { return PredTF(pair1, m); };
};


bool isLarger(const pair<string, double> &pair1, const pair<string, double> &pair2) {
    return pair1.second > pair2.second;
}

bool isLarger2(const pair<int, double> &pair1, const pair<int, double> &pair2) {
    return pair1.second > pair2.second;
}

void trim(string &str, const string val) {
    str.erase(0, str.find_first_not_of(val));
    str.erase(str.find_last_not_of(val) + val.size());
}


/************************************************************************/
/* 构造停用词表                                                                     */
/************************************************************************/
unordered_set<string> PreProcess::MakeStopSet() {
    unordered_set<string> stopwordsSet;
    ifstream ifile(STOP_WORD_PATH);
    while (!ifile.eof()) {
        string temp;
        trim(temp, " ");
        ifile >> temp;
        stopwordsSet.insert(temp);
    }
    ifile.close();
    return stopwordsSet;
}

//对语料进行预处理，主要是提取出每条文本的类别和文本的内容

void PreProcess::preProcessedText(vector<string> &lables, map<string,string> &filesPath,
                                  vector<string> &news, map<string, vector<int> > &articleIdinEachClass,
                                  map<int, string> &idToLable) {
    set<string> lableset;
    news.clear();
    articleIdinEachClass.clear();
    int index = 0;
    for(map<string,string>::iterator it=filesPath.begin();it!=filesPath.end();it++){
        index++;
        ifstream fin((it->first).c_str());
        lableset.insert(it->second);

        string article;
        while (!fin.eof()) {
            char buffer[1024*10*6];
            fin>>buffer;
            article+=buffer;
        }
        fin.close();
        news.push_back(article);
        idToLable[index] = it->second;
        articleIdinEachClass[it->second].push_back(index);
    }
    for (set<string>::iterator it = lableset.begin(); it != lableset.end(); it++) {
        lables.push_back(*it);
    }
    filesPath.clear();
//    copy(lableset.begin(), lableset.end(), lables.begin());//

}

//对语料进行预处理，主要是提取出每条文本的类别和文本的内容

void PreProcess::preProcessedText( map<string,string> &filesPath,vector<string> &news, map<string, vector<int> > &articleIdinEachClass,
                                   map<int, string> &idToLable) {
    news.clear();
    articleIdinEachClass.clear();
    int index = 0;
//    ofstream ofile("dict/vsmmmmmm",ios::binary);

    for(map<string,string>::iterator it=filesPath.begin();it!=filesPath.end();it++){
        index++;
        ifstream fin((it->first).c_str());
        string article;
        while (!fin.eof()) {
            char buffer[1024*10*6];
            fin>>buffer;
            article+=buffer;
        }
//        ofile <<article<<endl;
        fin.close();
        news.push_back(article);
        idToLable[index] = it->second;
        articleIdinEachClass[it->second].push_back(index);
    }
//    ofile.close();
}

//分词并构建词袋模型
void PreProcess::CutWords(vector<string> news, unordered_set<string> stopwords, DICTIONARY &mymap) {
    cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH);
    int i = 0;
    vector<string> words;
    // 使用迭代器 iterator 访问值
    vector<string>::iterator v = news.begin();
    while (v != news.end()) {
        words.clear();
        i++;
        jieba.CutForSearch(*v, words);
        vector<string> wordcollection = limonp::Seg(words.begin(), words.end(), stopwords);
        ConstructDictionary(mymap, wordcollection, i);
        v++;
    }
}

/************************************************************************/
/* 建立词袋子模型,构造词典
 * 词典结构：map<string, vector<pair<int, int> > >
 * 键key对应词名;值value是该词出现在每篇文章中的次数的集合，pair<int,int>分别指文章id和在该文章中出现的次数*/
/************************************************************************/
void PreProcess::ConstructDictionary(DICTIONARY &mymap, vector<string> wordcollection, int articleid) {
    clock_t start, finish;
    double totaltime;
    start = clock();
    if (wordcollection.size() > 0)//如果不是空字段的话
    {
        for (vector<string>::iterator strit = wordcollection.begin(); strit != wordcollection.end(); strit++) {

            if (mymap[*strit].empty())//该词没有出现在字典中
            {
                map<int, int> mytemppair;
                mytemppair[articleid]=1;
                mymap[*strit]=mytemppair;
            }
            else//该词出现在词典中
            {
                //  map<int, int>cur=mymap[*strit];
                //   for (it = mymap[*strit].rbegin(); it != mymap[*strit].rend(); it++) {
                int cur=mymap[*strit][articleid];
                cur++;
                mymap[*strit][articleid]=cur;
//                    it[articleid]=++(it[articleid]);
//                    if (it->first == articleid) {
//                        it->second = ++(it->second);
//                        break;
//                    }
//                }
//                if (it == mymap[*strit].rend()) {
//                    pair<int, int> mytemppair = make_pair(articleid, 1);
//                    mymapp[*strit]=mytemppair;
//                }
            }
        }
    }

    finish = clock();
    totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "处理第" << articleid << "篇文章共有了" << totaltime << endl;
    wordcollection.clear();

}

/************************************************************************/
/* 获得每个词的ContingencyTable
 * ContingencyTable结构：map<pair<string, string>, pair<int, int> >
*顶层map的键值为词的(term Text,classLabel)
内层map的键值为类别名称
pair<int,int>的第一个int表示某一类别c中含有term t的文章数目,第二个int表示该类别中不含有term t的文章数目
*/
/************************************************************************/
int PreProcess::GetContingencyTable(DICTIONARY &mymap, CONTINGENCY &contigencyTable,
                                    map<string, vector<int> > &articleIdsEachClass) {
    clock_t start, finish;
    double totaltime;
    start = clock();
    //对于词袋子模型中的每个词
    for (map<string, map<int, int> >::iterator mymap_it = mymap.begin();mymap_it != mymap.end(); ++mymap_it) {
        if (mymap_it->first != "" || mymap_it->first != " ") {
            for (map<string, vector<int> >::iterator articleId_name = articleIdsEachClass.begin(); //对于每个类别
                 articleId_name != articleIdsEachClass.end(); articleId_name++) {
                int cntTheClass = int((articleId_name->second).size());//该类别共有文章数目
                int termInTheClass = 0;//该词在该类中出现的次数
                for (map<int, int>::iterator name_articleId_it = (mymap_it->second).begin();
                     name_articleId_it != (mymap_it->second).end(); name_articleId_it++) {
                    termInTheClass += count((articleId_name->second).begin(), articleId_name->second.end(),
                                            name_articleId_it->first);//name_articleId_it->first单词对应的文章id
                }
                int termAbsentInTheClass = cntTheClass - termInTheClass;
                pair<string, string> compoundKey = make_pair(mymap_it->first, articleId_name->first);
                pair<int, int> valueInfo = make_pair(termInTheClass, termAbsentInTheClass);
                contigencyTable[compoundKey] = valueInfo;
                termInTheClass = 0;//清空计数；
            }
        }
    }
    finish = clock();
    totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "建立contingencyTable的时间为" << totaltime << endl;

    return contigencyTable.size() > 0;
}


/************************************************************************/
/* 计算CHI-square 值                                                */
/************************************************************************/
double PreProcess::CalChiSquareValue(double N11, double N10, double N01, double N00) {
    double chiSquare = 0;
    chiSquare = (N11 + N10 + N01 + N00) * pow((N11 * N00 - N10 * N01), 2) /
                ((N11 + N01) * (N11 + N10) * (N10 + N00) * (N01 + N00));
    return chiSquare;
}

/************************************************************************/
/*计算词袋子中的每一个词对某一类别的卡方值
/************************************************************************/
vector<pair<string, double> > PreProcess::ChiSquareFeatureSelectionForPerclass(DICTIONARY &mymap,
                                                                               CONTINGENCY &contingencyTable,
                                                                               vector<string> &tempvector,
                                                                               string classLabel) {
    int N = endIndex - beginIndex + 1;//总共的文章数目
    vector<pair<string, double> > chisquareInfo;
    //计算卡方值
    for (vector<string>::iterator ittmp = tempvector.begin(); ittmp != tempvector.end(); ittmp++) {
        int N1 = int(mymap[*ittmp].size());
        pair<string, string> compoundKey = make_pair(*ittmp, classLabel);
        double N11 = double(contingencyTable[compoundKey].first);
        double N01 = double(contingencyTable[compoundKey].second);
        double N10 = N1 - N11;
        double N00 = N - N1 - N01;
        double chiValue = CalChiSquareValue(N11, N10, N01, N00);
        chisquareInfo.push_back(make_pair(*ittmp, chiValue));
    }
    //按照卡方值从大到小将这些词排列起来
    stable_sort(chisquareInfo.begin(), chisquareInfo.end(), isLarger);
    return chisquareInfo;
}
/************************************************************************/
/* 卡方特征词选择算法                                                                     */
/************************************************************************/
vector<string> PreProcess::ChiSquareFeatureSelection(vector<string> classLabels, DICTIONARY &mymap,
                                                     CONTINGENCY &contingencyTable,
                                                     map<string, vector<int> > &articleIdinEachClass, int N) {
    clock_t start, finish;
    double totaltime;
    int totalTraingingCorpus = endIndex - beginIndex + 1;//训练语料库总共的文章数目
//    cout<<endIndex<<" "<<beginIndex<<endl;
    cout << "totalTraingingCorpus:"<<totalTraingingCorpus << endl;
    set<string> finalKeywords;//存放最终遴选出的特征词
    vector<pair<string, double> > chisquareInfo;
    start = clock();
    vector<string> tempvector;//词袋子中的所有词
    for (map<string, map<int, int> >::iterator it = mymap.begin(); it != mymap.end(); ++it) {
        tempvector.push_back(it->first);
    }
    for (vector<string>::iterator it = classLabels.begin(); it != classLabels.end(); it++) {
        //训练语料库中某个类别的文章数目
        int N_subClassCnt = int(articleIdinEachClass[*it].size());
        //threshold决定每个类别遴选多少个特征词
        int threshold= N_subClassCnt * N / totalTraingingCorpus;
//        int threshold = N_subClassCnt;
//        cout << tempvector.size() << endl;
        chisquareInfo = ChiSquareFeatureSelectionForPerclass(mymap, contingencyTable, tempvector, *it);
        cout << threshold << "  " << *it << ":" << chisquareInfo.size() << endl;
        for (vector<pair<string, double> >::size_type j = 0; j < chisquareInfo.size() && j < threshold; j++) {
            finalKeywords.insert(chisquareInfo[j].first);
        }
        chisquareInfo.clear();
    }
    int finalKeyWordsCount = int(finalKeywords.size());
    ofstream outfile(KEYWORDS_PATH);
    for (set<string>::iterator it = finalKeywords.begin(); it != finalKeywords.end(); it++) {
        outfile << *it << endl;
    }
    outfile.close();
    cout << "最后共选择特征词" << finalKeyWordsCount << endl;
    finish = clock();
    totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "遴选特征词共用了" << totaltime << endl;
    vector<string> myKeys(finalKeywords.size());
    copy(finalKeywords.begin(), finalKeywords.end(), myKeys.begin());//
    return myKeys;
}

/************************************************************************/
/* 获得特征词的maxTF,DF                                                 */
/************************************************************************/
vector<pair<int, int> >PreProcess::GetfinalKeysMaxTFDF(DICTIONARY &mymap, vector<string> &mykeys) {
//    vector<pair<int, int> > maxTFandDF;
//    for (vector<string>::iterator it = mykeys.begin(); it != mykeys.end(); it++) {
//        int DF = mymap[*it].size();
//        int maxTF = 0;
//        for (vector<pair<int, int> >::iterator subit = mymap[*it].begin(); subit != mymap[*it].end(); subit++) {
//            if (subit->second > maxTF) {
//                maxTF = subit->second;
//            }
//        }
//        maxTFandDF.push_back(make_pair(maxTF, DF));
//        //find_if(mymap[*it].begin(),mymap[*it].end(),
//    }
//    return maxTFandDF;
}

/************************************************************************/
/* 文档向量模型归一化                                                                     */
/************************************************************************/
vector<pair<int, double> >PreProcess::NormalizationVSM(vector<pair<int, double> > tempVSM) {

    double sum = 0;
    for (vector<pair<int, double> >::iterator vsmit = tempVSM.begin(); vsmit != tempVSM.end(); ++vsmit) {
        sum += pow(vsmit->second, 2);
    }
    for (vector<pair<int, double> >::iterator vsmit = tempVSM.begin(); vsmit != tempVSM.end(); ++vsmit) {
        vsmit->second /= sqrt(sum);
    }
    return tempVSM;

}

/*****************以下函数辅助完成聚类功能*********************************************************************8**********************/
/************************************************************************/
/* 建立文档向量模型                                                                     */
/************************************************************************/
int  PreProcess::VSMConstruction(DICTIONARY &mymap, DOCMATRIX &trainingsetVSM, vector<string> &myKeys) {
    clock_t start, finish;
    double totaltime;
    start = clock();
    int corpus_N = 8000;

//    vector<pair<int, int> > maxTFandDF = GetfinalKeysMaxTFDF(mymap, myKeys);
    for (int i = beginIndex; i <= endIndex; i++) {
        vector<pair<int, double> > tempVSM;
        vector<double> tempVSM2;
        for (vector<string>::size_type j = 0; j < myKeys.size(); j++) {
//            double TF = (double) count_if(mymap[myKeys[j]].begin(), mymap[myKeys[j]].end(), PredTFclass(i));
            double TF=0;
            double IDF;
            TF=mymap[myKeys[j]][i];
//            for (map<int, int>::iterator mymap_it = mymap[myKeys[j]].begin();mymap_it != mymap[myKeys[j]].end(); ++mymap_it) {
//               if(mymap_it->first==i){
//                   TF=mymap_it->second;
//                   break;
//               }
//            }
            IDF=log(corpus_N/(mymap[myKeys[j]].size()+1));
//            if(i==1)
            idfs.insert(map<int,double>::value_type(j,IDF));
//            cout<<myKeys[j]<<endl;
//            if(TF!=0)
//                cout<<"TF:"<<j<<" "<<i<<" "<<TF<<endl;

//            cout<<mymap[myKeys[j]]<<endl;

//            cout<<TF<<" ";
//            TF = 0.5 + TF / (maxTFandDF[j].first);
//            double IDF = log((double) corpus_N / (maxTFandDF[j].second + 1));
            double TF_IDF = TF * IDF;
//            if(j==0|j==1)
//                cout<<myKeys[j]<<mymap[myKeys[j]].size()<<" "<<TF<<" "<<IDF<<" "<<TF_IDF<<endl;

            tempVSM.push_back(make_pair(j, TF_IDF));

        }

        if (!tempVSM.empty()) {
//            tempVSM = NormalizationVSM(tempVSM);
            //
            for (vector<pair<int, double> >::iterator it = tempVSM.begin(); it != tempVSM.end(); it++) {
                tempVSM2.push_back(it->second);
            }
            trainingsetVSM[i] = tempVSM2;
        }
        tempVSM.clear();
        tempVSM2.clear();
    }
    finish = clock();
    totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "为训练语料库集合建立VSM模型共用了" << totaltime << endl;

    return trainingsetVSM.size() > 0;
}

/************************************************************************/
/* 获得待分类文档集合的VSM模型                                            */
/************************************************************************/
int  PreProcess::VSMConstructionForTest(int corpus_N, vector<string> news, unordered_set<string> stopwords,
                                        DICTIONARY &mymap, DOCMATRIX &testingsetVSM, vector<string> keywords) {
    clock_t start, finish;
    double totaltime;
    start = clock();
    cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH);
    int i = 0;
    vector<string> words;
    // 使用迭代器 iterator 访问值
    vector<string>::iterator v = news.begin();
    int ii=0;
    while (v != news.end()) {
        ii++;
        cout<<"Test: "<<ii<<endl;
        i++;
        jieba.CutForSearch(*v, words);
        vector<string> wordcollection = limonp::Seg(words.begin(), words.end(), stopwords);//表示这篇文章的词
        //   vector<pair<int, int> > maxTFandDF = GetfinalKeysMaxTFDF(mymap, keywords);
        vector<pair<int, double> > tempVSM;
        vector<double> vsm;


        for (vector<string>::size_type j = 0; j < keywords.size(); j++) {
//            double TF = (double) count_if(mymap[myKeys[j]].begin(), mymap[myKeys[j]].end(), PredTFclass(i));
            double TF=0;
            double IDF;
            TF=count(wordcollection.begin(),wordcollection.end(),keywords[j]);
//            TF=mymap[keywords[j]][i];
//            for ( vector<pair<int, int>  >::iterator mymap_it = mymap[keywords[j]].begin();mymap_it != mymap[keywords[j]].end(); ++mymap_it) {
//                if(mymap_it->first==i){
//                    TF=mymap_it->second;
//                    break;
//                }
//
//            }
            IDF=idfs[j];

            double TF_IDF = TF * IDF;
            tempVSM.push_back(make_pair(j, TF_IDF));

        }




//        for (vector<string>::size_type j = 0; j < keywords.size(); j++) {
//            double TF = (double) count(wordcollection.begin(), wordcollection.end(), keywords[j]);
////            TF = 0.5 + (double) TF / (maxTFandDF[j].first);
////            TF *= log((double) corpus_N / maxTFandDF[j].second);
//
//            TF = 0.5 + TF / (maxTFandDF[j].first);
//            double IDF = log((double) corpus_N / (maxTFandDF[j].second + 1));
//            double TF_IDF = TF * IDF;
//
//            tempVSM.push_back(make_pair(j, TF_IDF));
//        }
        if (!tempVSM.empty()) {
//            tempVSM = NormalizationVSM(tempVSM);
            for (vector<pair<int, double> >::iterator it = tempVSM.begin(); it != tempVSM.end(); it++) {
                vsm.push_back(it->second);
            }
            testingsetVSM[i] = vsm;
        }
        v++;
        tempVSM.clear();
        words.clear();
    }

    finish = clock();
    totaltime = totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "为测试集合建立VSM模型共耗时" << totaltime << endl;
    return testingsetVSM.size() > 0;
}


/************************************************************************/
/*  将VSM模型序列化到本地硬盘                                                                    */
/************************************************************************/
void PreProcess::SaveVSM(DOCMATRIX& VSMmatrix,const char* dest, map<int,string> &idToLable,vector<string> &lables)
{
    map<string,int>mm;
    int index=0;
    vector<string>::iterator it;
    for(it=lables.begin();it!=lables.end();it++){
        index++;
        mm[*it]=index;
    }
//cout<<mm<<endl;
    clock_t start,finish;
    double totaltime;
    start=clock();
    ofstream ofile(dest,ios::binary);
    for(map<int,vector<double> >::iterator it=VSMmatrix.begin();it!=VSMmatrix.end();++it)
    {
        ofile<<mm[idToLable[it->first]]<<" ";
        vector<double>::iterator subit;
        int index =1;
        for(subit=(it->second).begin();subit!=(it->second).end();++subit)
        {
            ofile<<index<<":"<<*subit<<" ";
            index++;
        }
        ofile<<endl;
    }
    ofile.close();
    idToLable.clear();
    finish=clock();
    totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
    cout<<"将语料库集合的VSM模型为序列化到硬盘的时间为"<<totaltime<<endl;


}

/************************************************************************/
/*  将VSM模型序列化到本地硬盘                                                                    */
/************************************************************************/
void PreProcess::SaveVSMForTest(DOCMATRIX& VSMmatrix,const char* dest, map<int,string> &idToLable,vector<string> &lables)
{
    map<string,int>mm;
    int index=0;
    vector<string>::iterator it;
    for(it=lables.begin();it!=lables.end();it++){
        index++;
        mm[*it]=index;
    }
    clock_t start,finish;
    double totaltime;
    start=clock();
    ofstream ofile(dest,ios::binary);
    for(map<int,vector<double> >::iterator it=VSMmatrix.begin();it!=VSMmatrix.end();++it) {

        ofile << mm[idToLable[it->first]] << " ";
        vector<double>::iterator subit;
        int index = 1;
        for (subit = (it->second).begin(); subit != (it->second).end(); ++subit) {
            ofile << index << ":" << *subit << " ";
            index++;
        }
        ofile << endl;

    }
    ofile.close();
    finish=clock();
    totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
    cout<<"将语料库集合的VSM模型为序列化到硬盘的时间为"<<totaltime<<endl;


}


/**计算向量内积*/
double PreProcess::CalDotProductOfVectors(const vector<double> &vector1, const vector<double> &vector2) {
    double result = 0.0f;
    for (int i = 0; i < vector1.size(); i++)
        result += vector1[i] * vector2[i];
    return result;
}

/**计算向量点积*/
double PreProcess::CalCosineofVectors(const vector<double> &vector1, const vector<double> &vector2) {
    double numerator = CalDotProductOfVectors(vector1, vector2);
    double denominator = CalDotProductOfVectors(vector1, vector1) * CalDotProductOfVectors(vector2, vector2);
    denominator = sqrt(denominator);
    return numerator / denominator;
}

/************************************************************************/
/* 对一篇文章分类获取其类别标签   N为KNN中的N的取值                                      */
/************************************************************************/
string PreProcess::KNNClassificationCell(int N, map<int, string> &idToLable, vector<double> vsm,
                                         vector<string> categorization,
                                         DOCMATRIX &trainingsetVSM) {

    string classLabel;
    //map<int,vector<double> >trainingsetVSM=VSMConstruction(mymap);
    //vector<double>toBeClassifyDoc=GetSingleVSM(articleId,mymap);
    vector<pair<int, double> > SimilaritySore;//保存待分类样本与训练样本集的测试得分
    //计算相似度得分
    for (map<int, vector<double> >::iterator it = trainingsetVSM.begin(); it != trainingsetVSM.end(); it++) {
        double score = CalCosineofVectors(vsm, it->second);
        SimilaritySore.push_back(make_pair(it->first, score));

    }
    //将相似度运算结果从高到底排序
    stable_sort(SimilaritySore.begin(), SimilaritySore.end(), isLarger2);
    string articleIds;
    vector<string> labels;
    for (vector<pair<int, double> >::size_type j = 0; j < N; j++) {
        labels.push_back(idToLable[SimilaritySore[j].first]);
    }
    for (vector<string>::iterator it = labels.begin(); it != labels.end(); it++) {
        trim(*it, " ");
    }
    vector<pair<string, int> > vectorAssit;
    for (int i = 0; i < categorization.size(); i++) {
        int num = count_if(labels.begin(), labels.end(), GT_cls(categorization[i]));
        vectorAssit.push_back(make_pair(categorization[i], num));
    }
    stable_sort(vectorAssit.begin(), vectorAssit.end(), isLarger);
    return vectorAssit[0].first;
}

/************************************************************************/
/* KNN分类器                                                               */
/************************************************************************/
void PreProcess::KNNclassifier(DOCMATRIX &trainingsetVSM, DOCMATRIX &testingsetVSM, vector<string> catigorization,
                               int N, map<int, string> &idToLable, RESULTINFO &classifyResults) {
    clock_t start, finish;
    double totaltime;
    start = clock();

    //vector<pair<int,string>>classifyResults;
    pair<int, string> temp;
    for (map<int, vector<double> >::iterator it = testingsetVSM.begin(); it != testingsetVSM.end(); it++) {

        string label = KNNClassificationCell(N, idToLable, (it->second), catigorization, trainingsetVSM);
        int id = it->first;
        temp = make_pair(id, label);
        cout<<"文章标号"<<temp.first<<"归为类别"<<temp.second<<endl;

        classifyResults.push_back(temp);

    }
    finish = clock();
    totaltime = (double) (finish - start) / CLOCKS_PER_SEC;
    cout << "对测试集进行KNN分类的时间为" << totaltime << endl;
    //return classifyResults;
}