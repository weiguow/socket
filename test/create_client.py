import time
import os
import asyncio
import subprocess

test = "./operation.py"
command_operat = './operation.py'

n = 0


async def hello():
    global n
    p = subprocess.call('python3 operation.py', shell=True)
    # p = os.system(command_operat)
    if p == 0:
        n += 1


def run():
    for i in range(100):
        loop.run_until_complete(hello())


loop = asyncio.get_event_loop()
run()
print("success rate is ", n / 100)
