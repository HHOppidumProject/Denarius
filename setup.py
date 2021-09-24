import sys

if (sys.version_info[0]  < 3) and (sys.version[1] < 8):
    print("#"*25)
    print("Need python version greater than 3.7")
    print("#"*25)

try:
    import subprocess, os, winreg
    import itertools, urllib3
except ImportError as e:
    subprocess.run(["pip", "install", "urllib3"])
    print("If this error was caused by urllib3, it has now been handled, and you can ignore this message")
    print("Report the following to Q. Aelius Volusus:")
    print("#"*25)
    print("IMPORT ERROR HAS BEEN CAUGHT")
    print(e)
    print("#"*25)

except ModuleNotFoundError as e:
    subprocess.run(["pip", "install", "urllib3"])
    print("If this error was caused by urllib3, it has now been handled, and you can ignore this message")
    print("Report the following to Q. Aelius Volusus:")
    print("#"*25)
    print("IMPORT ERROR HAS BEEN CAUGHT")
    print(e)
    print("#"*25)

def Help():
    print(f"""
Script for installing tools to help with development of the Hellenic Hospitality Oppidum currency, the solidus.
Syntax: {os.path.basename(__file__)}  [-h|-VSC|-VS|-vcpkg|-vcpkgPath|-njs|-f]

Options:
    h\t\tPrints this help function
        Alias\t-?|--help
    VSC\t\tInstalls Visual Studio Code [y/n] (Cannot be used with -VS)
        Alias\t--VSCode|--Visual-Studio-Code
    VS\t\tInstalls Visual Studio Community Edition [y/n] (Cannot be used with -VSC)
        Alias\t--VSCommunity|--Visual-Studio-Community
    vcpkg\t\tInstalls vcpkg package manager [y/n]
        Alias\t--vcp
    vcpkgPath\tPath to install vcpkg into
        Alias\t--vcpkgInstallPath| --vcpp
    njs\t\tInstalls Node.JS [y/n]
        Alias\t--Node.JS
""")

def DownloadFile(FileName, request):
    with open(FileName, "wb") as file:
            amt = 0
            for chunk in request.stream(1024):
                file.write(chunk)
                amt += 1024
                progress = int((amt/int(request.info()['Content-Length']))*100)
                amtToPrint = 25-int(progress/4)
                print(f'\r[{str("#"*int(progress/4))+str("-"*amtToPrint)}] {progress}%', end='')
    print()

def main(VSCode = False, VSCommunity = False, vcpkg = True, njs = True, force = False, vcpkgPath = ''):

    http = urllib3.PoolManager()

    if not force:
        if VSCode and VSCommunity:
            print("You cannot have both Visual Studio Code and Visual Studio Community installed on the same run.\n")
            Help()
            exit()
        elif vcpkg and vcpkgPath == "":
            print("As you want vcpkg installed, please set a path for it to install to using --vcpkgInstallPath.\n")
            Help()
            exit()

    if VSCode:
        print("Getting and installing Visual Studio Code")
        DownloadFile("VSCode-Installer.exe", http.request("GET", "https://code.visualstudio.com/sha/download?build=stable&os=win32-x64", preload_content=False))
        subprocess.run(["VSCode-Installer.exe"])
    else:
        print("Skipping installation of Visual Studio Code")

    if VSCommunity:
        print("Getting and installing Visual Studio Community Edition")
        DownloadFile("VS-Community-Installer.exe", http.request("GET", "https://download.visualstudio.microsoft.com/download/pr/22c17f05-944c-48dc-9f68-b1663f9df4cb/2492827d2bc58c5cd0a4e4ab40da86910d05ee96f71d69b561457885b3249a91/vs_Community.exe", preload_content=False))
        subprocess.run(["VS-Community-Installer.exe"])
    else:
        print("Skipping installation of Visual Studio Community Edition")

    if vcpkg:
        print(f"Getting vcpkg and installing to {vcpkgPath}")
        subprocess.run(["git", "clone", "https://github.com/microsoft/vcpkg", f"{vcpkgPath}"])
        key = winreg.OpenKey(reg.HKEY_LOCAL_MACHINE, r'SYSTEM\CurrentControlSet\Control\Session Manager\Environment', 0, reg.KEY_ALL_ACCESS)
        try:
            value, _ = winreg.QueryValueEx(key, "PATH")
        except WindowsError:
            value = ""
        value = ';'.join(s for s in value.split(";") if not r''.join([vcpkgPath]) in s)
        winreg.SetValueEx(key, "PATH", 0, winreg.REG_EXPAND_SZ, value)
        winreg.CloseKey(key)
    else:
        print("Skipping installation of vcpkg")

    if njs:
        print("Getting and installing Node.JS")
        DownloadFile("nodejs-installer.msi", http.request("GET", "https://nodejs.org/dist/v14.17.6/node-v14.17.6-x64.msi", preload_content=False))
        subprocess.run(["powershell", "$njsins = Start-Process \"./nodejs-installer.msi\" -PassThru ; $njsins.WaitForExit()"])
    else:
        print("Skipping installation of Node.JS")
    
    if subprocess.run(["git", "-C", os.path.dirname(__file__), "rev-parse"]).returncode == 0:
        subprocess.run(["git", "checkout", "origin/dev"])
        subprocess.run(["git", "pull"])
    else:
        print("Not in a git repository")

    if os.path.exists("VSCode-Installer.exe"):
        os.remove("VSCode-Installer.exe")
    if os.path.exists("VS-Community-Installer.exe"):
        os.remove("VS-Community-Installer.exe")
    if os.path.exists("nodejs-installer.msi"):
        os.remove("nodejs-installer.msi")
        

