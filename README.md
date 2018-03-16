
## 一、比对图片
将两组图片两两比较，如果对应点不同，则和未处理过的原图
进行对比，若处理过的两组图都和原图对应点不同，标记为黄
色;第一组和原图相同而第二组不同，标记为绿色;否则标记为
蓝色。

## 二、如何使用

* 下载源码，解压进入目录执行命令:  
```
cd run
cmake ..
make
./ComparePicture -n 8 -s ./report -p ./原图/src.txt ./图片组1/src.txt ./图片组2/src.txt
```
执行完上面的命令没有报错，说明程序已经可以正常运行。
* 调用方法  
```
./ComparePicture -n number -s reportPath -p path0.txt path1.txt path2.txt
```
* 参数解释  
-n number 要比对的图片数量;    
-s save_path 指定比对报告保存路径; 
-p path0.txt是原图相对于当前目录的图片路径，一张
图片的路径为一行;path1.txt和path2.txt分别指定要
比对的两组图片的路径。-p必须为最后一个参数.  

* 调用demo:  
```
./ComparePicture -n 8 -s ./report -p ./原图/src.txt ./图片组1/src.txt ./图片组2/src.txt
```

## 三、关于作者
```javascript
  var whoAmI = {
    name   : "Mannix1994",
    gitee  : "https://gitee.com/Mannix1994",
    github : "https://github.com/Mannix1994"
  }
```