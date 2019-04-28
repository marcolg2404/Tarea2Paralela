#include <iostream>
#include <time.h> //Libreria para el seed de numeros aleatorios
#include <stdlib.h>
#include <cmath> // Libreria matematica

int arreglo[100000]; //Declaración de arreglo de forma global
void llenar_arreglo(){ //Función void para llenar el arreglo de numeros aleatorios
  srand (time(NULL));
  for(int i=0; i<=100000;i++){
      int aleatorio = rand() % 10000+1;
      arreglo[i]=aleatorio;
  }
}

float suma(int arreglo[]){ //Función que recorre el arreglo sumando todos sus numeros aleatorios
  float suma;
  for(int i=0; i<=100000;i++){
    suma=suma+(arreglo[i]);
  }
  return suma;
}

float varianza (int arreglo[],float prom ){ // Función que calcula la varianza de los numeros dentro del arreglo
  float var;
  for(int i=0; i<=100000;i++){
    var += pow(arreglo[i]-prom,2);
    var=var/prom;
  }
}

using namespace std;

int main()
{
  float prom,desv; // Declaración de variables promedio y desviación
  llenar_arreglo(); // Se llama a la función void para llenar el arreglo
  prom=(suma(arreglo)/100000); //Se calcula el promedio del arreglo
  cout<<"El promedio es: "<<prom<<endl; //Salida por pantalla del promedio
  cout<<"La varianza es: "<<varianza(arreglo,prom)<<endl; // Salida por pantalla de la varianza
  desv=(sqrt(varianza(arreglo,prom))); // Se calcula la desviación utilizando la función de varianza
  cout<<"La desviación estandar es: "<<desv<<endl; //Salida por pantalla de la desviación estandar
  return 0;
}
