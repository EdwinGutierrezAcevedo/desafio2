#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "huesped.h"
using namespace std;

huesped::huesped() {}

void huesped::setDocumento(const unsigned int &doc){
    documento=doc;
}

void huesped::setContrasena(const string &contra){
    contrasena=contra;
}

string huesped::leerArchivoTXT(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo); // Abrir archivo en modo lectura
    if (!archivo) {
        return "Error: No se pudo abrir el archivo.";
    }

    ostringstream contenido;
    contenido << archivo.rdbuf(); // Leer todo el contenido del archivo

    return contenido.str(); // Retornar el contenido como string
}


bool huesped::verficarDatos(string datos, unsigned int cedulaIngresada, string contrasenaIngresada){
    std::istringstream iss(datos);
    std::string linea, cedulaStr, contrasena;
    while (std::getline(iss, linea)) {
        std::istringstream lineaStream(linea);

        // Extraer cédula y contraseña separadas por '|'
        if (std::getline(lineaStream, cedulaStr, '|') &&
            std::getline(lineaStream, contrasena)) {

            try {
                unsigned int cedula = std::stoul(cedulaStr);  // Convertir cédula a número

                // Comparar con los valores ingresados
                if (cedula == cedulaIngresada && contrasena == contrasenaIngresada) {
                    setDocumento(cedula);
                    setContrasena(contrasena);
                    return true;  // Autenticación exitosa
                }
            } catch (const std::invalid_argument&) {
                std::cerr << "Error: cédula no válida en el archivo." << std::endl;
            } catch (const std::out_of_range&) {
                std::cerr << "Error: cédula fuera de rango." << std::endl;
            }
        }
    }
    return false;  // No se encontró coincidencia
}
