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
    bool esBisiesto(short unsigned int anio);
    //void setFecha(short unsigned int d, short unsigned int  m, short unsigned int a);
    fecha(short unsigned int d, short unsigned int  m, short unsigned int a);
    short unsigned int getDia();
    short unsigned int getMes();
    short unsigned int getAnio();
    bool operator<( fecha& other);
    bool operator>( fecha& other);
    unsigned int aDias() const;
    unsigned int operator-(const fecha &otra) const;
    string getDiaSemana() const;
    void imprimir() const;

};

#endif // FECHA_H
