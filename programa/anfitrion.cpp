#include "Anfitrion.h"
#include "UdeaStay.h"  // Para acceder a UdeaStay::globalIteraciones
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

///// CONSTRUCTORES Y MÉTODOS EXISTENTES /////

Anfitrion::Anfitrion()
    : documento(""), contrasena(""), antiguedad(0u), puntuacion(0.0), alojamientos(nullptr), numAlojamientos(0u)
{
    // Constructor por defecto (sin iteraciones relevantes).
}

Anfitrion::Anfitrion(const string& doc,
                     const string& pass,
                     unsigned int antiguedad,
                     double punt,
                     const string* alojamientos_in,
                     unsigned int num)
    : documento(doc), contrasena(pass), antiguedad(antiguedad), puntuacion(punt), numAlojamientos(num)
{
    if (num > 0 && alojamientos_in != nullptr) {
        alojamientos = new string[num];
        // Copiar cada código de alojamiento y contar cada iteración.
        for (unsigned int i = 0u; i < num; i++) {
            alojamientos[i] = alojamientos_in[i];
            cout<<alojamientos[i]<<endl;
            UdeaStay::globalIteraciones++; // Cuenta de iteración en la copia.
        }
    } else {
        alojamientos = nullptr;
        numAlojamientos = 0u;
    }
}

Anfitrion::~Anfitrion() {
    if (alojamientos != nullptr) {
        delete[] alojamientos;
    }
}

string Anfitrion::getDocumento() const { return documento; }
string Anfitrion::getContrasena() const { return contrasena; }
unsigned int Anfitrion::getAntiguedad() const { return antiguedad; }
double Anfitrion::getPuntuacion() const { return puntuacion; }
unsigned int Anfitrion::getNumAlojamientos() const { return numAlojamientos; }

bool Anfitrion::gestionaAlojamiento(const string& codigo) const {
    for (unsigned int i = 0u; i < numAlojamientos; i++) {
        UdeaStay::globalIteraciones++; // Contar cada iteración del ciclo.
        if (alojamientos[i] == codigo) {
            return true;
        }
    }
    return false;
}

///// FUNCIÓN ESTÁTICA DE AUTENTICACIÓN /////

/*
   Esta función abre el archivo 'nombreArchivo' y recorre cada línea.
   Cada línea se espera tenga:
     documento contraseña antiguedad puntuacion alojamientos
   Si se encuentra una línea que coincide en documento y contraseña con los parámetros 'doc' y 'contrasena',
   se parsea el campo de alojamientos (separando por comas) y se crea un objeto Anfitrion utilizando el constructor parametrizado.
   Se incrementa UdeaStay::globalIteraciones en cada parte del proceso que requiera un ciclo (lectura de línea, parseo y copia).
*/
Anfitrion* Anfitrion::autenticar(const char* nombreArchivo, const string& doc, const string& contrasena) {
    ifstream file(nombreArchivo);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo de anfitriones." << endl;
        return nullptr;
    }

    string line;
    while(getline(file, line)) {
        UdeaStay::globalIteraciones++; // Cuenta cada línea leída.
        if (line.empty())
            continue;

        istringstream iss(line);
        string fileDoc, filePass, alojamientosField;
        unsigned int fileAntiguedad;
        double filePunt;
        // Leer los campos de la línea (suponiendo que están separados por espacios)
        if (!(iss >> fileDoc >> filePass >> fileAntiguedad >> filePunt >> alojamientosField)) {
            // Si ocurre algún error al leer la línea, saltar a la siguiente.
            continue;
        }
        UdeaStay::globalIteraciones++; // Cuenta la lectura de los campos.

        // Verificar que el documento y la contraseña coincidan.
        if (fileDoc == doc && filePass == contrasena) {
            // Procesar el campo de alojamientos sin usar vector.
            // Se espera que alojamientosField contenga cadenas separadas por comas, ej: "A001,A003,A005"
            unsigned int numAlojs = 0;
            if (!alojamientosField.empty()) {
                numAlojs = 1;
                for (size_t i = 0; i < alojamientosField.size(); i++) {
                    UdeaStay::globalIteraciones++; // Contar cada iteración en el conteo
                    if (alojamientosField[i] == ',')
                        numAlojs++;
                }
            }

            // Crear un arreglo dinámico de strings para guardar los códigos.
            string* arrAlojs = nullptr;
            if (numAlojs > 0) {
                arrAlojs = new string[numAlojs];
                // Separar la cadena utilizando find() y substr().
                size_t startPos = 0;
                unsigned int tokenIndex = 0;
                while (true) {
                    size_t pos = alojamientosField.find(',', startPos);
                    UdeaStay::globalIteraciones++; // Contar esta iteración.
                    if (pos == string::npos) {
                        // No se encontró más coma: guardar el segmento final.
                        arrAlojs[tokenIndex] = alojamientosField.substr(startPos);
                        UdeaStay::globalIteraciones++; // Contar la copia final.
                        tokenIndex++;
                        break;
                    }
                    arrAlojs[tokenIndex] = alojamientosField.substr(startPos, pos - startPos);
                    UdeaStay::globalIteraciones++; // Contar la copia del token.
                    tokenIndex++;
                    startPos = pos + 1;
                }
                // tokenIndex debería coincidir con numAlojs.
            }

            // Crear y retornar el objeto Anfitrion utilizando los datos cargados.
            Anfitrion* a = new Anfitrion(fileDoc, filePass, fileAntiguedad, filePunt, arrAlojs, numAlojs);
            delete[] arrAlojs;  // Se libera el arreglo temporal, ya que el constructor de Anfitrion copia los datos.
            return a;
        }
    }
    return nullptr; // Ningún registro coincidió.
}
