#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#define N 100

/*---------------------------FUNCIONES---------------------------*/
int* create_sub_vec(int begin, int end, int* origin){
    int* sub_vec;
    int size;
    int aux = 0;
    size = end - begin;
    sub_vec = (int*)malloc(size * sizeof(int));
    for (int i = begin; i < end; ++i) {
        *(sub_vec+aux) = *(origin+i);
        aux += 1;
    }
    return  sub_vec;
}

void llenar_arreglo(int arreglo[],int tamanoar){
  int aleatorio;
  srand(time(NULL));
  for(int i=0;i<=tamanoar;i++){
    aleatorio=rand()%10000+1;
    arreglo[i]=aleatorio;
  }
}
void print(int procesador, int procesos, int divdatos, int* sub_vec){
    printf("Process %d out of %d received sub_vecotr: [ ", procesador, procesos);
    for (int i = 0; i < divdatos; ++i)
    {
        printf("%d, ", *(sub_vec+i));
    }
    printf("]\n");
}

float calculaSubPromedio(int *subVector, int largoSubVector){
    float promedio;
    float sumaSubVector;
    int *punteroRecorre = subVector;
    for(int i = 0; i < largoSubVector; i++){
        sumaSubVector += *punteroRecorre;
        punteroRecorre++;
    }
    promedio = sumaSubVector/largoSubVector;
    return promedio;
}

/*---------------------------FUNCIONES---------------------------*/


using namespace std;

int main(void){

    int procesos;
    int procesador;

    int arreglo[N];
    int* sub_vec = NULL;
    int divdatos;
    llenar_arreglo(arreglo,N);

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &procesos); //=2
    MPI_Comm_rank(MPI_COMM_WORLD, &procesador);

    divdatos = (N/procesos);

    printf("Process %d calcula divdatos = %d\n", procesador, divdatos);

    if (procesador != 0) { //Procesos distintos de 0


        sub_vec = (int *)malloc(divdatos * sizeof(int));
        MPI_Recv(sub_vec, divdatos, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        print(procesador, procesos, divdatos, sub_vec);
        float subpromedioO=calculaSubPromedio(sub_vec,divdatos);
        cout<<"Subpromedio: "<<subpromedioO<<endl;
        MPI_Send(&subpromedioO,1,MPI_FLOAT,0,0,MPI_COMM_WORLD);

    } else { //Proceso 0

        printf("Distribuyendo dados\n");

        for (int i = 1; i < procesos; i++) {
            sub_vec = create_sub_vec(i*divdatos, (i*divdatos)+divdatos, arreglo);
            MPI_Send(sub_vec, divdatos, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        printf("Fin de distribucion de datos\n");

        sub_vec = create_sub_vec(0, divdatos, arreglo);
        print(procesador, procesos, divdatos, sub_vec);
        float subpromedio=calculaSubPromedio(sub_vec,divdatos);
        cout<<"Subpromedio nepe: "<<subpromedio<<endl;

        for(int i=1;i<procesos;i++){
        float subpromedioO=subpromedioO;
        MPI_Recv(&subpromedioO,1,MPI_FLOAT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        cout<<"Promedio de matrices divididas"<<((subpromedioO+subpromedio)/procesos)<<endl;
      }
    }

    MPI_Finalize();
    return 0;

}
