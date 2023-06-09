import xml.etree.cElementTree as ET
#android namespace
namespace = '{http://schemas.android.com/apk/res/android}'


def deal(manifest_path):
    usesPermissionArray = []
    #print(manifest_path)
    ET.register_namespace('android', "http://schemas.android.com/apk/res/android")
    tree = ET.parse(manifest_path)  # 打开xml文档
    root = tree.getroot()  # 获得root节点
    package_name = root.get('package')
    #print(package_name)
    for child in root.iter('uses-permission'):
        childName = child.get(namespace+'name')
        usesPermissionArray.append(childName)
        #print(childName)
    return usesPermissionArray
