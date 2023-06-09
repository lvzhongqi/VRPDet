import os

import alsmanifest
import alspack
import main
import shelldeal
import alsso


def apkAnalyze(file_name):

    ##apktool解析获取代码
    if os.path.exists(file_name):
        shelldeal.deal(file_name)
    permissions= []
    ##分析mainfast文件
    if os.path.exists(main.manifest_path):
        permissions=alsmanifest.deal(main.manifest_path)

    ##分析包名
    pkgs = alspack.deal(main.temp_path)

    ##分析so库
    sos = alsso.deal(main.temp_path)

    ##输出结果到文件
    if not os.path.exists(main.out_path):
        os.mkdir(main.out_path)
    file = open(main.out_file, 'w')
    # 向文件中输入字符串
    for permission in permissions :
        file.write("permission: " +permission + "\n")
    for pkg in pkgs :
        file.write("package: " +pkg + "\n")
    for so in sos :
        file.write("so: " +so + "\n")
    file.close()
