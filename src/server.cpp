#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "protocol/message.h" 
#include "protocol/chat.pb.h"  

std::vector<std::thread> clientThreads; // Vector donde se almacenan los threads de los clientes
std::deque<std::string> messagesBroadcast; // Variable donde almacenamos todos los mensajes en broadcast 
std::unordered_map<std::string, chat::UserStatus> usersState; // Variable donde almacenamos el status de los usuarios
std::unordered_map<std::string, int> userSockets; // Variable donde almacenamos los sockets de los usuarios
std::unordered_map<std::string, std::string> ipsUsers; // Variable donde almacenamos las ips de los usuarios
std::mutex clientsMutex; // Mutex para proteger las variables compartidas
std::unordered_map<std::string, int> usersTiming; // Variable donde almacenamos el tiempo de inactividad de los usuarios
int waitTime = 20; // Variable donde se guarda el tiempo de inactividad predeterminado

/**
 * Funcion que maneja el envío de mensajes broadcast a través de un socket
 * @param message Mensaje a enviar en broadcast
 * @param userSender Usuario que envía el mensaje
 */
void broadcastMessage(const std::string& message, const std::string& userSender) {
    // Bloqueamos el mutex para proteger la variable userSockets
    std::lock_guard<std::mutex> lock(clientsMutex);
    // Iteramos sobre todos los sockets de los usuarios
    std::cout << "Broadcasting message: " << message << "\n";
    for (const auto& [username, clientSocket] : userSockets) {
        // Creamos un mensaje de respuesta
        chat::Response response;
        response.set_operation(chat::Operation::INCOMING_MESSAGE);
        response.set_status_code(chat::StatusCode::OK);
        auto *incomingMessage = response.mutable_incoming_message();
        // Definimos el contenido del mensaje, tipo y el usuario que lo envía
        incomingMessage->set_content(message);
        incomingMessage->set_type(chat::MessageType::BROADCAST);
        incomingMessage->set_sender(userSender);
        // Enviamos el mensaje a través del socket
        if (!sendMessage(clientSocket, response)) {
            std::cerr << "Error sending broadcast message to client socket " << clientSocket << "\n";
        }
    }
}

/**
 * Funcion que maneja el envío de mensajes directos a través de un socket
 * 
 * @param message Mensaje a enviar en directo
 * @param userSender Usuario que envía el mensaje
 * @param recipient Usuario que recibe el mensaje
 */
void directMessage(const std::string& message, const std::string& userSender, const std::string& recipient) {
    {
        // Bloqueamos el mutex para proteger la variable userSockets
        std::lock_guard<std::mutex> lock(clientsMutex);
        // Verificamos si el usuario destinatario se encuentra en la lista de sockets
        if (userSockets.find(recipient) != userSockets.end()) {
            // Si el usuario se encuentra en la lista de sockets, creamos un mensaje de respuesta
            chat::Response response;
            response.set_operation(chat::Operation::INCOMING_MESSAGE);
            response.set_status_code(chat::StatusCode::OK);
            auto *incomingMessage = response.mutable_incoming_message();
            // Definimos el contenido del mensaje, tipo y el usuario que lo envía
            incomingMessage->set_content(message);
            incomingMessage->set_type(chat::MessageType::DIRECT);
            incomingMessage->set_sender(userSender);
            // Enviamos el mensaje a través del socket
            if (!sendMessage(userSockets[recipient], response)) {
                std::cerr << "Error sending direct message to client socket " << userSockets[recipient] << "\n";
            }

            // Creamos un mensaje de respuesta para el usuario que envía el mensaje
            chat::Response responseSender;
            responseSender.set_operation(chat::Operation::SEND_MESSAGE);
            responseSender.set_message("Message sent successfully.");
            responseSender.set_status_code(chat::StatusCode::OK);
            // Enviamos el mensaje a través del socket
            if (!sendMessage(userSockets[userSender], responseSender)) {
                std::cerr << "Error sending direct message to client socket " << userSockets[userSender] << "\n";
            }
        } else {
            // Si el usuario no se encuentra en la lista de sockets, creamos un mensaje de respuesta
            chat::Response response;
            response.set_operation(chat::Operation::INCOMING_MESSAGE);
            // Definimos el status code como error y el mensaje de error
            response.set_status_code(chat::StatusCode::INTERNAL_SERVER_ERROR);
            response.set_message("Recipient not found");
            // Enviamos el mensaje a través del socket
            if (!sendMessage(userSockets[userSender], response)) {
                std::cerr << "Error sending response to client socket " << userSockets[userSender] << "\n";
            }
        }
    }
}

/**
 * Funcion que envia todos los usuarios conectados
 * 
 * @param clientSocket Socket del cliente a enviar los usuarios
 */
