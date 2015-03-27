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
		exitError("Creacion socket listener");

	// Hacer que el SO libere el puerto inmediatamente luego de cerrar el socket.
	setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

	// Vincular el socket con una direccion de red almacenada en 'socket_cliente'.
	if (bind(listener, (struct sockaddr*) &socket_cliente, sizeof(socket_cliente)) != 0)
		exitError("Bind socket listener");

	// Escuchar nuevas conexiones entrantes.
	if (listen(listener, queueMax) != 0)
		exitError("Listen");

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

	if(getaddrinfo(ip, puerto, &hints, &serverInfo) != 0 )
		exitError("getaddrinfo() \n");

	if ((descriptor = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol)) < 0)
		exitError("socket() \n");

	if(connect(descriptor, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0 )
		exitError("connect() \n");

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
		exitError("No se pudo abrir el archivo de configuracion \n");
	}

	if(config != NULL)
			free(config);

}

void exitError(char* error){
	error_show(error);
	exit(1);
}


t_struct_select inicializarSelect(int listener){

	t_struct_select params;
	params.buffer = string_new();

	FD_ZERO(&params.master);
	FD_ZERO(&params.temp);

	params.maxSock = listener;

	params.temp = params.master;

	FD_SET(listener, &params.master);

	return params;
}


int getSocketChanged(t_struct_select* params, int sockListener) {

	//--Multiplexa conexiones
	if (select(params->maxSock + 1, &params->temp, NULL, NULL, NULL ) == -1)
		exitError("Select");

	//--Cicla las conexiones para ver cual cambió
	int i;
	for (i = 0; i <= params->maxSock; i++) {
		//--Si el i° socket cambió
		if (!FD_ISSET(i, &params->temp))
			continue;
		//--Si el que cambió es el listener
		if (i == sockListener) {
			//--Gestiona nueva conexión
			int socketNuevaConexion;
			if((socketNuevaConexion = accept(socketNuevaConexion, NULL, 0)) == -1)
				exitError("Accept");
			else {
				//--Agrega el nuevo listener
				FD_SET(socketNuevaConexion, &params->master);

				if (socketNuevaConexion > params->maxSock)
					params->maxSock = socketNuevaConexion;
			}
		} else {
			//--Gestiona un cliente ya conectado
			int nBytes;
			int bufferSize = sizeof(params->buffer);
			if ((nBytes = recibir(i, params->buffer, bufferSize)) <= 0) {
				//--Si cerró la conexión o hubo error
				if (nBytes == 0)
					printf("Fin de conexion del socket %d.", i);
				else
					error_show("Recv: %s", strerror(errno));
				//--Cierra la conexión y lo saca de la lista
				close(i);
				FD_CLR(i, &params->master);
			} else {
				return i;
			}
		}
	}

	return -1;
}

