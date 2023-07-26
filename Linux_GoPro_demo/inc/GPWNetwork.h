/* GPWNetwork.h/Open GoPro, Version 2.0 (C) Copyright 2021 GoPro, Inc. (http://gopro.com/OpenGoPro). */
/* This copyright was auto-generated on Sat Mar  5 01:05:53 UTC 2022 */

#pragma once

#include <unistd.h>
#include <system_error>
#include <string>
#include <iostream>
#include <netinet/in.h>


// class WSASession
// {
// public:
//     WSASession();
//     virtual ~WSASession();

// private:
//     WSAData data;
// };
typedef sockaddr SOCKADDR;
class UDPSocket
{
public:
    UDPSocket();
    virtual ~UDPSocket();

    void SendTo(const std::string& address, unsigned short port, const char* buffer, int len, int flags = 0);
    sockaddr_in RecvFrom(char* buffer, int len, int& received, int flags = 0);
    void Bind(unsigned short port);

private:
    int sock ;
};