void returnAllUsers(int clientSocket) {
    // Preparamos la respuesta
    chat::Response response;
    response.set_operation(chat::Operation::GET_USERS);
    response.set_status_code(chat::StatusCode::OK);

    // Creamos la lista que va a tener todos los usuarios
    chat::UserListResponse user_list;
    user_list.set_type(chat::UserListType::ALL);
    {
        // Bloqueamos el mutex para proteger la variable usersState
        std::lock_guard<std::mutex> lock(clientsMutex);
        // Recorremos la lista y vamos agregando los usuarios a la lista con su respectivo estado
        for (const auto& [user, state] : usersState){
            chat::User *newUser = user_list.add_users();
            newUser->set_username(user);
            newUser->set_status(state);
        }
    }

    std::cout << "All users fetched successfully." << "\n";

    // Copiamos la lista de usuarios a la respuesta
    response.mutable_user_list()->CopyFrom(user_list);

    // Enviamos la respuesta a través del socket
    if (!sendMessage(clientSocket, response)) {
        std::cerr << "Error sending users list to client socket " << clientSocket << "\n";
    }
}

/**
 * Funcion que envia la informacion de un usuario en especifico
 * 
 * @param clientSocket Socket del cliente a enviar la informacion del usuario
 * @param username Nombre del usuario que se pidio la informacion
 */
void returnUserInfo(int clientSocket, const std::string& username) {
    {
        // Bloqueamos el mutex para proteger la variable userSockets
        std::lock_guard<std::mutex> lock(clientsMutex);
        // Verificamos si el usuario se encuentra en la lista de sockets
        if (userSockets.find(username) == userSockets.end()) {
            // Si el usuario no se encuentra en la lista de sockets, creamos un mensaje de respuesta
            chat::Response response;
            response.set_operation(chat::Operation::GET_USERS);
            response.set_status_code(chat::StatusCode::INTERNAL_SERVER_ERROR);
            response.set_message("User not found");

            if (!sendMessage(clientSocket, response)) {
                std::cerr << "Error sending user info to client socket " << clientSocket << "\n";
            }
        } else {
            // Si el usuario se encuentra en la lista de sockets, creamos un mensaje de respuesta
            chat::Response response;
            response.set_operation(chat::Operation::GET_USERS);
            response.set_status_code(chat::StatusCode::OK);

            chat::UserListResponse user_list;
            user_list.set_type(chat::UserListType::SINGLE);
            chat::User *newUser = user_list.add_users();
            // Agregamos los datos del usuario, incluyendo el IP address
            newUser->set_username(username + " (IP: " + ipsUsers[username] + ")");
            newUser->set_status(usersState[username]);

            std::cout << "User info fetched successfully." << "\n";

            // Copiamos la lista del usuario en la respuesta
            response.mutable_user_list()->CopyFrom(user_list);

            // Enviamos la respuesta a través del socket
            if (!sendMessage(clientSocket, response)) {
                std::cerr << "Error sending user info to client socket " << clientSocket << "\n";
            }
        }
    }
}

/**
 * Maneja el cambio de estado de un usuario
 * 
 * @param clientSocket Socket del cliente a cambiar el estado
 * @param status Nuevo estado del usuario
 */
void changeStatus (int clientSocket, chat::UserStatus status){
    {
        // Bloqueamos el mutex para proteger la variable usersState
        std::lock_guard<std::mutex> lock(clientsMutex);
        // Buscamos el usuario que se quiere cambiar el estado
        std::string userToChange;
        // Recorremos la lista para encontrar el username del socket correspondiente
        for (const auto& [username, socket] : userSockets) {
            if (socket == clientSocket) {
                userToChange = username;
                break;
            }
        }
        // Cambiamos el estado del usuario
        usersState[userToChange] = status;
    }

    // Creamos la respuesta
    chat::Response response;
    response.set_operation(chat::Operation::UPDATE_STATUS);
    response.set_status_code(chat::StatusCode::OK);
    // Mencionamos que el update fue exitoso
    response.set_message("Status updated successfully");

    // Enviamos la respuesta a través del socket
    sendMessage(clientSocket, response);
}

void userScanner() {
    while (true) {
        // Recorremos la lista de sockets
        std::this_thread::sleep_for(std::chrono::seconds(1));
        for (const auto& [username, timer] : usersTiming) {
            // Verificamos si el tiempo de inactividad es mayor al tiempo establecido
            if (timer == waitTime) {
                // Si el tiempo de inactividad es mayor, cambiamos el estado del usuario a OFFLINE
                changeStatus(userSockets[username], chat::UserStatus::OFFLINE);
                usersTiming[username] = timer + 1;
            } else {
                // Si el tiempo de inactividad no es mayor, incrementamos el tiempo de inactividad
                usersTiming[username] = timer + 1;
            }            
        }
    }
}

/**
 * Maneja los requests de los clientes
 * 
 * @param clientSocket Socket del cliente
 * @param clientIp IP del cliente
 */
