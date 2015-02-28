#include "Libs.h"

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


void leerConfig(char* path, char* properties[], char** vars[], int cantProperties) {

	t_config* config = config_create(path);

	if(config->properties->table_current_size !=0){

		int i;

		for (i = 0; cantProperties >= i; i++){
			if(config_has_property(config, properties[i])){
				*vars[i] = config_get_string_value(config, properties[i]);
			} else error_show("No se pudo leer el parametro IP_KERNEL \n");
		}

	} else {
		error_show("No se pudo abrir el archivo de configuracion \n");
		exit(1);
	}

	if(config != NULL)
			free(config);

}

int main(){
	return 1;
}
