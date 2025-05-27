#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
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

std::string anfitrion::leerArchivoTXTreserva(const std::string &nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << nombreArchivo << std::endl;
        return "";
    }
    std::ostringstream oss;
    oss << archivo.rdbuf();
    return oss.str();
}

char*** anfitrion::crearMatrizReservaciones(const std::string &contenido, int capacidad, int &numFilas) {
    char*** matriz = new char**[capacidad];
    for (int i = 0; i < capacidad; i++) {
        matriz[i] = nullptr;
    }
    numFilas = 0;

    std::istringstream iss(contenido);
    std::string linea;
    while (std::getline(iss, linea) && numFilas < capacidad) {
        if (linea.empty()) continue;

        char* cstr = new char[linea.size() + 1];
        std::strcpy(cstr, linea.c_str());

        char** fila = new char*[COLUMNAS];
        int col = 0;
        char* token = std::strtok(cstr, "|");
        while (token != nullptr && col < COLUMNAS) {
            int len = std::strlen(token) + 1;
            fila[col] = new char[len];
            std::strcpy(fila[col], token);
            col++;
            token = std::strtok(nullptr, "|");
        }
        while (col < COLUMNAS) {
            fila[col] = new char[1];
            fila[col][0] = '\0';
            col++;
        }
        matriz[numFilas] = fila;
        numFilas++;
        delete[] cstr;
    }
    return matriz;
}

void anfitrion::agregarReservacion(char*** &matriz, int &numFilas, int capacidad, const char *datos[COLUMNAS]) {
    if (numFilas == capacidad) {
        std::cout << "Capacidad alcanzada (" << capacidad << "). No se puede agregar más reservaciones." << std::endl;
        return;
    }
    char** fila = new char*[COLUMNAS];
    for (int j = 0; j < COLUMNAS; j++) {
        int len = std::strlen(datos[j]) + 1;
        fila[j] = new char[len];
        std::strcpy(fila[j], datos[j]);
    }
    matriz[numFilas] = fila;
    numFilas++;
}

void anfitrion::eliminarReservacion(char*** &matriz, int &numFilas, int indice) {
    if (indice < 0 || indice >= numFilas) {
        std::cout << "Índice fuera de rango. No se puede eliminar." << std::endl;
        return;
    }
    for (int j = 0; j < COLUMNAS; j++) {
        delete[] matriz[indice][j];
    }
    delete[] matriz[indice];

    for (int i = indice; i < numFilas - 1; i++) {
        matriz[i] = matriz[i + 1];
    }
    matriz[numFilas - 1] = nullptr;
    numFilas--;
}

void anfitrion::imprimirMatrizReservaciones(char*** matriz, int numFilas) {
    std::cout << "\nMatriz de Reservaciones:" << std::endl;
    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            std::cout << std::left << std::setw(15) << matriz[i][j];
        }
        std::cout << std::endl;
    }
}

void anfitrion::liberarMatrizReservaciones(char*** &matriz, int numFilas) {
    for (int i = 0; i < numFilas; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            delete[] matriz[i][j];
        }
        delete[] matriz[i];
    }
    delete[] matriz;
    matriz = nullptr;
}

