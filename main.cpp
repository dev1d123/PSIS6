#include <bits/stdc++.h>
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

struct Puntaje{
	int id_usuario;
	int id_cancion;
	double puntaje;
	Puntaje() = default;
	Puntaje(int usuario, int cancion, double valor)
        : id_usuario(usuario), id_cancion(cancion), puntaje(valor) {}
	void mostrar() const {
		cout<<"Usuario: "<<id_usuario<<endl;
		cout<<"Canción: "<<id_cancion<<endl;
		cout<<"Puntaje: "<<puntaje<<endl;
	}

};
int main(){
	ifstream data("ratings.csv");
	string linea;
	set<int> songs;
	set<int> users;
	vector<Puntaje> arr;
		
	while(getline(data, linea)){
		stringstream ss(linea);
		string campo;
		vector<string> fila;
		
		while(getline(ss, campo, ',')){
			fila.push_back(campo);
		}
		
		Puntaje p;
		p.id_usuario = stoi(fila[0]);
        p.id_cancion = stoi(fila[1]);
        p.puntaje = stod(fila[2]);
		songs.insert(p.id_cancion);
		users.insert(p.id_usuario);
		arr.push_back(p);
	}
	data.close();


    int opcion;
    do {
        // Encabezado principal
        cout << BOLD << CYAN;
        cout << "\n========= SISTEMA DE CONSULTAS MUSICALES =========\n";
        cout << RESET;

        cout << YELLOW << "Cantidad de canciones: " << songs.size() << endl;
        cout << "Cantidad de usuarios: " << users.size() << endl;
        cout << "Cantidad total de puntuaciones: " << arr.size() << RESET << endl;

        cout << BOLD << "\n======================================" << RESET << endl;
        cout << BOLD << GREEN << "Seleccione la consulta\n" << RESET;
        cout << MAGENTA << "1. En función a una persona (Top 10 canciones favoritas del usuario X)\n";
        cout << "2. En función a una canción (Top 10 usuarios que votaron mejor la canción X)\n";
        cout << "3. Obtener las 10 canciones más votadas\n";
        cout << "4. Agrupar por gustos (Top 20 usuarios con gustos similares a X)\n";
        cout << "5. Mostrar las 10 recomendaciones de canciones.\n";
        cout << "6. Mostrar el usuario más activo (más puntuaciones hechas)\n";
        cout << "7. Mostrar el promedio de puntaje global\n";
        cout << "8. SALIR\n" << RESET;

        cout << BOLD << BLUE << "\nIngrese una opción: " << RESET;
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Consulta 1 seleccionada\n";
                break;
            case 2:
                cout << "Consulta 2 seleccionada\n";
                break;
            case 3:
                cout << "Consulta 3 seleccionada\n";
                break;
            case 4:
                cout << "Consulta 4 seleccionada\n";
                break;
            case 5:
                cout << "Consulta 5 seleccionada\n";
                break;
            case 6:
                cout << "Consulta 6 seleccionada\n";
                break;
            case 7:
                cout << "Consulta 7 seleccionada\n";
                break;
            case 8:
                cout << GREEN << "Saliendo del sistema. ¡Hasta pronto!\n" << RESET;
                break;
            default:
                cout << RED << "Opción inválida. Intente nuevamente.\n" << RESET;
        }

    } while (opcion != 8);

    return 0;
}
