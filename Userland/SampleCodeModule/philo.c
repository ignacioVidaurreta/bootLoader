#include <shell.h>
#include <stdlib.h>

char* phil_list[5]= {"Aristoteles", "Socrates", "Platon", "Simone De Beauvoir", "Kant"};

void philosophers(uint64_t i){
  //  while(1){
      int num = *((char*)i) - '0';
      char * name = phil_list[num];

      char* right_fork = malloc(sizeof(right_fork));
      char* left_fork = malloc(sizeof(left_fork));

      intToString((num+1)%5, right_fork);
      intToString(num, left_fork);


      if ( num%2 == 0){ //Si es par

              printf("%s  is waiting for the left fork... ", name);
              scroll();
              lockMutex(concat("__FORK", left_fork));
              printf("%s is waiting for the right fork... ", name);
              scroll();
              lockMutex(concat("__FORK", right_fork));
      }else{
              printf("%s is waiting for the right fork... ", name);
              scroll();
              lockMutex(concat("__FORK", right_fork));
              printf("%s  is waiting for the left fork... ", name);
              scroll();
              lockMutex(concat("__FORK", left_fork));
      }
      printf("%s is eating", name);
      scroll();



      if ( num%2 == 0){ //Si es par
              printf("%s stopped eating and is realeasing the left fork... ", name);
              scroll();
              unlockMutex(concat("__FORK", left_fork));
              printf("%s stopped eating and is realeasing the right fork... ", name);
              scroll();
              unlockMutex(concat("__FORK", right_fork));
      }else{
              printf("%s stopped eating and is realeasing the right fork... ", name);
              scroll();
              unlockMutex(concat("__FORK", right_fork));
              printf("%s stopped eating and is realeasing the left fork... ", name);
              scroll();
              unlockMutex(concat("__FORK", left_fork));
      }
      printf("%s is thinking... ", name);
      scroll();
//    }
}


/*
void philosophers(uint64_t i){
  printf("Hola, soy Olympe de Gauge %s", (char*)i);
  scroll();
}
*/





void initPhil(){
    for(int i = 0; i<5; i++){
        char * aux = (char*) malloc(sizeof(aux));
        createMutex(concat("__FORK", intToString(i, aux)));
    }

    createMutex("__PRINT_MUTEX__");
    //char* args[] = {"0", "1", "2", "3", "4"};
    char * cero[] = {"0"};
    char * uno[] = {"1"};
    char * dos[] = {"2"};
    char * tres[] = {"3"};
    char * cuatro[] = {"4"};
    //int64_t pids[5];
    for(int j = 0; j<5; j++){
        // ncPrint(args[j]);
        // ncScroll();

        switch(j){
            case 0:
                //pids[0] =
                start_proc_user("Aristoteles", (void*) philosophers, 1, cero, 0);
                break;
            case 1:
                //pids[1] =
                start_proc_user("Socrates", (void*) philosophers, 1, uno, 0);
                break;
            case 2:
                //pids[2] =
                start_proc_user("Platon", (void*) philosophers, 1, dos, 0);
                break;
            case 3:
                //pids[3] =
                start_proc_user("Simone de Beauvoir", (void*) philosophers, 1, tres, 0);
                break;
            case 4:
                //pids[4] =
                start_proc_user("Kant", (void*) philosophers, 1, cuatro, 0);
                break;
            // // default:
            //     start_proc(concat("Generic #", intToString(j, aux2)), (void*) philosophers, 1, args); //por si llegamos a ampliar
            //     break;
        }

    }




}

void testPhilosophers(){
    initPhil();
}
