#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>

void callback(Fl_Widget* w, void*) {
    printf("¡Botón presionado!\n");
}

int main() {
    Fl_Window window(300, 200, "Mi ventana");
    Fl_Button button(100, 80, 100, 30, "Presiona");
    button.callback(callback);
    window.end();
    window.show();
    return Fl::run();
}

