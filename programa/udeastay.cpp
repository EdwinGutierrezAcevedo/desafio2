#include "UdeaStay.h"
#include "menu.h"
#include "fecha.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Inicializamos la variable estática
using namespace std;
unsigned long UdeaStay::globalIteraciones = 0;

UdeaStay::UdeaStay() : memoriaUso(0) { }

std::string** UdeaStay::cargarMatrizReservaciones(const char* nombreArchivo, unsigned int &n_reservaciones, const unsigned int& reservacionesTotal) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        std::cout << "Error al abrir el archivo." << std::endl;
        return nullptr;
    }

    // Primera pasada: contar la cantidad de reservaciones.
    n_reservaciones = 0;
    std::string fechaEntrada, duracion, codigoReservacion, codigoAlojamiento, documentoHuesped, metodoPago, fechaPago, monto, comentarios;
    while (archivo >> fechaEntrada >> duracion >> codigoReservacion >>codigoAlojamiento >> documentoHuesped
           >> metodoPago >> fechaPago >> monto) {
        std::getline(archivo, comentarios);
        n_reservaciones++;
        globalIteraciones++;  // Cada lectura de línea cuenta como una iteración.
    }
    archivo.close();

    // Reabrir el archivo para reiniciar la lectura.
    archivo.open(nombreArchivo);
    if (!archivo.is_open()) {
        std::cout << "Error al reabrir el archivo." << std::endl;
        return nullptr;
    }
    //archivo >> fechaEntrada;  // Si hubiese cabecera o semilla, se omite.

    // Reservar memoria para la matriz dinámica:
    std::string** matrizReservaciones = new std::string*[reservacionesTotal];
    memoriaUso = n_reservaciones * sizeof(std::string*);
    for (unsigned int i = 0; i < reservacionesTotal; i++) {
        matrizReservaciones[i] = new std::string[9];
        memoriaUso += 9 * sizeof(std::string);
        globalIteraciones++;  // Cada asignación de fila cuenta como una iteración.
    }

    // Segunda pasada: leer y almacenar las reservaciones.
    for (unsigned int i = 0; i < n_reservaciones; i++) {
        if (archivo >> fechaEntrada >> duracion >> codigoReservacion >>codigoAlojamiento >> documentoHuesped
            >> metodoPago >> fechaPago >> monto) {
            std::getline(archivo, comentarios);
            if (!comentarios.empty() && comentarios[0] == ' ')
                comentarios.erase(0, 1);
            matrizReservaciones[i][0] = fechaEntrada;
            matrizReservaciones[i][1] = duracion;
            matrizReservaciones[i][2] = codigoReservacion;
            matrizReservaciones[i][3] = codigoAlojamiento;
            matrizReservaciones[i][4] = documentoHuesped;
            matrizReservaciones[i][5] = metodoPago;
            matrizReservaciones[i][6] = fechaPago;
            matrizReservaciones[i][7] = monto;
            matrizReservaciones[i][8] = comentarios;
            globalIteraciones++;  // Cada línea procesada en la segunda pasada cuenta como una iteración.
        }




    }

    archivo.close();
    return matrizReservaciones;
}

