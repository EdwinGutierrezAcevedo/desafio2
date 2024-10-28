#include "Maquina.h"
#include <fstream>
#include <iostream>

Maquina::Maquina(const std::string &nombre, const std::string &cedula) : nombre(nombre), cedula(cedula) {}

std::string Maquina::getNombre() const {
    return nombre;
}

void Maquina::setNombre(const std::string &nuevoNombre) {
    nombre = nuevoNombre;
}

std::string Maquina::getCedula() const {
    return cedula;
}

void Maquina::setCedula(const std::string &nuevaCedula) {
    cedula = nuevaCedula;
}

void Maquina::registrarVenta(const std::string &fecha, const std::string &hora, double cantidad, const std::string &categoria, const std::string &metodoPago, const std::string &documentoCliente, double cantidadDinero) {
    std::ofstream archivo("ventas.txt", std::ios::app);
    if (archivo.is_open()) {
        archivo << "Fecha: " << fecha << ", Hora: " << hora << ", Cantidad: " << cantidad << " litros, Categoría: " << categoria
                << ", Método de Pago: " << metodoPago << ", Documento Cliente: " << documentoCliente << ", Cantidad de Dinero: " << cantidadDinero << " COP\n";
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo de ventas.\n";
    }
}
