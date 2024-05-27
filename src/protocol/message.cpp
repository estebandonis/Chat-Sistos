// message_util.cpp
#include "./message.h"

/**
 * Funcion para manejar el envio de mensajes entre el servidor y el cliente
 * 
 * @param socket Socket a donde se enviar el mensaje
 * @param message Mensaje a enviar
*/
bool sendMessage(int socket, const google::protobuf::Message& message) {
    // Serialize the message to a string
    std::string serializedMessage = message.SerializeAsString();

    if (serializedMessage.size() > BufferSize) {
        std::cerr << "El mensaje es muy grande" << std::endl;
        return false;
    }

    // Send the size of the message
    ssize_t bytesSent = send(socket, serializedMessage.data(), serializedMessage.size(), 0);
    if (bytesSent < 0) {
        std::cerr << "Error al enviar el mensaje" << std::endl;
        return false;
    } else if (bytesSent < serializedMessage.size()) {
        std::cerr << "No se envio toda la data" << std::endl;
        return false;
    }

    return true;
}

/**
 * Funcion para manejar el recibir de mensajes entre el servidor y el cliente
 * 
 * @param socket Socket a donde se enviar el mensaje
 * @param message Mensaje a recibir
*/
bool receiveMessage(int socket, google::protobuf::Message& message) {
    // Receive the size of the message
    std::vector<char> buffer(BufferSize);

    ssize_t bytesRead = recv(socket, buffer.data(), buffer.size(), 0);
    if (bytesRead < 0) {
        std::cerr << "Error al recibir el mensaje" << std::endl;
        return false;
    }

    // Parse the received message using protobuf
    if (!message.ParseFromArray(buffer.data(), bytesRead)) {
        std::cerr << "Error al parsear el mensaje" << std::endl;
        return false;
    }

    return true;
}