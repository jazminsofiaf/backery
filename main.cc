
#ifdef BAKERY

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "MemoriaCompartida.h"
#include "Logger.h"
#include <string>
int calcularRandom (int i);
int get_index(pid_t arr[], pid_t element);

int main () {

  Logger logger;

  std::string archivo = "/bin/ls";

  pid_t childrenIds[10];

  for(int i= 0; i < 2; i++){
	     pid_t procId = fork ();
	      if ( procId == 0 ) {
      		// codigo del hijo
          		try {
              			MemoriaCompartida<int> buffer ( archivo,'A',10);

                    int random = calcularRandom (i);
			
                    logger.log("Hijo "+std::to_string(i)+": voy a dormir  "+std::to_string(random)+"("+std::to_string(getpid())+")");
                    sleep ( random );
              			logger.log("Hijo"+std::to_string(i)+ ": escribo " +std::to_string(random)+ " en la memoria compartida ("+std::to_string(getpid())+")");
              			buffer.escribir(i, random);


						logger.log("Hijo"+std::to_string(i)+ ": fin del proceso" +std::to_string(getpid()));
          		} catch ( std::string& mensaje ) {
                		std::cerr << mensaje << std::endl;
          		}
    		      exit(0);
        }
        else{
          childrenIds[i] = procId;
        }
	}
  // codigo del padre
  for(int i= 0; i < 2; i++){
    		try {
        			MemoriaCompartida<int> buffer ( archivo,'A',10 );
              // espera a que termine el hijo
        			logger.log("Padre: esperando a que termine el hijo");
        			pid_t childId =  wait(NULL);
              int index = get_index(childrenIds, childId);
              logger.log("Padre: mi hijo "+std::to_string(childId)+" escribio en la posicion "+std::to_string(index)+" de memoria compartida ");

        			int resultado = buffer.leer(index);
        			logger.log("Padre: mi hijo "+std::to_string(childId) +" durmio " +std::to_string(resultado)+" segundos.");

    		} catch ( std::string& mensaje ) {
    			    std::cerr << mensaje << std::endl;
    		}
	}
  exit(0);

}

int get_index(pid_t arr[], pid_t element){
  for (int i = 0; i < 10; i++){
       if (element  == arr[i]){
           return i;
       }
   }
   return -1;
}

int calcularRandom (int i) {
	srand ( (unsigned int) time(NULL)*i );
	int resultado = rand() % 10 + 1;
	return resultado;
}

#endif
