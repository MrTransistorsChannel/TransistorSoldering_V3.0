#define ENCODERPIN_A    9//пины энкодера
#define ENCODERPIN_B    10
#define ENCODERPIN_SW   11
#define ENCODERPIN_GND  12

int     x;//переменные для энкодера (были во всех предыдущих проектах с энкодером)
int     previous_x;
int     code=500;

char encoderStepped;

char encoderRead(){
/*******************************************ТУТ ОБЫЧНОЕ СЧИТЫВАНИЕ ЭНКОДЕРА ПО МОЕМУ АЛГОРИТМУ (НАДО БУДЕТ ЭТО ВСЕ В БИБЛИОТЕКУ ЗАГНАТЬ)***************************************************/
  encoderStepped = 0;// обнуление выходного значения
  
  previous_x=x; // Переменная, обозначающая значение предыдущего х записывает значение х из прошлого цикла//
  if(digitalRead(ENCODERPIN_A) && digitalRead(ENCODERPIN_B))//преобразование двоичного кода 00 в число 0//
  { 
    x=0;
  }
  if(digitalRead(ENCODERPIN_A) && !digitalRead(ENCODERPIN_B)){ //преобразование двоичного кода 01 в число 1//
    x=1;
  }
  if(!digitalRead(ENCODERPIN_A) && digitalRead(ENCODERPIN_B)){ //преобразование двоичного кода 10 в число 2//
    x=2;
  }
  if(!digitalRead(ENCODERPIN_A) && !digitalRead(ENCODERPIN_B)){//преобразование двоичного кода 11 в число 3//
    x=3;
  }
  if(x==0){
    code=500;
  } // вправо - 00 01 11 10 00 (0 1 3 2 0), влево - 00 10 11 01 00 (0 2 3 1 0)//
  if(previous_x!=x){
    if(x==1){
      code=code*3;
    }
    if(x==2){
      code=code+568;
    }
    if(x==3){
      code=code-256;
    }
  
    if(code==1812){
      encoderStepped = 'R';///////////если повернули вправо = 'R'
    }
    if(code==2436){
      encoderStepped = 'L';//////////если влево = 'L'
    }
  }
  /**************************************************************************************************/
  return encoderStepped;//возвращаем значение

}
