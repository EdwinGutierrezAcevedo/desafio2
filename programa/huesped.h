#ifndef HUESPED_H
#define HUESPED_H

#include <string>
#include "udeastay.h"
using std::string;

class Huesped {
private:
    string documento;
    unsigned int antiguedad;
    double puntuacion; // De 0 a 5.0
    string* reservas;  // Arreglo dinámico de códigos de reservación.
    unsigned int numReservas; // Número de reservas en el arreglo.

public:
    // Constructor: se espera que reservas_in apunte a un arreglo de string de tamaño numReservas.
    Huesped(const string& documento, unsigned int antiguedad, double puntuacion, const string* reservas_in, unsigned int numReservas);

    // Constructor de copia
    Huesped(const Huesped& other);

    // Operador de asignación
    Huesped& operator=(const Huesped& other);

    // Destructor para liberar el arreglo dinámico.
    ~Huesped();

    // Getters
    string getDocumento() const;
    unsigned int getAntiguedad() const;
    double getPuntuacion() const;
    unsigned int getNumReservas() const;
    // Retorna el código de reserva en la posición index (se debe validar el rango)
    string getReserva(unsigned int index) const;

    // Método para agregar una nueva reserva.
    void agregarReserva(const string& codigoReserva);

    static Huesped* autenticar(const char* nombreArchivo, const string& doc, const string& contrasena);
};

#endif // HUESPED_H
