#include "Menu.h"
#include "fecha.h"         // Para trabajar con la clase fecha
#include "Anfitrion.h"     // Para autenticación y uso del objeto Anfitrion
#include "Huesped.h"
#include "UdeaStay.h"      // Para cargar reservaciones y acceder al contador global de iteraciones
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

//////////////////////////////
// IMPLEMENTACIÓN DE MENU   //
//////////////////////////////

// Constructor
// Constructor de Menu: se carga la matriz de reservaciones desde "reservaciones.txt"
Menu::Menu() : reservaciones(nullptr), numReservaciones(0) {
    UdeaStay user;
    reservaciones = user.cargarMatrizReservaciones("reservaciones.txt", numReservaciones,numReservacionesTotal);
    if (reservaciones == nullptr) {
        cout << "No se pudieron cargar las reservaciones." << endl;
        numReservaciones = 0;
    } else {
        cout << "Reservaciones cargadas correctamente. Total: " << numReservaciones << endl;
    }
}

// Destructor de Menu: libera la memoria de la matriz de reservaciones
Menu::~Menu() {
    if (reservaciones != nullptr) {
        for (unsigned int i = 0; i < numReservaciones; i++) {
            delete[] reservaciones[i];
        }
        delete[] reservaciones;
    }
}

// Implementación de la función auxiliar leerFechaValida.
// Se solicita ingresar día, mes y año, se construye la fecha y se verifica si es válida.
// Si el objeto fecha es inválido (sus getters retornan 0), se vuelve a solicitar una entrada.
fecha Menu::leerFechaValida(const string &mensaje) {
    unsigned short d, m, a;
    fecha ftemp(0, 0, 0);
    bool valido = false;
    do {
        cout << mensaje;
        cin >> d >> m >> a;
        ftemp = fecha(d, m, a); // Si la fecha es inválida, el constructor asigna 0 a sus atributos.
        if (ftemp.getDia() == 0 && ftemp.getMes() == 0 && ftemp.getAnio() == 0) {
            cout << "Error: La fecha ingresada es inv\u00E1lida. Por favor, ingrese una fecha correcta." << endl;
        }
        else {
            valido = true;
        }
    } while (!valido);
    return ftemp;
}

// Función auxiliar que además verifica que la fecha ingresada no sea mayor que maxAllowed.
fecha Menu::leerFechaValida(const string &mensaje, fecha &maxAllowed) {
    fecha ftemp = leerFechaValida(mensaje);
    // Suponiendo que se hayan sobrecargado los operadores de comparación (<, >) en la clase fecha.
    while(ftemp > maxAllowed) {
        cout << "Error: La fecha ingresada excede el límite permitido ("
             << maxAllowed.getDia() << "/" << maxAllowed.getMes() << "/" << maxAllowed.getAnio()
             << ")." << endl;
        ftemp = leerFechaValida(mensaje, maxAllowed); // Volvemos a pedir la fecha
    }
    return ftemp;
}

// Función que muestra en pantalla una fecha en el formato requerido.
void Menu::mostrarFecha(fecha &f) const {
    cout << f.getDiaSemana() << ", "
         << f.getDia() << " de "
         << f.getNombreMes() << " del "
         << f.getAnio();
}

// Función que, dada la fecha de inicio y la duración (en días), muestra en pantalla
// la fecha de inicio y la fecha calculada de finalización de la reserva.
void Menu::mostrarReserva( fecha &fechaInicio, unsigned int duracion) const {
    fecha fechaFin = fechaInicio.obtenerFechaSalida(duracion);
    cout << "Fecha de inicio de la reserva: ";
    mostrarFecha(fechaInicio);
    cout << "\nFecha de finalización de la reserva: ";
    mostrarFecha(fechaFin);
    cout << "\n";
}

