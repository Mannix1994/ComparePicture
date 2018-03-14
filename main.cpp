#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <map>
#include "tools.h"
#include "CompareMats.h"

using namespace std;
using namespace cv;

int str2int(string str);
void compare(int count,string txtName,string reportPath,vector<string> paths);
void parse(int argc,char** argv,map<string,vector<string>> &args);

int main( int argc, char** argv )
{
    if(argc==1){
        printf("调用方法: ./ComparePicture -c count -t txtName -s reportPath -p path1 path2\n"
                       "\n"
                       "-c 要比对的图片数量;\n"
                       "-t path1和path2目录中含有目录中图片名字的txt文件名，每个目\n"
                       "录中的txt名字都需要相同;\n"
                       "-s 指定比对报告保存路径;\n"
                       "-p 指定两组图片的路径,必须为最后一个参数.\n"
                       "\n"
                       "调用demo:\n"
                       "./ComparePicture -c 8 -t src.txt -s ./reportPath -p ./图片组1 ./图片组2\n\n\n"
                       "按文件名数字排序命令：ls *.bmp | sort -t. -k1.1n >src.txt\n\n"
                       );
        return 0;
    }

    //解析参数
    map<string,vector<string>> args;
    parse(argc,argv,args);

    //删除旧目录，新建报告目录
    string reportPath = args["-s"][0];
//    cout<<reportPath<<endl;
#ifdef __linux__
    system(("rm -rf "+reportPath).c_str());
    system(("mkdir "+reportPath).c_str());
#elif defined(_WIN32)
    system(("rmdir /s /q "+reportPath).c_str());
    system(("md "+reportPath).c_str());
#else
#error Not soupport this system
#endif

    //提取参数
    auto count = str2int(args["-c"][0]);
    ASSERT(count>0,"合并的图片组数数量小于1");

    string txtName = args["-t"][0];
//    cout<<txtName<<endl;

    //提取路径
    vector<string> paths = args["-p"];
//    for(int i=0;i<paths.size();i++){
//        cout<<paths[i]<<" ";
//    }
//    cout<<endl;

    compare(count,txtName,reportPath,paths);

    return 0;
}

/**
 * 解析参数
 * @param argc 参数个数
 * @param argv 参数数组
 * @param args 解析结果
 */
void parse(int argc,char** argv,map<string,vector<string>> &args){

    //检查参数数量
    ASSERT(argc>9,"参数数量不足");

    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){

            if(strcmp(argv[i],"-c")==0){//count
                ASSERT(args.count("-c")==0,"参数-c重复");
                vector<string> value;
                value.emplace_back(argv[++i]);
                args.insert(make_pair("-c",value));
            }
            else if(strcmp(argv[i],"-t")==0){//txt name
                ASSERT(args.count("-t")==0,"参数-t重复");
                vector<string> value;
                value.emplace_back(argv[++i]);
                args.insert(make_pair("-t",value));
            }
            else if(strcmp(argv[i],"-s")==0){//save path
                ASSERT(args.count("-s")==0,"参数-s重复");
                vector<string> value;
                value.emplace_back(argv[++i]);
                args.insert(make_pair("-s",value));
            }
            else if(strcmp(argv[i],"-p")==0){//各组图片路径
                ASSERT(args.count("-p")==0,"参数-p重复");
                vector<string> value;
                for(int j=++i;j<argc;j++)
                    value.emplace_back(argv[j]);
                args.insert(make_pair("-p",value));
                break;
            }
        }else{
            throw runtime_error("参数格式错误");
        }
    }

    if(args.size()<4){
        throw runtime_error("参数数量不足");
    }
}

/**
 * 字符数组转数字
 * @param str 数字字符数组，例如 char str[]="123";
 * @return 转换成功返回对应数值123,转换失败会抛出异常退出程序
 */
long charArray2Long(char *str){
    char *stop;
    long num = strtol(str,&stop,10);
    if(strlen(stop)>0){
        printf("字符数组:%s含有非法字符\n",str);
        throw runtime_error("非法字符，转换为数字失败");
    }
    return num;
}

/**
 * 字符串转数字
 * @param str 数字字符串
 * @return 装换完成的数字
 */
int str2int(string str){
    stringstream stream(str);
    int num;
    stream>>num;
    if(!stream.eof()){
        throw runtime_error("数字"+str+"不正确，请检查参数");
    }
    return num;
}

/**
 * 合并count组图片;
 * @param count 要比对图片的组数
 * @param txtName 各个文件夹中指定文件名的txt文件;
 * 得到这个文件可以通过运行以下命令：ls | sort -t. -k1.1n >src.txt
 * @param reportPath 比对报告保存路径
 * @param paths 指定要比对的来那个个路径
 */
void compare(int count,string txtName,string reportPath,vector<string> paths){

    ASSERT(paths.size()==2,"指定的路径数量超过m*n");

    //初始化各个文件夹内指定图片名字的txt文件的输入流
    vector<ifstream> nameStreams;
    for(int i=0;i<paths.size();i++){
        nameStreams.emplace_back(ifstream(paths[i]+"/"+txtName,ios::in));
        if(!nameStreams[i].is_open()){
            throw runtime_error("打开文件\""+paths[i]+"/"+txtName+"\"异常，请检查该文件是否存在");
        }
    }

    string name1,name2;
    ofstream o(reportPath+"/统计.txt");
    for(int i=0;i<count;i++){
        nameStreams[0]>>name1;
        nameStreams[1]>>name2;
        string path1=paths[0]+"/"+name1;
        string path2=paths[1]+"/"+name2;
//        cout<<path1<<path2<<endl;
        Mat mat1 = imread(path1);
        Mat mat2 = imread(path2);
        CompareMats ci(mat1,mat2);
        cout<<"第"<<i+1<<":"<<ci.report()<<endl;
        o<<"第"<<i+1<<":"<<ci.report()<<endl;
        imwrite(reportPath+"/"+to_string(i+1)+".bmp",ci.mask());
    }

    nameStreams[0].close();
    nameStreams[1].close();
}