// Función que ordena la matriz de reservaciones por fecha (columna 0) usando bubble sort.
void UdeaStay::ordenarMatrizReservacionesPorFecha(std::string** reservaciones, unsigned int n_reservaciones) {
    // Algoritmo de bubble sort para ordenar por fecha (campo en reservaciones[i][0])
    for (unsigned int i = 0; i < n_reservaciones - 1; i++) {
        for (unsigned int j = 0; j < n_reservaciones - i - 1; j++) {
            globalIteraciones++; // Contar cada comparación
            // Obtener la fecha de entrada de la fila j y j+1 (formato: "AAAA-MM-DD")
            string dateStr1 = reservaciones[j][0];
            string dateStr2 = reservaciones[j+1][0];

            // Extraer año, mes y día para cada fecha
            unsigned short anio1 = static_cast<unsigned short>(stoi(dateStr1.substr(0, 4)));
            unsigned short mes1  = static_cast<unsigned short>(stoi(dateStr1.substr(5, 2)));
            unsigned short dia1  = static_cast<unsigned short>(stoi(dateStr1.substr(8, 2)));
            unsigned short anio2 = static_cast<unsigned short>(stoi(dateStr2.substr(0, 4)));
            unsigned short mes2  = static_cast<unsigned short>(stoi(dateStr2.substr(5, 2)));
            unsigned short dia2  = static_cast<unsigned short>(stoi(dateStr2.substr(8, 2)));

            // Crear objetos fecha para la comparación
            fecha f1(dia1, mes1, anio1);
            fecha f2(dia2, mes2, anio2);

            // Si f2 es menor que f1, se intercambian las filas
            if (f2 < f1) {
                string* temp = reservaciones[j];
                reservaciones[j]   = reservaciones[j+1];
                reservaciones[j+1] = temp;
                globalIteraciones++; // Contar el swap
            }
        }
    }
    for (unsigned int i = 0; i < n_reservaciones; i++) {
        cout << "Reservación #" << i + 1 << ":\n";
        cout << "  Fecha Entrada     : " << reservaciones[i][0] << "\n";
        cout << "  Duración          : " <<reservaciones[i][1] << "\n";
        cout << "  Código Reservación: " << reservaciones[i][2] << "\n";
        cout << "  Código Alojamiento: " << reservaciones[i][3] << "\n";
        cout << "  Documento Huésped : " << reservaciones[i][4] << "\n";
        cout << "  Método de Pago    : " << reservaciones[i][5] << "\n";
        cout << "  Fecha de Pago     : " << reservaciones[i][6] << "\n";
        cout << "  Monto             : " << reservaciones[i][7] << "\n";
        cout << "  Comentarios       : " << reservaciones[i][8] << "\n";
        cout << "-------------------------------------\n";
    }

}

unsigned long UdeaStay::getIteraciones() const {
    return globalIteraciones;
}

unsigned long UdeaStay::getMemoriaUso() const {
    return memoriaUso;
}

