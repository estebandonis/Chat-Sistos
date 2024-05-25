#include <iostream>
#include <string>
#include <deque>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include "protocol/message.h"  
#include "protocol/chat.pb.h"    

std::deque<std::string> messages; // Cola para ir guardando los mensajes broadcast
std::atomic<bool> receivingResponse{true}; // Variable que detemina si se sigue recibiendo responses del servidor
std::unordered_map<std::string, std::deque<std::string>> privateMessages; // Mapa para guardar los mensajes privados

/**
 * Escucha los responses del servidor
 *
 * @param clientSocket Interger que posee el socket utilizado por nuestro cliente.
 */
void messageReceiver(int clientSocket) {
  // Mientras se siga recibiendo mensajes del servidor y la función receivingResponse sea verdadera
  while (receivingResponse == true) {
    // Se crea un objeto de tipo chat::Response para guardar la respuesta del servidor
    chat::Response response;
    // Si se recibe un mensaje del servidor
    if (receiveMessage(clientSocket, response)) {
      // Se verifica si el status code de la respuesta es diferente de OK
      if (response.status_code() != chat::StatusCode::OK) {
        // En caso de que no sea OK, se imprime un mensaje de error
        std::cerr << "There has been an error from the server\n";
        std::cerr << "Status code: " << response.status_code() << "\n";
        std::cerr << "Message: " << response.message() << "\n";
      } 
      // Se verifica si la operación son de tipo mensaje entrante
      else if (response.operation() == chat::Operation::INCOMING_MESSAGE) {
        // Se verifica que si exista un mensaje entrante
        if (response.has_incoming_message()) {
          // Se crea un string para guardar el mensaje
          std::string message;
          const auto &mensaje = response.incoming_message();
          // Se verifica si el mensaje es de tipo broadcast
          if (mensaje.type() == chat::MessageType::BROADCAST){
            // Se guarda el mensaje en la variable message, con el tag de broadcast
            std::string type = "Broadcast";
            message = "<" + type + ">" + " [" + mensaje.sender() + "]" + ": " + mensaje.content();
            // Se guarda el mensaje en la cola de mensajes
            {
              std::lock_guard<std::mutex> lock(messagesMutex);
              messages.push_back(message);
            }
          } else {
            // Se guarda el mensaje en la variable message, con el tag de direct
            std::string type = "Direct";
            message = "<" + type + ">" + " [" + mensaje.sender() + "]" + ": " + mensaje.content();
            // Se guarda el mensaje en el mapa de mensajes privados, utilizando un mutex lock para evitar problemas de concurrencia
            {
              std::lock_guard<std::mutex> lock(privateMessagesMutex);
              privateMessages[mensaje.sender()].push_back(message);
            }
          }
        }
      } 
      // Si la operacion es de obtener usuarios
      else if (response.operation() == chat::Operation::GET_USERS) {
        // Se obtiene la lista de usuarios
        const auto &user_list = response.user_list();
        // Se verifica si la lista de usuarios es de tipo single
        if (user_list.type() == chat::UserListType::SINGLE){
          // Si es asi se obtiene el status del usuario, y se coloca de manera adecuada para su impresion
          std::string tempStatus;
          if (user_list.users(0).status() == chat::UserStatus::ONLINE){
            tempStatus = "Online";
          } else if (user_list.users(0).status() == chat::UserStatus::BUSY){
            tempStatus = "Busy";
          } else {
            tempStatus = "Offline";
          }
          // Se imprime la informacion del usuario
          std::cout << "User info: \n" << "Username: " + user_list.users(0).username() << "\n" << "Status: " << tempStatus << "\n";

        } 
        // Si la lista de usuarios es de tipo all
        else if (user_list.type() == chat::UserListType::ALL){
          // Se imprime la cantidad de usuarios conectados, con sus respectivos status y nombres
          std::cout << "Connected users: " << "\n";
          for (const auto &user : response.user_list().users()) {
            std::string tempStatus;
            if (user.status() == chat::UserStatus::ONLINE){
              tempStatus = "Online";
            } else if (user.status() == chat::UserStatus::BUSY){
              tempStatus = "Busy";
            } else {
              tempStatus = "Offline";
            }
            // Se imprime la informacion del usuario
            std::cout << "Username: " << user.username() << "\n" << "Status: " << tempStatus << "\n";
          }
          std::cout << "\n";
        }
      } 
      // Si la operacion es de tipo update status
      else if (response.operation() == chat::Operation::UPDATE_STATUS) {
        // Se imprime el mensaje del servidor
        std::cout << response.message() << "\n";
      }
    }
  }
}

