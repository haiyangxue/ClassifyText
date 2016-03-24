#ifndef TEXTCLASSIFICATION_UTILS_H
#define TEXTCLASSIFICATION_UTILS_H

#include <iostream>
#include <string>
#include "string.h"
#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class PropUtil {
private:

public:
    PropUtil(){
    }
    ~PropUtil(){

    }
    /*
    *parameter: cfgfilepath 文件的绝对路径名如: /user/home/my.cfg
    *           key         文本中的变量名
    *           value       对应变量的值，用于保存
    */
    bool getConfig(const char * cfgfilepath, const string & key, string & value);


};

void split(const string &src, const string &separator, vector<string> &dest);

void getFiles(const char *path, map<string,string> & newslist, int level);


#endif //TEXTCLASSIFICATION_UTILS_H
