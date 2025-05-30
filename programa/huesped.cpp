#include "Huesped.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>      // Para remove() y rename()
#include <cstring>     // Para strcpy, strncat, strlen
#include <cstdlib>     // Para exit() en caso de error
using std::ifstream;
using std::ofstream;
using std::ios;
using std::cout;
using std::cerr;
using std::endl;
using namespace std;

// Constructor
Huesped::Huesped(const string& documento, unsigned int antiguedad, double puntuacion, const string* reservas_in, unsigned int numReserva)
    : documento(documento), antiguedad(antiguedad), puntuacion(puntuacion), numReservas(numReserva)
{
    if(numReservas > 0 && reservas_in != nullptr) {
        reservas = new string[numReservas];
        for (unsigned int i = 0; i < numReservas; i++) {
            reservas[i] = reservas_in[i];
        }
    } else {
        reservas = nullptr;
        numReservas = 0;
    }
}

// Constructor de copia
Huesped::Huesped(const Huesped& other)
    : documento(other.documento), antiguedad(other.antiguedad), puntuacion(other.puntuacion), numReservas(other.numReservas)
{
    if (numReservas > 0 && other.reservas != nullptr) {
        reservas = new string[numReservas];
        for (unsigned int i = 0; i < numReservas; i++) {
            reservas[i] = other.reservas[i];
        }
    } else {
        reservas = nullptr;
    }
}

// Operador de asignación
Huesped& Huesped::operator=(const Huesped& other) {
    if (this != &other) {
        delete[] reservas;
        documento = other.documento;
        antiguedad = other.antiguedad;
        puntuacion = other.puntuacion;
        numReservas = other.numReservas;
        if (numReservas > 0 && other.reservas != nullptr) {
            reservas = new string[numReservas];
            for (unsigned int i = 0; i < numReservas; i++) {
                reservas[i] = other.reservas[i];
            }
        } else {
            reservas = nullptr;
        }
    }
    return *this;
}

// Destructor
Huesped::~Huesped() {
    delete[] reservas;
}

// Getters
string Huesped::getDocumento() const {
    return documento;
}

unsigned int Huesped::getAntiguedad() const {
    return antiguedad;
}

double Huesped::getPuntuacion() const {
    return puntuacion;
}

unsigned int Huesped::getNumReservas() const {
    return numReservas;
}

string Huesped::getReserva(unsigned int index) const {
    if (index < numReservas)
        return reservas[index];
    else
        return "";
}

// Método para agregar una nueva reserva
void Huesped::agregarReserva(const string& codigoReserva) {
    string* nuevoArreglo = new string[numReservas + 1];
    for (unsigned int i = 0; i < numReservas; i++) {
        nuevoArreglo[i] = reservas[i];
    }
    nuevoArreglo[numReservas] = codigoReserva;
    delete[] reservas;
    reservas = nuevoArreglo;
    numReservas++;
}
// Función de autenticación para Huesped.
// Lee el archivo y, si encuentra una línea que coincide con el documento y la contraseña,
// separa (mediante find/substr, sin usar vector) el campo de códigos de reservación.
// Si no hay reservaciones, se establece que numReservas es 0.
Huesped* Huesped::autenticar(const char* nombreArchivo, const string& doc, const string& contrasena) {
    ifstream file(nombreArchivo);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo de huespedes." << endl;
        return nullptr;
    }
    string line;
    while(getline(file, line)) {
        if (line.empty())
            continue;
        istringstream iss(line);
        string fileDoc, filePass, reservasField;
        unsigned int fileAntiguedad;
        double filePuntuacion;
        // Se leen los primeros cuatro campos.
        if (!(iss >> fileDoc >> filePass >> fileAntiguedad >> filePuntuacion))
            continue;
        // Intentar leer el campo de reservaciones.
        if(!(iss >> reservasField))
            reservasField = ""; // Puede ser que no tenga reservaciones (0 reservas).

        // Verificar el documento y la contraseña.
        if (fileDoc == doc && filePass == contrasena) {
            // Contar el número de reservaciones.
            unsigned int numRes = 0;
            if (!reservasField.empty()) {
                numRes = 1;  // Al menos un código
                for (unsigned int i = 0; i < reservasField.size(); i++) {
                    if (reservasField[i] == ',')
                        numRes++;
                }
            }
            // Crear el arreglo dinámico de códigos de reservación.
            string* arrRes = nullptr;
            if (numRes > 0) {
                arrRes = new string[numRes];
                unsigned int tokenIndex = 0;
                size_t startPos = 0;
                while (true) {
                    size_t pos = reservasField.find(',', startPos);
                    if (pos == string::npos) {
                        // Se guarda el último token.
                        arrRes[tokenIndex] = reservasField.substr(startPos);
                        tokenIndex++;
                        break;
                    }
                    arrRes[tokenIndex] = reservasField.substr(startPos, pos - startPos);
                    tokenIndex++;
                    startPos = pos + 1;
                }
            }
            // Crear y retornar el objeto Huesped.
            Huesped* h = new Huesped(fileDoc, fileAntiguedad, filePuntuacion, arrRes, numRes);
            delete[] arrRes; // El constructor ya copia los datos.
            return h;
        }
    }
    // Si no se encontró ningún registro coincidente.
    return nullptr;
}
