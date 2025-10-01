/****************************************************
 * Programa de Clasificación de Canciones con KNN
 *
 * Descripción general:
 * --------------------
 * Este programa implementa un clasificador de frases
 * (canciones, letras o expresiones cortas) utilizando
 * el algoritmo K-Nearest Neighbors (KNN).
 *
 * Flujo principal:
 * ----------------
 * 1. Inicializa un diccionario de palabras (vector de vocabulario).
 * 2. Inicializa el conjunto de entrenamiento (frases y sus tipos).
 * 3. Calcula los vectores de características de cada frase
 *    del set de entrenamiento en función del diccionario.
 * 4. Ofrece un menú al usuario:
 *      - Testear el clasificador con un conjunto de prueba.
 *      - Ingresar una nueva frase para clasificar.
 *      - Salir del programa.
 *
 * Constantes principales:
 * -----------------------
 *  N_diccionario   : Cantidad máxima de palabras en el diccionario.
 *  N_palabra       : Longitud máxima de cada palabra.
 *  N_oracion       : Longitud máxima de cada oración.
 *  N_entrenamiento : Cantidad de frases en el set de entrenamiento.
 *  N_classify      : Cantidad de frases en el set de prueba.
 *  Imprimir        : Bandera para mostrar o no características.
 *
 * Funciones declaradas:
 * ---------------------
 *  - InitDiccionario(...)               : Inicializa el diccionario.
 *  - InitTraining(...)                  : Inicializa set de entrenamiento.
 *  - InitClassifySet(...)               : Inicializa set de prueba.
 *  - ObtenerCaracteristicas(...)        : Genera vector de características
 *                                         para una frase.
 *  - ImprimirVectorCaracteristicas(...) : Debug de características.
 *  - decision_final(...)                : Determina clase según vecinos K.
 *  - obtener_vector_distancias(...)     : Calcula distancias a set entrenado.
 *  - ordenar_training_set(...)          : Ordena frases de entrenamiento por distancia.
 *  - calcular_norma(...)                : Calcula norma de un vector.
 *  - normalizar_vector(...)             : Normaliza vector de características.
 *  - calcular_distancia(...)            : Distancia entre dos vectores.
 *  - strstr_custom(...)                 : Función auxiliar para buscar subcadenas.
 *
 * Arreglos globales:
 * ------------------
 *  - diccionario[][]            : Contiene palabras del vocabulario.
 *  - training_set[][]           : Frases de entrenamiento.
 *  - vector_caracteristicas[][] : Características de cada frase entrenada.
 *  - vector_tipos[]             : Tipo/clase de cada frase (ej: banda/artista).
 *  - classify_set[][]           : Frases del conjunto de prueba.
 *  - vector_tipos_classify[]    : Tipos reales del set de prueba.
 *  - nueva_oracion[]            : Buffer para nuevas frases ingresadas.
 *  - vector_distancias[]        : Distancias de la frase actual al set entrenado.
 *
 * main():
 * -------
 *  - Define etiquetas de tipo (ej: "La Renga", "Charly").
 *  - Inicializa diccionario y set de entrenamiento.
 *  - Genera vectores de características para entrenamiento.
 *  - Ejecuta un menú interactivo para clasificar frases.
 *
 ****************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N_diccionario 108788
#define N_palabra 20
#define N_oracion 150
#define N_entrenamiento 100
#define N_classify 20
#define Imprimir 0

// Inicializacion de diccionario y set de entrenamiento:
int InitDiccionario(char diccionario [N_diccionario][N_palabra]);
void InitTraining(char training_set[N_entrenamiento][N_oracion],int vector_tipos [N_entrenamiento]);

// Inicializacion de frases a clasificar:
void InitClassifySet(char classify_set[N_classify][N_oracion],int vector_tipos_classify[N_classify]);

// Funciones para manipular los vectores
int ObtenerCaracteristicas(char diccionario [N_diccionario][N_palabra], char oracion[], float vector_caracteristicas[], int tipo, int imprimir);
void ImprimirVectorCaracteristicas(char diccionario [N_diccionario][N_palabra],char oracion[],float vector_caracteristicas[],int tipo);
int decision_final(int vector_tipos[N_entrenamiento] ,int K);
void obtener_vector_distancias ( char nueva_oracion [N_oracion] , float vector_caracteristicas [N_entrenamiento][N_diccionario] , char diccionario [N_diccionario][N_palabra] , float distancias [N_entrenamiento] , int imprimir_caracteristicas);
void ordenar_training_set(float vector_distancias[], char training_set [N_entrenamiento][N_oracion],int vector_tipos[N_entrenamiento],float vector_caracteristicas[N_entrenamiento][N_diccionario],  int N);


// Funciones auxiliares
float calcular_norma(float vector[],int dimension);
void normalizar_vector(float vector[],int dimension);
float calcular_distancia(float vector1[],float vector2[],int dimension);
int strstr_custom(char cadena[],char palabra[]);


// Declaracion de arreglos (globales para que le asignen mas espacio)
char diccionario [N_diccionario][N_palabra]={0};
char training_set [N_entrenamiento][N_oracion]={0};
float vector_caracteristicas[N_entrenamiento][N_diccionario]={0};
int vector_tipos [N_entrenamiento]={0};

char classify_set[N_classify][N_oracion]={0};
int vector_tipos_classify[N_classify]={0};

char nueva_oracion [N_oracion];
float vector_distancias [N_entrenamiento];


int main()
{
    char T0 [N_palabra] = "La Renga";
    char T1 [N_palabra] = "Charly";

    int K = 5;
    int i;


    InitDiccionario(diccionario);
    InitTraining(training_set,vector_tipos);

    for(i=0;i<N_entrenamiento;i++)
    {
        ObtenerCaracteristicas(diccionario,training_set[i],vector_caracteristicas[i],vector_tipos[i],Imprimir);
    }

    printf("Bienvenido al programa de clasificaci�n de canciones con KNN\n");
    int seleccion;
    do{
        printf("1) Testear las frases del conjunto de prueba \n");
        printf("2) Ingresar nueva frase \n");
        printf("3) Salir \n");


        scanf("%d",&seleccion);fflush(stdin);

        switch(seleccion)
        {
            case 1:

                InitClassifySet(classify_set,vector_tipos_classify);
                int clasificaciones_correctas =0;
                int clasificaciones_incorrectas =0;

                for(i=0;i<N_classify;i++)
                {


                    obtener_vector_distancias(classify_set[i],vector_caracteristicas,diccionario,vector_distancias,Imprimir);

                    ordenar_training_set(vector_distancias,training_set,vector_tipos,vector_caracteristicas,N_entrenamiento);

                    int tipo_nueva_muestra = decision_final(vector_tipos,K);


                    if(tipo_nueva_muestra != vector_tipos_classify[i]){
                      printf("\nFrase: %s de tipo %s \n",classify_set[i],(vector_tipos_classify[i]==0)?T0:T1);
                      printf("---- Clasifico esta muestra como: %s ---- \n",(tipo_nueva_muestra==0)? T0:T1);
                      clasificaciones_incorrectas++;

                    }
                    else
                    {
                        clasificaciones_correctas++;
                    }

                }
                printf("Clasificaciones correctas: %d\n Clasificaciones incorrectas: %d\n" , clasificaciones_correctas, clasificaciones_incorrectas );
                break;

            case 2:
                printf("Ingrese una nueva oracion: \n");
                gets( nueva_oracion );

                obtener_vector_distancias(nueva_oracion,vector_caracteristicas,diccionario,vector_distancias,Imprimir);

                ordenar_training_set(vector_distancias,training_set,vector_tipos,vector_caracteristicas,N_entrenamiento);

                int tipo_nueva_muestra = decision_final(vector_tipos,K);

                printf("\n--- Los %d elementos mas cercanos son: ---\n",K);

                for (int i = 0;i<K; i++)
                {
                    printf("%s -> Tipo: %s (distancia %f)\n",training_set[i],(vector_tipos[i]==0) ? T0:T1,vector_distancias[i]);
                }

                printf("\n ---- Clasifico esta nueva muestra como: %s ---- \n",(tipo_nueva_muestra==0)? T0:T1);
                break;
            case 3:
                break;
        }
    }while(seleccion != 3);

    return 0;
}

/****************************************************
 * Función: InitDiccionario
 * ------------------------
 * Carga el diccionario de palabras desde el archivo
 * "Diccionario.txt" en la matriz diccionario[].
 *
 * Retorna:
 *   0 si se cargó correctamente,
 *   1 si hubo error al abrir el archivo.
 ****************************************************/

