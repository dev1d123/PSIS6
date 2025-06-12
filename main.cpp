#include <bits/stdc++.h>
using namespace std;

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
		arr.push_back(p);
	}
	data.close();
	for(auto elem: arr){
		elem.mostrar();
		
	}
	cout<<"cantidad de cancones: "<<songs.size()<<endl;
	return 0;
}
