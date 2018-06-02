#include <time.h>
#include <stdlib.h>
#include <shell.h> //ESTO SACARLO CUANDO SE SOLUCIONE LO DEL \N PORQUE NO VAMOS A NECESITAR SCROLL

void getTime(){
  int day, month, year;
  day = getDay();
  month = getMonth();
  year = getYear();
  printDate(day, month, year); //TODO averiguar como imprimir el año como 2018 en nvez de 18

  printf(" --- ");

  int hr, min, sec;
  hr = getHour();
  min = getMinutes();
  sec = getSeconds();
  printTime(hr, min, sec);
  scroll();
}

int getDay(){
  return int80(6,0,0,0,0,5);
}

int getMonth(){
  return int80(7,0,0,0,0,5);
}

int getYear(){
  return int80(8,0,0,0,0,5);
}

void printDate(int day, int month, int year){
  char months[][13] = {"-", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio",
                    "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
  printf("%d de %s del %d", day, months[month], year);
}

int getHour(){
  return int80(4,0,0,0,0,5);
}

int getMinutes(){
  return int80(3,0,0,0,0,5);
}

int getSeconds(){
  return int80(2,0,0,0,0,5);
}

void printTime(int hr, int min, int sec){
  printf("%d:%d:%d", hr, min, sec );
}
