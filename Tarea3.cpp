#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
/*Se define N como la cantidad de datos que tendrá el arreglo original*/
#define N 100000

/*---------------------------FUNCIONES---------------------------*/

/* Función que calcula la varianza de los numeros dentro del arreglo */
float varianza (int arreglo[],float prom ){
        float var;
        for(int i=0; i<=N; i++) {
                var += pow(arreglo[i]-prom,2);
                var=var/prom;
        }
        return var;
}

/* Función que crea P subvectores para ser distrbuidos uniformemente a cada proceso*/

int* Crear_sub_vec(int comienzo, int final, int* origen){
        int* sub_vec;
        int tamano;
        int aux = 0;
        tamano = final - comienzo;
        sub_vec = (int*)malloc(tamano * sizeof(int));
        for (int i = comienzo; i < final; ++i) {
                *(sub_vec+aux) = *(origen+i);
                aux += 1;
        }
        return sub_vec;
}

/* Función que llena el arreglo N original con numeros aleatorios de un intervalo de 1-100000 */

void llenar_arreglo(int arreglo[],int tamanoar){
        int aleatorio;
        srand(time(NULL));
        for(int i=0; i<=tamanoar; i++) {
                aleatorio=rand()%10000+1;
                arreglo[i]=aleatorio;
        }
}

/*Función que calcula y retorna el promedio de el subvector ingresado en la funcion*/
float calculaSubPromedio(int *sub_vector, int largoSubVector){
        float promedio;
        float sumaSubVector;
        int *puntero = sub_vector;
        for(int i = 0; i < largoSubVector; i++) {
                sumaSubVector += *puntero;
                puntero++;
        }
        promedio = sumaSubVector/largoSubVector;
        return promedio;
}

/*---------------------------FUNCIONES---------------------------*/


using namespace std;

/*---------------------------Inicio Main---------------------------*/
int main(void){

        int procesos; //Cantidad de procesos totales en el sistema
        int procesador; //Procesador a utilizar
        int arreglo[N]; //Declaración de arreglo original con N datos
        int* sub_vec = NULL; //Declaración del subvector en NULL
        int divdatos; //Declaración de variable con el fin de saber cuanto datos es para cada proceso de manera uniforme
        float sumapromedios; //Variable para el calculo de la suma de promedios
        float subpromedioO; //Variable para el ingreso del subpromedio 0
        float promediototal; //Variable para el ingreso del promedio total calculado por P=0
        float desv; //Variable para el ingreso de la desviación total

/*Se inicia al llenado de numero aleatorios del arreglo original*/
        llenar_arreglo(arreglo,N);

/*Se inicia MPI*/
        MPI_Init(NULL, NULL);
        MPI_Comm_size(MPI_COMM_WORLD, &procesos);
        MPI_Comm_rank(MPI_COMM_WORLD, &procesador);

        divdatos = (N/procesos); //Se calcula la cantidad de datos a distribuir uniformemente

        printf("Proceso %d calcula = %d datos\n", procesador, divdatos);
/**************************Procesos distintos de 0*********************************/
        if (procesador != 0) {
                sub_vec = (int *)malloc(divdatos * sizeof(int)); //Se genera el espacio para recivir los subvectores generados
                MPI_Recv(sub_vec, divdatos, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //Se recibe el subvector gracias a Mpi_recv
                float subpromedioO=calculaSubPromedio(sub_vec,divdatos); //Se calcula el subpromedio del subvector P[i]
                cout<<"Subpromedio "<<procesador<<": "<<subpromedioO<<endl; //Se imprime por pantalla el subpromedio de sub_vector[i]
                MPI_Send(&subpromedioO,1,MPI_FLOAT,0,0,MPI_COMM_WORLD);//Se envía el subpromedio a P=0 para posteriores calculos
/**************************Procesos distintos de 0*********************************/


/**************************Procesos para procesador 0******************************/

        } else { //Proceso 0

                cout<<"-----------------Distribuyendo dados-----------------"<<endl;

                for (int i = 1; i < procesos; i++) {
                        sub_vec = Crear_sub_vec(i*divdatos, (i*divdatos)+divdatos, arreglo); //Se generan P subvectores a enviar
                        MPI_Send(sub_vec, divdatos, MPI_INT, i, 0, MPI_COMM_WORLD);//Se envía el sub vector a cada proceso!=0
                }
                sub_vec = Crear_sub_vec(0, divdatos, arreglo); //Se genera el subvector para P=0
                //print(procesador, procesos, divdatos, sub_vec);
                float subpromedio=calculaSubPromedio(sub_vec,divdatos); //Se calcula el subpromedio del subvector perteneciente a P=0
                cout<<"Subpromedio 0: "<<subpromedio<<endl; //Se imprime por pantalla el subpromedo calculado

                for(int i=1; i<procesos; i++) { //Se comienzan a recibir los subpromedios de los P!=0
                        subpromedioO=subpromedioO;
                        MPI_Recv(&subpromedioO,1,MPI_FLOAT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE); //Se reciben los subpromedios gracias a Mpi_recv
                        subpromedio += subpromedioO; //Se suman la totalidad de los diferentes subpromedios
                }
                promediototal=(subpromedio/procesos); //Se calcula el promedio total de todos los subpromedios recibidos en P=0
                cout<<"Promedio total:"<<promediototal<<endl; //Salida por pantalla del promedio total
                cout<<"Varianza: "<<varianza(arreglo,(promediototal))<<endl; //Salida por pantalla de la varianza (Se llama a la función varianza() )
                desv=(sqrt(varianza(arreglo,promediototal))); // Se calcula la desviación utilizando la función de varianza
                cout<<"Desviación: "<<desv<<endl; //Salida por pantalla de la desviación

        }
/**************************Procesos para procesador 0******************************/

/*Se finaliza MPI (Siempre debe realizarse)*/
        MPI_Finalize();
        return 0;
}
/*---------------------------Fin Main---------------------------*/