// Función que actualiza el histórico: mueve a un archivo (historico.txt) todas las reservaciones
// cuya fecha en la columna 0 sea menor que la fecha de corte ingresada.
// Después de escribir al archivo en modo append (sin sobreescribir),
// se eliminan esas filas de la matriz (se desplazan las filas restantes).
// Finalmente, se muestra un resumen de la operación (iteraciones y memoria) y se retorna la fecha de corte.
fecha UdeaStay::actualizarHistorico(std::string** &reservaciones, unsigned int &n_reservaciones) {
    // Solicitar al anfitrión la fecha de corte:
    unsigned short dia, mes, anio;
    cout << "\nIngrese la fecha de corte para actualizar histórico (día mes año): ";
    cin >> dia >> mes >> anio;
    // Contar iteración de entrada (opcional)
    globalIteraciones++;
    fecha fechaCorte(dia, mes, anio);

    // Debido a que la matriz está ordenada por fecha, se cuentan las reservaciones con fecha menor que la fecha de corte.
    unsigned int k = 0;
    for (unsigned int i = 0; i < n_reservaciones; i++) {
        string dateStr = reservaciones[i][0];
        unsigned short anioR = static_cast<unsigned short>(stoi(dateStr.substr(0, 4)));
        unsigned short mesR  = static_cast<unsigned short>(stoi(dateStr.substr(5, 2)));
        unsigned short diaR  = static_cast<unsigned short>(stoi(dateStr.substr(8, 2)));
        fecha f(diaR, mesR, anioR);
        globalIteraciones++; // Contar cada comparación
        if (fechaCorte > f)
            k++;
        else
            break;  // Como está ordenada, una vez que f >= fechaCorte se detiene el ciclo.
    }

    // Abrir el archivo historico.txt en modo append para agregar las reservaciones.
    if (k > 0) {
        ofstream file("historico.txt", ios::app);
        if (!file.is_open()) {
            cerr << "Error al abrir historico.txt" << endl;
        } else {
            // Escribir cada reservación (de 0 a k-1) en el archivo.
            for (unsigned int i = 0; i < k; i++) {
                globalIteraciones++; // Contar iteración por cada escritura
                file << reservaciones[i][0] << " "   // fechaEntrada
                     << reservaciones[i][1] << " "   // duración
                     << reservaciones[i][2] << " "   // códigoReservación
                     << reservaciones[i][3] << " "   // códigoAlojamiento
                     << reservaciones[i][4] << " "   // documentoHuésped
                     << reservaciones[i][5] << " "   // métodoPago
                     << reservaciones[i][6] << " "   // fechaPago
                     << reservaciones[i][7] << " "   // monto
                     << reservaciones[i][8] << "\n";  // comentarios
            }
            file.close();
        }
    }

    // Eliminar de la matriz las k reservaciones que ya se pasaron al histórico.
    // Primero se libera la memoria de estas filas.
    for (unsigned int i = 0; i < k; i++) {
        delete[] reservaciones[i];
        globalIteraciones++;  // Contar eliminación
    }
    // Luego, se desplazan las filas restantes hacia el inicio del arreglo.
    for (unsigned int i = 0; i < n_reservaciones - k; i++) {
        reservaciones[i] = reservaciones[i + k];
        globalIteraciones++;  // Contar cada desplazamiento
    }
    n_reservaciones -= k;  // Actualizar el número total de reservaciones

    /* Calcular la memoria aproximada actual de la matriz (después de la eliminación)
    unsigned long memoriaPunteros = n_reservaciones * sizeof(string*);
    unsigned long memoriaFilas = n_reservaciones * 9 * sizeof(string);
    unsigned long totalMemoria = memoriaPunteros + memoriaFilas;

    // Utilizar el método mostrarResumenFinal de la clase Menu para mostrar el resumen de la operación.
    Menu m;
    m.mostrarResumenFinal("Actualización del Histórico", totalMemoria);

     Retornar la fecha de corte, ya que se usará para otras funcionalidades.
    */
    return fechaCorte;
}


// Función para validar fechas utilizando el operador '-' sobrecargado en la clase fecha.
bool UdeaStay::validarFechas( fecha &f1, unsigned int dur1,
                             fecha &f2, unsigned int dur2) {
    // Determinar cuál fecha es menor (más antigua)
    const fecha* menor;
    const fecha* mayor;
    unsigned int duracionMenor;

    if (f1 < f2) {
        menor = &f1;
        mayor = &f2;
        duracionMenor = dur1;
    } else {
        menor = &f2;
        mayor = &f1;
        duracionMenor = dur2;
    }

    // Utiliza el operador '-' ya sobrecargado para obtener la diferencia en días.
    // Se asume que ( *mayor - *menor ) retorna un unsigned int con la diferencia en días.
    unsigned int diff = *mayor - *menor;
    globalIteraciones++; // Contar la operación de resta

    // Si la diferencia es mayor que la duración asociada a la fecha menor, la validación falla.
    if (diff > duracionMenor)
        return false;
    else
        return true;
}

std::string** UdeaStay::cargarMatrizAlojamientos(const char* nombreArchivo, unsigned int &n_alojamientos) {
    ifstream file(nombreArchivo);
    if (!file.is_open()){
        cerr << "Error al abrir el archivo de alojamientos." << endl;
        n_alojamientos = 0;
        return nullptr;
    }

    // Primero se cuenta el número de líneas (filas) que no estén vacías.
    n_alojamientos = 0;
    string line;
    while(getline(file, line)) {
        if(!line.empty())
            n_alojamientos++;
    }

    // Volver al inicio del archivo
    file.clear();
    file.seekg(0, ios::beg);

    // Se crea una matriz dinámica con n_alojamientos filas, cada una con 9 columnas.
    string** matriz = new string*[n_alojamientos];
    for (unsigned int i = 0; i < n_alojamientos; i++) {
        matriz[i] = new string[9];
    }

    unsigned int row = 0;
    // Ahora se procesa cada línea para asignar los valores a cada columna.
    while(getline(file, line)) {
        if(line.empty())
            continue;
        // Se utilizará el delimitador '|' para extraer cada campo.
        unsigned int col = 0;
        size_t startPos = 0;
        size_t pos;
        while ((pos = line.find('|', startPos)) != string::npos && col < 9) {
            string token = line.substr(startPos, pos - startPos);
            matriz[row][col] = token;
            col++;
            startPos = pos + 1;
        }
        row++;
    }

    file.close();
    return matriz;
}

