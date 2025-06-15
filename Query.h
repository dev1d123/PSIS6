#ifndef QUERY_H
#define QUERY_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "Puntaje.h" 
#include "Rate.h"
using namespace std;

class Query {
private:
    vector<Puntaje> datos; 
    set<int> canciones;    
    set<int> usuarios;    
	unordered_map<int, vector<Rate>> user_ratings;
	int quantity;
public:
    Query(const vector<Puntaje>& puntuaciones, const set<int>& canciones, 
      const set<int>& usuarios, const unordered_map<int, vector<Rate>>& user_ratings, int K);
    void mostrarMenu(); 

    vector<string> cantidadCanciones() const;
    vector<string> cantidadUsuarios() const;
    vector<string> cantidadTotalPuntuaciones() const;

   
    vector<string> topCancionesPorUsuario(int id_usuario);           
    vector<string> topUsuariosPorCancion(int id_cancion);            
    vector<string> topCancionesMasVotadas();                        
    vector<string> usuariosSimilares(int id_usuario);                
    vector<string> recomendacionesParaUsuario(int id_usuario);      
    vector<string> usuarioMasActivo();                               
    vector<string> promedioGlobal();                                 

};

#endif // QUERY_H

