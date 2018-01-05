#include <LowPower.h>// Библиотека сна

#include <LiquidCrystal_1602_RUS.h>// Доработанная мной библиотека русских символов, если у вас стоит оригинальная, обязательно замените, иначе будут проблемы с lcd.clear();
#include "EncoderRead.h"// Один из приложенных файлов - считывание энкодера

LiquidCrystal_1602_RUS  lcd(2, 3, 4, 5, 6, 7);// Объект дисплея

#define                 HEAT_PIN        A1// Пин мосфета
#define                 TC_OP           A0// Пин термопары
#define                 LCD_BACKLIGHT   8// Пин подсветки дисплея
#define                 ON_BUTTON       A2// пин кнопки включения

#define                 MIN_PRESET      200// Минимальная устанавливаемая температура
#define                 MAX_PRESET      480// Максимальная устанавливаемая температура

unsigned long           lcd_tmr;// Переменные для отработки времени
unsigned long           time;

boolean                 lcd_flg = true;// Флаг перерисовки экрана
boolean                 slp_flg;// Флаг засыпания


int                     temperature;// Текущая температура
int                     preset_temperature = MIN_PRESET;// Предустановленная температура

#include "ISR.h"// Файл с прерыванием, приложен к скетчу

void setup() {
  pinMode(ENCODERPIN_GND, OUTPUT);//Инициализация пинов энкодера и пина кнопки включения
  pinMode(ENCODERPIN_A, INPUT_PULLUP);//У меня земля энкодера берется с цифрового пина
  pinMode(ENCODERPIN_B, INPUT_PULLUP);
  pinMode(ENCODERPIN_SW, INPUT_PULLUP);
  pinMode(ON_BUTTON, INPUT_PULLUP);

  pinMode(LCD_BACKLIGHT, OUTPUT);// Пин подсветки - выход
  lcd.begin(16, 2);// Запускаем экран
  lcd.clear();// И очищаем от мусора

  PCICR = 0b00000010;// Разрешаем прерывания в общем
  PCMSK1 = 0b00000100;//Разрешаем прерывания на PCINT линии номер 1 (аналоговые пины)

  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);// Засыпаем
  
  digitalWrite(LCD_BACKLIGHT, 1);// Сюда попадем только если проснулись (по прерыванию в обработчике)
  
  lcd.begin(16, 2);// Еще раз запускаем экран, на всякий случай
  lcd.clear();// И очищаем
  lcd.print(L"Загрузка...");// Пишем "Загрузка..."
  delay(1000);// Пауза
  if(analogRead(TC_OP) > 720){// Проверка наличия паяльника
    lcd.clear();// Если нет паяльника, пишем "Нет паяльника" и ждем подключения паяльника
    lcd.print(L"Нет паяльника!!!");
    
    while(analogRead(TC_OP) > 720);
    lcd_flg = true;
    
    lcd.clear();
    lcd.print(L"Загрузка...");
  }
  lcd.print(L"ОК");// Иначе
  delay(300);
  lcd.clear();
  lcd.print(L"ПАЯЛЬНАЯ СТАНЦИЯ");// Выводим приветственное сообщение
  lcd.setCursor(0, 1);
  lcd.print("V 3.0");
  delay(1500);
  lcd.clear();
  lcd.print("by Mr Transistor");
  delay(1500);
  lcd.clear();
  lcd.print(L"Уст. t°C: ");// Выводим информацию на экран
  lcd.setCursor(0, 1);
  lcd.print(L"Текущ. t°C: ");
  pinMode(HEAT_PIN, OUTPUT);// Инициализируем пин нагревателя
  
}

void clearChars(int startPosX, int startPosY, int numOf){// Функция стирания чисел с экрана, параметры - начальные координаты стиания, стираем numOf символов
  lcd.setCursor(startPosX, startPosY);// Ставим курсор на начальные координаты,
  for(int i = 0; i < numOf; i++){// повторяем numOf раз
    lcd.print(" ");// рисование пробела
  }
  
}