def boolCheck(arg, key, default = False):
    if arg.lower() in ["true","y", "", " "]:
        res = True
    elif arg.lower() in ["false","n"]:
        res = False
    else:
        print(f"\"{arg}\" Unexpected value for {key} ignoring and setting to default value {default}")
        res = default
    return res

def GetArgs():
    VSCode = False
    VSCommunity = False
    vcpkg = True
    njs = True
    force = False
    vcpkgPath = ''
    args = sys.argv[1:]

    flags = [
            ["-h", "-?", "--help"],
            ["-VSC", "--VSCode", "--Visual-Studio-Code"],
            ["-VS", "--VSCommunity", "--Visual-Studio-Community"],
            ["-vcpkg", "--vcp"],
            ["-vcpkgPath", "--vcpkgInstallPath", "--vcpp"], 
            ["-njs", "--NodeJS"],
            ["-f", "-F", "--force"]]

    while len(args) > 0:
        key = args[0]
      
        if key in flags[0]:
            Help()
            exit()
        elif key in flags[1]:
            try:
                if not (args[1] in list(itertools.chain.from_iterable(flags))):
                    VSCode = boolCheck(args[1], key)
                    args.pop(0)
                else: 
                    VSCode = True
            except IndexError:
                VSCode = True
            args.pop(0)
        elif key in flags[2]:
            try:
                if not (args[1] in list(itertools.chain.from_iterable(flags))):
                    VSCommunity = boolCheck(args[1], key)
                    args.pop(0)
                else: 
                    VSCommunity = True
            except IndexError:
                VSCommunity = True
            args.pop(0)
        elif key in flags[3]:
            try:
                if not (args[1] in list(itertools.chain.from_iterable(flags))):
                    vcpkg = boolCheck(args[1], key)
                    args.pop(0)
                else: 
                    vcpkg = True
            except IndexError:
                vcpkg = True
            args.pop(0)
        elif key in flags[4]:
            try:
                vcpkgPath = args[1]
            except IndexError:
                print(f"{key} needs to be supplied a path")
            args.pop(0)
            args.pop(0)
        elif key in flags[5]:
            try:
                if not (args[1] in list(itertools.chain.from_iterable(flags))):
                    njs = boolCheck(args[1], key)
                    args.pop(0)
                else: 
                    njs = True
            except IndexError:
                njs = True
            args.pop(0)
        elif key in flags[6]:
            force = True
            args.pop(0)
        else:
            Help()
            exit()
    main(VSCode, VSCommunity, vcpkg, njs, force, vcpkgPath)


if __name__ == "__main__":
    GetArgs()