// Función auxiliar que muestra, luego de cada operación, el total de iteraciones utilizadas y la memoria en uso.
// Posteriormente reinicializa el contador global.
void Menu::mostrarResumenFinal() {
    // Se crea un objeto temporal para consultar la memoria dinámica asignada (UdeaStay mantiene ese dato)
    UdeaStay stay;
    unsigned long memoriaDinamica = stay.getMemoriaUso();
    unsigned long memoriaUdeaStay = sizeof(stay);
    // Estimación de memoria de algunas variables locales (valor aproximado)
    unsigned long memoriaLocales = sizeof(unsigned int) + sizeof(char*);
    unsigned long totalMemoria = memoriaDinamica + memoriaUdeaStay + memoriaLocales;

    cout << "\n--- Resumen Final de la Operación ---" << endl;
    cout << "Iteraciones requeridas: " << UdeaStay::globalIteraciones << " iteraciones." << endl;
    cout << "Memoria en uso (aproximada): " << totalMemoria << " bytes." << endl;

    // Reinicializar el contador global para la próxima operación
    UdeaStay::globalIteraciones = 0;
}


// Método que inicia la ejecución principal del menú.
void Menu::iniciar() {
    seleccionarTipoUsuario();
    // Al finalizar (antes de salir del programa) se reescribe el archivo de reservaciones
    // utilizando la matriz actualizada.

    // Además, se muestra un resumen final (por ejemplo, iteraciones y memoria en uso).
    user.reescribirReservacionesTXT(reservaciones,numReservaciones);
    mostrarResumenFinal();
}

