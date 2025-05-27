#ifndef ANFITRION_H
#define ANFITRION_H
#include <string>
#include <iomanip>
#include <sstream>
#define COLUMNAS 6
using namespace std;
class anfitrion
{
    unsigned int documentoAnfitrion;
    string contrasenaAnfitrion;
    friend string leerArchivoTXT(const string& nombreArchivo);
    bool verficarDatosAnfitrion(string datos, unsigned int cedulaIngresada, string contrasenaIngresada);

public:
    void setDocumentoAnfitrion(const unsigned int &doc);
    void setContrasenaAnfitrion(const string &contra);
    anfitrion();
    static string leerArchivoTXTreserva(const string &nombreArchivo);
    static char*** crearMatrizReservaciones(const string &contenido, int capacidad, int &numFilas);
    static void agregarReservacion(char*** &matriz, int &numFilas, int capacidad, const char *datos[COLUMNAS]);
    static void eliminarReservacion(char*** &matriz, int &numFilas, int indice);
    static void imprimirMatrizReservaciones(char*** matriz, int numFilas);
    static void liberarMatrizReservaciones(char*** &matriz, int numFilas);
};

#endif // ANFITRION_H
