// message_util.h
#ifndef MESSAGE_UTIL_H
#define MESSAGE_UTIL_H

#include <vector>
#include <string>
#include <cstdint>                   // For uint32_t
#include <sys/types.h>               // For ssize_t
#include <sys/socket.h>              // For send, recv, and MSG_WAITALL
#include <netinet/in.h>              // For htonl, ntohl
#include <google/protobuf/message.h> // For Google Protobuf

// Function to send a message
bool sendMessage(int socket, const google::protobuf::Message &message);

// Function to receive a message
bool receiveMessage(int socket, google::protobuf::Message &message);

#endif