#include <bits/stdc++.h>
#include "Query.h"
#include "Rate.h"
#include "Puntaje.h"

int K;

vector<string> ret;

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


vector<string> Query::cantidadCanciones() const {
	ret.clear();
	ret.push_back("Cantidad de canciones: " + canciones.size());
	return ret;
}

vector<string> Query::cantidadUsuarios() const {
    ret.clear();
	ret.push_back("Cantidad de usuarios: " + usuarios.size());
	return ret;
}

vector<string> Query::cantidadTotalPuntuaciones() const {
    ret.clear();
	ret.push_back("Cantidad total de puntuaciones: " + datos.size());
	return ret;
}
	

vector<string> Query::topCancionesPorUsuario(int id_usuario) {
	ret.clear();
	priority_queue<Rate, vector<Rate>, RateCompare> pq;
	for (const Rate& rate : user_ratings[id_usuario]) {
        pq.push(rate);
    }
	ret.push_back("Canciones mejor puntuadas del usuario " + to_string(id_usuario));
	int sz = K;
	while(!pq.empty() && sz){
		Rate r = pq.top(); pq.pop();
		ret.push_back("Canción: " + to_string(r.song_id) + " -> Puntaje: " + to_string(r.score));	
		sz--;
	}
	return ret;
}

vector<string> Query::topUsuariosPorCancion(int id_cancion) {
	ret.clear();
	priority_queue<Puntaje, vector<Puntaje>, PuntajeCompare> pq;
	for (Puntaje pun : datos) {
    	if (pun.id_cancion == id_cancion) {
        	pq.push(pun);
        }
    }

    if (pq.empty()) {
        ret.push_back("No hay puntuaciones registradas para la canción con ID " + to_string(id_cancion));
        return ret;
    }

	ret.push_back("Usuarios con mejores puntuaciones para la canción " + to_string(id_cancion));

	int sz = 20;
	while(!pq.empty() && sz){
		Puntaje p = pq.top(); pq.pop();
		ret.push_back("Usuario: " + to_string(p.id_usuario) + " -> Puntaje: " + to_string(p.puntaje));
		sz--;
	}
	return ret;
}

vector<string> Query::topCancionesMasVotadas() {
	ret.clear();

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

	string header = "ID Votos 0.5 1 1.5 2 2.5 3 3.5 4 4.5 5";
	ret.push_back(header);

    for (int i = 0; i < k; ++i) {
        int song_id = vote_vector[i].first;
        stringstream ss;
        ss << song_id
           << " " << song_votes[song_id];

        for (double score = 0.5; score <= 5.0; score += 0.5) {
            int count = song_kind[song_id][score];
            ss << " " << count;
        }
        ret.push_back(ss.str());
    }

	return ret;
}

vector<string> Query::usuariosSimilares(int id_usuario) {
	ret.clear();

	if(usuarios.find(id_usuario) == usuarios.end()){
		ret.push_back("Usuario no encontrado");
		return ret;
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
	
	ret.push_back("Usuarios más similares al usuario: " + to_string(id_usuario));
	for (const auto& [user, sim] : similar_users) {
        stringstream ss;
        ss << "Usuario " << user << ": similitud = " << fixed << setprecision(3) << sim;
        ret.push_back(ss.str());
    }

	return ret;
}

vector<string> Query::recomendacionesParaUsuario(int id_usuario) {
	ret.clear();

	if (usuarios.find(id_usuario) == usuarios.end()) {
        ret.push_back("Usuario no encontrado");
        return ret;
    }

    unordered_set<int> canciones_usuario;
    for (const auto& r : user_ratings[id_usuario]) {
		canciones_usuario.insert(r.song_id);
    }

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

	ret.push_back("Recomendaciones para el usuario " + to_string(id_usuario) + ":");
    int count = 0;
    for (const auto& [song_id, score] : recomendaciones) {
        stringstream ss;
        ss << "Canción: " << song_id << " -> Puntuación ponderada: " << fixed << setprecision(2) << score;
        ret.push_back(ss.str());
        if (++count == K) break;
    }

	return ret;
}

vector<string> Query::usuarioMasActivo() {
	ret.clear();
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
        ret.push_back("Usuario más activo: " + to_string(max_usuario) + " con " + to_string(max_puntajes) + " puntuaciones.");
    } else {
        ret.push_back("No se encontraron usuarios con puntuaciones.");
    }

	return ret;
}

vector<string> Query::promedioGlobal() {
	ret.clear();
	if (datos.empty()) {
        ret.push_back("No hay puntuaciones registradas.");
        return ret;
    }

    double suma = 0.0;
    for (const Puntaje& pun : datos) {
        suma += pun.puntaje;
    }

    double promedio = suma / datos.size();
	stringstream ss;
	ss << "Promedio global de puntuaciones: " << fixed << setprecision(2) << promedio;
	ret.push_back(ss.str());

	return ret;
}