void handleClient(int clientSocket, std::string clientIp) {
    // Se guarda el username del cliente
    std::string username;
    // Se crea un loop infinito para recibir requests del socket del cliente
    while (true) {
        // Se crea un objeto request para recibir el mensaje
        chat::Request request;
        // Se verifica si el mensaje fue recibido correctamente
        if (!receiveMessage(clientSocket, request)) {
            // En caso no se imprime el error y se cierra el socket del cliente
            std::cerr << "Error receiving request or client disconnected\n";
            {
                // Se bloquea el mutex para proteger la variable usersState
                std::lock_guard<std::mutex> lock(clientsMutex);
                // Se verifica si el usuario se encuentra en la lista de sockets, en caso no se haya eliminado previamente, después de su desregistro
                if (userSockets.find(username) != userSockets.end()) {
                    usersState.erase(username);
                    userSockets.erase(username);
                    ipsUsers.erase(username);
                    usersTiming.erase(username);
                }
            }
            close(clientSocket);
            break;
        }
        // Se verifica el tipo de operacion que se quiere realizar
        if (request.operation() == chat::Operation::REGISTER_USER) {
            // Si se quiere registrar un usuario se crea un response
            chat::Response response;
            // Se obtiene el username del request
            username = request.register_user().username();
            {
                // Se bloquea el mutex para proteger la variable usersState
                std::lock_guard<std::mutex> lock(clientsMutex);
                // Se verifica si el username ya existe
                if (usersState.find(username) != usersState.end()) {
                    // Si ya existe se envia un mensaje de error y se cierra el socket del cliente
                    std::cout << "Username already taken\n";
                    response.set_message("Username already taken");
                    response.set_status_code(chat::StatusCode::INTERNAL_SERVER_ERROR);
                    if (!sendMessage(clientSocket, response)) {
                        std::cerr << "Error sending response\n";
                        close(clientSocket);
                        break;
                    }
                    close(clientSocket);
                    break;
                }
                for (const auto& [user, Ip] : ipsUsers) {
                    if (Ip == clientIp) {
                        std::cout << "Encontramos un cliente con esta IP\n";
                        response.set_message("Ya existe un usuario registrado con esta IP");
                        response.set_status_code(chat::StatusCode::INTERNAL_SERVER_ERROR);
                        if (!sendMessage(clientSocket, response)) {
                            std::cerr << "Error sending response\n";
                            close(clientSocket);
                            break;
                        }
                        close(clientSocket);
                        break;
                    }
                }
                // Si no existe, se agrega el username a la lista de usuarios y se guarda su ip, su socket y su estado
                ipsUsers[username] = clientIp;
                usersState[username] = chat::UserStatus::ONLINE;
                userSockets[username] = clientSocket;
                usersTiming[username] = 0;
            }
            std::cout << "User registered: " << username << "\n";

            // Se imprime la lista de usuarios conectados
            for (const auto& [user, clientSocket] : userSockets) {
                std::cout << user << "\n";
            }

            // Se envia un mensaje de exito al cliente
            response.set_message("User registered successfully");
            response.set_status_code(chat::StatusCode::OK);

            if (!sendMessage(clientSocket, response)) {
                std::cerr << "Error sending user info to client socket " << clientSocket << "\n";
            }
        } else if (request.operation() == chat::Operation::UNREGISTER_USER) {
            // Si se quiere desregistrar un usuario se crea un response
            chat::Response response;
            response.set_operation(chat::Operation::UNREGISTER_USER);
            // Se obtiene el username del request
            username = request.unregister_user().username();
            {
                // Se bloquea el mutex para proteger la variable usersState
                std::lock_guard<std::mutex> lock(clientsMutex);
                ipsUsers.erase(username);
                usersState.erase(username);
                userSockets.erase(username);
                usersTiming.erase(username);
            }
            // Se imprime el username del usuario que se desregistro
            std::cout << "User unregistered: " << username << "\n";
            response.set_message("User unregistered successfully");
            response.set_status_code(chat::StatusCode::OK);
            // Se envia un mensaje de exito al cliente
            if (!sendMessage(clientSocket, response)) {
                std::cerr << "Error sending response\n";
            }
            // Se cierra el socket del cliente
            close(clientSocket);
            break;
        } else if (request.operation() == chat::Operation::SEND_MESSAGE) {
            if (usersTiming.find(username) != usersTiming.end()){
                {
                    // Bloqueamos el mutex para proteger las variables compartidas
                    std::lock_guard<std::mutex> lock(clientsMutex);
                    usersTiming[username] = 0;
                }
                changeStatus(clientSocket, chat::UserStatus::ONLINE);
            }
            // Si se quiere enviar un mensaje se crea un response
            if (request.send_message().recipient() == "") {
                // Si el mensaje no tiene un recipient, se envia en broadcast
                std::string message = request.send_message().content();
                // Armamos el mensaje con el username del cliente y el contenido del mensaje
                std::cout << "Message received: " << "[" + username + "]" + ": " + message << "\n";
                // Se utiliza la funcion auxiliar para envia el mensaje en broadcast
                broadcastMessage(message, username);
            } else {
                // Si el mensaje tiene un recipient, se envia en directo
                std::string recipient = request.send_message().recipient();
                std::string message = request.send_message().content();
                // Armamos el mensaje con el username del cliente, el contenido del mensaje y el recipient
                std::cout << "Direct message received: " << "[" + username + "]" + ": " + message << "\n";
                // Se utiliza la funcion auxiliar para envia el mensaje en directo, colocando el recipient
                directMessage(message, username, recipient);
            }
            continue;
        } else if (request.operation() == chat::Operation::GET_USERS){
            // Si se quiere obtener los usuarios se crea un response
            if (request.get_users().username().empty()) {
                // Si no se especifica un usuario, se envian todos los usuarios
                returnAllUsers(clientSocket);
            } else {
                // Si se especifica un usuario, se envia la informacion de ese usuario
                returnUserInfo(clientSocket, request.get_users().username());
            }
            continue;
        } else if (request.operation() == chat::Operation::UPDATE_STATUS) {
            // Si se quiere actualizar el estado de un usuario 
            auto status_request = request.update_status();
            // Se utiliza la funcion auxiliar para cambiar el estado del usuario, agregando el nuevo estado del cliente
            changeStatus(clientSocket, status_request.new_status());
        } else {
            // Si la operacion no es reconocida, se envia un mensaje de error
            chat::Response response;
            std::cerr << "Unknown operation\n";
            // Se envia con un status code de Bad Request
            response.set_status_code(chat::StatusCode::BAD_REQUEST);
            response.set_message("Unknown operation");
            if (!sendMessage(clientSocket, response)) {
                std::cerr << "Error sending response\n";
            }
        }
    }
    // Si 
    {
        // Bloqueamos el mutex para proteger las variables compartidas
        std::lock_guard<std::mutex> lock(clientsMutex);
        // Se verifica si el usuario se encuentra en la lista de sockets, en caso no se haya eliminado previamente, después de su desregistro
        if (userSockets.find(username) != userSockets.end()) {
            usersState.erase(username);
            userSockets.erase(username);
            ipsUsers.erase(username);
            usersTiming.erase(username);
        }
    }
}

