#include <stdint.h>
#include <mutex.h>
#include <naiveConsole.h>
#include <process.h>
#include <philosophers.h>
#include <lib.h>
#include <buddy.h>


void philosophers(int i){
    /*
    while(1){
        char * name =get_current_proc()->name;
        ncPrint(name);
        ncPrint("  is Waiting for his or hers");
        if ( i%2 == 0){ //Si es par
            ncPrint("left fork");
            wait(fork[(i+1)%5]);
            ncPrint("right fork");
            wait(fork[i]);
        }else{
            ncPrint("right fork")
            wait(fork[i]);
            ncPrint("left fork");
            wait(fork[(i+1)%5]);
        }
        ncPrint(name);
        ncPrint("  is Eating");
        int j = 0
        while(j<1000){
            j++;
        }
        ncPrint(name);
        ncPrint(" stopped eating and is releasing ");
        if ( i%2 == 0){ //Si es par
            ncPrint("left fork");
            signal(fork[(i+1)%5]);
            ncPrint("right fork");
            signal(fork[i]);
        }else{
            ncPrint("right fork")
            signal(fork[i]);
            ncPrint("left fork");
            signal(fork[(i+1)%5]);
        }

        ncPrint(name);
        ncPrint("  is thinking ...");
        j = 0;
         while(j<1000){
            j++;
        }

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


    for(int j = 0; j<5; j++){
        char* aux2 = mymalloc(sizeof(aux2)) ;
        switch(j){
            case 0:
                start_proc("Aristoteles", (void*) philosophers );
                break;
            case 1:
                start_proc("Socrates", (void*) philosophers );
                break;
            case 2:
                start_proc("Platon", (void*) philosophers);
                break;
            case 3:
                start_proc("Pitagoras", (void*) philosophers);
                break;
            case 4:
                start_proc("Kant", (void*) philosophers);
                break;
            case 5:
                start_proc("Olympe", (void*) philosophers);
                break;
            default:
                start_proc(concat("Generic #", intToString(j, aux2)), (void*) philosophers); //por si llegamos a ampliar
                break;
        }
    }


}
