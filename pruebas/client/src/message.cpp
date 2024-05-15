#include "message.h"
#include <sys/socket.h>
#include <unistd.h>

bool sendMessage(int socket, const Message& message) {
  // Serialize the message into a format suitable for sending
  std::string serializedMessage = std::to_string(message.messageType) + ":" + message.content;

  // Send the serialized message over the socket
  ssize_t bytesSent = send(socket, serializedMessage.c_str(), serializedMessage.length(), 0);

  // Check for errors during sending
  if (bytesSent < 0) {
    std::cerr << "Error sending message\n";
    return false;
  }

  // Check if the entire message was sent
  if (bytesSent != serializedMessage.length()) {
    std::cerr << "Not all bytes of the message were sent\n";
    return false;
  }

  return true;
}

bool receiveMessage(int socket, Message& message) {
  // Buffer to store the received message
  char buffer[1024];
  ssize_t bytesRead = recv(socket, buffer, sizeof(buffer) - 1, 0);

  // Check for errors during receiving
  if (bytesRead < 0) {
    std::cerr << "Error receiving message\n";
    return false;
  }

  // Check if the connection was closed by the other side
  if (bytesRead == 0) {
    std::cerr << "Connection closed by the other side\n";
    return false;
  }

  // Null-terminate the received data
  buffer[bytesRead] = '\0';

  // Parse the received message
  std::string serializedMessage(buffer);
  size_t delimiterPos = serializedMessage.find(':');
  if (delimiterPos == std::string::npos) {
    std::cerr << "Invalid message format\n";
    return false;
  }

  // Extract the message type and content
  std::string typeString = serializedMessage.substr(0, delimiterPos);
  std::string content = serializedMessage.substr(delimiterPos + 1);

  // Convert the message type to an integer
  int messageType;
  try {
    messageType = std::stoi(typeString);
  } catch (const std::exception& e) {
    std::cerr << "Invalid message type\n";
    return false;
  }

  // Set the received message fields
  message.messageType = messageType;
  message.content = content;

  return true;
}