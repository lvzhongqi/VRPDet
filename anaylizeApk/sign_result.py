import binascii

from Crypto.Cipher import PKCS1_OAEP
from Crypto.Signature import PKCS1_v1_5
from Crypto.PublicKey import RSA
from Crypto.Hash import SHA256
import base64

from cryptography.hazmat.primitives import hashes, serialization
from cryptography.hazmat.primitives.asymmetric import padding


def sign(byte_stream):
    hex_string = binascii.hexlify(byte_stream).decode('utf-8')
    print(hex_string)
    print(byte_stream)
    print(len(byte_stream))
    # 计算文件的哈希值，例如使用SHA-256
    data_s_hashencode = SHA256.new(byte_stream)
    print(data_s_hashencode)
    print(data_s_hashencode.hexdigest())
    # 加载PEM格式的私钥
    with open("private_key.pem", "rb") as key_file:
        private_key_data = key_file.read()
        #private_key = RSA.import_key(private_key_data)
        private_key = serialization.load_pem_private_key(
            private_key_data,None
        )

    # pri_cipher = PKCS1_v1_5.new(private_key)
    # signature = pri_cipher.sign(data_s_hashencode)
    # 使用私钥对哈希值进行加密
    signature = private_key.sign(
        byte_stream,
        padding.PKCS1v15(),
        hashes.SHA256()
    )
    print(signature)
    print(binascii.hexlify(signature).decode('utf-8'))
    print(len(signature))
    return signature.hex()
