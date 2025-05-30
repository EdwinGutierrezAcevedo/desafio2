#ifndef FECHA_H
#define FECHA_H
#include <string>
using namespace std;
class fecha
{
private:
    short unsigned int dia,mes, anio;


public:
    //fecha();
    bool fechaValida(short unsigned int dia, short unsigned int  mes, short unsigned int anio);
    unsigned short diasDelMes(unsigned short m, unsigned short a) const;
    fecha(short unsigned int d, short unsigned int  m, short unsigned int a);
    unsigned short getAnio() { return anio; }
    unsigned short getMes() { return mes; }
    unsigned short getDia() { return dia; }
    bool operator<( fecha &other) ;
    bool operator>(fecha &other) ;
    unsigned int aDias() const;
    unsigned int operator-(const fecha &otra) const;
    string getDiaSemana() const;
    string getNombreMes() const;
    fecha obtenerFechaSalida(unsigned int duracion) const ;

};

#endif // FECHA_H