int InitDiccionario(char diccionario [N_diccionario][N_palabra])
{
    FILE* manejador_archivo;
    if((manejador_archivo=fopen("Diccionario.txt","r")) == NULL){
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

/****************************************************
 * Función: InitTraining
 * ---------------------
 * Carga el conjunto de entrenamiento desde los
 * archivos "la_renga_training.txt" y 
 * "charly_training.txt".
 *
 * Parámetros:
 *  - training_set[N_entrenamiento][N_oracion] :
 *      Matriz donde se almacenan las frases leídas.
 *  - vector_tipos[N_entrenamiento] :
 *      Vector con la clase de cada frase:
 *        0 = La Renga, 1 = Charly.
 *
 * Retorno:
 *  - void (no retorna valor).
 ****************************************************/

void InitTraining(char training_set[N_entrenamiento][N_oracion],int vector_tipos [N_entrenamiento])
{
    FILE* manejador_archivo;
    if((manejador_archivo=fopen("la_renga_training.txt","r")) == NULL){
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


    if((manejador_archivo=fopen("charly_training.txt","r")) == NULL){
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


/****************************************************
 * Función: InitClassifySet
 * ------------------------
 * Inicializa el conjunto de frases a clasificar (set de prueba)
 * y el vector con sus clases correspondientes.
 *
 * Parámetros:
 *  - classify_set[N_classify][N_oracion] :
 *      Matriz donde se guardan las frases leídas desde archivos.
 *  - vector_tipos_classify[N_classify] :
 *      Vector que indica el tipo (clase) de cada frase:
 *          0 -> Frase de "La Renga"
 *          1 -> Frase de "Charly"
 *
 * Descripción:
 *  1. Abre el archivo "la_renga_classify.txt" y lee N_classify/2 frases.
 *     Cada frase se guarda en classify_set y se etiqueta con tipo 0.
 *  2. Abre el archivo "charly_classify.txt" y lee las frases restantes,
 *     que se etiquetan con tipo 1.
 *  3. Se eliminan los saltos de línea generados por fgets().
 *
 * Notas:
 *  - Si los archivos no existen, imprime un error en consola.
 *  - No hay control de overflow: se asume que los archivos
 *    contienen la cantidad esperada de frases.
 ****************************************************/

void InitClassifySet(char classify_set[N_classify][N_oracion],int vector_tipos_classify[N_classify])
{
    FILE* manejador_archivo;
    if((manejador_archivo=fopen("la_renga_classify.txt","r")) == NULL){
        printf("Error al abrir el archivo");
    }
    rewind(manejador_archivo);

    char oracion [N_oracion];
    int index = 0;

    while (index < N_classify/2)
    {
        fgets(oracion,N_oracion,manejador_archivo);
        oracion[strcspn(oracion, "\n")] = 0;    // Aca le saco el salto de linea que mete el fgets (no se si sera necesario)
        strcpy(classify_set[index],oracion);
        vector_tipos_classify [index] = 0;
        index++;
    }
    fclose(manejador_archivo);


    if((manejador_archivo=fopen("charly_classify.txt","r")) == NULL){
        printf("Error al abrir el archivo");
    }
    rewind(manejador_archivo);

    while (index < N_classify)
    {
        fgets(oracion,N_oracion,manejador_archivo);
        oracion[strcspn(oracion, "\n")] = 0;    // Aca le saco el salto de linea que mete el fgets (no se si sera necesario)
        strcpy(classify_set[index],oracion);
        vector_tipos_classify [index] = 1;
        index++;
    }

}

/****************************************************
 * Función: ObtenerCaracteristicas
 * -------------------------------
 * Genera el vector de características de una oración
 * a partir del diccionario de palabras.
 *
 * Parámetros:
 *  - diccionario[N_diccionario][N_palabra] :
 *      Lista de palabras que conforman el vocabulario.
 *  - oracion[] :
 *      Cadena con la frase a analizar.
 *  - vector_caracteristicas[] :
 *      Vector de salida con las características
 *      (aparición de cada palabra del diccionario).
 *  - tipo :
 *      Clase asociada a la oración (0 = La Renga,
 *      1 = Charly).
 *  - imprimir :
 *      Si es 1, muestra el vector generado.
 *
 * Retorno:
 *  - 0 en todos los casos (éxito).
 ****************************************************/

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

/****************************************************
 * Función: ImprimirVectorCaracteristicas
 * --------------------------------------
 * Muestra en pantalla el vector de características
 * asociado a una oración.
 *
 * Parámetros:
 *  - diccionario[N_diccionario][N_palabra] :
 *      Palabras del vocabulario.
 *  - oracion[] :
 *      Frase analizada.
 *  - vector_caracteristicas[] :
 *      Vector de características normalizado.
 *  - tipo :
 *      Clase de la oración (0 = La Renga,
 *      1 = Charly).
 *
 * Retorno:
 *  - void (no retorna valor).
 ****************************************************/

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

/****************************************************
 * Función: obtener_vector_distancias
 * ----------------------------------
 * Calcula las distancias entre una nueva oración
 * y todas las frases del set de entrenamiento.
 *
 * Parámetros:
 *  - nueva_oracion[N_oracion] :
 *      Frase ingresada para clasificar.
 *  - vector_caracteristicas[N_entrenamiento][N_diccionario] :
 *      Vectores de características de entrenamiento.
 *  - diccionario[N_diccionario][N_palabra] :
 *      Palabras del vocabulario.
 *  - distancias[N_entrenamiento] :
 *      Vector de salida con las distancias calculadas.
 *  - imprimir_caracteristicas :
 *      Si es 1, imprime el vector de características
 *      de la nueva oración.
 *
 * Retorno:
 *  - void (no retorna valor).
 ****************************************************/

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


/****************************************************
 * Función: ordenar_training_set
 * ------------------------------
 * Ordena el conjunto de entrenamiento y sus vectores
 * asociados según las distancias crecientes a una
 * nueva oración.
 *
 * Parámetros:
 *  - vector_distancias[] :
 *      Distancias entre la nueva oración y el set.
 *  - training_set[N_entrenamiento][N_oracion] :
 *      Frases del set de entrenamiento.
 *  - vector_tipos[N_entrenamiento] :
 *      Clases de cada frase (0 = La Renga, 1 = Charly).
 *  - vector_caracteristicas[N_entrenamiento][N_diccionario] :
 *      Vectores de características del entrenamiento.
 *  - N :
 *      Cantidad de frases a ordenar.
 *
 * Retorno:
 *  - void (no retorna valor).
 ****************************************************/

void ordenar_training_set(float vector_distancias[], char training_set [N_entrenamiento][N_oracion],int vector_tipos[N_entrenamiento],float vector_caracteristicas[N_entrenamiento][N_diccionario], int N) {
    int i, j;
    float temp;
    char temp_str[N_oracion];
    int temp_int;
    for (i = 0; i < N-1; i++) {
        for (j = 0; j < N-i-1; j++) {
            if (vector_distancias[j] > vector_distancias[j+1]) {

                // intercambio los elementos en el arreglo de distancias
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

                // intercambio elementos en el arreglo de caracteristicas (para que siga estando todo ordenado):
                int k;
                float caracteristicas_temp [N_diccionario];
                for (k =0;k< N_diccionario;k++)
                {
                    caracteristicas_temp[k] = vector_caracteristicas[j][k];
                }
                for (k =0;k< N_diccionario;k++)
                {
                    vector_caracteristicas[j][k] = vector_caracteristicas[j+1][k];
                }
                for (k =0;k< N_diccionario;k++)
                {
                    vector_caracteristicas[j+1][k] = caracteristicas_temp[k];
                }



            }
        }
    }
}


/****************************************************
 * Función: decision_final
 * ----------------------
 * Determina la clase final de una nueva oración
 * según los K vecinos más cercanos.
 *
 * Parámetros:
 *  - vector_tipos[N_entrenamiento] :
 *      Clases ordenadas de las frases más cercanas.
 *  - K :
 *      Número de vecinos a considerar.
 *
 * Retorno:
 *  - 0 : Clase "La Renga"
 *  - 1 : Clase "Charly"
 *  - -1: Empate entre clases
 ****************************************************/

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


/****************************************************
 * Función: strstr_custom
 * ---------------------
 * Cuenta cuántas veces aparece una palabra completa
 * dentro de una cadena, considerando mayúsculas y minúsculas.
 *
 * Parámetros:
 *  - cadena[] :
 *      Texto donde se busca la palabra.
 *  - palabra[] :
 *      Palabra a buscar dentro de la cadena.
 *
 * Retorno:
 *  - Número de ocurrencias de la palabra en la cadena.
 ****************************************************/

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

/****************************************************
 * Función: calcular_distancia
 * --------------------------
 * Calcula la distancia euclidiana entre dos vectores.
 *
 * Parámetros:
 *  - vector1[] :
 *      Primer vector.
 *  - vector2[] :
 *      Segundo vector.
 *  - dimension :
 *      Número de elementos en los vectores.
 *
 * Retorno:
 *  - Distancia euclidiana entre vector1 y vector2.
 ****************************************************/

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


/****************************************************
 * Función: normalizar_vector
 * --------------------------
 * Normaliza un vector dividiendo cada componente
 * por su norma. Si la norma es cero, se aplica
 * un valor alternativo para evitar que la oración
 * tenga peso.
 *
 * Parámetros:
 *  - vector[] :
 *      Vector de características a normalizar.
 *  - dimension :
 *      Número de elementos en el vector.
 *
 * Retorno:
 *  - void (no retorna valor).
 ****************************************************/

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

/****************************************************
 * Función: calcular_norma
 * -----------------------
 * Calcula la norma (longitud) de un vector.
 *
 * Parámetros:
 *  - vector[] :
 *      Vector de entrada.
 *  - dimension :
 *      Número de elementos en el vector.
 *
 * Retorno:
 *  - Norma euclidiana del vector.
 ****************************************************/

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