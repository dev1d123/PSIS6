#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Box.H>
#include <bits/stdc++.h>
#include "Query.h"
#include "Puntaje.h"
#include "Rate.h"

using namespace std;

// Variables globales

Query* query = nullptr;
Fl_Text_Buffer* textbuf = nullptr;
int q = 20;  // valor por defecto

bool obtenerIDDesdeInput(Fl_Input* input, int& id_out, string& error) {
    string texto = input->value();
    if (texto.empty()) {
        error = "Por favor, ingrese un ID antes de continuar.";
        return false;
    }
    try {
        id_out = stoi(texto);
    } catch (...) {
        error = "El ID ingresado no es válido. Debe ser un número entero.";
        return false;
    }
    return true;
}


void mostrarResultado(const vector<string>& resultado) {
    textbuf->text("");  // limpia
    stringstream ss;
    for (const auto& linea : resultado) {
        ss << linea << "\n";
    }
    textbuf->text(ss.str().c_str());
}

void mostrarResultadoTabla(const vector<string>& resultado) {
    textbuf->text("");  // limpia
    stringstream ss;
    for (const auto& linea : resultado) {
        stringstream lineStream(linea);
        string palabra;
        while (lineStream >> palabra) {
            ss << palabra << '\t';
        }
        ss << '\n';
    }
    textbuf->text(ss.str().c_str());
}

// Handlers
void topUsuarioCB(Fl_Widget*, void* input_ptr) {
    int id;
    string error;
    if (!obtenerIDDesdeInput((Fl_Input*)input_ptr, id, error)) {
        mostrarResultado({error});
        return;
    }
    vector<string> res = query->topCancionesPorUsuario(id);
    mostrarResultado(res);
}

void topCancionCB(Fl_Widget*, void* input_ptr) {
    int id;
    string error;
    if (!obtenerIDDesdeInput((Fl_Input*)input_ptr, id, error)) {
        mostrarResultado({error});
        return;
    }
    vector<string> res = query->topUsuariosPorCancion(id);
    mostrarResultado(res);
}

void topVotadasCB(Fl_Widget*, void*) {
    vector<string> res = query->topCancionesMasVotadas();
    mostrarResultadoTabla(res);
}

void similaresCB(Fl_Widget*, void* input_ptr) {
    int id;
    string error;
    if (!obtenerIDDesdeInput((Fl_Input*)input_ptr, id, error)) {
        mostrarResultado({error});
        return;
    }
    vector<string> res = query->usuariosSimilares(id);
    mostrarResultado(res);
}

void recomendacionesCB(Fl_Widget*, void* input_ptr) {
    int id;
    string error;
    if (!obtenerIDDesdeInput((Fl_Input*)input_ptr, id, error)) {
        mostrarResultado({error});
        return;
    }
    vector<string> res = query->recomendacionesParaUsuario(id);
    mostrarResultado(res);
}

void masActivoCB(Fl_Widget*, void*) {
    vector<string> res = query->usuarioMasActivo();
    mostrarResultado(res);
}

void promedioCB(Fl_Widget*, void*) {
    vector<string> res = query->promedioGlobal();
    mostrarResultado(res);
}

int main(int argc, char** argv) {
    // Leer archivo CSV
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

    // Inicializar objeto Query
    query = new Query(arr, songs, users, user_ratings, q);

    // Crear ventana
    Fl_Window* win = new Fl_Window(880, 600, "Consultas Musicales");
    win->begin();

    Fl_Box* titulo = new Fl_Box(20, 10, 760, 30, "Sistema de Consultas Musicales");
    titulo->labelsize(20);
    titulo->labelfont(FL_BOLD);

// Variable viva (no temporal)
	string resumen_str = "Usuarios: " + to_string(users.size()) +
                     " | Canciones: " + to_string(songs.size()) +
                     " | Puntuaciones: " + to_string(arr.size());

	Fl_Box* resumenBox = new Fl_Box(20, 40, 400, 30, resumen_str.c_str());
	resumenBox->labelsize(14);


    Fl_Input* input_id = new Fl_Input(600, 50, 160, 30, "ID de usuario o canción:");
    input_id->labelsize(14);


    // Botones
    int y = 140;
    Fl_Button* b1 = new Fl_Button(20, y, 300, 30, "Top canciones favoritas del usuario");
    b1->callback(topUsuarioCB, input_id);

    Fl_Button* b2 = new Fl_Button(20, y += 40, 300, 30, "Top usuarios que votaron mejor una canción");
    b2->callback(topCancionCB, input_id);

    Fl_Button* b3 = new Fl_Button(20, y += 40, 300, 30, "Canciones más votadas");
    b3->callback(topVotadasCB);

    Fl_Button* b4 = new Fl_Button(20, y += 40, 300, 30, "Usuarios con gustos similares");
    b4->callback(similaresCB, input_id);

    Fl_Button* b5 = new Fl_Button(20, y += 40, 300, 30, "Recomendaciones de canciones para el usuario");
    b5->callback(recomendacionesCB, input_id);

    Fl_Button* b6 = new Fl_Button(20, y += 40, 300, 30, "Usuario más activo");
    b6->callback(masActivoCB);

    Fl_Button* b7 = new Fl_Button(20, y += 40, 300, 30, "Promedio global de puntuaciones");
    b7->callback(promedioCB);

    // Área de texto para mostrar resultados
    textbuf = new Fl_Text_Buffer();
    Fl_Text_Display* display = new Fl_Text_Display(340, 140, 520, 400);
    display->buffer(textbuf);
    display->textfont(FL_COURIER);  // Fuente monoespaciada para formateo tabular
    display->textsize(12);

    win->end();
    win->show(argc, argv);
    return Fl::run();
}

