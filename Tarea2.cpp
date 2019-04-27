#include <iostream>
#include <time.h>
#include <stdlib.h>

int arreglo[100000];
void llenar_arreglo(){
  srand (time(NULL));
  for(int i=0; i<=100000;i++){
      int aleatorio = rand() % 10000+1;
      arreglo[i]=aleatorio;
  }
}

float suma(int arreglo[]){
  float suma;
  for(int i=0; i<=100000;i++){
    suma=suma+(arreglo[i]);
  }
  return suma;
}

using namespace std;

int main()
{
  float prom;
  llenar_arreglo();
  prom=(suma(arreglo)/p);
  cout<<prom<<endl;
  return 0;
}
