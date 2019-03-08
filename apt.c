//Recordar define. Antes de compilar sustituye todos los defines por los valores
//Macro: Más rapido que las funciones.


#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
//Header files .h
//¿Qué paasa con dos librerias con la msma variable global? 
//Checan variables de compilación
// Para hacerlo en dos lineas
#define handle_error_en(en, msg) \
	   do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

int
main(int argc, char *argv[])
{
   int s, j;
   
   cpu_set_t cpuset; // cpu_set_t es un conjunto de banderas, arreglo de bits que indica tantos procesadores tenga (Dentro de pthread)
   pthread_t thread;

   thread = pthread_self();    //Encontrar hilo de control

   /* Set affinity mask to include CPUs 0 to 7 */

   CPU_ZERO(&cpuset); //Borra todo el cpu_set_t (las banderas)
   for (j = 0; j < 1; j++) //Pone un 1 (set). Afinidad, si tu no eliges el hilo, lo elige el sistema operativo. 
	   CPU_SET(j, &cpuset);//modificamos el CPU que queremos que corra

   s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
   if (s != 0)
	   handle_error_en(s, "pthread_setaffinity_np");

   /* Check the actual affinity mask assigned to the thread */

   s = pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset); // donde esta corriendo 
   if (s != 0)
	   handle_error_en(s, "pthread_getaffinity_np");

   printf("Set returned by pthread_getaffinity_np() contained:\n");
   for (j = 0; j < CPU_SETSIZE; j++)
	   if (CPU_ISSET(j, &cpuset)) //Core disponibles
		   printf("    CPU %d\n", j);

   while(1) {}

   exit(EXIT_SUCCESS);
}