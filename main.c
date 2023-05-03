#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N_diccionario 9382
#define N_palabra 20
#define N_oracion 100
#define N_entrenamiento 40
#define Imprimir 0

// Inicializacion de diccionario y set de entrenamiento:
int InitDiccionario(char diccionario [N_diccionario][N_palabra]);
void InitTraining(char training_set[N_entrenamiento][N_oracion],int vector_tipos [N_entrenamiento]);

// Funciones para manipular los vectores
int ObtenerCaracteristicas(char diccionario [N_diccionario][N_palabra], char oracion[], float vector_caracteristicas[], int tipo, int imprimir);
void ImprimirVectorCaracteristicas(char diccionario [N_diccionario][N_palabra],char oracion[],float vector_caracteristicas[],int tipo);
int decision_final(int vector_tipos[N_entrenamiento] ,int K);
void obtener_vector_distancias ( char nueva_oracion [N_oracion] , float vector_caracteristicas [N_entrenamiento][N_diccionario] , char diccionario [N_diccionario][N_palabra] , float distancias [N_entrenamiento] , int imprimir_caracteristicas);
void ordenar_traning_set(float vector_distancias[], char training_set [N_entrenamiento][N_oracion],int vector_tipos[N_entrenamiento], int N);


// Funciones auxiliares
float calcular_norma(float vector[],int dimension);
void normalizar_vector(float vector[],int dimension);
float calcular_distancia(float vector1[],float vector2[],int dimension);
int strstr_custom(char cadena[],char palabra[]);



int main()
{
    char T0 [N_palabra] = "La Renga";
    char T1 [N_palabra] = "Callejeros";

    int K = 5;
    int i;
    char diccionario [N_diccionario][N_palabra]={0};
    char training_set [N_entrenamiento][N_oracion]={0};
    float vector_caracteristicas[N_entrenamiento][N_diccionario]={0};
    int vector_tipos [N_entrenamiento]={0};

    char nueva_oracion [N_oracion];
    float vector_distancias [N_entrenamiento];

    InitDiccionario(diccionario);
    InitTraining(training_set,vector_tipos);

    for(i=0;i<N_entrenamiento;i++)
    {
        ObtenerCaracteristicas(diccionario,training_set[i],vector_caracteristicas[i],vector_tipos[i],Imprimir);
    }

    char opcion;
    do{

        system("cls");
        printf("Ingrese una nueva oracion: \n");
        gets( nueva_oracion );

        obtener_vector_distancias(nueva_oracion,vector_caracteristicas,diccionario,vector_distancias,Imprimir);

        ordenar_traning_set(vector_distancias,training_set,vector_tipos,N_entrenamiento);

        int tipo_nueva_muestra = decision_final(vector_tipos,K);

        printf("\n--- Los %d elementos mas cercanos son: ---\n",K);

        for (int i = 0;i<K; i++)
        {
            printf("%s -> Tipo: %s (distancia %f)\n",training_set[i],(vector_tipos[i]==0) ? T0:T1,vector_distancias[i]);
        }

        printf("\n ---- Clasifico esta nueva muestra como: %s ----",(tipo_nueva_muestra==0)? T0:T1);

        printf("¿Otra? (s/n)\n");
        scanf("%c",&opcion);fflush(stdin);

    }while(opcion == 's');




    return 0;
}

int InitDiccionario(char diccionario [N_diccionario][N_palabra])
{
    FILE* manejador_archivo;
    if((manejador_archivo=fopen("diccionario_medio.txt","r")) == NULL){
        printf("Error al abrir el archivo");
        return 1;
    }

    char palabra [N_palabra];
    int index = 0;

    while (index < N_diccionario)
    {
        fscanf(manejador_archivo,"%s",palabra);
        strncpy(diccionario[index], palabra, N_palabra - 1);
        index++;
    }

    return 0;
}

void InitTraining(char training_set[N_entrenamiento][N_oracion],int vector_tipos [N_entrenamiento])
{
    FILE* manejador_archivo;
    if((manejador_archivo=fopen("la_renga.txt","r")) == NULL){
        printf("Error al abrir el archivo");
    }
    rewind(manejador_archivo);

    char oracion [N_oracion];
    int index = 0;

    while (index < N_entrenamiento/2)
    {
        fgets(oracion,N_oracion,manejador_archivo);
        oracion[strcspn(oracion, "\n")] = 0;    // Aca le saco el salto de linea que mete el fgets (no se si sera necesario)
        strcpy(training_set[index],oracion);
        vector_tipos [index] = 0;
        index++;
    }
    fclose(manejador_archivo);


    if((manejador_archivo=fopen("callejeros.txt","r")) == NULL){
        printf("Error al abrir el archivo");
    }
    rewind(manejador_archivo);

    while (index < N_entrenamiento)
    {
        fgets(oracion,N_oracion,manejador_archivo);
        oracion[strcspn(oracion, "\n")] = 0;    // Aca le saco el salto de linea que mete el fgets (no se si sera necesario)
        strcpy(training_set[index],oracion);
        vector_tipos [index] = 1;
        index++;
    }

}

