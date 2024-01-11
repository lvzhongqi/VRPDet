import os


def deal(temp_path):
    mysos = set()
    for file_name in os.listdir(temp_path):
        #print(file_name)
        if file_name == "lib":
            lib = temp_path + file_name
            #print(lib)
            for root, dirs, files in os.walk(lib):
                #print(files)
                for so in files:
                    #print(type(so))
                    if so.endswith(".so"):
                        #print(so)
                        mysos.add(so)
    return mysos