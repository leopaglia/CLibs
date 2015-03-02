/*
 * Test.c
 *
 *  Created on: 2/3/2015
 *      Author: utnso
 */
#include "Libs.h"

int main (){

	char* numero;
	char* letra;
	char* propiedades[2];
	char* mensaje;
	char* handshake;

	char** variables[2]= {&numero, &letra};
	char** strings[] = {&numero, &letra, &propiedades[0], &propiedades[1], &mensaje, &handshake};

	inicializarStrings(strings, 6);

	string_append(&propiedades[0],"numero");
	string_append(&propiedades[1],"letra");

	leerConfig("/home/utnso/git/CLibs/src/config.cfg", propiedades, variables, 2);

	printf("numero: %d, letra: %s \n", atoi(numero), letra);
	printf("digitos del numero: %d \n", contarDigitos(atoi(numero)));

	int socketServer = conectar("127.0.0.1", "5455");

	string_append(&mensaje, "hola server como te va");
	string_append(&handshake, string_itoa(strlen(mensaje)));

	enviar(socketServer, handshake, strlen(handshake));
	enviar(socketServer, mensaje, strlen(mensaje));

	return 0;
}
