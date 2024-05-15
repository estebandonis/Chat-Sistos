#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "./chat.pb.h" // Generated Protocol Buffers code

int main(int argc, char* argv[]) {
  // Check the number of command-line arguments
  if (argc != 2) {
    std::cerr << "Usage: server <server_ip>\n";
    return 1;
  }

  // Extract the command-line argument
  std::string serverIP = argv[1];

  // Create a TCP socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket < 0) {
    std::cerr << "Error creating socket\n";
    return 1;
  }

  // Bind the socket to a specific IP address and port
  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str()); // Accept connections on any interface
  serverAddress.sin_port = htons(8080); // Port number
  if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    std::cerr << "Error binding socket\n";
    return 1;
  }

  // Listen for incoming connections
  if (listen(serverSocket, 1) < 0) {
    std::cerr << "Error listening on socket\n";
    return 1;
  }

  std::cout << "Server started. Listening on port 8080...\n";

  while (true) {
    // Accept a client connection
    sockaddr_in clientAddress{};
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
      std::cerr << "Error accepting client connection\n";
      continue;
    }

    std::cout << "Client connected\n";

    // Receive the request from the client
    chat::Request request;
    char buffer[1024];
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead < 0) {
      std::cerr << "Error receiving request\n";
      close(clientSocket);
      continue;
    }

    // Parse the received data using Protocol Buffers
    if (!request.ParseFromArray(buffer, bytesRead)) {
      std::cerr << "Error parsing request\n";
      close(clientSocket);
      continue;
    }

    std::cout << "Received request: " << request.mutable_send_message() << "\n";

    // Process the request and prepare the response
    chat::Response response;
    // response.set_status(200);
    response.set_message("Request received successfully");

    // Serialize theresponse into a binary format
    std::string serializedResponse;
    if (!response.SerializeToString(&serializedResponse)) {
      std::cerr << "Error serializing response\n";
      close(clientSocket);
      continue;
    }

    // Send the response to the client
    if (send(clientSocket, serializedResponse.c_str(), serializedResponse.length(), 0) < 0) {
      std::cerr << "Error sending response\n";
    }

    // Close the client connection
    close(clientSocket);
    std::cout << "Response sent\n";
  }

  // Close the server socket
  close(serverSocket);

  return 0;
}