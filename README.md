# TransistorSoldering_V3.0
## Паяльная станция на Ардуино - версия 3.0

![IMAGE](https://github.com/MrTransistorsChannel/TransistorSoldering_V3.0/raw/master/Readme_src/Station.JPG)
# Оглавление

  * [Описание проекта](#Description)
  * [Настройки в прошивке](#Settings)
  * [Описание папок в репозитории](#Folders_description)
  * [Компоненты и материалы](#Components)
  * [FAQ](#FAQ)
  
<a id="Description"></a>
## Описание проекта
#### *Что это?*
Это один из крупнейших моих проектов - паяльная станция. Крупнейшим он является не по количеству входящих в него деталей, а по времени работы над ним и количеству промежуточных версий. Самая первая была собрана 16 июня прошлого уже 2017 года. Вот видео про нее: https://vk.com/wall367284984_1137. 19 июня была собрана версия 2.0, уже имевшая экран. Но проект был внезапно закончен по техническим причинам: в Ардуино УНО из-за КЗ 5-вольтовой линии на землю в 5-вольтовую часть станции попало напряжение 17 вольт. Микроконтроллер и операционный усилитель чудом уцелели, а вот экран и встроенный в дуню программатор выпустили волшебный дымок и перестали работать (как известно, все электронные компоненты работают за счет него). Ну и совсем недавно была разработана последняя на сегодняшний момент версия 3.0. В ней был добавлен режим сна, был оптимизирован вывод информации на экран, а также реле включения паяльника было заменено на мощный полевой транзистор IRF3205 для последующей разработки режимов поддержания температуры. Заморский англоязычный интерфейс был заменен родным русским, паяльник стал подключаться через удобный разъем, регулировка температуры была реализована на энкодере, а все устройство было убрано в небольшой деревянный корпус.

#### *История* 
Мне давно хотелось сделать переносную паяльную станцию на аккумуляторе. Гугление выдавало то, что люди делают аккумуляторные паяльники по типу выжигателя (импульсные). Так как в трудные времена (когда после летних каникул я забыл паяльник в деревне) я паял детским выжигателем, что было ну очень неудобно, мне эта конструкция не понравилась. И тут я решил узнать про паяльники от паяльных станций, про которые я слышал, что они очень быстро греются. И когда я пошел в магазин радиодеталей я купил первый попавшийся паяльник от станции "Lukey 702". Именно им я паяю постоянно и именно он подключается к моей станции. Тогда мне улыбнулась удача - паяльник оказался на 24 вольта 50 ватт и он отличто работал от блока питания зарядника шуруповерта на 17 вольт. Я тогда уже знал про великое творение китайцев - повышающие модули. И когда у меня появился такой (был куплен для питания усилителя звука), я решил попробовать все 24 вольта с аккума от модели на 12 вольт. Паяльник разогрелся за считанные секунды. Тогда то и было положено начало этому проекту.

#### *Что дальше?*
Планируется дорабатывать прошивку для повышения удобства работы, т. к. в этой версии, например, используется обработка энкодера без прерываний, которую я хочу в будующем доработать с помощью PCINT (Pin Change INTerrupts - прерывания на всех пинах микроконтроллера). Ну и конечно же будет придумано крепление модельного акума, внутрь будет установлен повышающий модуль, а сверху - ручка для переноски.
  
<a id="Settings"></a>
## Настройки в коде прошивки

    #define MIN_PRESET    200// Минимальная устанавливаемая температура
    #define MAX_PRESET   480// Максимальная устанавливаемая температура
    
<a id="Folders_description"></a>
## Описание папок в репозитории
  - **libraries** - требуемые для компиляции прошивки библиотеки - закинуть в C://Program Files/Arduino/libraries
  - **src** - код прошивки
  - **scheme** - схемы подключения
  - **Readme_src** - это вам не надо - это картинки, вставленные в README.md

<a id="Components"></a>
## Материалы и компоненты
  - Паяльник для паяльной станции Lukey 702 (Не брать с нагревателем Hakko!!!)
  - [Arduino NANO](http://ali.pub/20o36t)
  - [LCD1602](http://ali.pub/21wqla)
  - Макетные платы (магазин радиодеталей, но лучше взять тут http://ali.pub/70kct)
  - Микросхема LM358 (магазин радиодеталей)
  - Микросхема КР142ЕН5А (магазин радиодеталей)
  - [Транзистор IRF3205](http://ali.pub/s58ow)
  - Мелкие компоненты - резисторы/конденсаторы/дроссели (перечислять не буду, есть на схемах, магазин радиодеталей)
  - Энкодер (магазин радиодеталей, но дешевле взять модуль энкодера для Ардуино тут, например http://ali.pub/1rcuzf)
  - Кнопка КМД 1-1 (советская, можно попробовать найти в магазине радиодеталей или на радиорынке, но можно поставить типо такой http://ali.pub/fm2kg )
  - Разъем 5.5 * 2.5 мама (магазин радиодеталей)
  - Разъем DIN-5 мама + папа (магазин радиодеталей)
  - Колпачок на потенциометр (надо искать тот, у которого отверстие крепления полукруглое, а не круглое, т. к. это на энкодер, магазин радиодеталей)
  - Радиаторы на силовой транзистор IRF3205 и на микросхему КР142ЕН5А (не обязательно, они не греются, у меня для фен-шуя))))
  - Подстроечный резистор на 10 килоом (регулировка контрастности экрана, магазин радиодеталей)
  - Неизвестное количество монтажного и силового провода (магазин радиодеталей, берите побольше, никогда не помешает)
  
  - Корпус - фанера 3 мм, термоклей (для крышки, чтобы можно было оторвать, если что), клей ПВА или обычный Момент (для стенок)
  
<a id="FAQ"></a>
## FAQ
### Основные вопросы
В: Как скачать с этого сайта?  
О: На главной странице проекта (где ты читаешь этот текст) вверху справа зелёная кнопка **Clone or download**, вот её жми, там будет **Download ZIP**

В: Скачался какой то файл .zip, куда его теперь?  
О: Это архив. Можно открыть стандартными средствами Windows, но думаю у всех на компьютере установлен WinRAR?, или, на крайний случай, 7-zip, архив нужно правой кнопкой и извлечь.

В: Я совсем новичок! Что мне делать с Ардуиной, где взять все программы?  
О: Читай и смотри видосы, например, этот http://alexgyver.ru/arduino-first/

В: Компьютер никак не реагирует на подключение Ардуины!  
О: Возможно у тебя зарядный USB кабель, а нужен именно data-кабель, по которому можно данные передавать
О: Если у тебя китайская Ардуино, то для нее надо поставить драйвера, гугли "Драйвера CH340G" или сразу лезь [сюда](http://smart-chip.ru/drajver-dlya-arduino/)

В: Ошибка! Скетч не компилируется!  
О: Проверь, правильно ли выбрана плата, порт.
О: Иногда встречается такое "Путь к скетчу не должен содержать кириллицу. Положи его в корень диска", но сам такого не замечал, хотя все скетчи храню на флешке, весь путь к ним на кириллице.

В: Сколько стоит?  
О: Ничего не продаю, а по отдельности зависит от уровня инфляции в стране и мест покупки)))
