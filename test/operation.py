import os
import random
import string
import pexpect
import time
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


start = time.clock()
# 启动客户端
child = pexpect.spawn(client)

# 进行100增删改查操作次操作
n = 0
while n < 1000:
    a = random.randint(2, 5)
    child.expect("Input operation num:")
    child.sendline(str(a))

    if a == 2:
        # print("####DELETE_BOOK####")
        b = random.randint(0, 1000)
        file_delete_log = open("./delete_log", mode='ab+')
        child.logfile = file_delete_log
        child.expect("Please input book id")
        child.sendline(str(b))

    elif a == 3:
        # print("####MODIFY_BOOK####")
        b = random.randint(0, 1000)
        c = random.randint(1, 3)

        file_modify_log = open("./modify_log", mode='ab+')
        child.logfile = file_modify_log

        child.expect("Please input book id:")
        child.sendline(str(b))

        child.expect("Input operation num:")
        child.sendline(str(c))

        if c == 1:
            child.expect("Enter the book name you want to change:")
            child.sendline(random_str())

        if c == 2:
            child.expect("Enter the book author you want to change:")
            child.sendline(random_str())

        if c == 3:
            child.expect("Enter the book description you want to change:")
            child.sendline(random_str())

    elif a == 4:
        # print("####SEARCH_BOOK####")
        b = random.randint(0, 1000)
        file_search_log = open("./search_log", mode='ab+')
        child.logfile = file_search_log
        child.expect("Please input book id:")
        child.sendline(str(b))

    n += 1

end = time.clock()
run_time = end - start
print(run_time)

















