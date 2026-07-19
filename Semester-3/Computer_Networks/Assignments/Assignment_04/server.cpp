#include <iostream>
#include <string>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET)
    {
        cout << "Socket creation failed\n";
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    bind(server_fd, (sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    cout << "Server listening on 127.0.0.1:5000\n";

    SOCKET client_fd = accept(server_fd, NULL, NULL);

    char buffer[1024];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes <= 0)
            break;

        string msg(buffer);

        if (!msg.empty() && msg.back() == '\n')
            msg.pop_back();

        if (_stricmp(msg.c_str(), "quit") == 0)
        {
            string reply = "bye\n";
            send(client_fd, reply.c_str(), reply.length(), 0);
            cout << "Received=" << msg << " | Replied=bye\n";
            break;
        }

        string reversed = msg;
        reverse(reversed.begin(), reversed.end());

        string reply = "echo:" + msg + " | rev:" + reversed + "\n";
        send(client_fd, reply.c_str(), reply.length(), 0);

        cout << "Received=" << msg << " | Replied=" << reply;
    }

    closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
