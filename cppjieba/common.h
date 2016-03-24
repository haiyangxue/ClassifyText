#ifndef _COMMON_H
#define _COMMON_H

#include<iostream>
#include<map>
#include<set>
#include<vector>
#include<string>
#include<iomanip>
#include<fstream>
#include<algorithm>
#include<cmath>
#include<sstream>
#include<limits>
#include"boost/algorithm/string.hpp"
using namespace std;
typedef map <string,vector<pair<int,int> > > DICTIONARY;//定义字典数据结构
typedef map<pair<string,string>,pair<int,int> >   CONTINGENCY;//定义关联表数据结构
typedef map<int,vector<double> >  DOCMATRIX;//文档向量矩阵
typedef vector<pair<int,string> > RESULTINFO;//最后的分类和聚类结果信息


#endif