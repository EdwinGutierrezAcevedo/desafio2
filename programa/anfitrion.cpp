#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "anfitrion.h"

anfitrion::anfitrion() {}

void anfitrion::setDocumentoAnfitrion(const unsigned int &doc){
    documentoAnfitrion=doc;
}

void anfitrion::setContrasenaAnfitrion(const string &contra){
    contrasenaAnfitrion=contra;
}

bool anfitrion::verficarDatosAnfitrion(string datos, unsigned int cedulaIngresada, string contrasenaIngresada){
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
                    setDocumentoAnfitrion(cedula);
                    setContrasenaAnfitrion(contrasena);
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
