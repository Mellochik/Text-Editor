#include "text_editor.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <coniow.h>

using namespace std;

Editor::Editor(int x_1, int y_1, int x_2, int y_2, int ct, int cf) { 
   // конструктор x1, y1, x2, y2, цвет текста = 0x7, цвет выделения=0x70
   begin = 1; // верхняя строка экрана
   start = 1; // крайний символ строки
   line = 0; // текущая строка
   symbol = 1; // текущий символ
   x1 = x_1; // координаты отриски окна
   y1 = y_1;
   x2 = x_2;
   y2 = y_2;
   color_text = ct; // цвет текста
   color_fill = cf; // цвет выделения
}

void Editor::edit() { // выполнять редактирование
   int k; // номер клавиши
   string s; // техническая трока
   char c; // технический символ

   if (text.size() > y2 - y1 + 1) // расчитывание области отриски экрана
      begin = text.size() - y2 + y1;

   line = count_line(); // устанавилавет курсор в конец текста

   if (text[line - 1].size() > x2 - x1 + 1) // расчитывание области отриски экрана
      start = text[line - 1].size() - x2 + x1 + 1;

   symbol = text[line - 1].size() + 1; // устанавливает курсор в конец строки

   choose_button(special_key); // получает функцию для обратоки специальных клавиш
   while (1) { // основной цикл редактирования
      if (symbol > text[line - 1].size() + 1) // ограничения для курсора на экране
         symbol = text[line - 1].size() + 1;
      if (symbol < start)
         start = text[line - 1].size() - x2 + x1 + 1;
      if (start < 1)
         start = 1;

      update_screen(); // обновление экрана
      gotoxy(symbol - start + 1, line - begin + 1); // перемещение курсора

      s = ""; 
      k = getkbm();
      c = (char) k;
      s += c;
      if (32 <= k && k <= 255) { // клавиши текста
         if (!isView()) {
            if (symbol == start + x2 - x1)
               start++;
            add_symbol(s);
         }
         continue;
      }
      switch (k) {
      case KEY_LEFT + KEY_SPECIAL: // клавиша "ВЛЕВО"
         if (symbol > start) // перемещение курсора влево
            symbol--;
         else if (start > 1) {
            start--;
            symbol--;
         }
         break;
      case KEY_UP + KEY_SPECIAL: // клавиша "ВВЕРХ"
         if (line != begin) // перемещение курсора вверх
            line--;
         else if (begin > 1) {
            begin--;
            line--;
         }
         break;
      case KEY_RIGHT + KEY_SPECIAL: // клавиша "ВПРАВО"
         if (symbol == start + x2 - x1) // перемещение курсора вправо
            start++;
         symbol++;
         break;
      case KEY_DOWN + KEY_SPECIAL: // клавиша "ВНИЗ"
         if (line != begin + y2 - y1 && line != count_line()) // перемещение курсора вниз
            line++;
         else if (line == begin + y2 - y1 && line != count_line()) {
            begin++;
            line++;
         }
         break;
      case KEY_BACKSPACE: // клавиша "BACKSPACE"
         if (!isView()) { // флаг для редактирования
            if (symbol > 1) { // если символы в строке есть удаляет символ
               symbol--;
               remove_symbol();
            }
            else if (line != 1) { // если символы закончились - копирует строку и вставляет ее в строку ввыше
               string temp = "";
               if (text[line - 1].size() > 0) {
                  for (int i = symbol - 1; i < text[line - 1].size(); i++)
                     temp += text[line - 1][i];
               }
               if (line == begin)
                  begin--;
               remove_line(line - 1); // удаление строки
               symbol = text[line - 1].size() + 1;
               add_symbol(temp); // добавление строки ниже
               symbol = text[line - 1].size() + 1;
            }
         }
         break;
      case KEY_DELETE + KEY_SPECIAL: // клавиша "DELETE"
         if (!isView())
            if (text[line - 1].size() != 0)
               remove_symbol();
         break;
      case KEY_PGUP + KEY_SPECIAL: // клавиша "PAGEUP"
         begin = 1;
         start = 1;
         line = 1;
         symbol = 1;
         break;
      case KEY_PGDN + KEY_SPECIAL: // клавиша "PAGEDWN"
         begin = text.size() - y2 + y1; // обратока границ экрана
         if (begin < 1)
            begin = 1;
         line = text.size();
         symbol = text[line - 1].size() + 1;
         if (text[line - 1].size() > x2 - x1 + 1)
            start = text[line - 1].size() - x2 + x1 + 1;
         break;
      case KEY_HOME + KEY_SPECIAL: // клавиша "HOME"
         start = 1;
         symbol = 1;
         break;
      case KEY_END + KEY_SPECIAL: // клавиша "END"
         symbol = text[line - 1].size();
         break;
      case KEY_ENTER: // клавиша "ENTER"
         if (!isView()) {
            if (symbol <= text[line - 1].size()) { // если курсор находиться где-то в строке переносяться символы правее курсора
               string temp = "";
               for (int i = symbol - 1; i < text[line - 1].size(); i++) {
                  temp += text[line - 1][i];
               }
               text[line - 1].erase(symbol - 1, text[line - 1].size() - symbol + 1);
               if (line == begin + y2 - y1)
                  begin++;
               add_line(temp, line);
            }
            else { // если символ находиться в конце строки создается пустая строка
               if (line == begin + y2 - y1)
                  begin++;
               add_line("");
               symbol = 1;
            }
         }
         break;
      default:
         if (fun != nullptr) // обработка специальных клавиш
            fun(k);
      }
   };
}