/**
 * Imprime los mensajes de broadcast
 */
void BroadcastMessagesPrinter(){
  std::cout << "***********************************" << "\n";
  std::cout << "Broadcast Messages: " << "\n";
  std::cout << "***********************************" << "\n";
  for (const auto& message : messages) {
    std::cout << message << "\n";
  }
  std::cout << "***********************************" << "\n\n";
}

/**
 * Imprime los mensajes directos
 */
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

/**
 * Envia mensaje al servidor para desregistrar al usuario
 *
 * @param clientSocket Interger que posee el socket utilizado por nuestro cliente.
 * @param userName variable tipo string que posee el nombre de usuario de nuestro cliente.
 */
void unregisterUser(int clientSocket, const std::string& userName) {
  // Se desactiva la variable receivingResponse
  receivingResponse = false;
  // Se crea un objeto de tipo chat::Request para enviar la solicitud al servidor
  chat::Request request;
  // Se establece la operacion de unregister user
  request.set_operation(chat::Operation::UNREGISTER_USER);
  auto *unregisterUser = request.mutable_unregister_user();
  // Se establece el nombre de usuario de nuestro cliente
  unregisterUser->set_username(userName);

  // Se envia el request al servidor
  sendMessage(clientSocket, request);
}

/**
 * Envia mensaje al servidor
 *
 * @param clientSocket Interger que posee el socket utilizado por nuestro cliente.
 * @param message Mensaje a enviar por canal broadcast.
 */
void sendMessageBroadcast(int clientSocket, const std::string& message) {
  // Se crea el string para guardar el mensaje
  std::string mensaje;
  // Se solicita al usuario que ingrese el mensaje
  std::cout << "Enter message to broadcast: ";
  std::cin.ignore(); // Se limpia el buffer
  std::getline(std::cin, mensaje);

  // Se crea un objeto de tipo chat::Request para enviar la solicitud al servidor
  chat::Request request;
  // Se establece la operacion de send message
  request.set_operation(chat::Operation::SEND_MESSAGE);
  auto *newMensaje = request.mutable_send_message();
  // Se establece el contenido del mensaje
  newMensaje->set_content(mensaje);

  // Se envia el request al servidor
  sendMessage(clientSocket, request);
}

/**
 * Envia mensaje al servidor el request del mensaje directo
 *
 * @param clientSocket Interger que posee el socket utilizado por nuestro cliente.
 * @param userName Username de nuestro cliente.
 */
void sendMessageDirect(int clientSocket, const std::string& userName) {
  // Se solicita al usuario que ingrese el nombre del destinatario
  std::string recipient;
  std::cout << "Enter recipient username: ";
  std::cin >> recipient;
  
  // Se solicita al usuario que ingrese el mensaje
  std::string mensaje;
  std::cout << "Enter message: ";
  std::cin.ignore();
  std::getline(std::cin, mensaje);

  // Se crea un objeto de tipo chat::Request para enviar la solicitud al servidor
  chat::Request request;
  // Se establece la operacion de send message
  request.set_operation(chat::Operation::SEND_MESSAGE);
  auto *newMensaje = request.mutable_send_message();
  // Se establece el contenido del mensaje
  newMensaje->set_content(mensaje);
  // Se establece el destinatario del mensaje
  newMensaje->set_recipient(recipient);

  // Se envia el request al servidor
  sendMessage(clientSocket, request);

  // Se crea un objeto de tipo chat::Response para esperar la respuesta del servidor
  chat::Response response;
  // Si se recibe un mensaje del servidor
  if (receiveMessage(clientSocket, response)){
    // Y si el status code de la respuesta es OK
    if (response.status_code() == chat::StatusCode::OK){
      // Se guarda el mensaje en el mapa de mensajes privados
      std::string type = "Direct";
      std::string tempMessage;
      tempMessage = "<" + type + ">" + " [" + userName + "]" + ": " + mensaje;
      {
        std::lock_guard<std::mutex> lock(privateMessagesMutex);
        privateMessages[recipient].push_back(tempMessage);
      }
    } else {
      std::cerr << "Error sending message: " << response.message() << "\n";
    }
  }
}

