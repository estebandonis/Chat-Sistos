# CC3064/2024 - Proyecto Chat
Sistema de chat creado usando el lenguaje de C++
## Requerimientos para Compilación
Se debe de tener las librerías: Protobuf, Abseil, gcc y Cmake instaladas dentro del computador.
Para ello, si se encuentra en un sistema Linux, puede ejecutar el siguiente comando:
```shell
sudo apt update
sudo apt install protobuf-compiler libprotobuf-dev libabseil-dev cmake build-essential
```

## Compilar Programa
Para compilar el programa debemos de dirigirnos en la carpeta de tools y ejecutamos el .sh
```shell
cd tools
./make_proto.sh
```

Luego regresamos a la carpeta principal y ejecutamos
```shell
cd ..
./run.sh
```

Para finalizar nos dirigimos a la carpeta de "build" y ya podes ejecutar los dos programas (server y client):
```shell
cd build
```

## Ejecución de programas
### Server
Para el server debemos de elegir una dirección IP y un puerto de nuestro computador que no esten en uso,
luego solo corremos el siguiente comando en nuestra terminal. El puerto se debe de cambiar dentro del programa (Puerto Predefinido: 8080):
```shell
./server 127.0.0.1
```

### Client
Para el cliente debemos de colocar el username que queramos, la dirección IP donde está localizada nuestro servidor 
y el puerto que se está utilizando para recibir requests:
```shell
./client Esteban 127.0.0.1 8080
```

## MacOS
Si se está utilizando un sistema MacOS, para la compilación se debe de descomentar las dos líneas comentadas en target_link_libraries:
```txt
target_link_libraries(protocol
    ${PROTOBUF_LIBRARIES}
    # absl::log_internal_message
    # absl::log_internal_check_op
)
```