int ObtenerCaracteristicas(char diccionario [N_diccionario][N_palabra], char oracion[], float vector_caracteristicas[], int tipo, int imprimir)
{
    int index;

    for (index=0;index<N_diccionario;index++)
    {
        vector_caracteristicas[index]=strstr_custom(oracion,diccionario[index]);

    }
    normalizar_vector(vector_caracteristicas,N_diccionario);

    if(imprimir == 1)
    {
       ImprimirVectorCaracteristicas(diccionario,oracion,vector_caracteristicas,tipo);
    }

    return 0;
}

void ImprimirVectorCaracteristicas(char diccionario [N_diccionario][N_palabra],char oracion[],float vector_caracteristicas[],int tipo)
{
    int i;
    printf("%s: %s. (Norma: %f)\n",(tipo == 0 ) ? "Tipo 0":"Tipo 1",oracion,calcular_norma(vector_caracteristicas,N_diccionario));
    for (i=0;i<N_diccionario;i++)
    {
        if((vector_caracteristicas[i] != 0) && (vector_caracteristicas[i] != 10))
        {
           printf("%s: %f\n",diccionario[i],vector_caracteristicas[i]);
        }

    }
}


void obtener_vector_distancias ( char nueva_oracion [N_oracion] , float vector_caracteristicas [N_entrenamiento][N_diccionario] , char diccionario [N_diccionario][N_palabra] , float distancias [N_entrenamiento] ,int imprimir_caracteristicas)
{
    float vector_caracteristicas_nueva_oracion [N_diccionario];
    ObtenerCaracteristicas( diccionario , nueva_oracion , vector_caracteristicas_nueva_oracion, 1 , imprimir_caracteristicas );

    int i;
    for (i=0;i<N_entrenamiento;i++)
    {
       distancias[i] = calcular_distancia(vector_caracteristicas_nueva_oracion,vector_caracteristicas[i],N_diccionario);
    }

}

void ordenar_traning_set(float vector_distancias[], char training_set [N_entrenamiento][N_oracion],int vector_tipos[N_entrenamiento], int N) {
    int i, j;
    float temp;
    char temp_str[N_oracion];
    int temp_int;
    for (i = 0; i < N-1; i++) {
        for (j = 0; j < N-i-1; j++) {
            if (vector_distancias[j] > vector_distancias[j+1]) {
                // intercambio los elementos en el arreglo de floats
                temp = vector_distancias[j];
                vector_distancias[j] = vector_distancias[j+1];
                vector_distancias[j+1] = temp;

                // intercambio los elementos en el arreglo de strings
                strcpy(temp_str, training_set[j]);
                strcpy(training_set[j], training_set[j+1]);
                strcpy(training_set[j+1], temp_str);

                // intercambio elementos en el arreglo de resultados
                temp_int = vector_tipos[j];
                vector_tipos[j]=vector_tipos[j+1];
                vector_tipos[j+1]=temp_int;

            }
        }
    }
}

int decision_final(int vector_tipos[N_entrenamiento] ,int K)
{

    int unos=0,ceros=0;
    int i;

    for (i=0;i<K;i++)
    {
        if(vector_tipos[i] == 0){ceros++;}
        if(vector_tipos[i] == 1){unos++;}
    }

    if( unos > ceros )
    {
        return 1;
    }
    else if (ceros > unos)
    {
        return 0;
    }
    else
    {
        return -1;

    }

}

int strstr_custom(char cadena[],char palabra[])
{
    int i=0;
    int j,flag;
    int ocurrencias=0;
    int N = strlen(palabra);

    while(cadena[i] != '\0' )
    {

        if( (i == 0) || (cadena[i] == ' ') )
        {
             if(cadena[i] == ' ' ){i++;}

             if( (cadena[i] == palabra[0]) || (cadena[i] + 'a'-'A' == palabra[0]) || (cadena[i]) == palabra[0] + 'a'-'A' )
             {
                flag = 1;
                for (j =1;j<N;j++)
                {
                    if(cadena[i+j] != palabra[j] )
                    {
                        flag = 0;
                        break;
                    }
                }

                if( (flag == 1) && ( (cadena[i+N] == ' ') ||(cadena[i+N]) == '\0'  ) )
                {
                   ocurrencias++;
                   flag=0;
                }
             }
        }
        i++;
    }
    return ocurrencias;
}


float calcular_distancia(float vector1[],float vector2[],int dimension)
{
    int i;
    float distancia=0;
    for (i=0;i<dimension;i++)
    {
        distancia = distancia + pow((vector1[i] - vector2[i]),2 );
    }
    distancia = sqrt(distancia);
    return distancia;
}

void normalizar_vector(float vector[],int dimension)
{
    float norma = calcular_norma(vector,N_diccionario);
    int i;
    if(norma != 0)
    {
        for(i=0;i<dimension;i++)
        {
            vector[i] = vector[i] / norma;
        }
    }
    // Esto es medio sucio lo hago para que si no encuentra ninguna palabra en el diccionario no pese la oracion para nada
    else
    {
        for(i=0;i<dimension;i++)
        {
            vector[i] = vector[i] + 10;
        }
    }
}

float calcular_norma(float vector[],int dimension)
{
    float norma = 0;
    int i = 0;
    for (i=0;i<dimension;i++)
    {
        norma = norma + pow(vector[i],2);
    }
    return sqrt(norma);
}



