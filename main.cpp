#include <text_editor.hpp>
#include <cstring>
#include <iostream>

using namespace std;

Editor text(2, 2, 119, 26, 0x1F, 0xF0); // ����� ��� ��������������
Editor help(5, 3, 116, 25, 0x5F); // ���� ������ � ������� ��������� ���������
string file; // ��� �����

int l1 = -1, l2 = -1; // ������ � ������ ������ ���������
int F5flag = -1; // ���� ��� �������� ������� ������� F5
int F6flag = -1; // ���� ��� �������� ������� ������� F6

int main(int argc, char *argv[]) {
   setlocale(LC_ALL,""); // ������� ���� ��� �������
   file = argv[2]; // ��������� ��� ����� �� ��������� ������

   // ���������� �� ��������������/��������
   if (strcmp(argv[1], "edit") == 0)
      text.setView(false);

   //������ �� �����
   open_file(&text, file);
   open_file(&help, "help.txt");

   // ��������� ����
   draw_frame(file);

   // �������������� �����
   text.edit();

   return 0;
}

void special_key(int k) { // ������� ��� �������� ����������� ������
   switch (k) {
   case KEY_F1 + KEY_SPECIAL: // ����� ������
      help.update_screen(); // ����� �� ����� ������
      getch();
      break;
   case KEY_F2 + KEY_SPECIAL: // ���������� � ����
      save_file(text, file); // ��������� � ����
      break;
   case KEY_F3 + KEY_SPECIAL: // ������ ���������
      l1 = text.number_line(); // �������� ����� ������ ������
      break;
   case KEY_F4 + KEY_SPECIAL: // ��������� ���������
      l2 = text.number_line(); // �������� ����� ������ ������
      break;
   case KEY_F5 + KEY_SPECIAL: // �������� � ����������� �����
      if (F5flag == -1 && l1 != -1 && l2 != -1) { // �������� �� ������ �������
         if (l1 > l2) { // ������ ������� ������ �����, ���� ������� ���� ����� �����
            int t = l2;
            l2 = l1;
            l1 = t;
         }
         F5flag *= -1; // ��������� �����
      }
      else {
         for (int i = 0; i <= l2 - l1; i++) {
            text.add_line(text.print_highlighted_line(i)); // �������� ����� �� ������
         }
         l1 = -1; // ������ ���������
         l2 = -1;
         F5flag *= -1; // ��������� �����
      }
      text.fill_lines(l1, l2); // �������� ��� ������� ���������
      break;
   case KEY_F6 + KEY_SPECIAL: // �������� � ����������� �����
      if (F6flag == -1 && l1 != -1 && l2 != -1) {
         if (l1 > l2) { // ������ ������� ������ �����, ���� ������� ���� ����� �����
            int t = l2;
            l2 = l1;
            l1 = t;
         }
         F6flag *= -1; // ��������� �����
      }
      else {
         for (int i = l1; i <= l2; i++)
            text.remove_line(l1 - 1); // �������� �����
         l1 = -1;
         l2 = -1;
         F6flag *= -1; // ��������� �����
      }
      text.fill_lines(l1, l2); // �������� ��� ������� ���������
      break;
   case KEY_F10 + KEY_SPECIAL: // ������� ���������
      save_file(text, file); // ��������� � ����
      exit(1);
      break;
   }
}

void open_file(Editor *t, string f) { // ������� ���� � �������� � ����
   string line;
   ifstream input(f);
   if (!input.is_open()) exit(1);
   while (getline(input, line)) {
      t->add_line(line);
   }
   input.close();
}

void save_file(Editor t, string f) { // ��������� � ���� �� ������
   ofstream output(f);
   for (int i = 0; i < t.count_line(); i++) {
      output << t.print_line(i) << endl;
   }
   output.close();
}