import os

import main


def deal(file_name):
    if len(file_name) > 4:
        if file_name[-4:] == '.apk':
            shell_md = "apktool d -f " + file_name + " -o " + main.temp_path
            print(shell_md)
            os.system(shell_md)
