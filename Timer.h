//Parrallel Code Execution
//Delayed Code Execution
//Repeated Code Execution
//Thread library
//Multiple threads can run simultaneously in a single process
//Record keystrokes AND send mail
//Multiple threads can share same variables and etc, and open file descriptors (Threads share the same memory address space)
//Multiple processes cannot share same stuff
//before C++ 11 we don't have threads native to C++ (standard support)
//Windows API management of threads is very difficult
//Threads even though it seems like its simultaneously, its not really in reality it is very fast execution of threads (not simultaneously)

//Chrono Library - time
//Very precise time measurements, uses Windows API Measurements
//Squaric Performance counter (used to measure in in milli,mico,nano seconds

//In the main function, we have the while loop (which is the main thread) cannot do anything else EXCEPT if we create other threads if we want to do something else
//Header can block or delay the main thread, if we need to.

#ifndef TIMER_H
#define TIMER_H

#include "mingw.thread.h"
#include <chrono>

//used for asynchronous code execution
class Timer
{
    std::thread Thread; //creates a variable of type thread from thread library, use this variable to run asynchronous code execution without  blocking the main thread
    bool Alive = false; //state of the timer to check if the program / thread is running
    long CallNumber = -1L; //how many times, this particular variable to determine how many times we wanna call a function. -1L = no times, L stands for long so it doesnt become a int.
    //we use Long because the code runs for a year, and we want to be certain that an overflow will not happen
    long repeat_count = -1L; //count amount of times we have called a certain function
    std::string threadName = "default_thread";
    std::chrono::milliseconds interval = std::chrono::milliseconds(0); //interval = 0 default, represent interval between function calls
    std::function<void(void)> funct = nullptr; //new in C++ 11, new type which can contain function very similar to a pointer to a function (callback?) accepts a function that takes and returns nothing. default no function

    void SleepAndRun()
    {
        std::this_thread::sleep_for(interval); //current thread which creates the timer object, sleep_for = block for time interval
        if(Alive)
            Function()(); //()() public method which will be explains down below. first () calls the function called function = Function(), second () calls the function that the first function returns
    }

    void ThreadFunc()
    {
        try{
            //passed to the thread
            //cannot tell the thread to do 5 times, pass this function to give ourselves a degree of control like call, sleep, run function x5
            if(CallNumber == Infinite) //if infinite
                while(Alive)
                    SleepAndRun();
            else
                while (repeat_count > 0)
                {
                    repeat_count--;
                    Helper::WriteAppLog(Helper::ToString(threadName + " Started Method: Thread Function"));
                    SleepAndRun();
                } //if repeat count > 0
            //alive = false?
            Helper::WriteAppLog(Helper::ToString(threadName + " Dying Peacefully"));
        } catch (...) {
            Helper::WriteAppLog(Helper::ToString("Error with Timer: " + GetLastError()));
        }
    }


    //all public methods and getters and setters
public:
    static const long Infinite = -1L; //define infinite is -1L, timer will work until manually stop
    //e.g. Timer::Infinite to use it (cause its static)
    //create a constructor for Timer
    Timer(){}//create timer as Global Variables, why? Because variables will die at the end of the function. set our own values by using functions below (cause empty constructor)

    Timer(const std::function<void(void)> &f) : funct(f){} //point it to a function ( reference to a constant typed function<void(void)> object into &f and you assign the function into funct variable

    Timer(const std::function<void(void)> &f,
          const unsigned long &i,
          const long repeat = Timer::Infinite) :    funct(f),
                                                    interval(std::chrono::milliseconds(i)),
                                                    CallNumber(repeat){}

    Timer(const std::function<void(void)> &f,
          const unsigned long &i,
          const long repeat = Timer::Infinite,
          const std::string &tName = "defaultName") :               funct(f),
                                                    interval(std::chrono::milliseconds(i)),
                                                    CallNumber(repeat),
                                                    threadName(tName){}

    void Start(bool Async = true)
    {
        if(IsAlive()) //if already running, dont need do anything
            return;

        try{
            //if not running
            Alive = true;
            repeat_count = CallNumber; //set repeat count to call number
            if(Async) //if asynchronous running
                Thread = std::thread(ThreadFunc, this); //its our first variable, putting it into it with parameters, a function which will be execute and with a pointer to our class (this)
            else
                this->ThreadFunc(); //blocks the main thread

            Helper::WriteAppLog(Helper::ToString(threadName + " Thread Added..."));
        } catch(...)
        {
            Helper::WriteAppLog(Helper::ToString("Error with Timer: " + GetLastError()));
        }
    }

    //we can successfully start our timer, method to manually stop the timer
    void Stop()
    {
        Helper::WriteAppLog(Helper::ToString(threadName + " Timer Stopped Manually"));
        Alive = false;
        Thread.join(); //join with main thread? no concurrent execution, this will stop the timer? hmmm Crashes the program?
    }

    //getters and setters
    bool IsAlive() const {return Alive;}
    long GetLeftCount() const {return repeat_count;} //get the current number of executions/iteration left
    long RepeatCount() const {return CallNumber;} //how many times (total)
    unsigned long Interval() const {return interval.count();} //count of milliseconds left
    const std::function<void(void)> &Function() const //this is a method, it returns a function, when it is returned, we want it to be called
    {
        return funct;
    }

    void RepeatCount(const long r)
    {
        if(Alive)
            return;
        CallNumber = r;
    }

    void SetFunction(const std::function<void(void)> &f) //set function which will be executed, create timer empty contructor then set our own function and variables
    {
        funct = f;
    }

    void SetInterval (const unsigned long &i)
    {
        if(Alive)
            return;
        interval = std::chrono::milliseconds(i);
    }



};

#endif
