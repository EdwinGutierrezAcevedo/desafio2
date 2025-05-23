#ifndef ANFITRION_H
#define ANFITRION_H
#include <string>
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
};

#endif // ANFITRION_H
