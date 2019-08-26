import os
import random
import string
import pexpect
import sys

# 获取当前执行路径
pwd = os.getcwd()
father_path = os.path.abspath(os.path.dirname(pwd)+os.path.sep+".")

client = os.path.join(father_path+"/client")


def random_str():
    c = random.randint(2, 9)
    # 生成随机长度的随机字符串
    ran_str = ''.join(random.sample(string.ascii_letters + string.digits, c))
    return ran_str


# 启动客户端
child = pexpect.spawn(client)

n = 0
while n < 1000:
    # Add Book
    file_add_log = open("./add_log", mode='ab+')
    child.logfile = file_add_log

    child.expect("Input operation num:")
    child.sendline("1")

    child.expect("book name")
    child.sendline(random_str())

    child.expect("book author")
    child.sendline(random_str())

    child.expect("book des")
    child.sendline(random_str())

    n += 1

