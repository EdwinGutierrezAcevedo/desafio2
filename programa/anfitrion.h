#ifndef ANFITRION_H
#define ANFITRION_H

#include <string>
using std::string;

class Anfitrion {
private:
    string documento;         // Número de documento del anfitrión.
    string contrasena;        // Contraseña para la autenticación.
    unsigned int antiguedad;  // Antigüedad en la plataforma (en meses).
    double puntuacion;        // Puntuación (de 0 a 5.0).
    string* alojamientos;     // Arreglo dinámico con los códigos de los alojamientos que administra.
    unsigned int numAlojamientos;  // Número de alojamientos que administra.

public:
    // Constructor por defecto.
    Anfitrion();

    // Constructor parametrizado.
    // Se asume que 'alojamientos_in' es un arreglo de códigos y 'numAlojamientos' es siempre positivo.
    Anfitrion(const string& doc,
              const string& pass,
              unsigned int antiguedad,
              double punt,
              const string* alojamientos_in,
              unsigned int numAlojamientos);

    // Destructor.
    ~Anfitrion();

    // Getters.
    string getDocumento() const;
    string getContrasena() const;
    unsigned int getAntiguedad() const;
    double getPuntuacion() const;
    unsigned int getNumAlojamientos() const;

    // Retorna true si el código de alojamiento se encuentra en el arreglo.
    bool gestionaAlojamiento(const string& codigo) const;

    // Función estática para autenticar a un anfitrión leyendo de un archivo con el formato:
    // documento contraseña antiguedad puntuacion alojamientos
    // Se espera que el campo 'alojamientos' contenga códigos separados por comas (por ejemplo, "A001,A003").
    // Cada iteración en la lectura, el parseo y el copiado se sumará al contador global de iteraciones (UdeaStay::globalIteraciones).
    static Anfitrion* autenticar(const char* nombreArchivo, const string& doc, const string& contrasena);
};

#endif // ANFITRION_H
