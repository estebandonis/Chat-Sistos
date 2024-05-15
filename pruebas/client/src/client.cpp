#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "message.h"

int main(int argc, char* argv[]) {
  // Check the number of command-line arguments
  if (argc != 4) {
    std::cerr << "Usage: client <user_name> <server_ip> <server_port>\n";
    return 1;
  }

  // Extract the command-line arguments
  std::string userName = argv[1];
  std::string serverIP = argv[2];
  int serverPort = std::stoi(argv[3]);

  // Create a TCP socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    std::cerr << "Error creating socket\n";
    return 1;
  }

  // Connect to the server
  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
  serverAddress.sin_port = htons(serverPort);

  if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    std::cerr << "Error connecting to the server\n";
    return 1;
  }

  std::cout << "Connected to the server\n";

  // Receive a message from the server
  Message receivedMessage;
  if (!receiveMessage(clientSocket, receivedMessage)) {
    std::cerr << "Error receiving message from the server\n";
    close(clientSocket);
    return 1;
  }

  // Process the received message
  int messageType = receivedMessage.messageType;
  std::string content = receivedMessage.content;
  std::cout << "Received message from server - Type: " << messageType << ", Content: " << content << std::endl;

  // Send a message to the server
  Message message;
  message.messageType = 2;
  message.content = "Hello, server! This is " + userName;

  if (!sendMessage(clientSocket, message)) {
    std::cerr << "Error sending message to the server\n";
    close(clientSocket);
    return 1;
  }

  // Close the client socket
  close(clientSocket);

  return 0;
}