import pymem
from pymem.process import module_from_name

pm = pymem.Pymem("ac_client.exe")

gameModule = module_from_name(pm.process_handle, "ac_client.exe").lpBaseOfDll

def GetPtrAddr(base, offsets):
    addr = pm.read_int(base)
    for i in offsets:
        if i != offsets[-1]:
            addr = pm.read_int(addr + i)
    return addr + offsets[-1]

while True:
    pm.write_int(GetPtrAddr(gameModule + 0x0017E0A8, [0xEC]), 100)
