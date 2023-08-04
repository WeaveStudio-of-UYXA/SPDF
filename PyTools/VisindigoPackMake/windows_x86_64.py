import sys
import os
"""
This file is only for Windows x86_64 deploy
"""
InstallPackName = "InstallPackCache\\SPDF"
VersionFolderName = "00"
versionMajor = 0
versionMinor = 0
versionPatch = 0
versionFullString = ""
def backupsTemplate(path):
    # copy SPDFTemplateProject to SPDFTemplateProjectBackup
    print("backups template")
    if os.path.exists(path+"/SPDFTemplateProjectBackup"):
        os.system("rd /s /q "+path.replace("/","\\")+"\\SPDFTemplateProjectBackup")
    os.system("xcopy "+path.replace("/","\\")+"\\SPDFTemplateProject "+path.replace("/","\\")+"\\SPDFTemplateProjectBackup /e /i /h")
    if os.path.exists(path+"/SPDFTemplateFileBackup"):
        os.system("rd /s /q "+path.replace("/","\\")+"\\SPDFTemplateFileBackup")
    os.system("xcopy "+path.replace("/","\\")+"\\SPDFTemplateFile "+path.replace("/","\\")+"\\SPDFTemplateFileBackup /e /i /h")
    print("backups template done")
    
def recoverTemplate(path):
    # copy SPDFTemplateProjectBackup to SPDFTemplateProject
    print("recover template")
    if os.path.exists(path+"/SPDFTemplateProject"):
        os.system("rd /s /q "+path.replace("/","\\")+"\\SPDFTemplateProject")
    os.system("xcopy "+path.replace("/","\\")+"\\SPDFTemplateProjectBackup "+path.replace("/","\\")+"\\SPDFTemplateProject /e /i /h")
    if os.path.exists(path+"/SPDFTemplateFile"):
        os.system("rd /s /q "+path.replace("/","\\")+"\\SPDFTemplateFile")
    os.system("xcopy "+path.replace("/","\\")+"\\SPDFTemplateFileBackup "+path.replace("/","\\")+"\\SPDFTemplateFile /e /i /h")
    print("recover template done")

def getProjectVersion(path):
    global versionMajor, versionMinor, versionPatch, versionFullString, InstallPackName, VersionFolderName
    # get version from SPDF/VICore/VIVersion.h, the macro is VI_VERSION_MAJOR and VI_VERSION_MINOR
    versionFile = open(path+"/SPDF/SPDF/private/SPDFCompileMacro.h", "r", encoding="utf-8")
    for line in versionFile.readlines():
        if "#define SPDF_VERSION_MAJOR" in line:
            versionMajor = int(line.split(" ")[-1])
        elif "#define SPDF_VERSION_MINOR" in line:
            versionMinor = int(line.split(" ")[-1])
        elif "#define SPDF_VERSION_PATCH" in line:
            versionPatch = int(line.split(" ")[-1])
    versionFile.close()
    if versionMajor == 0 and versionMinor == 0 and versionPatch == 0:
        print("Can not get version from /SPDF/SPDF/private/SPDFCompileMacro.h")
        return False
    else:
        versionString = str(versionMajor)+"."+str(versionMinor)
        versionFullString = versionString+"."+str(versionPatch)
        VersionFolderName = InstallPackName + "/" + versionFullString
        print("SPDF version now is : "+versionFullString)
        return True
        

def copyFileinDllDebug(path):
    global VersionFolderName
    #copy all the files in x64/DllDebug into VersionFolderName/x64/Debug
    print("copy debug files")
    if not os.path.exists(path+"/"+VersionFolderName+"/x64/Debug"):
        os.makedirs(path+"/"+VersionFolderName+"/x64/Debug")
    os.system("xcopy "+path.replace("/","\\")+"\\x64\\DllDebug "+(path+"/"+VersionFolderName+"/x64/Debug").replace("/","\\")+" /e /i /h")
    print("copy debug files done")

