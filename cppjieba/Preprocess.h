#ifndef _Preprocess_H
#define  _Preprocess_H

#include"common.h"

/************************************************************************/
/* WkaPreprocess类完成如下功能
将文本集合分词-》去停用词-》建立词袋子模型=》特征词选择=》对文章建立VSM模型=
》写成weka数据格式（arff）-》输出聚类信息                                    */
/************************************************************************/
//一些谓词函数
using namespace std;

class Preprocess
{

private:

	int beginIndex;//开始聚类的文章id
	int endIndex;//结束聚类的文章id
public:
	typedef vector<string>(Preprocess::*FUNCSEG)(string,set<string>);
	Preprocess(int beginIndex,int endIndex)
	{

		this->beginIndex=beginIndex;
		this->endIndex=endIndex;




	}
	/*Preprocess()
    {

    }*/

	~Preprocess()
	{



	}
	void trim(string  &str,const string val);//去除字符串首尾空白
	//构建倒排表： key=word,val= a list of pairs which consists of articleid,and count, count=tf
	int ConstructDictionary(DICTIONARY& mymap,FUNCSEG seg,string tablename);
	//保存词袋子到硬盘
	void SaveDictionary(DICTIONARY& mymap,char *address);
	//从内存中加载词袋子模型
	void LoadDictionary(DICTIONARY& mymap,char *address);
	//打印词袋子模型
	void print(DICTIONARY& mymap);
	//窄字符串转化成宽字符串
	wstring myMultibyteToWideChar(string sResult);
	//宽字符串转化成窄字符串
	string myWideCharToMultibyte(wstring wsResult);
	//调用ICTclass分词
	string ICTsplit(const char *sInput);
	//构造停用词表
	set<string>MakeStopSet();
	//去除停用词，噪声词
	vector<string>goodWordsinPieceArticle(string rawtext,set<string> stopwords);
	//整数转化成字符串
	string do_fraction(int val);
	//浮点数转化成字符串
	string do_fraction(double val, int decplaces=5);
	//特征词选择算法
	void DFcharicteristicWordSelection(DICTIONARY& mymap,int DFthreshold,char * address);
	//获得数据库中每个类别的文章ID
	//map<string,vector<int> > GetArticleIdinEachClass(vector<string> classLabels);
	//获得每个词的contingencytable
	int GetContingencyTable(DICTIONARY& mymap, vector<string> classLabels,CONTINGENCY& contingencyTable,string tablename);
	//计算卡方值 改成double以免溢出
	double CalChiSquareValue(double N11,double N10,double N01,double N00);//计算卡方值
	double CalInformationGain(double N11,double N10,double N01,double N00);//计算IG值
	double CalPointWiseMI(double N11,double N10,double N01,double N00);//计算pointwise MI;
	//用Chi方法对分别计算词袋子模型中的词对每个类别的卡方值
	vector<pair<string,double> >ChiSquareFeatureSelectionForPerclass(DICTIONARY& mymap,CONTINGENCY& contingencyTable,string classLabel);
	vector<pair<string,double> >InformationGainFeatureSelectionForPerclass(DICTIONARY& mymap,CONTINGENCY& contingencyTable,string classLabel);
	vector<pair<string,double> >PointWiseMIFeatureSelectionForPerclass(DICTIONARY& mymap,CONTINGENCY& contingencyTable,string classLabel);
	vector<pair<string,double> >LocalDFFeatureSelectionForPerclass(DICTIONARY& mymap,CONTINGENCY& contingencyTable,string classLabel);//局部DF法针对每个词对每个类别进行排序
	void DFFeatureSelection(vector<string> classLabels,DICTIONARY &mymap,CONTINGENCY& contingencyTable,int N,char *address);//调用局部DF特征词选择函数
	void ChiSquareFeatureSelection(vector<string > classLabels,DICTIONARY& mymap,CONTINGENCY& contingencyTable,int N,char * address);
	void InformationGainFeatureSelection(vector<string > classLabels,DICTIONARY& mymap,CONTINGENCY& contingencyTable,int N,char * address);
	void PointWiseMIFeatureSelection(vector<string > classLabels,DICTIONARY& mymap,CONTINGENCY& contingencyTable,int N,char * address);

	//获取最后的特征词
	vector<string> GetFinalKeyWords(char* address);
	//获取特征词的maxTF，DF
	vector<pair<int,int> >GetfinalKeysMaxTFDF(DICTIONARY& mymap,char *address);
	//文档向量模型规范化
	vector<pair<int,double> > NormalizationVSM(vector<pair<int,double> > tempVSM);
	int VSMConstruction(DICTIONARY& mymap,DOCMATRIX& traingsetVSM,char* keywordsaddress);
	/**计算向量的内积*****************8*/
	double CalDotProductOfVectors(const vector<double>&vector1,const vector<double>&vector2);
	/************计算余弦相似度*******/
	double CalCosineofVectors(const vector<double>&vector1,const vector<double>&vector2);
	vector<string> mySplit(string s,set<string> stopwords);//分割关键词
	/********************************************************************************************************8*****/
	int GetArticleIdinEachClass(vector<string > labels,string tablename,map<string,vector<int> >&articleIdinEachClass );
	vector<string >GetClassification(string articleIds,string tablename);//获得该篇文章对应的类别
	string GetCategorizationInfoById(int articleId,string tablename);
	int GetManyVSM(int begin,int end,string tablename,DICTIONARY& mymap,DOCMATRIX& testingsetVSM,char *keywordsaddress);//获得待分类文档的VSM模型
	string KNNClassificationCell(int N,string tablename,vector<double> vsm,vector<string>catigorization,DOCMATRIX& trainingsetVSM);//KNN分类
	//KNN分类
	void KNNclassifier(string tablename,DOCMATRIX& trainingsetVSM,DOCMATRIX& testingsetVSM,vector<string>catigorization,int N,RESULTINFO& classifyResults);
	void SaveContingencyTable(CONTINGENCY& contingencyTable,char *address);
	void LoadContingencyTable(CONTINGENCY& contingencyTable,char *address);
	void SaveVSM(DOCMATRIX& VSMmatrix,char *dest);
	void LoadVSM(DOCMATRIX& VSMmatrix,char *dest);
	/**********************************************************获得分类评价指标*********************************************************/
	double getTP(string classLabel,RESULTINFO classifyResults,string tablename);//
	double getCategorizationNum(string classLabel,string tablename);
	//计算分类准确率
	double getPrecision(string classLabel,RESULTINFO classifyResults,string tablename);
	//计算召回率
	double getRecall(string classLabel,RESULTINFO classifyResults,string tablename);
	//计算F值
	double getFscore(string classLabel,RESULTINFO classifyResults,string tablename);








};



#endif
