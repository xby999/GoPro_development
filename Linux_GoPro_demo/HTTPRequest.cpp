/* HTTPRequest.cpp/Open GoPro, Version 2.0 (C) Copyright 2021 GoPro, Inc. (http://gopro.com/OpenGoPro). */
/* This copyright was auto-generated on Sat Mar  5 01:05:53 UTC 2022 */

#include "HTTPRequest.h"
#include <errno.h>
#include <fcntl.h>
#include<arpa/inet.h>
#include<iostream>
#include <sys/ioctl.h>

HTTPRequest::HTTPRequest(const std::string& host, const short port)
    : Host(host), Port(port)
{
    InitWinsock();
    //if ((Sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    if ((Sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        int err = errno;
        //throw std::exception("Couldn't create socket " );
        printf("Couldn't create socket %d\n",err);
        exit(1);
    }
}

HTTPRequest::~HTTPRequest()
{
    //WSACleanup();
    //closesocket(Sock);
    close(Sock);
}


std::string HTTPRequest::get_response()
{
    return Response;
}

bool HTTPRequest::InitWinsock()
{
    // WSADATA wsaData;

    // if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
    // {
    //     return false;
    // }

    return true;
}

bool HTTPRequest::loop_recieve()
{
    while (true)
    {
        char recvBuf[256];

        auto nret = recv(Sock, recvBuf, sizeof(recvBuf), 0);
        if (nret == -1)
        {
            return false;
        }
        else if (nret == 0)
        {
            break;
        }

        Response.append(recvBuf, nret);
    }

    return true;
}

bool HTTPRequest::resolve_and_connect()
{
    bool retbool = true;
    if (connected)
        return retbool;
    sockaddr_in add;
    add.sin_family = AF_INET;
    //InetPtonA(AF_INET, Host.c_str(), &add.sin_addr.s_addr);
    printf(Host.c_str());
    inet_pton(AF_INET, Host.c_str(), &add.sin_addr.s_addr);
    add.sin_port = htons(Port);
    int ret = -1;

    // Set non-blocking 
    unsigned long modeB = 1;
    //ret = fcntl(Sock, FIONBIO, &modeB);

    int flags = fcntl(Sock, F_GETFL, 0);
    //ret = fcntl(Sock, F_SETFL, flags | O_NONBLOCK);

    // if (ret == -1)
    //     printf("fcntl failed\n");

    try
    {
        ret = connect(Sock, (struct sockaddr* )&add, sizeof(add));
        perror("Err");
        printf("\n ret 98 : %d \n",ret);
        if (ret == -1)
            printf("connect failed\n");
        ret = fcntl(Sock, F_SETFL, flags & ~O_NONBLOCK);
        printf("ret 102 : %d \n",ret);
        if (ret == -1)
            printf("fcntl failed\n");
        
        if (ret < 0)
        {
            int boo = errno;
            if (boo == 11)
            {
                printf("connect in progress - selecting\n");
                fd_set setW, setE;

                FD_ZERO(&setW);
                FD_SET(Sock, &setW);
                FD_ZERO(&setE);
                FD_SET(Sock, &setE);

                timeval time_out = { 0 };
                time_out.tv_sec = 2;
                time_out.tv_usec = 0;

                ret = select(0, NULL, &setW, &setE, &time_out);
                if (ret <= 0)
                {
                    // select() failed or connection timed out
                    //closesocket(Sock);
                    close(Sock);
                    if (ret == 0)
                    {
                        TimedOut = true;
                        //WSASetLastError(WSAETIMEDOUT);
                        printf("time out! \n");
                    }
                    ret = -1;
                }
                if (FD_ISSET(Sock, &setE))
                {
                    // connection failed
                    //closesocket(Sock);
                    close(Sock);
                    ret = -1;
                }
                // put socked in blocking mode...
                modeB = 0;
                if (fcntl(Sock, FIONBIO, &modeB) == -1)
                {
                    //closesocket(Sock);
                    close(Sock);
                    ret = -1;
                }
            }
            else
            {
                printf("Here return -1 \n");
                ret = -1;
            }
        }
    }
    catch(...)
    {

    }
    if (ret < 0)
    {
        int err = errno;
        char buf[128] = { 0 };
        sprintf(buf, "error opening socket %d", err);
        std::cout<<buf<<std::endl;
    }
    else
    {
        connected = true;
    }

    return retbool;
}

bool HTTPRequest::get_request(const std::string& path)
{
    if (!resolve_and_connect())
        return false;

    std::string request = "GET " + path + " HTTP/1.1" + "\r\n";
    request += "Host: " + Host + "\r\n";
    request += "Connection: close\r\n";
    request += "\r\n";

    if (send(Sock, request.c_str(), request.length(), 0) == -1)
    {
        return false;
    }

    return loop_recieve();
}