// Método que pregunta al usuario su perfil (Anfitrión o Huésped)
void Menu::seleccionarTipoUsuario() {
    int opcion=0;
    while (opcion != 3){
        cout << "\n------ MENÚ PRINCIPAL -----\n";
        cout << "1. Iniciar como Anfitrión\n";
        cout << "2. Iniciar como Huésped\n";
        cout << "3. Salir\n";
        cout << "Ingrese su opción: ";
        cin >> opcion;
        cin.ignore();
        if (opcion == 1) {
            anfitrionMenu(reservaciones,numReservaciones);
        } else if (opcion == 2) {
            huespedMenu();
            for (unsigned int i=0; i<numReservacionesTotal;i++){
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
        } else if (opcion != 3) {
            cout << "Opción inválida. Intente nuevamente." << endl;
        }
    }
}

// Menú específico para el anfitrión.
void Menu::anfitrionMenu(string ** &reservaciones,  unsigned int & numReservaciones) {
    // Solicitar credenciales para iniciar sesión.
    string doc, pass;
    cout << "\n--- Autenticación del Anfitrión ---" << endl;
    cout << "Ingrese su documento: ";
    cin >> doc;
    cout << "Ingrese su contraseña: ";
    cin >> pass;
    cin.ignore();  // Limpiar buffer

    // Se asume que 'anfitrion' es un miembro (por ejemplo, de tipo Anfitrion*) previamente creado
    // y que su método estático autenticar() asigna y retorna el objeto autenticado.
    anfitrion = Anfitrion::autenticar("anfitrion.txt", doc, pass);
    if (anfitrion == nullptr) {
        cout << "Credenciales incorrectas o anfitrión no encontrado." << endl;
        return;
    }
    cout << "Autenticación exitosa. Bienvenido, "
         << anfitrion->getDocumento() << "\n" << endl;

    int opcion = 0;
    do {
        cout << "\n--- Menú Anfitrión ---" << endl;
        cout << "1. Anular reservación" << endl;
        cout << "2. Consultar reservaciones de mis alojamientos" << endl;
        cout << "3. Actualizar histórico" << endl;
        cout << "4. Salir al menú principal" << endl;
        cout << "Ingrese la opción deseada: ";
        cin >> opcion;
        cin.ignore();  // Limpiar el buffer

        switch(opcion) {
        case 1:
            // Invocar la función para eliminar la reservación.
            eliminarReservacion();
            break;
        case 2:
            // Invocar la función para consultar las reservaciones de los alojamientos administrados.
            mostrarReservacionesAnfitrionConFecha(*anfitrion, reservaciones, numReservaciones);
            break;
        case 3: {
            // Opción de actualizar el histórico.
            user.ordenarMatrizReservacionesPorFecha(reservaciones, numReservaciones);
            fecha fechaCorte = user.actualizarHistorico(reservaciones, numReservaciones);
            cout << "\nHistórico actualizado. Fecha de corte utilizada: "
                 << fechaCorte.getDia() << "/" << fechaCorte.getMes() << "/"
                 << fechaCorte.getAnio() << endl;
            break;
        }
        case 4:
            cout << "\nRegresando al menú principal..." << endl;
            break;
        default:
            cout << "\nOpción inválida en el menú de anfitrión." << endl;
            break;
        }
    } while (opcion != 4);

    // Liberar el objeto de autenticación.
    delete anfitrion;
    anfitrion = nullptr;
}


void Menu::huespedMenu() {
    // Autenticación del huésped.
    string doc, pass;
    cout << "\n--- Autenticación del Huésped ---" << endl;
    cout << "Ingrese su documento: ";
    cin >> doc;
    cout << "Ingrese su contraseña: ";
    cin >> pass;

    Huesped* huesped = Huesped::autenticar("huesped.txt", doc, pass);
    if (huesped == nullptr) {
        cout << "Credenciales incorrectas o huésped no encontrado." << endl;
        return;
    }
    cout << "Autenticación exitosa. Bienvenido, " << huesped->getDocumento() << "\n" << endl;
    int opcion=0;
    // Presentar submenú de funcionalidades.
    cout << "--- Funcionalidades del Huésped ---" << endl;
    cout << "1. Reservar alojamiento por municipio" << endl;
    cout << "2. Reservar alojamiento por código" << endl;
    cout << "3. Salir" << endl;
    cout << "Ingrese su opción: ";

    cin >> opcion;
    cin.ignore();  // Limpiar el buffer
    fecha fechaCorte(1, 1, 2025);  // Fecha de corte retornada por actualizarHistorico.
    // La fecha máxima permitida ser� fechaCorte + 1 año (mismo día y mes, anio + 1).
    fecha fechaMaxima(fechaCorte.getDia(), fechaCorte.getMes(), fechaCorte.getAnio() + 1);

    // Ahora, se utiliza la versión sobrecargada de leerFechaValida para asegurar no exceder fechaMaxima.
    fecha fechaEntrada = leerFechaValida("Ingrese la fecha de entrada (día mes año): ", fechaMaxima);

    unsigned int duracion;
    cout << "Ingrese la duración (en noches): ";
    cin >> duracion;

    // Cargar la matriz de alojamientos y de reservaciones.
    unsigned int n_alojamientos = 0;
    string** matrizAlojamientos = user.cargarMatrizAlojamientos("alojamientos.txt", n_alojamientos);
    if (matrizAlojamientos == nullptr) {
        cout << "No se pudo cargar la matriz de alojamientos." << endl;
        delete huesped;
        return;
    }

    int idxAloja = -1; // Índice del alojamiento seleccionado.

    if(opcion == 1) {
        // Búsqueda por municipio.
        cin.ignore();  // Limpiar buffer.
        string municipio;
        cout << "Ingrese el municipio donde desea reservar: ";
        getline(cin, municipio);

        // Preguntar si desea aplicar un filtro del costo máximo por noche.
        char aplicarFiltro;
        double costoMaximo = 0;
        cout << "¿Desea aplicar un filtro de costo máximo por noche? (S/N): ";
        cin >> aplicarFiltro;
        if(tolower(aplicarFiltro) == 's'){
            cout << "Ingrese el costo máximo por noche: ";
            cin >> costoMaximo;
        }
        cin.ignore(); // Limpiar buffer después de leer el número

        // Se recorre la matriz de alojamientos para filtrar por municipio, costo máximo (si se aplicó) y sin conflicto.
        unsigned int* indicesDisponibles = new unsigned int[n_alojamientos];
        unsigned int contDisponibles = 0;
        for (unsigned int i = 0; i < n_alojamientos; i++) {
            // Verificar que el alojamiento pertenezca al municipio solicitado.
            if(matrizAlojamientos[i][4] != municipio)
                continue;

            // Si se aplicó un filtro por costo máximo, se verifica que el precio (columna 7) sea menor o igual.
            if(costoMaximo > 0) {
                // Convertir el precio (almacenado en cadena) a un valor numérico.
                double precio = stod(matrizAlojamientos[i][7]);
                if(precio > costoMaximo)
                    continue;
            }
            bool conflictoAlojamiento = false;
            string codigoAlojamiento = matrizAlojamientos[i][1];
            for (unsigned int j = 0; j < numReservaciones; j++) {
                if (reservaciones[j][3] == codigoAlojamiento) {
                    string fechaStr = reservaciones[j][0];
                    unsigned short rAnio = 0, rMes = 0, rDia = 0;
                    istringstream iss(fechaStr);
                    char delim;  // Variable para leer el delimitador '-'
                    if (!(iss >> rAnio >> delim >> rMes >> delim >> rDia)) {
                        cout << "Formato de fecha incorrecto en la reserva: " << fechaStr << endl;
                        continue;  // Salta a la siguiente iteración si la fecha no se pudo parsear correctamente
                    }
                    fecha fechaReserva(rDia, rMes, rAnio);
                    unsigned int durReserva = static_cast<unsigned int>(stoi(reservaciones[j][1]));
                    if (user.validarFechas(fechaReserva, durReserva, fechaEntrada, duracion)) {
                        conflictoAlojamiento = true;
                        break;
                    }
                }
            }
            if(!conflictoAlojamiento) {
                indicesDisponibles[contDisponibles++] = i;
                cout << "\nOpción " << contDisponibles << ":" << endl;
                cout << "Nombre: "           << matrizAlojamientos[i][0] << endl;
                cout << "Código: "           << matrizAlojamientos[i][1] << endl;
                cout << "Anfitrión: "        << matrizAlojamientos[i][2] << endl;
                cout << "Departamento: "     << matrizAlojamientos[i][3] << endl;
                cout << "Municipio: "        << matrizAlojamientos[i][4] << endl;
                cout << "Tipo: "             << matrizAlojamientos[i][5] << endl;
                cout << "Dirección: "        << matrizAlojamientos[i][6] << endl;
                cout << "Precio por noche: " << matrizAlojamientos[i][7] << endl;
                cout << "Amenidades: "       << matrizAlojamientos[i][8] << endl;
            }
        }
        if(contDisponibles == 0) {
            cout << "\nNo se encontraron alojamientos disponibles en " << municipio
                 << " para las fechas solicitadas." << endl;
            delete[] indicesDisponibles;
            for (unsigned int i = 0; i < n_alojamientos; i++) {
                delete[] matrizAlojamientos[i];
            }
            delete[] matrizAlojamientos;
            delete huesped;
            return;
        }
        cout << "\nIngrese el número de la opción que desea reservar: ";
        unsigned int seleccion;
        cin >> seleccion;
        while(seleccion < 1 || seleccion > contDisponibles) {
            cout << "Opción inválida. Ingrese nuevamente: ";
            cin >> seleccion;
        }
        idxAloja = indicesDisponibles[seleccion - 1];
        delete[] indicesDisponibles;
    }
    else if(opcion == 2) {
        // Búsqueda por código del alojamiento.
        cin.ignore();
        string codigoBuscado;
        cout << "\nIngrese el código del alojamiento: ";
        getline(cin, codigoBuscado);
        for (unsigned int i = 0; i < n_alojamientos; i++) {
            if(matrizAlojamientos[i][1] == codigoBuscado) {
                idxAloja = i;
                break;
            }
        }
        if(idxAloja == -1) {
            cout << "\nNo se encontró un alojamiento con el código " << codigoBuscado << endl;
            for (unsigned int i = 0; i < n_alojamientos; i++) {
                delete[] matrizAlojamientos[i];
            }
            delete[] matrizAlojamientos;
            delete huesped;
            return;
        }
        cout << "\nAlojamiento encontrado:" << endl;
        cout << "Nombre: "           << matrizAlojamientos[idxAloja][0] << endl;
        cout << "Código: "           << matrizAlojamientos[idxAloja][1] << endl;
        cout << "Anfitrión: "        << matrizAlojamientos[idxAloja][2] << endl;
        cout << "Departamento: "     << matrizAlojamientos[idxAloja][3] << endl;
        cout << "Municipio: "        << matrizAlojamientos[idxAloja][4] << endl;
        cout << "Tipo: "             << matrizAlojamientos[idxAloja][5] << endl;
        cout << "Dirección: "        << matrizAlojamientos[idxAloja][6] << endl;
        cout << "Precio por noche: " << matrizAlojamientos[idxAloja][7] << endl;
        cout << "Amenidades: "       << matrizAlojamientos[idxAloja][8] << endl;
    }
    string documento=huesped->getDocumento();
    if(user.hayConflictoHuesped(reservaciones,numReservaciones,documento,fechaEntrada,duracion)) {
        cout << "\nYa tiene reservaciones en las fechas solicitadas. No puede hacer la reserva." << endl;
        for (unsigned int i = 0; i < n_alojamientos; i++) {
            delete[] matrizAlojamientos[i];
        }
        delete[] matrizAlojamientos;
        //delete huesped;
        return;
    }

    // Generar el código de reservación usando el formato: dia_mes_codigoAlojamiento.
    string codigoAlojamientoSeleccionado = matrizAlojamientos[idxAloja][1];
    string diaStr = to_string(fechaEntrada.getDia());
    string mesStr = to_string(fechaEntrada.getMes());
    string codigoReserva = diaStr + "_" + mesStr + "_" + codigoAlojamientoSeleccionado;

    // Solicitar datos adicionales para la reserva.
    cin.ignore();
    string metodoPago;
    cout << "Ingrese el método de pago (ej: PSE, TCredito): ";
    getline(cin, metodoPago);

    string comentarios;
    do {
        cout << "Ingrese comentarios (opcional, máximo 1000 caracteres): ";
        getline(cin, comentarios);
        if (comentarios.size() > 1000)
            cout << "Error: Los comentarios deben tener como máximo 1000 caracteres. Por favor, ingrese nuevamente." << endl;
    } while(comentarios.size() > 1000);

    // Agregar el código de la reserva al arreglo de reservas del huésped.
    huesped->agregarReserva(codigoReserva);

    // Agregar la nueva reservación a la matriz de reservaciones.

    string anioStr = to_string(fechaEntrada.getAnio());
    string fechaEntradaStr = anioStr + "-" + mesStr + "-" + to_string(fechaEntrada.getDia());
    reservaciones[numReservaciones][0] = fechaEntradaStr;
    cout << "  Fecha Entrada     : " << reservaciones[numReservaciones][0] << "\n";
    reservaciones[numReservaciones][1] = to_string(duracion);
    cout << "  duracion     : " << reservaciones[numReservaciones][1] << "\n";
    reservaciones[numReservaciones][2] = codigoReserva;
    cout << "  codigoRes     : " << reservaciones[numReservaciones][2] << "\n";
    reservaciones[numReservaciones][3] = codigoAlojamientoSeleccionado;
    cout << "  codAloja     : " << reservaciones[numReservaciones][3] << "\n";
    reservaciones[numReservaciones][4] = huesped->getDocumento();
    cout << "  Doc     : " << reservaciones[numReservaciones][4] << "\n";
    reservaciones[numReservaciones][5] = metodoPago;
    cout << "  MetPago     : " << reservaciones[numReservaciones][5] << "\n";
    reservaciones[numReservaciones][6] = "";
    cout << "  fechaPago    : " << reservaciones[numReservaciones][6] << "\n";
    reservaciones[numReservaciones][7] = "0";
    cout << "  monto    : " << reservaciones[numReservaciones][7] << "\n";
    reservaciones[numReservaciones][8] = comentarios;
    cout << "  coment    : " << reservaciones[numReservaciones][8] << "\n";

    cout << "\nReserva realizada con éxito." << endl;

    // Mostrar la reserva (fecha inicio y fecha final) con el formato requerido.
    cout << "\n---------------------------------------\n";
    cout << "Datos de la reserva:" << endl;
    cout << "Código de reserva: " << codigoReserva << endl;
    mostrarReserva(fechaEntrada, duracion);
    cout << "---------------------------------------\n";
    numReservaciones ++;

    //for (unsigned int i = 0; i < numReservaciones; i++) {
        //cout << "Reservación #" << i + 1 << ":\n";

    //}


    // Liberar memoria temporal.
    for (unsigned int i = 0; i < n_alojamientos; i++) {
        delete[] matrizAlojamientos[i];
    }
    delete[] matrizAlojamientos;
    if (opcion==3){
        //delete huesped;
        return;

    }
}

// Función que solicita un rango de fechas y muestra las reservaciones de los alojamientos
// administrados por el anfitrión. Se utiliza la nueva estructura de 9 campos.
void Menu::mostrarReservacionesAnfitrionConFecha(const Anfitrion &anfitrion, std::string** reservaciones, unsigned int n_reservaciones) {
    unsigned short diaIni, mesIni, anioIni;
    unsigned short diaFin, mesFin, anioFin;

    cout << "\nIngrese la fecha de inicio (día, mes, año, separados por espacios): ";
    cin >> diaIni >> mesIni >> anioIni;
    fecha fechaInicio(diaIni, mesIni, anioIni);

    cout << "Ingrese la fecha de fin (día, mes, año, separados por espacios): ";
    cin >> diaFin >> mesFin >> anioFin;
    fecha fechaFin(diaFin, mesFin, anioFin);

    cout << "\nReservaciones para el Anfitrión (Documento: "
         << anfitrion.getDocumento() << ") en el rango "
         << fechaInicio.getDia() << "/" << fechaInicio.getMes() << "/" << fechaInicio.getAnio()
         << " - "
         << fechaFin.getDia() << "/" << fechaFin.getMes() << "/" << fechaFin.getAnio()
         << ":\n" << endl;

    bool encontrado = false;
    for (unsigned int i = 0; i < n_reservaciones; i++) {
        // Verificar que la reservación pertenezca a un alojamiento administrado por el anfitrión.
        // Se utiliza el campo código de alojamiento (índice 3)
        string codAlojamiento = reservaciones[i][3];
        if (!anfitrion.gestionaAlojamiento(codAlojamiento))
            continue;

        // Parsear la fecha de entrada (índice 0) – se espera que el formato sea "AAAA-MM-DD".
        string fechaStr = reservaciones[i][0];
        unsigned short anioRes = 0, mesRes = 0, diaRes = 0;
        istringstream iss(fechaStr);
        char delim; // Variable para leer el delimitador '-' (se ignora)

        if (!(iss >> anioRes >> delim >> mesRes >> delim >> diaRes)) {
            // Si falla el parseo, se notifica el problema y se salta este registro.
            cout << "Formato de fecha incorrecto en la reservación: " << fechaStr << endl;
            continue;
        }

        // Construir el objeto fecha con los valores extraídos.
        fecha fechaReserva(diaRes, mesRes, anioRes);

        if (fechaReserva < fechaInicio || fechaFin < fechaReserva)
            continue;

        // Imprimir los detalles de la reservación
        cout << "Reservación " << i + 1 << ":" << endl;
        cout << "  Fecha Entrada:       " << reservaciones[i][0] << endl;
        cout << "  Duración:            " << reservaciones[i][1] << endl;
        cout << "  Código Reservación:  " << reservaciones[i][2] << endl;
        cout << "  Código Alojamiento:  " << reservaciones[i][3] << endl;
        cout << "  Documento Huésped:   " << reservaciones[i][4] << endl;
        cout << "  Método de Pago:      " << reservaciones[i][5] << endl;
        cout << "  Fecha de Pago:       " << reservaciones[i][6] << endl;
        cout << "  Monto:               " << reservaciones[i][7] << endl;
        cout << "  Comentarios:         " << reservaciones[i][8] << "\n" << endl;
        encontrado = true;
    }
    if (!encontrado)
        cout << "No se encontraron reservaciones para los alojamientos del Anfitrión en el rango especificado." << endl;
}

void Menu::eliminarReservacion() {
    string codigoAlojamiento;
    cout << "\nIngrese el código del alojamiento de la reservación a eliminar: ";
    getline(cin, codigoAlojamiento);
    if(user.eliminarReservacionPorAlojamiento(codigoAlojamiento, reservaciones, numReservaciones)) {
        // Para actualizar la matriz local de Menu se puede recargar, si así se desea.
        // Por ejemplo:
        delete[] reservaciones;  // Libera el arreglo de punteros (las filas eliminadas ya se liberaron internamente)
        reservaciones = user.cargarMatrizReservaciones("reservaciones.txt", numReservaciones,numReservacionesTotal);
    } else {
        cout << "No se pudo eliminar la reservación." << endl;
    }
}
