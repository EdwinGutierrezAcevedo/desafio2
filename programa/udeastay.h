#ifndef UDEA_STAY_H
#define UDEA_STAY_H

#include <string>
#include "fecha.h"

class UdeaStay {
public:
    // Variable estática que acumula todas las iteraciones globales (ahora es pública)
    static unsigned long globalIteraciones;

private:
    // Memoria asignada para la matriz dinámica
    unsigned long memoriaUso;

public:
    UdeaStay();

    // Cambiamos el parámetro a unsigned int &, ya que el número de reservaciones es siempre positivo.
    std::string** cargarMatrizReservaciones(const char* nombreArchivo, unsigned int &n_reservaciones, const unsigned int &reservacionesTotal);

    // Getter para el contador global de iteraciones.
    unsigned long getIteraciones() const;

    // Getter para la memoria utilizada.
    unsigned long getMemoriaUso() const;

    void ordenarMatrizReservacionesPorFecha(std::string** reservaciones, unsigned int n_reservaciones);
    fecha actualizarHistorico(std::string** &reservaciones, unsigned int &n_reservaciones);
    bool validarFechas( fecha &f1, unsigned int dur1,
                       fecha &f2, unsigned int dur2);

    std::string** cargarMatrizAlojamientos(const char* nombreArchivo, unsigned int &n_alojamientos);
    bool eliminarReservacionPorAlojamiento(const string &codigoAlojamiento, string **matrizReservaciones, unsigned int &n_reservaciones);
    bool reescribirReservacionesTXT(string** matriz, unsigned int n_filas);
    bool hayConflictoHuesped(string **reservaciones,
                             unsigned int numReservaciones,
                             const string &documento,
                             fecha &fechaEntrada,
                             unsigned int duracion);
};

#endif // UDEA_STAY_H
