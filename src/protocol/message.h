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

// Indicating the static size of the buffer
constexpr size_t BufferSize = 64 * 1024;

/**
 * Funcion para manejar el envio de mensajes entre el servidor y el cliente
 * 
 * @param socket Socket a donde se enviar el mensaje
 * @param message Mensaje a enviar
*/
bool sendMessage(int socket, const google::protobuf::Message &message);

/**
 * Funcion para manejar el recibir de mensajes entre el servidor y el cliente
 * 
 * @param socket Socket a donde se enviar el mensaje
 * @param message Mensaje a recibir
*/
bool receiveMessage(int socket, google::protobuf::Message &message);

#endif