/*
 * Libs.h
 *
 *  Created on: 27/2/2015
 *      Author: utnso
 */

#ifndef SRC_LIBS_H_
#define SRC_LIBS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <resolv.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#include <commons/log.h>
#include <commons/error.h>
#include <commons/string.h>
#include <commons/config.h>

/*
 * crearListener (puerto)
 * Crea un socket listener (hay que hacer el accept de las conexiones)
 * Devuelve el descriptor del socket
 */
int crearListener (int);

/*
 * conectar (ip, puerto)
 * Crea un socket y lo conecta
 * Devuelve el descriptor del socket
 */
int conectar (char*, char*);

/*
 * enviar (descriptor, data, datalength)
 * Manda data
 * Devuelve bytecount del send
 */
int enviar (int, char*, int);

/*
 * recibir (descriptor, buffer, datalength)
 * Recibe los datos en el buffer
 * Devuelve bytecount del recv
 */
int recibir (int, char*, int);

/*
 * contarDigitos (numero)
 * Devuelve la cantidad de digitos del numero
 * Devuelve la cantidad de digitos + 1 si es negativo
 */
int contarDigitos (int);

/*
 *leerConfig(path, propiedades, variables, cantidadDePropiedades)
 *Lee el valor de las propiedades
 *del cfg ubicado en el path
 *y lo asigna en las variables
 *
 *Ej: leerConfig(/utnso/asd, {"ip","puerto"}, {&ip, &puerto}, 2);
 */
void leerConfig(char*, char* [], char** [], int);

/*
 *inicializarStrings(arrayDePunterosAStrings, cantidadDeElementosEnElArray)
 *Inicializa todos los strings con string_new()
 */
void inicializarStrings(char** [], int);


#endif /* SRC_LIBS_H_ */
