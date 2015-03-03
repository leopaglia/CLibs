#include "Libs.h"

int crearListener (int puerto){

	// AF_INET: Socket de internet IPv4
	// SOCK_STREAM: Orientado a la conexion, TCP
	// 0: Usar protocolo por defecto para AF_INET-SOCK_STREAM: Protocolo TCP/IPv4

	//config
	int queueMax = 10;
	int optval = 1;

	int listener; // descriptor de socket a la escucha

	struct sockaddr_in socket_cliente;
	socket_cliente.sin_family = AF_INET;
	socket_cliente.sin_addr.s_addr = htons(INADDR_ANY );
	socket_cliente.sin_port = htons(puerto);

	// Crear el socket.
	if((listener = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error_show("Creacion socket listener");

	// Hacer que el SO libere el puerto inmediatamente luego de cerrar el socket.
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	// Vincular el socket con una direccion de red almacenada en 'socket_cliente'.
	if (bind(listener, (struct sockaddr*) &socket_cliente, sizeof(socket_cliente)) != 0)
		error_show("Bind socket listener");

	// Escuchar nuevas conexiones entrantes.
	if (listen(listener, queueMax) != 0)
		error_show("Listen");

	printf("Escuchando conexiones en puerto %d \n", puerto);

	return listener;
}

int conectar (char* ip, char* puerto){

	struct addrinfo hints;
	struct addrinfo *serverInfo;
	int descriptor;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(ip, puerto, &hints, &serverInfo) != 0 ){
		error_show("getaddrinfo() \n");
		exit(1);
	}

	if ((descriptor = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol)) < 0){
		error_show("socket() \n");
		exit(1);
	}

	if(connect(descriptor, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0 ){
		error_show("connect() \n");
		exit(1);
	}

	freeaddrinfo(serverInfo);

	return descriptor;
};


int enviar (int descriptor, char* data, int datalength){

	int bytecount;

	if ((bytecount = send(descriptor, data, datalength, 0)) == -1){
		error_show("Send bytecount = -1 \n");
	}

	return (bytecount);
}


int recibir (int descriptor, char* buffer, int datalength) {

	int bytecount;

	memset(buffer, 0, datalength);

	bytecount = recv(descriptor, buffer, datalength, 0);

	return (bytecount);
}


int contarDigitos (int num) {

	int count = 1;

	if(num < 0){
		count++;
		num = num * -1;
	}

	while (num / 10 > 0) {
		num = num / 10;
		count++;
	}

	return count;
}


void inicializarStrings(char** arrayVars[], int cantVars){
	int i = 0;
	for(i = 0; i < cantVars; i++){
		*arrayVars[i] = string_new();
	}
}

void leerConfig(char* path, char* properties[], char** vars[], int cantProperties) {

	t_config* config = config_create(path);

	if(config->properties->table_current_size !=0){

		int i;

		for (i = 0; cantProperties > i; i++){
			if(config_has_property(config, properties[i])){

				string_append(vars[i],config_get_string_value(config, properties[i]));

			} else error_show("No se pudo leer el parametro %s \n", properties[i]);
		}

	} else {
		error_show("No se pudo abrir el archivo de configuracion \n");
		exit(1);
	}

	if(config != NULL)
			free(config);

}

/*TODO

 	fd_set master, temp;
	struct sockaddr_in myAddress;
	struct sockaddr_in remoteAddress;
	int maxSock;

	FD_ZERO(master);
	FD_ZERO(temp);

	FD_SET(*sockListener, master);
	*maxSock = *sockListener;

	signed int getSocketChanged(fd_set *master, fd_set *temp, int *maxSock, int sockListener, struct sockaddr_in *remoteAddress, void *buf, int bufSize) {

		int addressLength;
		int i;
		int newSock;
		int nBytes;
		*temp = *master;

		//--Multiplexa conexiones
		if (select(*maxSock + 1, temp, NULL, NULL, NULL ) == -1){
			error_show("Select");
			exit(1);
		}

		//--Cicla las conexiones para ver cual cambió
		for (i = 0; i <= *maxSock; i++) {
			//--Si el i° socket cambió
			if (FD_ISSET(i, temp)) {
				//--Si el que cambió es el listener
				if (i == sockListener) {
					addressLength = sizeof(*remoteAddress);
					//--Gestiona nueva conexión
					if((newSock = accept(sockListener, (struct sockaddr *) remoteAddress, (socklen_t *) &addressLength)) == -1)
						error_show("Accept");
					else {
						//--Agrega el nuevo listener
						FD_SET(newSock, master);

						if (newSock > *maxSock)
							*maxSock = newSock;
					}
				} else {
					//--Gestiona un cliente ya conectado
					if ((nBytes = recibir(i, buf, bufSize)) <= 0) {
						//--Si cerró la conexión o hubo error
						if (nBytes == 0)
							error_show("Fin de conexion del socket %d.", i);
						else
							error_show("Recv: %s", strerror(errno));
						//--Cierra la conexión y lo saca de la lista
						close(i);
						FD_CLR(i, master);
					} else {
						return i;
					}
				}
			}
		}

		return -1;
	}

*/
