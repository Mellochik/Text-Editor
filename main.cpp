#include <text_editor.hpp>
#include <cstring>
#include <iostream>

using namespace std;

Editor text(2, 2, 119, 26, 0x1F, 0xF0); // текст для редактирования
Editor help(5, 3, 116, 25, 0x5F); // файл помощи с кратким описанием программы
string file; // имя файла

int l1 = -1, l2 = -1; // первая и вторая строка выделения
int F5flag = -1; // флаг для проверки нажатия клафиши F5
int F6flag = -1; // флаг для проверки нажатия клафиши F6

int main(int argc, char *argv[]) {
   setlocale(LC_ALL,""); // русский язык для консоли
   file = argv[2]; // сохраняет имя файла из командной строки

   // разрешение на редактирование/просмотр
   if (strcmp(argv[1], "edit") == 0)
      text.setView(false);

   //чтение из файла
   open_file(&text, file);
   open_file(&help, "help.txt");

   // отрисовка окна
   draw_frame(file);

   // редактирвоание файла
   text.edit();

   return 0;
}

void special_key(int k) { // функция для обратоки специальных клавиш
   switch (k) {
   case KEY_F1 + KEY_SPECIAL: // вывод помощи
      help.update_screen(); // вывод на экран помощи
      getch();
      break;
   case KEY_F2 + KEY_SPECIAL: // сохранение в файл
      save_file(text, file); // сохранить в файл
      break;
   case KEY_F3 + KEY_SPECIAL: // начать выделение
      l1 = text.number_line(); // получить номер первой строки
      break;
   case KEY_F4 + KEY_SPECIAL: // закончить выделение
      l2 = text.number_line(); // получить номер второй строки
      break;
   case KEY_F5 + KEY_SPECIAL: // выделить и скопировать текст
      if (F5flag == -1 && l1 != -1 && l2 != -1) { // проверка на первое нажатие
         if (l1 > l2) { // меняет местами номера строк, если введены были снизу вверх
            int t = l2;
            l2 = l1;
            l1 = t;
         }
         F5flag *= -1; // изменение флага
      }
      else {
         for (int i = 0; i <= l2 - l1; i++) {
            text.add_line(text.print_highlighted_line(i)); // вставить текст из буфера
         }
         l1 = -1; // убрать выделение
         l2 = -1;
         F5flag *= -1; // изменение флага
      }
      text.fill_lines(l1, l2); // выделяет или уберает выделение
      break;
   case KEY_F6 + KEY_SPECIAL: // выделить и скопировать текст
      if (F6flag == -1 && l1 != -1 && l2 != -1) {
         if (l1 > l2) { // меняет местами номера строк, если введены были снизу вверх
            int t = l2;
            l2 = l1;
            l1 = t;
         }
         F6flag *= -1; // изменение флага
      }
      else {
         for (int i = l1; i <= l2; i++)
            text.remove_line(l1 - 1); // удаление строк
         l1 = -1;
         l2 = -1;
         F6flag *= -1; // изменение флага
      }
      text.fill_lines(l1, l2); // выделяет или уберает выделение
      break;
   case KEY_F10 + KEY_SPECIAL: // закрыть программу
      save_file(text, file); // сохранить в файл
      exit(1);
      break;
   }
}

void open_file(Editor *t, string f) { // открыть файл и записать в файл
   string line;
   ifstream input(f);
   if (!input.is_open()) exit(1);
   while (getline(input, line)) {
      t->add_line(line);
   }
   input.close();
}

void save_file(Editor t, string f) { // сохранить в файл из класса
   ofstream output(f);
   for (int i = 0; i < t.count_line(); i++) {
      output << t.print_line(i) << endl;
   }
   output.close();
}