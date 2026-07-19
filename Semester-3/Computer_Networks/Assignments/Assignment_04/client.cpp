#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (sockaddr *)&server_addr, sizeof(server_addr));

    while (true)
    {
        string input;
        cout << "Client> ";
        getline(cin, input);

        input += "\n";
        send(sock, input.c_str(), input.length(), 0);

        char buffer[1024]{};
        recv(sock, buffer, sizeof(buffer), 0);

        cout << "Server> " << buffer;

        if (input == "quit\n")
            break;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