bool Editor::isView() const { // проверка на редактирование
   return view;
}

void Editor::setView(bool t) { // получение разрешения на редактирование
   view = t;
}

int Editor::count_line() { // количество строк
   return text.size();
}

int Editor::number_line() const { // номер текущей строки - нумерация с 0
   return line;
}

int Editor::number_symbol() const { // номер текущего символа в строке
   return symbol;
}

void Editor::move(int r, int c) { // перейти на строку, символ (r, c=0)
   if (r > y2 - y1) // ограничение перемещения курсора
      start = r - y2 + y1;
   line = r;
   if (text[line - 1].size() + 1< c)
      symbol = text[line - 1].size() + 1;
   else
      symbol = c;
   if (symbol > x2 - x1)
      start = symbol - x2 + x1;
}

string Editor::print_line(int r) const { // получить строку (r=-1) - string, r - номер строки, если -1, то текущую
   if (r == -1)
      return text[line - 1];
   return text[r];
}

string Editor::print_highlighted_line(int r) const { // получить выделенную строку из буфера
   return highlighted_lines[r];
}

void Editor::replace_line(string s, int r) { // заменить строку (string, r=-1)
   if (r != -1)
      text[r] = s;
   else
      text[line - 1] = s;
}

void Editor::remove_line(int r) { // удалить строку (r=-1)
   vector<string>::iterator it;
   if (r != -1) {
      it = text.begin() + r;
   }
   else {
      it = text.begin() + line - 1;
      r = line - 1;
   }
   text.erase(it);
   line--;
}

void Editor::add_line(string s, int r) { // добавить строку (string, r=-1)
   if (r == -1) {
      r = line;
   }
   vector<string>::iterator it = text.begin()+r;
   text.insert(it, s);
   if (r <= line)
      line++;
   fill_text.resize(line);
}

void Editor::add_symbol(string s) { // добавить символы (string) в текущее местоположение курсора
   text[line - 1].insert(symbol - 1, s);
   symbol++;
}

void Editor::remove_symbol(int c) { // удалить символы (c=1) начиная с текущего
   text[line - 1].erase(symbol - 1, c);
}

void Editor::fill_lines(int n1, int n2) { // выделить строки (n1,n2) c n1-й по n2-ю, если оба аргумента = -1, убрать выделение
   if (n1 == -1 && n2 == -1) { // очистить буфер выделенных строк и выленных строк
      fill_text.clear();
      fill_text.resize(count_line());
      highlighted_lines.clear();
   }
   else
      for (int i = n1; i <= n2; i++) { // выделение строк с n1-строки по n2-строку
         fill_text[i - 1] = 1;
         highlighted_lines.push_back(print_line(i - 1));
      }
}

void Editor::choose_button(void (*f)(int)) { // установить обработчик нераспознанных клавиш (void (*f)(int))
   fun = f;
}

void Editor::update_screen() { // обработка экрана
   window(x1, y1, x2, y2); // ограничение области экрана
   clrscr(); // очистка экрана
   for (int i = begin - 1; i < y2 - y1 + begin; i++) { // вывод на экран текста
      for (int j = start - 1; j < text[i].size(); j++) {
         if (j > x2 - y1 + start - 1) continue;
         gotoxy(j - start + 2, i - begin + 2);
         if (fill_text[i] == 1) // выделяет строку
            textattr(color_fill);
         else // отрисовывает обычным цветом
            textattr(color_text);
         putch(text[i][j]);
      }
   }
}

void draw_frame(string s) { // отрисовка рамки
   clrscr();
   textattr(BLUE * 16 + WHITE);
   for (int i = 1; i < 27; i++) {
      for (int j = 1; j < 121; j++) {
         gotoxy(j, i);
         putwch(32);
      }
   }
   for (int i = 2; i < 120; i++) {
      gotoxy(i, 1);
      putwch(0x2550);
   }
   for (int i = 2; i < 27; i++) {
      gotoxy(1, i);
      putwch(0x2551);
   }
   for (int i = 2; i < 120; i++) {
      gotoxy(i, 27);
      putwch(0x2550);
   }
   for (int i = 2; i < 27; i++) {
      gotoxy(120, i);
      putwch(0x2551);
   }
   gotoxy(1, 1);
   putwch(0x2554);
   gotoxy(120, 1);
   putwch(0x2557);
   gotoxy(1, 27);
   putwch(0x255A);
   gotoxy(120, 27);
   putwch(0x255D);
   textattr(BLUE * 16 + WHITE);
   gotoxy(2, 1);
   cputs(s.c_str());

   textattr(DARKGRAY * 16 + WHITE);
   gotoxy(2, 29);
   string line;
   ifstream input("bar.txt");
   if (!input.is_open()) exit(1);
   while (getline(input, line)) {
      input >> line;
      cputs(line.c_str());
   }
   input.close();
}