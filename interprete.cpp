#include <iostream>
#include <string>
#include <vector> // Define las características de implementación identificadas por el estándar POSIX .
#include <unistd.h>
#include <sys/wait.h> // Define funciones utilizadas para retener procesos.
#include <fcntl.h>  //  Define las opciones de control de archivos.

using namespace std;

int main() {
	

    string input;	// Variable para almacenar la entrada del usuario
    const string prompt = "Ingrese el comando o programa > "; // Cadena de texto que se muestra al usuario
    const string bin_dir = "/bin/"; // Ruta relativa al directorio bin

    do {
        cout << prompt; // Muestra el prompt y espera la entrada del usuario
        getline(cin, input);

        if (input != "salir") { // Si la entrada no es "salir", se procede a analizar y ejecutar el comando
            vector<string> args;
          
           // Analiza la entrada en busca de argumentos y redirección de salida
            for (size_t i = 0; i < input.size(); i++) {
            	
				if (input[i] != ' ') {
                    size_t arg_start = i; // se utiliza para almacenar la posición inicial del primer argumento del comando
                    while (i < input.size() && input[i] != ' ' && input[i] != '>') {
                        i++;
                    }
                    args.push_back(input.substr(arg_start, i - arg_start));
                    i--;
                }
            }
        }
     } while (input != "salir");

