#include <iostream>
#include <string>
#include <vector>


using namespace std;

int main(){

string input;
const string prompt = "Ingrese el comando o programa : "; // Prompt personalizado
const string bin_dir = "/bin/"; // ruta al directorio bin 

do {
    cout << prompt; 
    getline(cin, input);
    
} while (input != "salir");
 return 0;
}
