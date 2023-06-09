import os
import shutil
import sys
import analizeapk

from PyQt5.Qt import *

# 全局变量声明与初始化
file_name = ""
out_path = "./out/"
out_file = out_path+"result.txt"
temp_path = "./tem/"
manifest_path = temp_path + "AndroidManifest.xml"
apk_path = "./APK/"

if __name__ == '__main__':
    app = QApplication(sys.argv)

    w = QWidget()  # 主体
    w.setFixedWidth(w.width())
    w.setWindowTitle("VARP静态分析模块")
    main_layout = QVBoxLayout()  # 主体布局


    # 加固文件选择模块
    groupbox_file = QGroupBox("选择要分析的文件：")  # 文件选择box
    lineedit = QLineEdit()
    lineedit.setReadOnly(True)  # 输入布局文件
    pushbutton_choose = QPushButton("选择")  # 选择按钮


    ##选择文件
    def on_buttonc_clicked():
        global file_name
        file_name, _ = QFileDialog.getOpenFileName(None, "选取文件", apk_path, "Apk文件(*.apk)")
        # print (file_name)
        lineedit.setText(file_name)
        textedit_log.append("加固文件：" + file_name + "。\n")
        textedit_log.moveCursor(QTextCursor.End)


    pushbutton_choose.clicked.connect(on_buttonc_clicked)  # 按钮逻辑
    sub_layout_1 = QHBoxLayout()  # 路径选择整体布局
    sub_layout_1.addWidget(lineedit)
    sub_layout_1.addWidget(pushbutton_choose)
    groupbox_file.setLayout(sub_layout_1)  # 路径布局传入box

    textedit_log = QTextEdit()  # 打log的地方
    file_log_name = out_path + 'log.txt'  # 文件打log
    pushbutton_begin = QPushButton("开始分析APK")  # 加固按钮


    # 加固处理
    def on_buttonb_clicked():
        global file_name
        pushbutton_begin.setEnabled(False)
        ##清空文件夹
        if not os.path.exists(out_path):
            os.mkdir(out_path)
        else:
            shutil.rmtree(out_path)
            os.mkdir(out_path)
        if not os.path.exists(temp_path):
            os.mkdir(temp_path)
        else:
            shutil.rmtree(temp_path)
            os.mkdir(temp_path)
        ##分析apk
        try:
            analizeapk.apkAnalyze(file_name)
            print("分析完成")
        except Exception as e:
            print("分析失败")
            print(e.with_traceback())
        finally:
            if not os.path.exists(temp_path):
                os.mkdir(temp_path)
            else:
                shutil.rmtree(temp_path)
                os.mkdir(temp_path)
        pushbutton_begin.setEnabled(True)

    pushbutton_begin.clicked.connect(on_buttonb_clicked)
    main_layout.addWidget(groupbox_file)
    main_layout.addWidget(pushbutton_begin)
    main_layout.addWidget(textedit_log)
    w.setLayout(main_layout)
    w.show()
    app.exec_()
