#pragma comment(lib, "WS2_32.lib")
#include <string>
#include <iostream>
#include "Winsock2.h"
#include "Error.h"
#include <fstream>
#include <synchapi.h>
#include "Error.h"
#include "Time.h"
#pragma warning(disable : 4996)
#include "SectionControlUnit.h"

#define COUNT 5                             // quantity of packages for sending
#define LOCALHOST                           // use localhost
#define MAX_MESSAGE 50
#define PORT 2000                           // port
#define LOCAL_ADDRESS "192.168.43.220"      // local address PC

#define MESSAGE_ENTER   "ENTER"
#define MESSAGE_LEAVE   "LEAVE"
#define MESSAGE_SUCCESS "SUCCESS"

using namespace std;

int main(int argc, char* argv[])
{

    std::cout << "address: " << argv[1] << "Name app: " << argv[2] << std::endl;
    WSADATA wsaData;
    SOCKET cS;

    SOCKADDR_IN clnt;
    memset(&clnt, 0, sizeof(clnt));   // mem zero

    clnt.sin_family = AF_INET;
    clnt.sin_port = htons(PORT);

#ifdef LOCALHOST
    clnt.sin_addr.s_addr = inet_addr(argv[1]);
    CA ca = InitCA((char*)argv[1], (char*)argv[3]);
#endif

#ifndef LOCALHOST
    clnt.sin_addr.s_addr = inet_addr(LOCAL_ADDRESS);
    CA ca = InitCA((char*)LOCAL_ADDRESS, (char*)argv[3]);
#endif

    int lc = sizeof(clnt);
    char ibuf[50];                              // buffer for input
    int  lb = 0;                                // input buffer length
    char obuf[50];                              // buffer for output
    int  lobuf = 0;                             // output buffer length
    char buffer[20];                            // array for func concat (itoa)

    try {
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw SetErrorMsgText("Startup: ", WSAGetLastError());

        if ((cS = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
            throw SetErrorMsgText("socket: ", WSAGetLastError());

        for (int i = 1; i <= COUNT; i++) {

            lobuf = sendto(cS, (char*)MESSAGE_ENTER, MAX_MESSAGE, 0, (sockaddr*)&clnt, lc);
            if (lobuf == SOCKET_ERROR)
                throw SetErrorMsgText("send:", WSAGetLastError());

            RequestAccess(ca);

            lb = recvfrom(cS, ibuf, MAX_MESSAGE, 0, (sockaddr*)&clnt, &lc);
            if (lb == SOCKET_ERROR)
                throw SetErrorMsgText("recv:", WSAGetLastError());

            EnterCA(ca);

            if (strcmp(ibuf, MESSAGE_SUCCESS) != 0)
                return -1;

            for (int j = 1; j <= COUNT; j++) {

                auto fout = new std::fstream;
                fout->open(argv[3], std::ios::app);

                if (!fout->is_open()) {
                    std::cerr << "Error open file" << std::endl;
                    break;
                }

                *fout << argv[2] << " " << GetTime() << std::endl;
                fout->close();
                std::cout << "Record resource " << argv[3] << std::endl;
                Sleep(2000);
            }

            lobuf = sendto(cS, (char*)MESSAGE_LEAVE, MAX_MESSAGE, 0, (sockaddr*)&clnt, lc);
            if (lobuf == SOCKET_ERROR)
                throw SetErrorMsgText("send:", WSAGetLastError());

            LeaveCA(ca);
            std::cout << "--------------------" << std::endl << std::endl;
        }

        CloseCA(ca);

        if (closesocket(cS) == SOCKET_ERROR)
            throw SetErrorMsgText("closesocket: ", WSAGetLastError());

        if (WSACleanup() == SOCKET_ERROR)
            throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
        system("pause");
    }
    catch (string errorMsgText)
    {
        cout << endl << errorMsgText << endl;
        system("pause");
    }
    return 0;
}