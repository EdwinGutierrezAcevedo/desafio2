#include <string>
#include <iostream>
#include "fecha.h"

using namespace std;
//fecha::fecha() {}
bool fecha::esBisiesto(short unsigned int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}
bool fecha::fechaValida( short unsigned int dia, short unsigned int  mes, short unsigned int anio){

    if(dia<1 || dia>31) return false;
    else if(mes<1 || mes>12 ) return false;

    else switch (mes){
        case 4: case 6: case 9: case 11:
            if(dia>30) return false;
            break;
        case 2:
            if(dia>29) return false;
            if((dia==29 && !esBisiesto(anio))) return false;
            break;

        }
    return true;

}
//setters
fecha::fecha(short unsigned int d, short unsigned int  m, short unsigned int a){
    if(fechaValida(d,m,a)){
        dia = d;
        mes = m;
        anio = a;
    }else{
        dia = 0;
        mes = 0;
        anio = 0;
    }

}
//getters
short unsigned int fecha::getDia(){
    return dia;
}
short unsigned int fecha::getMes(){
    return mes;
}
short unsigned int fecha::getAnio(){
    return anio;
}
bool fecha::operator<( fecha& other){
    if(anio>other.getAnio()) return false;
    if(mes>other.getMes()) return false;
    if(dia>=other.getDia()) return false;
    return true;
}

bool fecha::operator>( fecha& other){
    if(anio<other.getAnio()) return false;
    if(mes<other.getMes()) return false;
    if(dia<=other.getDia()) return false;
    return true;
}

 // Convierte la fecha a un número entero de días usando la fórmula del día juliano.
unsigned int fecha::aDias() const {
    int a = (14 - mes) / 12;
    int y = anio + 4800 - a;
    int m = mes + 12 * a - 3;
    return dia + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}

unsigned int fecha::operator-(const fecha &otra) const {
    return this->aDias() - otra.aDias();
}


// Método para determinar el día de la semana usando la Congruencia de Zeller.
// Se mapea el resultado de h a un día en español:
//   h = 0 -> Sábado, 1 -> Domingo, 2 -> Lunes, 3 -> Martes, 4 -> Miércoles, 5 -> Jueves, 6 -> Viernes.
string fecha::getDiaSemana() const {
    int d = dia;
    int m = mes;
    int y = anio;

    // Si el mes es enero o febrero, se tratan como 13° y 14° mes del año anterior.
    if (m < 3) {
        m += 12;
        y -= 1;
    }

    int K = y % 100;   // Año dentro del siglo
    int J = y / 100;   // Cero basado en el siglo

    int h = (d + (13 * (m + 1)) / 5 + K + (K / 4) + (J / 4) + 5 * J) % 7;
    string dias[7] = {"Sabado", "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes"};
    return dias[h];
}
//pasar a apartado de despliegue
void fecha::imprimir() const {
    cout << (dia < 10 ? "0" : "") << dia << "/"
              << (mes < 10 ? "0" : "") << mes << "/"
              << anio;
}
