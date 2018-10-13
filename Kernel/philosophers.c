#include <stdint.h>
#include <mutex.h>
#include <naiveConsole.h>
#include <process.h>
#include <philosophers.h>
#include <lib.h>
#include <buddy.h>


void philosophers(int i){

    //while(1){
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

    //}

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
