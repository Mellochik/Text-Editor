#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <coniow.h>

using namespace std;

class Editor {
   vector <string> text; // ������ ����� �������� �����
   vector <string> highlighted_lines; // ����� ��� ��������� �����
   vector <int> fill_text; // ������ ����� ������ ������ ���� �������� �� ������
   int begin; // ����� ������� ������ ������
   int start; // ����� �������� ������� ������ ������
   int line; // ������� ������ - ������� � 0
   int symbol; // ������� ������ - ������� � 0
   int x1, y1, x2, y2, color_text, color_fill; // ��������� ����
   bool view = true; // ����� ���������/��������������
   void (*fun)(int) = nullptr; //��������� �� �������
public:
   Editor(int x_1, int y_1, int x_2, int y_2, int ct = 0x7, int cf = 0x70); // ����������� x1, y1, x2, y2, 
//���� ������ = 0x7, ���� ���������=0x70
   void edit(); // ��������� ��������������
   bool isView() const; // �������� �� ��������������
   void setView(bool t); // ��������� ���������� �� ��������������
   int count_line(); // ���������� �����
   int number_line() const; // ����� ������� ������ - ��������� � 0
   int number_symbol() const; // ����� �������� ������� � ������
   void move(int r, int c = 0); // ������� �� ������, ������ (r, c=0)
   string print_line(int r = -1) const; // �������� ������ (r=-1) - string, r - �����
//������, ���� -1, �� �������
   string print_highlighted_line(int r) const; // �������� ���������� ������ �� ������
   void replace_line(string s, int r = -1); // �������� ������ (string, r=-1)
   void remove_line(int r = -1); // ������� ������ (r=-1)
   void add_line(string s, int r = -1); // �������� ������ (string, r=-1)
   void add_symbol(string s); // �������� ������� (string) � ������� �������������� �������
   void remove_symbol(int c = 1); // ������� ������� (c=1) ������� � ��������
   void fill_lines(int n1, int n2); // �������� ������ (n1,n2) c n1-� �� n2-�, ���� ��� 
//��������� = -1, ������ ���������
   void choose_button(void (*f)(int)); // ���������� ���������� �������������� 
//������ (void (*f)(int))
   void update_screen(); // ���������� ������
};

void draw_frame(string s); // ��������� ����� ����
void special_key(int k); // ��������� ���������� ������
void open_file(Editor *t, string f); // ���������� �� �����
void save_file(Editor t, string f); // ���������� � ����

#endif