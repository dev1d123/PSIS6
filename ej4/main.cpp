#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string nombreFichero, linea;
    cout << "Introduce el nombre del fichero: ";
    getline(cin, nombreFichero);

    ifstream archivo(nombreFichero);

    if (!archivo) {
        cerr << "No se pudo abrir el archivo: " << nombreFichero << endl;
        return 1;
    }

    int contador = 0;
    while (getline(archivo, linea)) {
        cout << linea << endl;
        contador++;

        if (contador % 10 == 0) {
            cout << "\n--- Pulsa Enter para continuar ---";
            cin.get();
        }
    }

    if (contador % 10 != 0) {
        cout << "\n--- Fin del archivo ---" << endl;
    }

    archivo.close();
    return 0;
}