def copyFileinDllRelease(path):
    global VersionFolderName
    #copy all the files in x64/DllRelease into VersionFolderName/x64/Release
    print("copy release files")
    if not os.path.exists(path+"/"+VersionFolderName+"/x64/Release"):
        os.makedirs(path+"/"+VersionFolderName+"/x64/Release")
    os.system("xcopy "+path.replace("/","\\")+"\\x64\\DllRelease "+(path+"/"+VersionFolderName+"/x64/Release").replace("/","\\")+" /e /i /h")
    print("copy release files done")

def copyTemplateFile(path):
    global VersionFolderName
    #copy all the files in SPDFTemplateFile into VersionFolderName/SPDFTemplateFile
    print("copy template files")
    if not os.path.exists(path+"/"+VersionFolderName+"/SPDFTemplateFile"):
        os.makedirs(path+"/"+VersionFolderName+"/SPDFTemplateFile")
    os.system("xcopy "+path.replace("/","\\")+"\\SPDFTemplateFile "+(path+"/"+VersionFolderName+"/SPDFTemplateFile").replace("/","\\")+" /e /i /h")
    print("copy template files done")

def zipFullFolder(path):
    global versionFullString, InstallPackName
    #use "bandzip.exe" to zip the full folder to the zip file with the same name
    print("zip full folder")
    if not os.path.exists(path+"/InstallPack"):
        os.makedirs(path+"/InstallPack")
    #delete the old zip file if exists
    if os.path.isfile(path+"/InstallPack/SPDF_"+versionFullString+"_Windows_x64.zip"):
        print("delete old zip file")
        os.remove(path+"/InstallPack/SPDF_"+versionFullString+"_Windows_x64.zip")
    os.system("bz a "+path+"/InstallPack/SPDF_"+versionFullString+"_Windows_x64.zip "+(path+"/"+InstallPackName).replace("/","\\"))
    print("zip full folder done")

def deployNow(path):
    global InstallPackName
    # copy all the files in version folder to the deploy version folder
    print("deploy now")
    if not os.path.exists("D:/SPDF"):
        os.makedirs("D:/SPDF")
    os.system("xcopy "+path.replace("/","\\")+"\\"+InstallPackName+" D:\\SPDF /e /i /h /y")

    if not os.path.exists("C:/Users/"+os.getlogin()+"/Documents/Visual Studio 2022/Templates/ItemTemplates"):
        print("Can not find Visual Studio 2022")
    else:
        if os.path.exists("C:/Users/"+os.getlogin()+"/Documents/Visual Studio 2022/Templates/ItemTemplates"+"/SPDFTemplateFile"):
            os.system("rd /s /q "+"C:\\Users\\"+os.getlogin()+"\\Documents\\Visual Studio 2022\\Templates\\ItemTemplates"+"\\SPDFTemplateFile")
        os.system("xcopy "+(path+"/"+VersionFolderName).replace("/","\\")+"\\SPDFTemplateFile \"C:\\Users\\"+os.getlogin()+"\\Documents\\Visual Studio 2022\\Templates\\ItemTemplates\\SPDFTemplateFile\" /e /i /h /y")
    print("deploy done")
    
def windows_x86_64_PackMake(path):
    global InstallPackName
    # create a new folder if not exists
    if os.path.exists(path+"/"+InstallPackName):
        os.system("rd /s /q "+path.replace("/","\\")+"\\"+InstallPackName)
    os.makedirs(path+"/"+InstallPackName)
    backupsTemplate(path) # backup the template project, in case of broken
    if not getProjectVersion(path):
        recoverTemplate(path)
        print("InstallPack failed, as Template version can not be changed, all changes have been recovered")
        return
    copyFileinDllDebug(path) # copy SPDF Debug files (include, lib, dll, pdb, ilk, exp)
    copyFileinDllRelease(path) # copy SPDF Release files (include, lib, dll, exp)
    copyTemplateFile(path) # zip the Visual Studio Item Template
    zipFullFolder(path) # generate the full folder zip file
    deployNow(path) # copy SPDF to the deploy folder, copy template to VS template folder

if __name__ == "__main__":
    path:str = sys.path[0]
    print(path)
    os.chdir(path+"/../../")
    path = os.getcwd()
    windows_x86_64_PackMake(path)