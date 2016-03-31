#include "cabecera.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h>
#include <sstream>
using namespace std;

// Convierte int a string
string intToStr(int n){
	string toRet;
	stringstream out;
	out << n;
	toRet = out.str();
	return(toRet);
}

// Maneja los argumentos y saca del programa si la sintaxis es erronea
int manageArguments(int argc, char *argv[], string &rutaConf, int &puerto){
	int toRet = 0;
	string err = "Error. Se debe indicar el puerto del servidor y la ruta al archivo de configuracion\n\rSintaxis: http_server [puerto] [-c fichero_configuracion]\n\r";

	if(argc == 2) puerto = atoi(argv[1]);
	
	if(argc == 3){
		if(strcmp(argv[1],"-c") == 0){ rutaConf = argv[2]; }
		else toRet = 1;
	}

	if(argc == 4){
		if(strcmp(argv[1],"-c") == 0){
			rutaConf = argv[2];
			puerto = atoi(argv[3]);
		}
		else if(strcmp(argv[2],"-c") == 0){
			rutaConf = argv[3];
			puerto = atoi(argv[1]);
		}
		else toRet = 1;
	}

	if(toRet == 1 || argc > 4){ toRet = 1; cout << err; }

	return toRet;
}

// Crea el socket y lo devuelve, si hay error lo muestra
int inicializarSocket(){

	string err = "Error. No se puede abrir el socket\n\r";

	int nuevoSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (nuevoSocket == -1) cout << err; 
	else cout << "Socket creado sin errores\n\r";

	return nuevoSocket;

}

// Devuelve un STRING que contiene un fichero de error
string parsearFichero(int codErr, string ruta){
	string toRet = "";
	string temp;
	string modRuta = ruta + "/" + intToStr(codErr);

        ifstream fe(modRuta.c_str());
	while(getline(fe, temp)){ toRet += temp; }

	return(toRet);
}

// Construye una respuesta a partir del codigo de error que se le pasa
string construirRespuestaError(int codErr, string ruta){
	string toRet = "HTTP/1.1";
	string html = parsearFichero(codErr, ruta);

	switch(codErr){
		case 400: toRet += " 400 bad request"; break;
		case 403: toRet += " 403 forbidden"; break;
		case 404: toRet += " 404 not found"; break;
		case 405: toRet += " 405 Method Not Allowed";	break;
		case 503: toRet += " 503 Service Unavailable"; break;
		case 505: toRet += " 505 HTTP version not Supported";	break;
		default: toRet += " 500 Internal Server Error";
	}

	toRet += "\nConnection: close\nContent-Type: text/html\nServer: MiniHTTPServer/0.1\nContent-Lenght: ";
	toRet += html.length();
	toRet += "\n\n";
	toRet += html;

	return(toRet);
}
