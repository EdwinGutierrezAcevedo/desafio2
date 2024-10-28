#include "Estacion.h"
using namespace std;
int main() {
    // Crear una estación de servicio
    Estacion estacion("Estación Central", "EC123", "Juan Pérez", "Región Norte", "6.2442, -75.5812", 1000, 500, 300);

    // Mostrar el estado inicial de la estación
    estacion.mostrarEstado();

    // Editar la cantidad de gasolina en el tanque
    estacion.setCantidadRegular(800);
    estacion.setCantidadPremium(400);
    estacion.setCantidadEcoExtra(200);
    estacion.mostrarEstado();

    // Renombrar una máquina y cambiar su cédula
    estacion.renombrarMaquina(0, "Maquina Principal");
    estacion.setCedulaMaquina(0, "Cedula Principal");
    //cout<< "\nDespués de renombrar una máquina y cambiar su cédula:\n";
    estacion.mostrarEstado();

    // Registrar una venta
    estacion.venderCombustible(0, "2024-10-28", "08:30", 50, "Regular", "Efectivo", "12345678", 200000);
    estacion.mostrarEstado();

    return 0;
}
