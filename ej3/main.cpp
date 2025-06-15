#include <bits/stdc++.h>
using namespace std;

int main() {
    string frase;
    ofstream archivo("frases.txt");

    if (!archivo) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return 1;
    }

    cout << "Introduce frases (pulsa Enter sin escribir nada para terminar):" << endl;
    while (true) {
        getline(cin, frase);
        if (frase.empty()) break;
        archivo << frase << endl;
    }

    archivo.close();

    ifstream archivoLectura("frases.txt");
    if (!archivoLectura) {
        cerr << "Error al abrir el archivo para lectura." << std::endl;
        return 1;
    }

    cout << "\nContenido del archivo frases.txt:\n" << endl;
    while (getline(archivoLectura, frase)) {
        cout << frase << endl;
    }

    archivoLectura.close();

    return 0;
}

