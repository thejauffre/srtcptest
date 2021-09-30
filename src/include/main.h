#ifndef _MAIN_HEADER
#define _MAIN_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <vector>
#include <string>
#include <mutex>
#include <semaphore.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
#include <queue>
#include <cmath>
#include <chrono>
#include <semaphore.h>
#include <type_traits>

#define PACKET_SIZE 1024
#define DELIMITER ':'
#define POINT_DELIMITER '/'
#define STX "\x02"
#define ETX "\x03"


// int _lenze_sockfd;
int listenerPort, commanderPort;
std::string hostname;
bool closed = false;
void error(std::string errmsg);
void log(std::string mess);
void openTCPSocket(bool isListener);
void commandListener();
void readFromSocket(int _sockFD);
void sendPack(int _sockFD, std::string message);
void sim_displayMenu();
std::vector<std::string> split (const std::string &s, char delim);

class Point
{
public:

    Point(std::string pstr)
    {
        std::vector<std::string> splitpoint = split(pstr, POINT_DELIMITER);

        _x = stoi(splitpoint[0]);
        _y = stoi(splitpoint[1]);
    };

    Point(int x, int y)
    {
        _x = x;
        _y = y;
    };

    Point(const Point& p)
    {
        _x = p._x;
        _y = p._y;
    };
    ~Point(){};

    Point()
    {
        _x = -1;
        _y = -1;
    };

    int _x;
    int _y;
};

class Message
{
public:
    Message()
    {
        timestamp = "";
        id = "";
        A = new Point();
        B = new Point();
        C = new Point();
        D = new Point();
        center = new Point();
    };

    Message(const Message& m)
    {
        timestamp = m.timestamp;
        id = m.id;
        A = m.A;
        B = m.B;
        C = m.C;
        D = m.D;
        center = m.center;
    };
    
    ~Message(){};

    Message(std::string recmessage)
    {
        std::vector<std::string> splitmess = split(recmessage, DELIMITER);
        timestamp = splitmess[0];
        id = splitmess[1];
        A = new Point(splitmess[2]);
        B = new Point(splitmess[3]);
        C = new Point(splitmess[4]);
        D = new Point(splitmess[5]);
        center = new Point(splitmess[6]);
    };

    std::string timestamp;
    std::string id;
    Point* A;
    Point* B;
    Point* C;
    Point* D;
    Point* center;
};

#endif