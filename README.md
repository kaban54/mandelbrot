# mandelbrot

![](./result.png)

В ходе работы были протестированны две версии программы, рисующей множество Мандельброта,
и измерено значение FPS для каждой версии.

___________________________________________________________

Все тесты проводилиись на процессоре Intel® Core™ i5-1135G7\
с компилятором gcc версии 11.3.0\
Флаги компиляции: -O3 -mavx2

## Краткое описание версий ##

**v1:**\
В первой версии представлена скалярная реализация,\
т.е. рассчёты проводятся отдельно для каждой точки.

**v2:**\
Во второй версии предсавлена веркотрная реализация,\
в ней используются SIMD-инструкции, и вычисления ведутся параллельно для восьми точек

___________________________________________________________

Целью работы было узнать, во сколько раз оптимизация ускорит программу.

Т.к. оптимизации никак не затрагивали процесс рисования, то
для каждой версии было проведено 3 теста:\
вычисления для каждой точки проводились 1, 10 и 30 раз.\
При этом вывод точки на экран производился только 1 раз.

В таблице представлены значения FPS для разных версий и разного количества повторений
 
Версия   | 1 повторение | 10 повторений | 30 повторений |
---------|--------------|---------------|---------------|
 v1      |  4.8         | 0.48          | 0.16          |
 v2      | 31.4         | 3.40          | 1.14          |
 
Пусть k - коэффициент ускорения.

Тогда\
для  1 повторения k = 6.54\
для 10 повторений k = 7.08\
для 30 повторений k = 7.13

Видно, что полученное значение k соответствует ожидаемому значению, т.е. чуть меньше 8.

## Вывод ##

С помощью SIMD инструкций можно в несколько раз ускорить работу некоторых программ,\
при этом компилятор не способен сам производить такие оптимизации.\
Поэтому умение использовать SIMD инструкции является полезным навыком.