/**
 * Hace el request al servidor para obtener la lista de usuarios
 *
 * @param clientSocket Interger que posee el socket utilizado por nuestro cliente.
 */
void requestUsersList(int clientSocket) {
  // Se crea un objeto de tipo chat::Request para enviar la solicitud al servidor
  chat::Request request;
  request.set_operation(chat::Operation::GET_USERS);
  auto *userList = request.mutable_get_users();
  // No se coloca ningun username, por lo que se obtendran todos los usuarios

  sendMessage(clientSocket, request);
}

/**
 * Hace el request al servidor para obtener la información de un usuario en particular
 *
 * @param clientSocket Interger que posee el socket utilizado por nuestro cliente.
 */
void requestUserInfo(int clientSocket) {
  // Se solicita al usuario que ingrese el nombre del usuario a buscar
  std::string userRequested;
  std::cout << "Enter the username to get info: ";
  std::cin >> userRequested;

  // Se crea un objeto de tipo chat::Request para enviar la solicitud al servidor
  chat::Request request;
  request.set_operation(chat::Operation::GET_USERS);
  auto *userInfo = request.mutable_get_users();
  // Se establece el nombre de usuario a buscar
  userInfo->set_username(userRequested);

  sendMessage(clientSocket, request);
}

/**
 * Cambia el status del usuario
 *
 * @param clientSocket Interger que posee el socket utilizado por nuestro cliente.
 * @param status Entero que posee el status del usuario.
 */
void changeStatus(int clientSocket, int status) {
  // Se crea un objeto de tipo chat::Request para enviar la solicitud al servidor
  chat::Request request;
  // Se establece la operacion de update status
  request.set_operation(chat::Operation::UPDATE_STATUS);
  auto *status_request = request.mutable_update_status();
  // Se traduce la eleccion del usuario con el status del usuario
  if (status == 0){
    status_request->set_new_status(chat::UserStatus::ONLINE);
  } else if (status == 1){
    status_request->set_new_status(chat::UserStatus::BUSY);
  } else {
    status_request->set_new_status(chat::UserStatus::OFFLINE);
  }

  sendMessage(clientSocket, request);
}

/**
 * Función principal del cliente.
 *
 * @param argc Cantidad de argumentos de la línea de comandos.
 * @param argv Argumentos de la línea de comandos.
 */
