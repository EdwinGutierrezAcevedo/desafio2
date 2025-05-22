#include <iostream>
#include "fecha.h"
using namespace std;

int main()
{
    fecha obj1;
    fecha obj2;
    obj1.setFecha(28,1,2024);
    obj2.setFecha(29,2,2024);

    if(obj1<obj2){
        cout<<"hola"<<endl;
    }

    return 0;
}
