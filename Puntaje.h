#ifndef PUNTAJE_H
#define PUNTAJE_H

#include <iostream>
using namespace std;

struct Puntaje {
    int id_usuario;
    int id_cancion;
    double puntaje;

    Puntaje() = default;

    Puntaje(int usuario, int cancion, double valor)
        : id_usuario(usuario), id_cancion(cancion), puntaje(valor) {}

    void mostrar() const {
        cout << "Usuario: " << id_usuario << endl;
        cout << "Canción: " << id_cancion << endl;
        cout << "Puntaje: " << puntaje << endl;
    }
};

#endif // PUNTAJE_H

