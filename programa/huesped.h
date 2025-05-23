#ifndef HUESPED_H
#define HUESPED_H
#include <string>
using namespace std;
class huesped
{
    unsigned int documento;
    string contrasena;
public:
    huesped();
    string leerArchivoTXT(const string& nombreArchivo);
    bool verficarDatos(string datos, unsigned int cedulaIngresada, string contrasenaIngresada);
    void setDocumento(const unsigned int &doc);
    void setContrasena(const string &contra);
};

#endif // HUESPED_H
