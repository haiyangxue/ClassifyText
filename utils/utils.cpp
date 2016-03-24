#include "utils.h"

/*
 *parameter: cfgfilepath 文件的绝对路径名如: /user/home/my.cfg
 *           key         文本中的变量名
 *           value       对应变量的值，用于保存
 */
bool PropUtil::getConfig(const char * cfgfilepath, const string & key, string & value)
{
    fstream cfgFile;
    cfgFile.open(cfgfilepath);//打开文件
    if( ! cfgFile.is_open())
    {
        cout<<"can not open cfg file!"<<endl;
        return false;
    }
    char tmp[1000];
    while(!cfgFile.eof())//循环读取每一行
    {
        cfgFile.getline(tmp,1000);//每行读取前1000个字符，1000个应该足够了
        string line(tmp);
        size_t pos = line.find('=');//找到每行的“=”号位置，之前是key之后是value
        if(pos==string::npos) return false;
        string tmpKey = line.substr(0,pos);//取=号之前
        if(key==tmpKey)
        {
            value = line.substr(pos+1);//取=号之后
            return true;
        }
    }
    return false;
}


void split(const string &src, const string &separator, vector<string> &dest) {
    string str = src;
    string substring;
    string::size_type start = 0, index;

    do {
        index = str.find_first_of(separator, start);
        if (index != string::npos) {
            substring = str.substr(start, index - start);
            dest.push_back(substring);
            start = str.find_first_not_of(separator, index);
            if (start == string::npos) return;
        }
    } while (index != string::npos);

    //the last token
    substring = str.substr(start);
    dest.push_back(substring);
}


void getFiles(const char *path, map<string,string> & newslist, int level) {
    struct dirent *ent = NULL;
    DIR *pDir;
    pDir = opendir(path);

    if (pDir == NULL) {
        //被当作目录，但是执行opendir后发现又不是目录，比如软链接就会发生这样的情况。
        return;
    }
    while (NULL != (ent = readdir(pDir))) {
        if (ent->d_type == 8) {
            //file
//            for (int i = 0; i < level; i++) {
//                printf("-");
//            }
            //printf("%s/%s\n", path,ent->d_name);
            string str(ent->d_name);
            string con("/");
            string oldname =path +con+ str;
            ifstream fin(oldname.c_str());
            vector<string> temp;
            split(oldname,"/",temp);
            newslist[oldname]=temp[temp.size()-2];
          //  cout<<temp[temp.size()-1]<<endl;
//            for(vector<string>::iterator it=temp.begin();it<temp.end();it++){
//                cout<<*it<<" ";
//            }
//            cout<<endl;
//            outfile<<temp[temp.size()-1]<<"~*~";
//            while (!fin.eof()) {
//                char buffer[1024*10*6];
//                fin>>buffer;
//                outfile<<buffer;
//            }
//            outfile<<endl;
            fin.close();


        } else {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }
            //directory
            std::string _path(path);
            std::string _dirName(ent->d_name);
            std::string fullDirPath = _path + "/" + _dirName;
            for (int i = 0; i < level; i++) {
                printf(" ");
            }
            printf("%s\n", ent->d_name);
            getFiles(fullDirPath.c_str(),newslist, level + 1);
        }
    }

}


//int main(){
//    string str;
//    PropUtil p;
//    p.getConfig("/Users/liukaidingding/Documents/NLP/TextClassification/utils/test.prop","ip",str);
//    cout<<str<<endl;
//    map<string,string> newsList;
//    getFiles("/Users/liukaidingding/Documents/NLP/TextClassification/dict/corpus_wangyi_test",newsList, 0);
//    for(map<string,string>::iterator it =newsList.begin();it!=newsList.end();it++){
//        cout<<it->first<<" "<<it->second<<endl;
//    }
////cout<<newsList<<endl;
// //   outfile.close();
//    return 0;
//}