void loop() {
  /*******ОБРАБОТКА ЭНКОДЕРА*******/
  char step = encoderRead();// Читаем энкодер, используя функцию
  if(step == 'R'){// Если шагнули по часовой стрелке
    preset_temperature++;// то увеличиваем предустановленную температуру
    lcd_flg = true;// и поднимаем флаг, чтобы сменить температуру на экране
  }
  if(step == 'L'){// Если шагнули влево
    preset_temperature--;// то уменьшаем
    lcd_flg = true;// и поднимаем флаг
  }
  preset_temperature = constrain(preset_temperature, MIN_PRESET, MAX_PRESET);// Если ушли за рамки настройки, то исправляем это (constrain() - ограничение)
  /********************************/

  /***********ЧТЕНИЕ ТЕКУЩЕЙ ТЕМПЕРАТУРЫ************/
  temperature = map(analogRead(TC_OP), 73, 321, 100, 400);// Читаем значения аналогово пина и масштабируем к значениям, рассчитанным по калибровочной таблице и мультиметру
  if(temperature > preset_temperature){// Если перегрели выше необходимой
    digitalWrite(HEAT_PIN, 0);//выключаем нагрев
  }
  if(temperature < preset_temperature - 1){// Небольшой гистерезис - 1 градус. Если опять остыло
    digitalWrite(HEAT_PIN, 1);// то включаем нагрев
  }
  /*************************************************/

  /*ОБНОВЛЕНИЕ ЭКРАНА ПО ФЛАГУ ИЛИ КАЖДЫЕ ПОЛСЕКУНДЫ*/
  if(millis() - lcd_tmr > 500 || lcd_flg){// Если надо что-то изменить на экране (пришло время или поднят флаг)
    if(lcd_flg){// Так как флаг поднимается только при изменении предустановки
      clearChars(10, 0, 3);// то стиаем только предустановленную температуру
      lcd.setCursor(10, 0);//и
      lcd.print(preset_temperature);//рисуем новую предустановку
      lcd_flg = false;// и опускаем флаг
    }
    else{// Иначе случится только, если прошло 500 милисекунд - надо перерисовать текущую
      clearChars(12, 1, 3);// Стираем ее
      if(temperature > 50){// и рисуем
        lcd.setCursor(12, 1);
        lcd.print(temperature);//новую, если она больше 50 градусов
      }
      else{// иначе выводим "--"
        lcd.setCursor(12, 1);
        lcd.print("--");
      }
      lcd_tmr = millis();//и обновляем таймер
    }
  }
  /**************************************************/

  if(analogRead(TC_OP) > 720){//проверяем наличие паяльника, если его нет
    digitalWrite(HEAT_PIN, 0);// выключаем питание на разъеме
    lcd.clear();
    lcd.print(L"Нет паяльника!!!");//и пишем "Нет паяльника"
    
    while(analogRead(TC_OP) > 720);//потом ждем, пока не подключим паяльник
    lcd_flg = true;// Поднимаем флаг, чтобы нарисовать все
    
    lcd.clear();
    lcd.print(L"Уст. t°C: ");// и пишем заголовки
    lcd.setCursor(0, 1);
    lcd.print(L"Текущ. t°C: ");
  }

  /*********ОБРАБОТКА ЗАСЫПАНИЯ ПО УДЕРЖАНИЮ КНОПКИ*********/
  if(!digitalRead(ON_BUTTON)) time = millis();// Здесь проверка удержания в течение 1500 милисекунд
  while(!digitalRead(ON_BUTTON)){
    if(millis() - time > 1500){// Если держали полторы секунды
      
      digitalWrite(HEAT_PIN, 0);//выключаем паяльник
      
      digitalWrite(LCD_BACKLIGHT, 0);// выключаем подсветку
      
      lcd.noDisplay();// усыпляем экран
      
      while(!digitalRead(ON_BUTTON));// дальше проверка, если ее стереть, то не работает, поэтому не трогаем, оно усыпляется сразу, без отпускания, в следующей версии я это исправлю
      
      PCMSK1 = 0b00000100;//Разрешаем прерывания на PCINT линии номер 1 (аналоговые пины)
      
      slp_flg = true;//поднимаем флаг, чтобы вывести сообщение по просыпании
      
      LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);// Уходим в самый глубокий сон, из которого нас выведет только прерывание
    }
  }
  if(slp_flg){// Если проснулись (флаг поднят)
    
    digitalWrite(LCD_BACKLIGHT, 1);// включаем подсветку экрана
    
    lcd.display();// будим экран
    
    slp_flg = false;// опускаем флаг сна
    
    lcd.begin(16, 2);// на всякий перезапускаем экран
    lcd.clear();
    lcd.print(L"Загрузка...");//опять то же, что и в сетапе
    
    delay(1000);
    
    if(analogRead(TC_OP) > 720){//проверка паяльника
      
      lcd.clear();
      lcd.print(L"Нет паяльника!!!");
    
      while(analogRead(TC_OP) > 720);
    
      lcd.clear();
      lcd.print(L"Загрузка...");
    }
    lcd.print(L"ОК");
    
    delay(300);
    
    lcd.clear();
    
    lcd.print(L"ПАЯЛЬНАЯ СТАНЦИЯ");
    lcd.setCursor(0, 1);
    lcd.print("V 3.0");
    
    delay(1500);
    
    lcd.clear();
    lcd.print("by Mr Transistor");
    
    delay(1500);
    
    lcd.clear();
    lcd.print(L"Уст. t°C: ");
    lcd.setCursor(0, 1);
    lcd.print(L"Текущ. t°C: ");
    lcd_flg = true;
  }

}//Тут все
