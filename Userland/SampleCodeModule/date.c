#include <date.h>
#include <stdlib.h>
#include <shell.h>

void getDate(){
  int day, month, year;
  day = getDay();
  month = getMonth();
  year = getYear();
  printDate(day, month, year);

  printf("   ");

  int hr, min, sec;
  hr = getHour();
  min = getMinutes();
  sec = getSeconds();
  printTime(hr, min, sec);
  scroll();
}

int getDay(){
  return int80(6,0,0,0,0,SYS_TIME);
}

int getMonth(){
  return int80(7,0,0,0,0,SYS_TIME);
}

int getYear(){
  return int80(8,0,0,0,0,SYS_TIME);
}

void printDate(int day, int month, int year){
  char months[][13] = {"-", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio",
                    "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
  printf("%d de %s del %d", day, months[month], 2000 + year);
}

int getHour(){
  return int80(4,0,0,0,0,SYS_TIME);
}

int getMinutes(){
  return int80(3,0,0,0,0,SYS_TIME);
}

int getSeconds(){
  return int80(2,0,0,0,0,SYS_TIME);
}

void printTime(int hr, int min, int sec){
  if(hr >= 10 && min >= 10 && sec >= 10)
    printf("%d:%d:%d (GMT)", hr, min, sec );
  else if(hr >= 10 && min >= 10)
    printf("%d:%d:0%d (GMT)", hr, min, sec );
  else if(hr >= 10 && sec >= 10)
    printf("%d:0%d:%d (GMT)", hr, min, sec );
  else if(min >= 10 && sec >= 10)
    printf("0%d:%d:%d (GMT)", hr, min, sec );
  else if(hr >= 10)
    printf("%d:0%d:0%d (GMT)", hr, min, sec );
  else if(min >= 10)
    printf("0%d:%d:0%d (GMT)", hr, min, sec );
  else if(sec >= 10)
    printf("0%d:0%d:%d (GMT)", hr, min, sec );
  else
    printf("0%d:0%d:0%d (GMT)", hr, min, sec );
}
