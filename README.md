VRPDet 是用来检测基于应用虚拟化的重打包的插件，开发者可以使用该插件保护Android应用免受这种攻击方式。

VRPDet 分为静态分析模块（anaylizeApk）和动态检测模块（VRPDet）。

使用步骤：
  1.生成自己的公私钥用于签名验证，替换anaylizeApk中的私钥文件和VRPDet中的公钥
  2.将原始的apk使用anaylizeApk进行分析，得到一个result.txt文件，放到原apk代码的assest目录下。
  3.build VRPDet的模块，把so库，jar包放到对应原app的项目文件下，并引入。
  4.再重要的方法中调用VRPDet的VACheck()或者HookCheck()方法。
