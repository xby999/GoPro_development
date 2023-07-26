/* HTTPRequest.h/Open GoPro, Version 2.0 (C) Copyright 2021 GoPro, Inc. (http://gopro.com/OpenGoPro). */
/* This copyright was auto-generated on Sat Mar  5 01:05:53 UTC 2022 */

#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
typedef sockaddr SOCKADDR;
class HTTPRequest
{

public:
    HTTPRequest(const std::string& host, const short port);

    virtual ~HTTPRequest();
    std::string get_response();

    bool get_request(const std::string& path);

    bool TimedOut = false;

private:
    std::string Host="172.25.156.51";
    short Port;
    bool connected = false;
    int Sock;
    

    std::string Response;

    bool loop_recieve();
    bool resolve_and_connect();
    bool InitWinsock();
};

