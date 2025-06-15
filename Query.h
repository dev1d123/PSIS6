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

    void cantidadCanciones() const;
    void cantidadUsuarios() const;
    void cantidadTotalPuntuaciones() const;

   
    void topCancionesPorUsuario(int id_usuario);           
    void topUsuariosPorCancion(int id_cancion);            
    void topCancionesMasVotadas();                        
    void usuariosSimilares(int id_usuario);                
    void recomendacionesParaUsuario(int id_usuario);      
    void usuarioMasActivo();                               
    void promedioGlobal();                                 
};

#endif // QUERY_H

