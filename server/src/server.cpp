#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.h"  // Replace with the path to your message_util.h
#include "chat.pb.h"  // Generated Protocol Buffers code

std::vector<std::thread> clientThreads;
std::deque<std::string> messagesBroadcast;
std::unordered_map<std::string, int> userSockets;
std::mutex clientsMutex;

void broadcastMessage(const std::string& message, const std::string& userSender) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    std::cout << "Broadcasting message: " << message << "\n";
    for (const auto& [username, clientSocket] : userSockets) {
        chat::Response response;
        response.set_operation(chat::Operation::INCOMING_MESSAGE);
        response.set_status_code(chat::StatusCode::OK);
        auto *incomingMessage = response.mutable_incoming_message();
        incomingMessage->set_content(message);
        incomingMessage->set_type(chat::MessageType::BROADCAST);
        incomingMessage->set_sender(userSender);
        if (!sendMessage(clientSocket, response)) {
            std::cerr << "Error sending broadcast message to client socket " << clientSocket << "\n";
        }
    }
}

void directMessage(const std::string& message, const std::string& userSender, const std::string& recipient) {
    chat::Response response;
    response.set_operation(chat::Operation::INCOMING_MESSAGE);
    response.set_status_code(chat::StatusCode::OK);
    auto *incomingMessage = response.mutable_incoming_message();
    incomingMessage->set_content(message);
    incomingMessage->set_type(chat::MessageType::DIRECT);
    incomingMessage->set_sender(userSender);
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        if (userSockets.find(recipient) != userSockets.end()) {
            if (!sendMessage(userSockets[recipient], response)) {
                std::cerr << "Error sending direct message to client socket " << userSockets[recipient] << "\n";
            }
        } else {
            response.set_message("Recipient not found");
            if (!sendMessage(userSockets[userSender], response)) {
                std::cerr << "Error sending response to client socket " << userSockets[userSender] << "\n";
            }
        }
    }
}

void handleClient(int clientSocket) {
    std::string username;
    
    while (true) {
        chat::Request request;
        if (!receiveMessage(clientSocket, request)) {
            std::cerr << "Error receiving request or client disconnected\n";
            close(clientSocket);
            break;
        }

        chat::Response response;
        if (request.operation() == chat::Operation::REGISTER_USER) {
            username = request.register_user().username();
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                userSockets[username] = clientSocket;
            }
            std::cout << "User registered: " << username << "\n";

            for (const auto& [user, clientSocket] : userSockets) {
                std::cout << user << "\n";
            }

            response.set_message("User registered successfully");
        } else if (request.operation() == chat::Operation::UNREGISTER_USER) {
            username = request.unregister_user().username();
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                userSockets.erase(username);
            }
            std::cout << "User unregistered: " << username << "\n";
            response.set_message("User unregistered successfully");
            close(clientSocket);
            break;
        } else if (request.operation() == chat::Operation::SEND_MESSAGE) {
            if (request.send_message().recipient() == "") {
                std::string message = request.send_message().content();
                std::cout << "Message received: " << "[" + username + "]" + ": " + message << "\n";
                broadcastMessage(message, username);
            } else {
                std::string recipient = request.send_message().recipient();
                std::string message = request.send_message().content();
                std::cout << "Direct message received: " << "[" + username + "]" + ": " + message << "\n";
                directMessage(message, username, recipient);
            }
            
            continue;
        } else {
            std::cerr << "Unknown operation\n";
            response.set_message("Unknown operation");
        }

        if (!sendMessage(clientSocket, response)) {
            std::cerr << "Error sending response\n";
            close(clientSocket);
            break;
        }
    }

    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        userSockets.erase(username);
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
