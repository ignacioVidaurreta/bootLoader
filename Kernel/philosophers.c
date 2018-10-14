#include <stdint.h>
#include <mutex.h>
#include <naiveConsole.h>
#include <process.h>
#include <philosophers.h>
#include <lib.h>
#include <buddy.h>


void philosophers(uint64_t i){
    ncScroll(); /*Por qué funciona sólo si está arriba?? ._. */
    ncPrint((char*)i);

    /*
        char * name =get_current_proc()->name;
        uint64_t pid = get_current_proc()->pid;

        char* right_fork = mymalloc(sizeof(right_fork));
        char* left_fork = mymalloc(sizeof(left_fork));

        intToString((i+1)%5, right_fork);
        intToString(i, left_fork);

        ncPrint(name);
        ncPrint("  is Waiting for his or hers");
        if ( i%2 == 0){ //Si es par
            ncPrint("left fork");
            ncScroll();
            lock(concat("__FORK", right_fork), pid);
            ncPrint("right fork");
            ncScroll();
            lock(concat("__FORK", left_fork), pid);
        }else{
            ncPrint("right fork");
            ncScroll();
            lock(concat("__FORK", left_fork), pid);
            ncPrint("left fork");
            ncScroll();
            lock(concat("__FORK", right_fork), pid);
        }
        ncPrint(name);
        ncPrint("  is Eating");
        int j = 0;
        while(j<100000){
            j++;
        }
        ncPrint(name);
        ncPrint(" stopped eating and is releasing ");
        if ( i%2 == 0){ //Si es par
            ncPrint("left fork");
            ncScroll();
            unlock(concat("__FORK", right_fork), pid);
            ncPrint("right fork");
            ncScroll();
            unlock(concat("__FORK", left_fork), pid);
        }else{
            ncPrint("right fork");
            unlock(concat("__FORK", left_fork), pid);
            ncPrint("left fork");
            unlock(concat("__FORK", right_fork), pid);
        }

        ncPrint(name);
        ncPrint("  is thinking ...");
        ncScroll();
        j = 0;
         while(j<100000){
            j++;
        }
      */

}

void testPhilosophers(){
    initPhil();
}

void initPhil(){
    for(int i = 0; i<5; i++){
        char * aux = mymalloc(sizeof(aux));
        createMutex(concat("__FORK", intToString(i, aux)), 20); //Ese 20 es medio rancio
    }

    //char* args[] = {"0", "1", "2", "3", "4"};
    char * cero[] = {"0"};
    char * uno[] = {"1"};
    char * dos[] = {"2"};
    char * tres[] = {"3"};
    char * cuatro[] = {"4"};
    for(int j = 0; j<5; j++){
        // ncPrint(args[j]);
        // ncScroll();

        switch(j){
            case 0:
                start_proc("Aristoteles", (void*) philosophers, 1, cero);
                break;
            case 1:
                start_proc("Socrates", (void*) philosophers, 1, uno );
                break;
            case 2:
                start_proc("Platon", (void*) philosophers, 1, dos);
                break;
            case 3:
                start_proc("Simone de Beauvoir", (void*) philosophers, 1, tres);
                break;
            case 4:
                start_proc("Kant", (void*) philosophers, 1, cuatro);
                break;
            // // default:
            //     start_proc(concat("Generic #", intToString(j, aux2)), (void*) philosophers, 1, args); //por si llegamos a ampliar
            //     break;
        }

    }


}
