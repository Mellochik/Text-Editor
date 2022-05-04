#include "text_editor.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <coniow.h>

using namespace std;

Editor::Editor(int x_1, int y_1, int x_2, int y_2, int ct, int cf) { 
   // ����������� x1, y1, x2, y2, ���� ������ = 0x7, ���� ���������=0x70
   begin = 1; // ������� ������ ������
   start = 1; // ������� ������ ������
   line = 0; // ������� ������
   symbol = 1; // ������� ������
   x1 = x_1; // ���������� ������� ����
   y1 = y_1;
   x2 = x_2;
   y2 = y_2;
   color_text = ct; // ���� ������
   color_fill = cf; // ���� ���������
}

void Editor::edit() { // ��������� ��������������
   int k; // ����� �������
   string s; // ����������� �����
   char c; // ����������� ������

   if (text.size() > y2 - y1 + 1) // ������������ ������� ������� ������
      begin = text.size() - y2 + y1;

   line = count_line(); // ������������� ������ � ����� ������

   if (text[line - 1].size() > x2 - x1 + 1) // ������������ ������� ������� ������
      start = text[line - 1].size() - x2 + x1 + 1;

   symbol = text[line - 1].size() + 1; // ������������� ������ � ����� ������

   choose_button(special_key); // �������� ������� ��� �������� ����������� ������
   while (1) { // �������� ���� ��������������
      if (symbol > text[line - 1].size() + 1) // ����������� ��� ������� �� ������
         symbol = text[line - 1].size() + 1;
      if (symbol < start)
         start = text[line - 1].size() - x2 + x1 + 1;
      if (start < 1)
         start = 1;

      update_screen(); // ���������� ������
      gotoxy(symbol - start + 1, line - begin + 1); // ����������� �������

      s = ""; 
      k = getkbm();
      c = (char) k;
      s += c;
      if (32 <= k && k <= 255) { // ������� ������
         if (!isView()) {
            if (symbol == start + x2 - x1)
               start++;
            add_symbol(s);
         }
         continue;
      }
      switch (k) {
      case KEY_LEFT + KEY_SPECIAL: // ������� "�����"
         if (symbol > start) // ����������� ������� �����
            symbol--;
         else if (start > 1) {
            start--;
            symbol--;
         }
         break;
      case KEY_UP + KEY_SPECIAL: // ������� "�����"
         if (line != begin) // ����������� ������� �����
            line--;
         else if (begin > 1) {
            begin--;
            line--;
         }
         break;
      case KEY_RIGHT + KEY_SPECIAL: // ������� "������"
         if (symbol == start + x2 - x1) // ����������� ������� ������
            start++;
         symbol++;
         break;
      case KEY_DOWN + KEY_SPECIAL: // ������� "����"
         if (line != begin + y2 - y1 && line != count_line()) // ����������� ������� ����
            line++;
         else if (line == begin + y2 - y1 && line != count_line()) {
            begin++;
            line++;
         }
         break;
      case KEY_BACKSPACE: // ������� "BACKSPACE"
         if (!isView()) { // ���� ��� ��������������
            if (symbol > 1) { // ���� ������� � ������ ���� ������� ������
               symbol--;
               remove_symbol();
            }
            else if (line != 1) { // ���� ������� ����������� - �������� ������ � ��������� �� � ������ �����
               string temp = "";
               if (text[line - 1].size() > 0) {
                  for (int i = symbol - 1; i < text[line - 1].size(); i++)
                     temp += text[line - 1][i];
               }
               if (line == begin)
                  begin--;
               remove_line(line - 1); // �������� ������
               symbol = text[line - 1].size() + 1;
               add_symbol(temp); // ���������� ������ ����
               symbol = text[line - 1].size() + 1;
            }
         }
         break;
      case KEY_DELETE + KEY_SPECIAL: // ������� "DELETE"
         if (!isView())
            if (text[line - 1].size() != 0)
               remove_symbol();
         break;
      case KEY_PGUP + KEY_SPECIAL: // ������� "PAGEUP"
         begin = 1;
         start = 1;
         line = 1;
         symbol = 1;
         break;
      case KEY_PGDN + KEY_SPECIAL: // ������� "PAGEDWN"
         begin = text.size() - y2 + y1; // �������� ������ ������
         if (begin < 1)
            begin = 1;
         line = text.size();
         symbol = text[line - 1].size() + 1;
         if (text[line - 1].size() > x2 - x1 + 1)
            start = text[line - 1].size() - x2 + x1 + 1;
         break;
      case KEY_HOME + KEY_SPECIAL: // ������� "HOME"
         start = 1;
         symbol = 1;
         break;
      case KEY_END + KEY_SPECIAL: // ������� "END"
         symbol = text[line - 1].size();
         break;
      case KEY_ENTER: // ������� "ENTER"
         if (!isView()) {
            if (symbol <= text[line - 1].size()) { // ���� ������ ���������� ���-�� � ������ ������������ ������� ������ �������
               string temp = "";
               for (int i = symbol - 1; i < text[line - 1].size(); i++) {
                  temp += text[line - 1][i];
               }
               text[line - 1].erase(symbol - 1, text[line - 1].size() - symbol + 1);
               if (line == begin + y2 - y1)
                  begin++;
               add_line(temp, line);
            }
            else { // ���� ������ ���������� � ����� ������ ��������� ������ ������
               if (line == begin + y2 - y1)
                  begin++;
               add_line("");
               symbol = 1;
            }
         }
         break;
      default:
         if (fun != nullptr) // ��������� ����������� ������
            fun(k);
      }
   };
}


