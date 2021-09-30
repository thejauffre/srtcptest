#include "main.h"

using namespace std;

vector<string> split (const string &s, char delim) 
{
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

void sendPack(int _sockFD, string message)
{
    // TODO: check message format for SR2000W
    cout << "Sending " << message << endl;
    char txbuffer[PACKET_SIZE];
    txbuffer[0] = '\x02';
    memcpy(&txbuffer[1], message.c_str(), message.length());
    txbuffer[message.length() +1] = '\x03';
    int n = write(_sockFD, txbuffer, message.length()+2);
    if (n < 0)
        std::cout << "[Server] " << "Error writing to socket" << std::endl;
    else {
        cout << "[Server] Transmission completed" << endl;
    }
}

void readFromSocket(int _sockFD)
{
    // TODO: check message format for SR2000W
    char rxbuffer[PACKET_SIZE];
    int readres = recv(_sockFD, &rxbuffer, PACKET_SIZE, 0);
    if (readres < 0)
        std::cout << "[Server] " << "Error reading from socket" << std::endl;
    else if (rxbuffer[0] != NULL)
    {
        char *s = (char*)memmem(&rxbuffer[0], sizeof(rxbuffer), STX, 1);
        int start = s - rxbuffer;
        char *e = (char*)memmem(&rxbuffer[start+1], sizeof(rxbuffer)-start, ETX, 1);
        int end = e - rxbuffer;
        string tmp(&rxbuffer[start+1], &rxbuffer[end]);
        cout << "Received response: " << tmp << endl;
        // call messageInterpreter
        Message* ciaone = new Message(tmp);
        cout << ciaone->id << endl;
    }
}

void error(std::string errmsg)
{
    //TODO
    std::cout << __func__ << errmsg << std::endl;
}

void log(std::string mess)
{
    std::cout << __func__ << mess << std::endl;
}

void openTCPsocket(bool isListener)
{
    int port = isListener ? listenerPort : commanderPort;
    // Creating socket file descriptor
    struct sockaddr_in serv_addr;
    int _sockFD = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(_sockFD<0) log("socket");
    cout << "SockFD: " << _sockFD << "- Port: " << port <<  endl;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(hostname.c_str());   
    serv_addr.sin_port = htons(port);

    int conn = connect(_sockFD,(struct sockaddr *) &serv_addr,sizeof(serv_addr));

    if (conn == 0)
    {
        log("Connected");
        while(true)
        {
            if (isListener) 
            {
                readFromSocket(_sockFD);
                if (closed) close(_sockFD);
            }
            else 
            {
                int choice;
                sim_displayMenu();
                cin >> choice;
                switch(choice)
                {
                    case 1:
                        sendPack(_sockFD, "LON");
                        break;
                    case 2:
                        sendPack(_sockFD, "RESET");
                        break;
                    case 0:
                        sendPack(_sockFD, "QUIT");
                        close(_sockFD);
                        closed = true;
                        break;
                }
            }
        }
    }
    else
    {
        cout << "Connection error: " << errno << endl;
        exit(1);
    }
}

void sim_displayMenu()
{
    cout << "Menu\n";
    cout << "Please make your selection\n";
    cout << "\t1 - LON\n";
    cout << "\t2 - RESET\n";
    cout << "\t3 - CLOSE\n";
    cout << "\tSelection: " << endl;
}

int main()
{
    cout << "SR2000 TCP client test, v0.1" << endl;
    hostname = "192.168.0.233";
    commanderPort = 9004;
    listenerPort = 9005;

    // Create commander thread
    thread commthread(openTCPsocket, false);
    sleep(1);

    // create listener thread
    thread listenerthread(openTCPsocket, true);
    // Wait for threads to finish
    listenerthread.join();
    commthread.join();
}