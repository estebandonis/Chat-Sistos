#include <iostream>
#include <string>
#include <unordered_set>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.h"  // Replace with the path to your message_util.h
#include "chat.pb.h"  // Generated Protocol Buffers code

std::unordered_set<std::string> connectedUsers;
std::vector<std::thread> clientThreads;

void handleClient(int clientSocket) {
    while (true) {
        chat::Request request;
        if (!receiveMessage(clientSocket, request)) {
            std::cerr << "Error receiving request or client disconnected\n";
            close(clientSocket);
            return;
        }

        chat::Response response;
        if (request.operation() == chat::Operation::REGISTER_USER) {
            std::string username = request.register_user().username();
            connectedUsers.insert(username);
            std::cout << "User registered: " << username << "\n";

            for (const auto& user : connectedUsers) {
                std::cout << user << "\n";
            }

            response.set_message("User registered successfully");
        } else if (request.operation() == chat::Operation::UNREGISTER_USER) {
            std::string username = request.unregister_user().username();
            connectedUsers.erase(username);
            std::cout << "User unregistered: " << username << "\n";
            response.set_message("User unregistered successfully");
            close(clientSocket);
            return;
        } else {
            std::cerr << "Unknown operation\n";
            response.set_message("Unknown operation");
        }

        if (!sendMessage(clientSocket, response)) {
            std::cerr << "Error sending response\n";
            close(clientSocket);
            return;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: server <server_ip>\n";
        return 1;
    }

    std::string serverIP = argv[1];
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket: " << strerror(errno) << "\n";
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddress.sin_port = htons(8080);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding socket: " << strerror(errno) << "\n";
        close(serverSocket);
        return 1;
    }

    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Error listening on socket: " << strerror(errno) << "\n";
        close(serverSocket);
        return 1;
    }

    std::cout << "Server started. Listening on port 8080...\n";

    while (true) {
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket < 0) {
            std::cerr << "Error accepting client connection: " << strerror(errno) << "\n";
            continue;
        }

        std::cout << "Client connected\n";
        clientThreads.emplace_back(std::thread(handleClient, clientSocket));
    }

    for (auto& thread : clientThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    close(serverSocket);
    return 0;
}
