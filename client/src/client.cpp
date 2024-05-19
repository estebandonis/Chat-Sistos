#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include "./message.h"  // Replace with the path to your message_util.h
#include "./chat.pb.h"       // Generated Protocol Buffers code

std::deque<std::string> messages;

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
          std::string type = (mensaje.type() == chat::MessageType::BROADCAST) ? "Broadcast" : "Direct";
          message = "<" + type + ">" + " [" + mensaje.sender() + "]" + ": " + mensaje.content();
          std::cout << message << "\n";
        }
      }
    } else {
      std::cout << response.operation() << std::endl;
    }
  }
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
  std::cin.ignore(); // Clear the newline character from the input buffer
  std::getline(std::cin, mensaje);

  chat::Request request;
  request.set_operation(chat::Operation::SEND_MESSAGE);
  auto *newMensaje = request.mutable_send_message();
  newMensaje->set_content(mensaje);

  sendMessage(clientSocket, request);
}

void sendMessageDirect(int clientSocket, const std::string& message) {
  std::string recipient;
  std::cout << "Enter recipient username: ";
  std::cin >> recipient;
  
  std::string mensaje;
  std::cout << "Enter message: ";
  std::cin.ignore(); // Clear the newline character from the input buffer
  std::getline(std::cin, mensaje);

  chat::Request request;
  request.set_operation(chat::Operation::SEND_MESSAGE);
  auto *newMensaje = request.mutable_send_message();
  newMensaje->set_content(mensaje);
  newMensaje->set_recipient(recipient);

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
  if (receiveMessage(clientSocket, response)) {
    std::cout << "Received message from server - Type: " << response.message() << std::endl;
  } else {
    std::cerr << "Error receiving message from the server\n";
    close(clientSocket);
    return -1;
  }

  std::cout << "Regreso del servidor: " << response.message();

  std::thread receiver(messageReceiver, clientSocket);
  receiver.detach();

  int choice = 0;
  while (choice != 7)
  {
    std::cout << "Ingrese un número basándose en las opciones siguientes" << "\n";
    std::cout << "(1) Chatear con todos los usuarios" << "\n";
    std::cout << "(2) Enviar y recibir mensajes directos, privados, aparte del chat general" << "\n";
    std::cout << "(3) Cambiar de status" << "\n";
    std::cout << "(4) Listar los usuarios conectados al sistema de chat" << "\n";
    std::cout << "(5) Desplegar información de un usuario en particular" << "\n";
    std::cout << "(6) Ayuda" << "\n";
    std::cout << "(7) Salir" << "\n";
    std::cout << "Ingrese el número: \n";
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        sendMessageBroadcast(clientSocket, userName);
        break;
    case 2:
        sendMessageDirect(clientSocket, userName);
        break;
    case 3:
        std::cout << "opcion 3" << "\n";
        break;
    case 4:
        std::cout << "opcion 4" << "\n";
        break;
    case 5:
        std::cout << "opcion 5" << "\n";
        break;
    case 6:
        std::cout << "opcion 6" << "\n";
        break;
    case 7:
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