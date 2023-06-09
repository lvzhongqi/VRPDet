import os


def deal(temp_path):
    mypack = set()
    for file_name in os.listdir(temp_path):
        ##print(file_name)
        if file_name.startswith("smali"):
            smali = temp_path + file_name
            for pak1 in os.listdir(smali):
                com = smali + '/' + pak1
                if os.path.isdir(com):
                    for pak2 in os.listdir(com):
                        example = com + '/' + pak2
                        if os.path.isdir(example):
                            for pak3 in os.listdir(example):
                                lzq = example + '/' + pak3
                                if os.path.isdir(lzq):
                                    pkg = pak1+'.'+pak2+'.'+pak3
                                    #print(pkg)
                                    mypack.add(pkg)
                                else:
                                    pkg = pak1 + '.' + pak2
                                    #print(pkg)
                                    for pkgsave in mypack.copy():
                                        if pkgsave.startswith(pkg):
                                            mypack.remove(pkgsave)
                                    mypack.add(pkg)
                                    break
    return mypack