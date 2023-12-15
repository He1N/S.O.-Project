#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h> // Define funciones utilizadas para retener procesos
#include <fcntl.h> //  Define las opciones de control de archivos.

using namespace std;

int main() {
    string input; // Variable para almacenar la entrada del usuario
    const string prompt = "Ingrese el comando o programa > "; // Cadena de texto que se muestra al usuario
    const string bin_dir = "/bin/";  // Ruta relativa al directorio bin

    do {
        cout << prompt; // Muestra el prompt y espera la entrada del usuario
        getline(cin, input);

        if (input != "salir") { // Si la entrada no es "salir", se procede a analizar y ejecutar el comando
            vector<string> commands;
            vector<string> output_files; // Esta variable se utiliza para almacenar el nombre del archivo de salida.
            vector<bool> redirect_output; /* indicador , para saber si la salida se debe redireccionar */
            vector<bool> run_in_background;

            size_t i = 0;
            while (i < input.size()) {
                size_t command_start = i;
                while (i < input.size() && input[i] != '|') {
                    i++;
                }
                commands.push_back(input.substr(command_start, i - command_start));
                redirect_output.push_back(false);
                run_in_background.push_back(false);

                if (i < input.size() && input[i] == '|') {
                    i++;
                }
            }

            // Ejecutar comandos con tuberías.
            int input_fd = 0; // La entrada para el primer comando es stdin
            for (size_t j = 0; j < commands.size(); j++) {
                vector<string> args;
                string output_file;
                bool run_in_bg = false;

                // Comando de análisis de argumentos, redirección de salida y ejecución en segundo plano
                for (size_t k = 0; k < commands[j].size(); k++) {
                    if (commands[j][k] == '>') {
                        redirect_output[j] = true;
                        output_file = commands[j].substr(k + 2);
                        break;
                    } else if (commands[j][k] == '&') {
                        run_in_bg = true;
                        break;
                    } else if (commands[j][k] != ' ') {
                        size_t arg_start = k;
                        while (k < commands[j].size() && commands[j][k] != ' ' &&
                               commands[j][k] != '>' && commands[j][k] != '&') {
                            k++;
                        }
                        args.push_back(commands[j].substr(arg_start, k - arg_start));
                        k--;
                    }
                }

                // Creacion de tuberia
                int pipe_fd[2];
                if (pipe(pipe_fd) == -1) {
                    cerr << "Error al crear la tubería." << endl;
                    exit(1);
                }

                
                pid_t pid = fork();
                if (pid == 0) {
                    // Proceso hijo

                   
                    dup2(input_fd, STDIN_FILENO);
                    close(pipe_fd[0]); 

                
                    if (redirect_output[j]) {
                        int fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        dup2(fd, STDOUT_FILENO);
                        close(fd);
                    
                    /* La función open se utiliza para abrir el archivo de salida.*/
                    /* El primer argumento de open es el nombre de archivo que se desea abrir*/
                    /*  para abrir el archivo en modo de escritura, crearlo si no existe y truncarlo a cero bytes si ya existe, respectivamente.*/
                    // El tercer argumento (0644) especifica los permisos del archivo.
                    } else if (j < commands.size() - 1) {
                        // Redirige la salida a la tubería si no es el último comando
                        dup2(pipe_fd[1], STDOUT_FILENO);
                    }

                  
                    string command = args[0];
                    if (command.substr(0, 2) != "./" && command[0] != '/') {
                        command = bin_dir + command;
                    }

                    char** argv = new char*[args.size() + 1];
                    for (size_t k = 0; k < args.size(); k++) {
                        argv[k] = const_cast<char*>(args[k].c_str());
                    }
                    argv[args.size()] = NULL;

                    if (run_in_bg) {
                        // Separa el proceso hijo para ejecución en segundo plano
                        if (setsid() == -1) {
                            cerr << "Error: setsid fallo" << endl;
                            exit(1);
                        }
			                    }
				 /* Se ejecuta el comando o programa utilizando la función execv */
                    execv(command.c_str(), argv);
                    cerr << "Error: comando no encontrado" << endl;
                    exit(1);
                } else if (pid > 0) {
                    // Proceso padre 
                    close(pipe_fd[1]); // Cerrar el extremo de escritura de la tubería.
                    if (!run_in_bg) {
                        // Espera el proceso hijo si no se ejecuta en segundo plano
                        int status;
                        waitpid(pid, &status, 0);
                    }
                    input_fd = pipe_fd[0]; // Establece la entrada para el siguiente comando
                } else {
                    cerr << "Error: fork fallo" << endl;
                    exit(1);
                }
            }
        }
    } while (input != "salir");

    return 0;
}

