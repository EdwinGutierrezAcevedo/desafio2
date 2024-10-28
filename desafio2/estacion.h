#ifndef ESTACION_H
#define ESTACION_H

#include "Tanque.h"
#include "Maquina.h"
#include <string>

class Estacion {
private:
    std::string nombre;
    std::string codigoIdentificador;
    std::string gerente;
    std::string region;
    std::string ubicacionGeografica; // GPS coordinates in "latitude, longitude" format
    Tanque tanque;
    Maquina* maquinas[12];
public:
    Estacion(const string &nombre, const string &codigoIdentificador, const string &gerente, const string &region, const string &ubicacionGeografica, double cantidadInicialRegular, double cantidadInicialPremium, double cantidadInicialEcoExtra);
    ~Estacion();
    void mostrarEstado() const;
    void setCantidadRegular(double nuevaCantidad);
    void setCantidadPremium(double nuevaCantidad);
    void setCantidadEcoExtra(double nuevaCantidad);
    void renombrarMaquina(int indice, const string &nuevoNombre);
    void setCedulaMaquina(int indice, const string &nuevaCedula);
    void venderCombustible(int indice, const string &fecha, const string &hora, double cantidad, const string &categoria, const string &metodoPago, const string &documentoCliente, double cantidadDinero);
};

#endif // ESTACIOND_H