int main(int argc, char* argv[]) {
    // Se verifica que se haya ingresado la IP del servidor
    if (argc != 2) {
        std::cerr << "Usage: server <server_ip>\n";
        return 1;
    }

    // Se crea el socket del servidor con la IP ingresada
    std::string serverIP = argv[1];
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket: " << strerror(errno) << "\n";
        return 1;
    }

    // Se configura el socket del servidor con el puerto 8080
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddress.sin_port = htons(8080);

    // Se enlaza el socket del servidor con la dirección y el puerto
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error binding socket: " << strerror(errno) << "\n";
        close(serverSocket);
        return 1;
    }

    // Se pone el servidor a escuchar en el puerto 8080
    if (listen(serverSocket, 10) < 0) {
        std::cerr << "Error listening on socket: " << strerror(errno) << "\n";
        close(serverSocket);
        return 1;
    }

    std::cout << "Server started. Listening on port 8080...\n";

    // Se crea un hilo para recibir los mensajes del servidor
    std::thread statusWatcher(userScanner);
    statusWatcher.detach();
    // Se crea un loop infinito para aceptar conexiones de clientes
    while (true) {
        // Se acepta la conexión de un cliente
        sockaddr_in clientAddress{};
        socklen_t clientAddressLength = sizeof(clientAddress);
        // Se obtiene la IP del cliente
        // std::string ipAddress = inet_ntoa(clientAddress.sin_addr);
        // std::cout << "Client IP: " << ipAddress << "\n";
        // Se crea un socket para el cliente
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        // Se verifica si se aceptó la conexión correctamente
        if (clientSocket < 0) {
            std::cerr << "Error accepting client connection: " << strerror(errno) << "\n";
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
        std::cout << "Client IP: " << clientIP << "\n";

        // Convert const char* to std::string
        std::string clientIpStr = clientIP;

        // Se crea un thread para manejar las request del cliente
        clientThreads.emplace_back(std::thread(handleClient, clientSocket, clientIpStr));
    }

    // Se espera que el hilo de recepcion de mensajes termine
    if (statusWatcher.joinable())
    {
        statusWatcher.join();
    }

    // Cuando se cierra, se espera que todos los threads terminen
    for (auto& thread : clientThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    // Se cierra el socket del servidor
    close(serverSocket);
    return 0;
}
