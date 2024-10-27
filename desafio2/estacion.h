#include <string>
#ifndef ESTACION_H
#define ESTACION_H
#include "tanquecentral.h"

using namespace std;
class Estacion
{
    string nombre, codigo, gerente, region, ubicacion;
    Tanquecentral tanque1;

public:
    Estacion();
};

#endif // ESTACION_H
