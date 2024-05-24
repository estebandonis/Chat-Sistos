#include <iostream>
#include <string>
#include <deque>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "protocol/message.h"  
#include "protocol/chat.pb.h"    

std::deque<std::string> messages;
std::unordered_map<std::string, std::deque<std::string>> privateMessages;

void messageReceiver(int clientSocket) {
  while (true) {
    chat::Response response;
    if (receiveMessage(clientSocket, response)) {
      if (response.status_code() != chat::StatusCode::OK) {
        std::cerr << "Error receiving message from the server\n";
        break;
      } else if (response.operation() == chat::Operation::INCOMING_MESSAGE) {
        if (response.has_incoming_message()) {
          std::string message;
          const auto &mensaje = response.incoming_message();
          if (mensaje.type() == chat::MessageType::BROADCAST){
            std::string type = "Broadcast";
            message = "<" + type + ">" + " [" + mensaje.sender() + "]" + ": " + mensaje.content();
            messages.push_back(message);
          } else {
            std::string type = "Direct";
            message = "<" + type + ">" + " [" + mensaje.sender() + "]" + ": " + mensaje.content();
            privateMessages[mensaje.sender()].push_back(message);
          }
        }
      } else if (response.operation() == chat::Operation::GET_USERS) {
        const auto &user_list = response.user_list();
        if (user_list.type() == chat::UserListType::SINGLE){
          std::cout << "User info: \n" << "Username: " + user_list.users(0).username() << "\n";
        } else if (user_list.type() == chat::UserListType::ALL){
          std::cout << "Connected users: ";
          for (const auto &user : response.user_list().users()) {
            std::cout << user.username() << " ";
          }
          std::cout << "\n";
        }
      }
    } else {
      std::cout << response.operation() << std::endl;
    }
  }
}

void BroadcastMessagesPrinter(){
  std::cout << "***********************************" << "\n";
  std::cout << "Broadcast Messages: " << "\n";
  std::cout << "***********************************" << "\n";
  for (const auto& message : messages) {
    std::cout << message << "\n";
  }
  std::cout << "***********************************" << "\n\n";
}

void DirectMessagesPrinter(){
  std::cout << "***********************************" << "\n";
  std::cout << "Direct Messages: " << "\n";
  std::cout << "***********************************" << "\n";
  for (const auto& [sender, messages] : privateMessages) {
    std::cout << "Messages from " << sender << ": " << "\n";
    for (const auto& message : messages) {
      std::cout << message << "\n";
    }
    std::cout << "\n";
  }
  std::cout << "***********************************" << "\n\n";
}

void unregisterUser(int clientSocket, const std::string& userName) {
  chat::Request request;
  request.set_operation(chat::Operation::UNREGISTER_USER);
  auto *unregisterUser = request.mutable_unregister_user();
  unregisterUser->set_username(userName);

  sendMessage(clientSocket, request);

  chat::Response response;
  if (receiveMessage(clientSocket, response))
  {
    std::cout << "Server response: " << response.message() << std::endl;
  } else {
    std::cerr << "Connection closed." << std::endl;
  }
  std::cout << "Exiting..." << std::endl;
}

void sendMessageBroadcast(int clientSocket, const std::string& message) {
  std::string mensaje;
  std::cout << "Enter message to broadcast: ";
  std::cin.ignore();
  std::getline(std::cin, mensaje);

  chat::Request request;
  request.set_operation(chat::Operation::SEND_MESSAGE);
  auto *newMensaje = request.mutable_send_message();
  newMensaje->set_content(mensaje);

  sendMessage(clientSocket, request);
}

void sendMessageDirect(int clientSocket, const std::string& userName) {
  std::string recipient;
  std::cout << "Enter recipient username: ";
  std::cin >> recipient;
  
  std::string mensaje;
  std::cout << "Enter message: ";
  std::cin.ignore();
  std::getline(std::cin, mensaje);

  chat::Request request;
  request.set_operation(chat::Operation::SEND_MESSAGE);
  auto *newMensaje = request.mutable_send_message();
  newMensaje->set_content(mensaje);
  newMensaje->set_recipient(recipient);
  std::string type = "Direct";
  std::string tempMessage;
  tempMessage = "<" + type + ">" + " [" + userName + "]" + ": " + mensaje;
  privateMessages[recipient].push_back(tempMessage);

  sendMessage(clientSocket, request);
}

void requestUsersList(int clientSocket) {
  chat::Request request;
  request.set_operation(chat::Operation::GET_USERS);
  auto *userList = request.mutable_get_users();

  sendMessage(clientSocket, request);
}

void requestUserInfo(int clientSocket) {
  std::string userRequested;
  std::cout << "Enter the username to get info: ";
  std::cin >> userRequested;

  chat::Request request;
  request.set_operation(chat::Operation::GET_USERS);
  auto *userInfo = request.mutable_get_users();
  userInfo->set_username(userRequested);

  sendMessage(clientSocket, request);
}

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

  chat::Request request;
  request.set_operation(chat::Operation::REGISTER_USER);
  auto *registerUser = request.mutable_register_user();
  registerUser->set_username(userName);

  sendMessage(clientSocket, request);

  std::cout << "Request sent to the server\n";

  chat::Response response;
  if (receiveMessage(clientSocket, response)){
    std::cout << "Received message from server - Type: " << response.message() << std::endl;
    if (response.status_code() == chat::StatusCode::INTERNAL_SERVER_ERROR){
      std::cerr << "Error registering user: " << response.message() << "\n";
      close(clientSocket);
      return -1;
    }
  } else {
    std::cerr << "Error receiving message from the server\n";
    close(clientSocket);
    return -1;
  }

  std::cout << "Regreso del servidor: " << response.message();

  std::thread receiver(messageReceiver, clientSocket);
  receiver.detach();

  int choice = 0;
  while (choice != 9)
  {
    std::cout << "Ingrese un número basándose en las opciones siguientes" << "\n";
    std::cout << "(1) Chatear con todos los usuarios" << "\n";
    std::cout << "(2) Mostrar mensajes Broadcast" << "\n";
    std::cout << "(3) Enviar y recibir mensajes directos, privados, aparte del chat general" << "\n";
    std::cout << "(4) Mostrar mensajes Directos" << "\n";
    std::cout << "(5) Cambiar de status" << "\n";
    std::cout << "(6) Listar los usuarios conectados al sistema de chat" << "\n";
    std::cout << "(7) Desplegar información de un usuario en particular" << "\n";
    std::cout << "(8) Ayuda" << "\n";
    std::cout << "(9) Salir" << "\n";
    std::cout << "Ingrese el número: \n";
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        sendMessageBroadcast(clientSocket, userName);
        break;
    case 2:
        BroadcastMessagesPrinter();
        break;
    case 3:
        sendMessageDirect(clientSocket, userName);
        break;
    case 4:
        DirectMessagesPrinter();
        break;
    case 5:
        std::cout << "Cambio de estado" << "\n";
        break;
    case 6:
        requestUsersList(clientSocket);
        break;
    case 7:
        requestUserInfo(clientSocket);
        break;
    case 8:
        std::cout << "Ayuda" << "\n";
        break;
    case 9:
        unregisterUser(clientSocket, userName);
        break;
    default:
        std::cout << "Opción no válida" << "\n";
        break;
    }
  }

  std::cout << "Closing connection\n";

  if (receiver.joinable())
  {
    receiver.join();
  }

  close(clientSocket);
  return 0;
}