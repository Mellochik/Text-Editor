#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <coniow.h>

using namespace std;

class Editor {
   vector <string> text; // вектор строк хранящий текст
   vector <string> highlighted_lines; // буфер для выделеных строк
   vector <int> fill_text; // вектор какие строки должны быть выделены на экране
   int begin; // номер верхней строки экрана
   int start; // номер крайнего символа строки экрана
   int line; // текущая строка - начиная с 0
   int symbol; // текущий символ - начиная с 0
   int x1, y1, x2, y2, color_text, color_fill; // параметры окна
   bool view = true; // режим просмотра/редактирования
   void (*fun)(int) = nullptr; //указатель на функцию
public:
   Editor(int x_1, int y_1, int x_2, int y_2, int ct = 0x7, int cf = 0x70); // конструктор x1, y1, x2, y2, 
//цвет текста = 0x7, цвет выделения=0x70
   void edit(); // выполнять редактирование
   bool isView() const; // проверка на редактирование
   void setView(bool t); // получение разрешения на редактирование
   int count_line(); // количество строк
   int number_line() const; // номер текущей строки - нумерация с 0
   int number_symbol() const; // номер текущего символа в строке
   void move(int r, int c = 0); // перейти на строку, символ (r, c=0)
   string print_line(int r = -1) const; // получить строку (r=-1) - string, r - номер
//строки, если -1, то текущую
   string print_highlighted_line(int r) const; // получить выделенную строку из буфера
   void replace_line(string s, int r = -1); // заменить строку (string, r=-1)
   void remove_line(int r = -1); // удалить строку (r=-1)
   void add_line(string s, int r = -1); // добавить строку (string, r=-1)
   void add_symbol(string s); // добавить символы (string) в текущее местоположение курсора
   void remove_symbol(int c = 1); // удалить символы (c=1) начиная с текущего
   void fill_lines(int n1, int n2); // выделить строки (n1,n2) c n1-й по n2-ю, если оба 
//аргумента = -1, убрать выделение
   void choose_button(void (*f)(int)); // установить обработчик нераспознанных 
//клавиш (void (*f)(int))
   void update_screen(); // обновление экрана
};

void draw_frame(string s); // рисование рамки окна
void special_key(int k); // обработка специльных клавиш
void open_file(Editor *t, string f); // считывание из файла
void save_file(Editor t, string f); // сохранение в файл

#endif