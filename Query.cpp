#include <bits/stdc++.h>
#include "Query.h"
#include "Rate.h"
#include "Puntaje.h"

int K;

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
             const set<int>& users, const unordered_map<int, vector<Rate>>& ratings, int q)
    : datos(puntuaciones), canciones(songs), usuarios(users), user_ratings(ratings), quantity(q) {
		K = q;
	}

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
	cout << "Canciones mejor puntuadas del usuario " << id_usuario << ":\n";
	int sz = K;
	while(!pq.empty() && sz){
		Rate r = pq.top(); pq.pop();
		cout<<"Canción: "<<r.song_id<< " -> Puntaje: " <<r.score<<endl;	
		sz--;
	}
}

void Query::topUsuariosPorCancion(int id_cancion) {
	priority_queue<Puntaje, vector<Puntaje>, PuntajeCompare> pq;
	for (Puntaje pun : datos) {
    	if (pun.id_cancion == id_cancion) {
        	pq.push(pun);
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
    map<int, int> song_votes;                  
    map<int, map<double, int>> song_kind;      
    for (const Puntaje& pun : datos) {
        int song = pun.id_cancion;
        song_votes[song]++;
        song_kind[song][pun.puntaje]++;
    }
    vector<pair<int, int>> vote_vector(song_votes.begin(), song_votes.end());

    sort(vote_vector.begin(), vote_vector.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    int k = min(K, (int)vote_vector.size());

    cout << setw(12) << "Cancion ID" << setw(10) << "Votos"
         << setw(6) << "0.5" << setw(6) << "1" << setw(6) << "1.5"
         << setw(6) << "2" << setw(6) << "2.5" << setw(6) << "3"
         << setw(6) << "3.5" << setw(6) << "4" << setw(6) << "4.5"
         << setw(6) << "5" << endl;
    for (int i = 0; i < k; ++i) {
        int song_id = vote_vector[i].first;
        cout << setw(12) << song_id
             << setw(10) << song_votes[song_id];

        for (double score = 0.5; score <= 5.0; score += 0.5) {
            int count = song_kind[song_id][score];
            cout << setw(6) << count;
        }
        cout << endl;
    }
}

void Query::usuariosSimilares(int id_usuario) {
	if(usuarios.find(id_usuario) == usuarios.end()){
		cerr<<"Usuario no encontrado"<<endl;
		return;
	}
	const int k = K;
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

	if (usuarios.find(id_usuario) == usuarios.end()) {
        cerr << "Usuario no encontrado\n";
        return;
    }

    unordered_set<int> canciones_usuario;
    for (const auto& r : user_ratings[id_usuario]) {
		canciones_usuario.insert(r.song_id);
    }

    // Obtener usuarios similares
    vector<pair<int, double>> similar_users;
    unordered_map<int, double> target_ratings;
    for (const auto& r : user_ratings[id_usuario]) {
        target_ratings[r.song_id] = r.score;
    }

    for (int user : usuarios) {
        if (user == id_usuario) continue;

        unordered_map<int, double> current_ratings;
        for (const auto& r : user_ratings[user]) {
            current_ratings[r.song_id] = r.score;
        }

        double dot_product = 0.0, mag_target = 0.0, mag_current = 0.0;
        for (auto& pair : target_ratings) {
            int song_id = pair.first;
            if (current_ratings.count(song_id)) {
                dot_product += pair.second * current_ratings[song_id];
            }
            mag_target += pair.second * pair.second;
        }
        for (auto& pair : current_ratings) {
            mag_current += pair.second * pair.second;
        }

        mag_target = sqrt(mag_target);
        mag_current = sqrt(mag_current);

        double similarity = (mag_target == 0 || mag_current == 0) ? 0.0 : dot_product / (mag_target * mag_current);
        if (similarity > 0) {
            similar_users.emplace_back(user, similarity);
        }
    }

    sort(similar_users.begin(), similar_users.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    if (similar_users.size() > K) similar_users.resize(K);

    unordered_map<int, double> recomendadas;
    for (const auto& [user, sim] : similar_users) {
        for (const auto& rate : user_ratings[user]) {
            if (canciones_usuario.count(rate.song_id) == 0) {
                recomendadas[rate.song_id] += sim * rate.score;
            }
        }
    }

    vector<pair<int, double>> recomendaciones(recomendadas.begin(), recomendadas.end());
    sort(recomendaciones.begin(), recomendaciones.end(), [](auto& a, auto& b) {
        return a.second > b.second;
    });

    cout << "Recomendaciones para el usuario " << id_usuario << ":\n";
    int count = 0;
    for (const auto& [song_id, score] : recomendaciones) {
        cout << "Canción: " << song_id << " -> Puntuación ponderada: " << score << endl;
        if (++count == K) break;
    }
	
}
void Query::usuarioMasActivo() {
    unordered_map<int, int> conteo;
    for (const Puntaje& pun : datos) {
        conteo[pun.id_usuario]++;
    }

    int max_usuario = -1, max_puntajes = -1;
    for (const auto& [usuario, cantidad] : conteo) {
        if (cantidad > max_puntajes) {
            max_puntajes = cantidad;
            max_usuario = usuario;
        }
    }

    if (max_usuario != -1) {
        cout << "Usuario más activo: " << max_usuario << " con " << max_puntajes << " puntuaciones.\n";
    } else {
        cout << "No se encontraron usuarios con puntuaciones.\n";
    }
}

void Query::promedioGlobal() {
    if (datos.empty()) {
        cout << "No hay puntuaciones registradas.\n";
        return;
    }

    double suma = 0.0;
    for (const Puntaje& pun : datos) {
        suma += pun.puntaje;
    }

    double promedio = suma / datos.size();
    cout << "Promedio global de puntuaciones: " << fixed << setprecision(2) << promedio << endl;
}