bool UdeaStay::eliminarReservacionPorAlojamiento(const string &codigoAlojamiento, string**matrizReservaciones,unsigned int n_reservaciones) {
    //unsigned int n_reservaciones = 0;
    //string** matrizReservaciones = cargarMatrizReservaciones("reservaciones.txt", n_reservaciones);
    //if(matrizReservaciones == nullptr) {
    //  cout << "No se encontró ninguna reservación." << endl;
    //return false;
    //}

    int indiceEliminar = -1;
    // Se asume que la columna 3 (índice 3) contiene el código del alojamiento.
    for (unsigned int i = 0; i < n_reservaciones; i++) {
        if(matrizReservaciones[i][3] == codigoAlojamiento) {
            indiceEliminar = static_cast<int>(i);
            break;
        }
    }

    if(indiceEliminar == -1) {
        cout << "No se encontró una reservación para el alojamiento con código "
             << codigoAlojamiento << endl;
        // Liberar la matriz original.
        for (unsigned int i = 0; i < n_reservaciones; i++) {
            delete[] matrizReservaciones[i];
        }
        delete[] matrizReservaciones;
        return false;
    }

    // Libera la memoria de la fila a eliminar.
    delete[] matrizReservaciones[indiceEliminar];

    // Mueve los punteros de las filas que están debajo, para llenar el hueco.
    for (unsigned int i = indiceEliminar; i < n_reservaciones - 1; i++) {
        matrizReservaciones[i] = matrizReservaciones[i + 1];
    }
    // Se actualiza el número de filas válidas.
    unsigned int n_nueva = n_reservaciones - 1;
    // Pone el último puntero a nullptr (ya que ahora ese espacio está libre).
    matrizReservaciones[n_nueva] = nullptr;

    cout << "Reservación para el alojamiento con código " << codigoAlojamiento
         << " eliminada en memoria exitosamente." << endl;
    cout << "Nuevo total de reservaciones en memoria: " << n_nueva << endl;

    // **Importante:** No se reescribe el archivo aquí y tampoco se libera la memoria del arreglo
    // (excepto la fila eliminada). Se asume que este arreglo modificado se mantendrá en memoria
    // para luego, al terminar el programa, reescribir el archivo con la función reescribirReservacionesTXT.
    // Podrías, por ejemplo, almacenar 'matrizReservaciones' y 'n_nueva' en variables miembro de UdeaStay.

    // Por este ejemplo, simplemente retornamos true.
    return true;
}

bool UdeaStay::reescribirReservacionesTXT(string** matriz, unsigned int n_filas) {
    ofstream salida("reservaciones.txt", ios::trunc);
    if(!salida.is_open()){
        cout << "Error al abrir reservaciones.txt para escribir." << endl;
        return false;
    }

    // Cada fila se reescribe en el formato:
    // fechaInicio duracion codigoReserva codigoAlojamiento documentoHuesped metodoPago fechaPago monto comentarios
    // Las primeras 8 columnas se separan por un espacio y la última se escribe tal cual.
    for (unsigned int i = 0; i < n_filas; i++) {
        // Usamos un string stream para construir la línea.
        ostringstream oss;
        for (unsigned int col = 0; col < 8; col++) {
            oss << matriz[i][col] << " ";
        }
        // La columna de comentarios se añade sin modificar (puede tener espacios).
        oss << matriz[i][8];
        salida << oss.str() << "\n";
    }
    salida.close();
    cout << "El archivo reservaciones.txt ha sido actualizado correctamente con " << n_filas << " reservaciones." << endl;
    return true;
}
