
## 一、比对图片
将n组图片两两比较，并输出比较结果。

## 二、如何使用

* 下载源码，解压进入目录执行命令:  
```
cd run
cmake ..
make
./ComparePicture -c 8 -t src.txt -s ./reportPath -p ./图片组1 ./图片组2
```
执行完上面的命令没有报错，说明程序已经可以正常运行。
* 调用方法  
```
./ComparePicture -c count -t txtName -s reportPath -p path1 path2
```
* 参数解释  
-c 要比对的图片数量;    
-t -t path1和path2目录中含有目录中图片名字的txt文
件名，每个目录中的txt名字都需要相同;  
-s 指定比对报告保存路径;  
-p 指定两组图片的路径,必须为最后一个参数。  

* 调用demo:  
```
./ComparePicture -c 8 -t src.txt -s ./reportPath -p ./图片组1 ./图片组2
```

## 三、关于作者
```javascript
  var whoAmI = {
    name   : "Mannix1994",
    gitee  : "https://gitee.com/Mannix1994",
    github : "https://github.com/Mannix1994"
  }
```