bool Editor::isView() const { // �������� �� ��������������
   return view;
}

void Editor::setView(bool t) { // ��������� ���������� �� ��������������
   view = t;
}

int Editor::count_line() { // ���������� �����
   return text.size();
}

int Editor::number_line() const { // ����� ������� ������ - ��������� � 0
   return line;
}

int Editor::number_symbol() const { // ����� �������� ������� � ������
   return symbol;
}

void Editor::move(int r, int c) { // ������� �� ������, ������ (r, c=0)
   if (r > y2 - y1) // ����������� ����������� �������
      start = r - y2 + y1;
   line = r;
   if (text[line - 1].size() + 1< c)
      symbol = text[line - 1].size() + 1;
   else
      symbol = c;
   if (symbol > x2 - x1)
      start = symbol - x2 + x1;
}

string Editor::print_line(int r) const { // �������� ������ (r=-1) - string, r - ����� ������, ���� -1, �� �������
   if (r == -1)
      return text[line - 1];
   return text[r];
}

string Editor::print_highlighted_line(int r) const { // �������� ���������� ������ �� ������
   return highlighted_lines[r];
}

void Editor::replace_line(string s, int r) { // �������� ������ (string, r=-1)
   if (r != -1)
      text[r] = s;
   else
      text[line - 1] = s;
}

void Editor::remove_line(int r) { // ������� ������ (r=-1)
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

void Editor::add_line(string s, int r) { // �������� ������ (string, r=-1)
   if (r == -1) {
      r = line;
   }
   vector<string>::iterator it = text.begin()+r;
   text.insert(it, s);
   if (r <= line)
      line++;
   fill_text.resize(line);
}

void Editor::add_symbol(string s) { // �������� ������� (string) � ������� �������������� �������
   text[line - 1].insert(symbol - 1, s);
   symbol++;
}

void Editor::remove_symbol(int c) { // ������� ������� (c=1) ������� � ��������
   text[line - 1].erase(symbol - 1, c);
}

void Editor::fill_lines(int n1, int n2) { // �������� ������ (n1,n2) c n1-� �� n2-�, ���� ��� ��������� = -1, ������ ���������
   if (n1 == -1 && n2 == -1) { // �������� ����� ���������� ����� � �������� �����
      fill_text.clear();
      fill_text.resize(count_line());
      highlighted_lines.clear();
   }
   else
      for (int i = n1; i <= n2; i++) { // ��������� ����� � n1-������ �� n2-������
         fill_text[i - 1] = 1;
         highlighted_lines.push_back(print_line(i - 1));
      }
}

void Editor::choose_button(void (*f)(int)) { // ���������� ���������� �������������� ������ (void (*f)(int))
   fun = f;
}

void Editor::update_screen() { // ��������� ������
   window(x1, y1, x2, y2); // ����������� ������� ������
   clrscr(); // ������� ������
   for (int i = begin - 1; i < y2 - y1 + begin; i++) { // ����� �� ����� ������
      for (int j = start - 1; j < text[i].size(); j++) {
         if (j > x2 - y1 + start - 1) continue;
         gotoxy(j - start + 2, i - begin + 2);
         if (fill_text[i] == 1) // �������� ������
            textattr(color_fill);
         else // ������������ ������� ������
            textattr(color_text);
         putch(text[i][j]);
      }
   }
}

void draw_frame(string s) { // ��������� �����
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