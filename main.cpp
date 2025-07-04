#include <bits/stdc++.h>
#include "Puntaje.h"
#include "Query.h"
#include "Rate.h"
using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

const int K = 20;

int main() {
    ifstream data("ratings.csv");
    string linea;
    set<int> songs;
    set<int> users;
    vector<Puntaje> arr;
    unordered_map<int, vector<Rate>> user_ratings;

    while (getline(data, linea)) {
        stringstream ss(linea);
        string campo;
        vector<string> fila;

        while (getline(ss, campo, ',')) {
            fila.push_back(campo);
        }

        Puntaje p;
        p.id_usuario = stoi(fila[0]);
        p.id_cancion = stoi(fila[1]);
        p.puntaje = stod(fila[2]);
        songs.insert(p.id_cancion);
        users.insert(p.id_usuario);

        Rate r;
        r.song_id = p.id_cancion;
        r.score = p.puntaje;

        user_ratings[p.id_usuario].push_back(r);
        arr.push_back(p);
    }
    data.close();

    Query query(arr, songs, users, user_ratings, K);

    int opcion;
    do {
        cout << BOLD << CYAN;
        cout << "\n========= SISTEMA DE CONSULTAS MUSICALES =========\n";
        cout << RESET;

        cout << YELLOW << "Cantidad de canciones: " << songs.size() << endl;
        cout << "Cantidad de usuarios: " << users.size() << endl;
        cout << "Cantidad total de puntuaciones: " << arr.size() << RESET << endl;

        cout << BOLD << "\n======================================" << RESET << endl;
        cout << BOLD << GREEN << "Seleccione la consulta\n" << RESET;
        cout << MAGENTA << "1. En función a una persona (Top " << K << " canciones favoritas del usuario X)\n";
        cout << "2. En función a una canción (Top " << K << " usuarios que votaron mejor la canción X)\n";
        cout << "3. Obtener las " << K << " canciones más votadas\n";
        cout << "4. Agrupar por gustos (Top " << K << " usuarios con gustos similares a X)\n";
        cout << "5. Mostrar las " << K << " recomendaciones de canciones para el usuario X\n";
        cout << "6. Mostrar el usuario más activo\n";
        cout << "7. Mostrar el promedio de puntaje global\n";
        cout << "8. SALIR\n" << RESET;

        cout << BOLD << BLUE << "\nIngrese una opción: " << RESET;
        cin >> opcion;

        int id;
        vector<string> salida;

        switch (opcion) {
            case 1:
                cout << "Ingrese ID de usuario: ";
                cin >> id;
                salida = query.topCancionesPorUsuario(id);
                break;
            case 2:
                cout << "Ingrese ID de canción: ";
                cin >> id;
                salida = query.topUsuariosPorCancion(id);
                break;
            case 3:
                salida = query.topCancionesMasVotadas();
                break;
            case 4:
                cout << "Ingrese ID de usuario: ";
                cin >> id;
                salida = query.usuariosSimilares(id);
                break;
            case 5:
                cout << "Ingrese ID de usuario: ";
                cin >> id;
                salida = query.recomendacionesParaUsuario(id);
                break;
            case 6:
                salida = query.usuarioMasActivo();
                break;
            case 7:
                salida = query.promedioGlobal();
                break;
            case 8:
                cout << GREEN << "Saliendo del sistema. ¡Hasta pronto!\n" << RESET;
                break;
            default:
                cout << RED << "Opción inválida. Intente nuevamente.\n" << RESET;
        }

        // Mostrar resultados si hay
        if (!salida.empty()) {
            cout << BOLD << CYAN << "\n>> Resultado:\n" << RESET;
            for (const string& linea : salida) {
                cout << linea << endl;
            }
        }

    } while (opcion != 8);

    return 0;
}

