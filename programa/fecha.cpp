#include "fecha.h"


fecha::fecha() {}
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
void fecha::setFecha(short unsigned int d, short unsigned int  m, short unsigned int a){
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
