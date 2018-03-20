#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <fstream>
#include <unistd.h>
#include <map>
#include "tools.h"
#include "CompareMats.h"

using namespace std;
using namespace cv;

int str2int(string str);
void help();
void compare(int count,string reportPath,vector<string> paths);
void parse(int argc,char** argv,map<string,vector<string>> &args);

int main( int argc, char** argv )
{
    if(argc==1){
        help();
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
    auto count = str2int(args["-n"][0]);
    ASSERT(count>0,"要比对的图片组数数量小于1");

    //提取路径
    vector<string> paths = args["-p"];
//    for(int i=0;i<paths.size();i++){
//        cout<<paths[i]<<" ";
//    }
//    cout<<endl;

    compare(count,reportPath,paths);

    return 0;
}

void help(){
    printf("调用方法: ./ComparePicture -n number -s reportPath -p path0.txt path1.txt path2.txt\n"
                   "\n"
                   "-n number 要比对的图片数量;\n"
                   "-s save_path 指定比对报告保存路径;\n"
                   "-p path0.txt是原图相对于当前目录的图片路径，\n"
                   "一张图片的路径为一行;path1.txt和path2.txt\n"
                   "分别指定要比对的两组图片的路径。-p必须为最\n后一"
                   "个参数.\n"
                   "\n"
                   "调用demo:\n"
                   "./ComparePicture -n 8 -s ./report -p ./原图/src.txt "
                   "./图片组1/src.txt ./图片组2/src.txt\n\n\n"
    );
    printf("获取以上提到的txt文件，运行getSrcTxt.sh获得。\n\n");
    printf("生成脚本getSrcTxt.sh中:\n");
    ofstream o("getSrcTxt.sh");
    o<< "#!/usr/bin/env bash\n"
            "\n"
            "if [ $# = 0 ];then\n"
            "   echo '调用方法:./getSrcTxt.sh path type'\n"
            "   echo 'path:图片目录相对路径;type:图片类型比如bmp、jpeg等。'\n"
            "   echo '文件夹中的图片需要用数字命名，才能正确的排序。'\n"
            "else \n"
            "   if [ $# = 2 ];then\n"
            "       if [ ! -d \"$1\" ];then\n"
            "           echo \"不存在目录:$1\"\n"
            "           exit -1\n"
            "       fi\n"
            "\n"
            "       cd \"$1\"\n"
            "\n"
            "       if [ -f \"src.txt\" ];then\n"
            "           rm \"src.txt\"\n"
            "       fi\n"
            "\n"
            "       for a in $(ls *.$2|sort -t. -k1.1n);\n"
            "       do\n"
            "           echo \"$PWD/$a\" >>src.txt\n"
            "       done\n"
            "\n"
            "       cd ..\n"
            "   else\n"
            "       echo '参数不正确。如果路径中间有空格，请用在路径开头和结尾打上英文双引号。'\n"
            "   fi\n"
            "fi"<<endl;
    o.close();
    system("chmod +x getNames.sh");
    printf("生成完成\n");
}

/**
 * 解析参数
 * @param argc 参数个数
 * @param argv 参数数组
 * @param args 解析结果
 */
void parse(int argc,char** argv,map<string,vector<string>> &args){

    //检查参数数量
    ASSERT(argc==9,"参数数量不正确");

    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){

            if(strcmp(argv[i],"-n")==0){//count
                ASSERT(args.count("-n")==0,"参数-c重复");
                vector<string> value;
                value.emplace_back(argv[++i]);
                args.insert(make_pair("-n",value));
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
                ASSERT(value.size()==3,"输入路径不等于三个");
                args.insert(make_pair("-p",value));
                break;
            }
        }else{
            throw runtime_error("参数格式错误");
        }
    }

    if(args.size()<3){
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

string getRealPath(string path){
    char buffer[500];
    getcwd(buffer,500);
    return string(buffer)+"/"+path;
}

/**
 * 比对count组图片;
 * @param count 要比对图片的组数
 * @param txtName 各个文件夹中指定文件名的txt文件;
 * 得到这个文件可以通过运行以下命令：ls | sort -t. -k1.1n >src.txt
 * @param reportPath 比对报告保存路径
 * @param paths[0]原图路径，path[1]和path[2]指定要比对的路径
 */
void compare(int count,string reportPath,vector<string> paths){

    //初始化各个文件夹内指定图片名字的txt文件的输入流
    vector<ifstream> nameStreams;
    for(int i=0;i<paths.size();i++){
        nameStreams.emplace_back(ifstream(paths[i],ios::in));
        if(!nameStreams[i].is_open()){
            throw runtime_error("打开文件\""+paths[i]+"\"异常，请检查该文件是否存在");
        }
    }

    string name0,name1,name2;
    ofstream o(reportPath+"/统计.txt");
    for(int i=0;i<count;i++){

        getline(nameStreams[0],name0);
        getline(nameStreams[1],name1);
        getline(nameStreams[2],name2);

//        name0 = getRealPath(name0);
//        cout<<name0<<endl<<name1<<endl<<name2<<endl;

        Mat mat0 = imread(name0);
        Mat mat1 = imread(name1);
        Mat mat2 = imread(name2);

        ASSERT(!mat0.empty(), "不存在："+name0);
        ASSERT(!mat1.empty(), "不存在："+name1);
        ASSERT(!mat2.empty(), "不存在："+name2);

        CompareMats ci(mat0,mat1,mat2);
        cout<<"第"<<i+1<<":"<<ci.report()<<endl;
        o<<"第"<<i+1<<":"<<ci.report()<<endl;
        ci.saveReport(reportPath+"/"+to_string(i+1));
    }

    nameStreams[0].close();
    nameStreams[1].close();
    nameStreams[2].close();
}