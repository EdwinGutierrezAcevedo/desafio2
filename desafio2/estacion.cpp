#include "Estacion.h"
#include <iostream>
using namespace std;
Estacion::Estacion(const string &nombre, const string &codigoIdentificador, const string &gerente, const string &region, const string &ubicacionGeografica, double cantidadInicialRegular, double cantidadInicialPremium, double cantidadInicialEcoExtra)
    : nombre(nombre), codigoIdentificador(codigoIdentificador), gerente(gerente), region(region), ubicacionGeografica(ubicacionGeografica), tanque(cantidadInicialRegular, cantidadInicialPremium, cantidadInicialEcoExtra) {
    for (int i = 0; i < 12; ++i) {
        maquinas[i] = new Maquina("Maquina " + std::to_string(i + 1), "Cedula " + std::to_string(i + 1));
    }
}

Estacion::~Estacion() {
    for (int i = 0; i < 12; ++i) {
        delete maquinas[i];
    }
}

void Estacion::mostrarEstado() const {
    cout << "Estación: " << nombre << "\nCódigo: " << codigoIdentificador << "\nGerente: " << gerente << "\nRegión: " << region << "\nUbicación: " << ubicacionGeografica << endl;
    cout << "Cantidad de gasolina Regular en el tanque: " << tanque.getCantidadRegular() << " litros" << endl;
    cout << "Cantidad de gasolina Premium en el tanque: " << tanque.getCantidadPremium() << " litros" << endl;
    cout << "Cantidad de gasolina EcoExtra en el tanque: " << tanque.getCantidadEcoExtra() << " litros" << endl;
    for (int i = 0; i < 12; ++i) {
        cout << maquinas[i]->getNombre() << " - Cedula: " << maquinas[i]->getCedula() << endl;
    }
}

void Estacion::setCantidadRegular(double nuevaCantidad) {
    tanque.setCantidadRegular(nuevaCantidad);
}

void Estacion::setCantidadPremium(double nuevaCantidad) {
    tanque.setCantidadPremium(nuevaCantidad);
}

void Estacion::setCantidadEcoExtra(double nuevaCantidad) {
    tanque.setCantidadEcoExtra(nuevaCantidad);
}

void Estacion::renombrarMaquina(int indice, const string &nuevoNombre) {
    if (indice >= 0 && indice < 12) {
        maquinas[indice]->setNombre(nuevoNombre);
    } else {
        cout << "Índice de máquina inválido." << endl;
    }
}

void Estacion::setCedulaMaquina(int indice, const string &nuevaCedula) {
    if (indice >= 0 && indice < 12) {
        maquinas[indice]->setCedula(nuevaCedula);
    } else {
        cout << "Índice de máquina inválido." << endl;
    }
}

void Estacion::venderCombustible(int indice, const string &fecha, const string &hora, double cantidad, const string &categoria, const string &metodoPago, const string &documentoCliente, double cantidadDinero) {
    if (indice >= 0 && indice < 12) {
        maquinas[indice]->registrarVenta(fecha, hora, cantidad, categoria, metodoPago, documentoCliente, cantidadDinero);
        tanque.actualizarDisponibilidad(categoria, cantidad);
    } else {
        cout << "Índice de máquina inválido." << endl;
    }
}
