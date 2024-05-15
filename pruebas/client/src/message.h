#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>

// Define your message structure
struct Message {
  int messageType;
  std::string content;
};

// Function to send a message
bool sendMessage(int socket, const Message& message);

// Function to receive a message
bool receiveMessage(int socket, Message& message);

#endif  // MESSAGE_H