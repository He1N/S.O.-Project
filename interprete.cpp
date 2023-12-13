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
            string output_file; // Esta variable se utiliza para almacenar el nombre del archivo de salida.
            bool redirect_output = false; /* Esta línea inicializa un indicador booleano que
			 indica si la salida se debe redireccionar 
			a un archivo en lugar de mostrarse en la terminal.*/

           // Analiza la entrada en busca de argumentos y redirección de salida
            for (size_t i = 0; i < input.size(); i++) {
            	
                if (input[i] == '>') {
                    redirect_output = true;
                    output_file = input.substr(i + 2);
                    break;
                } else if (input[i] != ' ') {
                    size_t arg_start = i; // se utiliza para almacenar la posición inicial del primer argumento del comando
                    while (i < input.size() && input[i] != ' ' && input[i] != '>') {
                        i++;
                    }
                    args.push_back(input.substr(arg_start, i - arg_start));
                    i--;
                }
            }
            
            
            
            pid_t pid = fork();  // se utiliza para crear un proceso hijo que ejecutará el comando ingresado por el usuario
           
		    if (pid == 0) {
                // Proceso hijo
                string command = args[0];

                // revisión si es que el comando inicia con ./ o / "./"
                if (command.substr(0, 2) != "./" && command[0] != '/') {
                    // se considerará el directorio bin
                    command = bin_dir + command;
                }

                // Redirigir la salida si es necesario
                if (redirect_output) {
                    int fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

                    /* La función open se utiliza para abrir el archivo de salida.*/
                    /* El primer argumento de open es el nombre de archivo que se desea abrir*/
                    /*  para abrir el archivo en modo de escritura, crearlo si no existe y truncarlo a cero bytes si ya existe, respectivamente.*/
                    // El tercer argumento (0644) especifica los permisos del archivo.
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
        }
     } while (input != "salir");

