// message_util.cpp
#include "./message.h"

bool sendMessage(int socket, const google::protobuf::Message& message) {
    // Serialize the message to a string
    std::string serializedMessage = message.SerializeAsString();

    // Get the size of the serialized message
    uint32_t messageSize = htonl(serializedMessage.size());

    // Send the size of the message
    ssize_t bytesSent = send(socket, &messageSize, sizeof(messageSize), 0);
    if (bytesSent != sizeof(messageSize)) {
        return false;
    }

    // Send the serialized message
    bytesSent = send(socket, serializedMessage.data(), serializedMessage.size(), 0);
    if (bytesSent != serializedMessage.size()) {
        return false;
    }

    return true;
}

bool receiveMessage(int socket, google::protobuf::Message& message) {
    // Receive the size of the message
    uint32_t messageSize;
    ssize_t bytesRead = recv(socket, &messageSize, sizeof(messageSize), MSG_WAITALL);
    if (bytesRead != sizeof(messageSize)) {
        return false;
    }

    // Convert the size of the message to host byte order
    messageSize = ntohl(messageSize);

    // Prepare a buffer to receive the serialized message
    std::vector<uint8_t> buffer(messageSize);

    // Receive the serialized message
    bytesRead = recv(socket, buffer.data(), messageSize, MSG_WAITALL);
    if (bytesRead != messageSize) {
        return false;
    }

    // Parse the received message using protobuf
    if (!message.ParseFromArray(buffer.data(), messageSize)) {
        return false;
    }

    return true;
}