#include <bits/stdc++.h>
#include "Query.h"
#include "Rate.h"
#include "Puntaje.h"
struct RateCompare {
	bool operator()(const Rate& a, const Rate& b) const {
    	return a.score < b.score; 
    }
};

struct PuntajeCompare {
    bool operator()(const Puntaje& a, const Puntaje& b) const {
    	return a.puntaje < b.puntaje;
   	}
};



Query::Query(const vector<Puntaje>& puntuaciones, const set<int>& songs, 
             const set<int>& users, const unordered_map<int, vector<Rate>>& ratings)
    : datos(puntuaciones), canciones(songs), usuarios(users), user_ratings(ratings) {}

void Query::cantidadCanciones() const {
    cout << "Cantidad de canciones: " << canciones.size() << endl;


}

void Query::cantidadUsuarios() const {
    cout << "Cantidad de usuarios: " << usuarios.size() << endl;
}

void Query::cantidadTotalPuntuaciones() const {
    cout << "Cantidad total de puntuaciones: " << datos.size() << endl;
}
	
void Query::topCancionesPorUsuario(int id_usuario) {
	priority_queue<Rate, vector<Rate>, RateCompare> pq;
	for (const Rate& rate : user_ratings[id_usuario]) {
        pq.push(rate);
    }
	cout << "Top 20 canciones mejor puntuadas del usuario " << id_usuario << ":\n";
	int sz = 20;
	while(!pq.empty() && sz){
		Rate r = pq.top(); pq.pop();
		cout<<r.song_id<< "->" <<r.score<<endl;	
		sz--;
	}
}

void Query::topUsuariosPorCancion(int id_cancion) {
	priority_queue<Puntaje, vector<Puntaje>, PuntajeCompare> pq;
    
	for (Puntaje pun : datos) {
    	if (pun.id_cancion == id_cancion) {
        	pq.push(pun);
            if (pq.size() > 20) pq.pop(); 
        }
    }

    if (pq.empty()) {
        cout << "No hay puntuaciones registradas para la canción con ID " << id_cancion << ".\n";
        return;
    }
	int sz = 20;
	while(!pq.empty() && sz){
		Puntaje p = pq.top(); pq.pop();
		cout << "Usuario: " << p.id_usuario << " -> Puntaje: " << p.puntaje << endl;
	}
}

void Query::topCancionesMasVotadas() {
    // Implementación pendiente
    cout << "[topCancionesMasVotadas] Función en desarrollo\n";
}

void Query::usuariosSimilares(int id_usuario) {
    
	if(usuarios.find(id_usuario) == usuarios.end()){
		cerr<<"Usuario no encontrado"<<endl;
		return;
	}
	const int k = 20;
	vector<pair<int, double>> similar_users;
	
	unordered_map<int, double> target_ratings;	
	
	for (const auto& r : user_ratings[id_usuario]) {
    	target_ratings[r.song_id] = r.score;
    }

	for(int user: usuarios){
		if(user == id_usuario) continue;

		unordered_map<int, double> current_ratings;
		for (const auto& r : user_ratings[user]) {
        	current_ratings[r.song_id] = r.score;
        }
		double dot_product = 0.0, mag_target = 0.0, mag_current = 0.0;
		
		for(auto& pair: target_ratings){
			int song_id = pair.first;
			if(current_ratings.count(song_id)){
				dot_product += pair.second * current_ratings[song_id];
			}
			mag_target += pair.second * pair.second;
		}
		for(auto& pair: current_ratings){
			mag_current += pair.second * pair.second;
		}

        mag_target = sqrt(mag_target);
        mag_current = sqrt(mag_current);

		double similarity = (mag_target == 0 || mag_current == 0) ? 0.0 : dot_product / (mag_target * mag_current);
		similar_users.emplace_back(user, similarity);
	}
	sort(similar_users.begin(), similar_users.end(),
		[](const pair<int, double>& a, const pair<int, double>& b){
			return a.second > b.second;
		});

	if (similar_users.size() > k) {
        similar_users.resize(k);
   	}
	cout << "Usuarios más similares al usuario: " << id_usuario << endl;
    for (const auto& [user, sim] : similar_users) {
        cout << "Usuario " << user << ": similitud = " << sim << endl;
    }

}

void Query::recomendacionesParaUsuario(int id_usuario) {
    // Implementación pendiente
    cout << "[recomendacionesParaUsuario] Función en desarrollo\n";


}

void Query::usuarioMasActivo() {
    // Implementación pendiente
    cout << "[usuarioMasActivo] Función en desarrollo\n";
}


void Query::promedioGlobal() {
    // Implementación pendiente
    cout << "[promedioGlobal] Función en desarrollo\n";
}

