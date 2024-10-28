#ifndef MAQUINA_H
#define MAQUINA_H

#include <string>

class Maquina {
private:
    std::string nombre;
    std::string cedula;
public:
    Maquina(const std::string &nombre, const std::string &cedula);
    std::string getNombre() const;
    void setNombre(const std::string &nuevoNombre);
    std::string getCedula() const;
    void setCedula(const std::string &nuevaCedula);
    void registrarVenta(const std::string &fecha, const std::string &hora, double cantidad, const std::string &categoria, const std::string &metodoPago, const std::string &documentoCliente, double cantidadDinero);
};

#endif // MAQUINA_H
