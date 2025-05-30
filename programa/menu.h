#ifndef MENU_H
#define MENU_H

#include <string>
#include "UdeaStay.h"   // Para manejar la carga de reservaciones e iteraciones
#include "Anfitrion.h"   // Para la autenticación y manejo de datos del anfitrión

class Menu {
public:
    string **reservaciones;
    unsigned int numReservaciones, numReservacionesTotal=10;
    UdeaStay user;
    Anfitrion* anfitrion;
    Menu();
    ~Menu();

    // Inicia la ejecución principal del menú y coordina el flujo.
    void iniciar();

private:
    // Pregunta al usuario si es anfitrión o huésped.
    void seleccionarTipoUsuario();

    // Menú específico para anfitrión: autenticación y opciones (anular reserva, consultar reservaciones, etc.)
    void anfitrionMenu(string **&reservaciones, unsigned int & numReservaciones);
    void huespedMenu() ;
    fecha leerFechaValida(const string &mensaje);
    // Versión sobrecargada que además verifica que la fecha ingresada sea menor o igual a maxAllowed.
    fecha leerFechaValida(const string &mensaje, fecha &maxAllowed);
    void mostrarFecha( fecha &f) const;
    void mostrarReserva( fecha &fechaInicio, unsigned int duracion) const;

    // Solicita obligatoriamente un rango de fechas y muestra las reservaciones
    // filtradas de los alojamientos administrados por el anfitrión.
    void mostrarReservacionesAnfitrionConFecha(const Anfitrion &anfitrion, std::string** reservaciones, unsigned int n_reservaciones);

    // Muestra un resumen final de la operación: iteraciones realizadas y memoria en uso.
    void mostrarResumenFinal();
    void eliminarReservacion();
};

#endif // MENU_H
