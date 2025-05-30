#include <string>
#include <iostream>
#include "fecha.h"

using namespace std;

bool fecha::fechaValida(unsigned short d, unsigned short m, unsigned short a) {
    if(m < 1 || m > 12) return false;
    if(d < 1) return false;
    unsigned short maxDias = diasDelMes(m, a);
    return (d <= maxDias);
}

// Método auxiliar que retorna cuántos días tiene el mes 'm' en el año 'a'
unsigned short fecha::diasDelMes(unsigned short m, unsigned short a) const {
    switch(m) {
    case 1:  return 31;
    case 2:  {
        // Año bisiesto: divisible por 4, pero los centenarios deben ser divisibles por 400.
        if ((a % 4 == 0 && a % 100 != 0) || (a % 400 == 0))
            return 29;
        else
            return 28;
    }
    case 3:  return 31;
    case 4:  return 30;
    case 5:  return 31;
    case 6:  return 30;
    case 7:  return 31;
    case 8:  return 31;
    case 9:  return 30;
    case 10: return 31;
    case 11: return 30;
    case 12: return 31;
    default: return 30; // De forma segura (aunque no se debería llegar aquí)
    }
}

// Retorna el nombre del mes en minúsculas
string fecha::getNombreMes() const {
    switch(mes) {
    case 1:  return "enero";
    case 2:  return "febrero";
    case 3:  return "marzo";
    case 4:  return "abril";
    case 5:  return "mayo";
    case 6:  return "junio";
    case 7:  return "julio";
    case 8:  return "agosto";
    case 9:  return "septiembre";
    case 10: return "octubre";
    case 11: return "noviembre";
    case 12: return "diciembre";
    default: return "desconocido";
    }
}

// Calcula la fecha de salida sumando "duracion" días a la fecha actual,
// usando el número real de días que tiene cada mes.
fecha fecha::obtenerFechaSalida(unsigned int duracion) const {
    unsigned short d = dia;
    unsigned short m = mes;
    unsigned short a = anio;

    while(duracion > 0) {
        unsigned short diasMes = diasDelMes(m, a);
        // Si al sumar la duracion no se excede el mes actual:
        if(d + duracion <= diasMes) {
            d += duracion;
            duracion = 0;
        } else {
            // Restamos los días restantes del mes y avanzamos al mes siguiente
            unsigned int restante = diasMes - d + 1; // días que faltan para terminar el mes
            duracion -= restante;
            d = 1;
            m++;
            if(m > 12) {
                m = 1;
                a++;
            }
        }
    }
    return fecha(d, m, a);
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

bool fecha::operator<(fecha &other) {
    if(anio < other.getAnio())
        return true;
    if(anio > other.getAnio())
        return false;
    // Años iguales; comparar meses:
    if(mes < other.getMes())
        return true;
    if(mes > other.getMes())
        return false;
    // Años y meses iguales; comparar días:
    return dia < other.getDia();
}

bool fecha::operator>( fecha& other) {
    // Una forma sencilla es definirlo en función del operador <:
    return other < *this;
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