int main(int argc, char* argv[]) {
  // Verificar que se hayan pasado los argumentos correctos
  if (argc != 4) {
    std::cerr << "Usage: client <user_name> <server_ip> <server_port>\n";
    return 1;
  }

  // Extraer los argumentos de la linea de comando
  std::string userName = argv[1];
  std::string serverIP = argv[2];
  int serverPort = std::stoi(argv[3]);

  // Creamos el socket tcp
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket < 0) {
    std::cerr << "Error creating socket\n";
    return 1;
  }

  // Nos conectamos al servidor
  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  // Introducimos la direccion IP
  serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
  // Introducimos el puerto
  serverAddress.sin_port = htons(serverPort);

  // Nos conectamos al servidor
  if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
    std::cerr << "Error connecting to the server\n";
    return 1;
  }

  std::cout << "Connected to the server\n";

  // Creamos un objeto de tipo chat::Request para enviar la solicitud al servidor
  chat::Request request;
  // Establecemos la operacion de register user
  request.set_operation(chat::Operation::REGISTER_USER);
  auto *registerUser = request.mutable_register_user();
  // Establecemos el nombre de usuario
  registerUser->set_username(userName);

  // Enviamos el request al servidor
  sendMessage(clientSocket, request);

  std::cout << "Request sent to the server\n";

  // Creamos un objeto de tipo chat::Response para recibir la respuesta del servidor
  chat::Response response;
  // Si recibimos un mensaje del servidor
  if (receiveMessage(clientSocket, response)){
    // Imprimimos el mensaje recibido
    std::cout << "Received message from server - Type: " << response.message() << std::endl;
    // Si el status code de la respuesta es diferente de OK
    if (response.status_code() == chat::StatusCode::INTERNAL_SERVER_ERROR){
      // Se imprime un mensaje de error
      std::cerr << "Error registering user: " << response.message() << "\n";
      // Se cierra la conexion
      close(clientSocket);
      // Se retorna -1 para indicar un error
      return -1;
    }
  }
  // Si no se recibe un mensaje del servidor 
  else {
    std::cerr << "Error receiving message from the server\n";
    close(clientSocket);
    return -1;
  }

  std::cout << "Regreso del servidor: " << response.message();

  // Se crea un hilo para recibir los mensajes del servidor
  std::thread receiver(messageReceiver, clientSocket);
  receiver.detach();

  // Se crea un entero para guardar la eleccion del usuario
  int choice = 0;
  // Mientras la eleccion del usuario sea diferente de 9
  while (choice != 9)
  {
    // Se imprime el menu de opciones
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

    // Se crea un switch para manejar las opciones del usuario
    switch (choice)
    {
    case 1:
        // En caso de que la eleccion sea 1, se envia un mensaje broadcast
        sendMessageBroadcast(clientSocket, userName);
        break;
    case 2:
        // En caso de que la eleccion sea 2, se imprimen los mensajes broadcast
        BroadcastMessagesPrinter();
        break;
    case 3:
        // En caso de que la eleccion sea 3, se envia un mensaje directo
        sendMessageDirect(clientSocket, userName);
        break;
    case 4:
        // En caso de que la eleccion sea 4, se imprimen los mensajes directos
        DirectMessagesPrinter();
        break;
    case 5:
        // En caso de que la eleccion sea 5, se cambia el status del usuario
        int status;
        // Se solicita al usuario que ingrese el status deseado
        std::cout << "Ingrese el status deseado: " << "\n";
        std::cout << "(0) Online" << "\n";
        std::cout << "(1) Busy" << "\n";
        std::cout << "(2) Offline" << "\n";
        std::cin >> status;
        changeStatus(clientSocket, status);
        break;
    case 6:
        // En caso de que la eleccion sea 6, se solicita la lista de usuarios
        requestUsersList(clientSocket);
        break;
    case 7:
        // En caso de que la eleccion sea 7, se solicita la informacion de un usuario en particular
        requestUserInfo(clientSocket);
        break;
    case 8:
        // En caso de que la eleccion sea 8, se imprime la ayuda
        std::cout << "Ayuda" << "\n";
        break;
    case 9:
        // En caso de que la eleccion sea 9, se desregistra al usuario
        unregisterUser(clientSocket, userName);
        // Se espera el mensaje de respuesta del servidor
        chat::Response response;
        if (receiveMessage(clientSocket, response)){
          std::cout << "Servidor: " << response.message() << std::endl;
        }
        break;
    default:
        // En caso de que la eleccion no sea valida, se imprime un mensaje de error
        std::cout << "Opción no válida" << "\n";
        break;
    }
  }

  // Se imprime un mensaje de cierre de conexion
  std::cout << "Closing connection\n";

  // Se espera que el hilo de recepcion de mensajes termine
  if (receiver.joinable())
  {
    receiver.join();
  }

  // Se cierra el socket
  close(clientSocket);
  // Se retorna 0 para indicar que todo ha salido bien
  return 0;
}