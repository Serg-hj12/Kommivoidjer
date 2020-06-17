#pragma once
#include <glut.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <cmath>
using namespace std;

const int maxSize = 20;                      //������������ ���������� ������
extern int WinW;
extern int WinH;
struct vertCoord		                     //��������� ��������� ������
{
	int x, y;
};

class Graph
{
private:
	vector<int> vertList;														 //������ ������
	int adjMatrix[maxSize][maxSize];											 //������� ���������
public:
	Graph();																	 //����������� 
	~Graph();																	 //���������� 
	bool IsEmpty();																 //�������� �� �������
	bool IsFull();																 //�������� �� �������
	bool isFullMatrix();

	int getVertText(int i) { return vertList[i]; }                               //������� �������� �������
	vector<int> Shop() { return  vertList; }                                     //������� ������ ������
	int getAdjMatrixElem(int i, int j) { return adjMatrix[i][j]; }               //������� ������� ������� ���������
	void setEdgeZero(int i, int j) { adjMatrix[i][j] = 0; adjMatrix[j][i] = 0; } //�������� ������� ������� (��� ������������)
	int GetAmountVerts() { return vertList.size(); }                             //������� ���������� ������ �����

	int GetVertPos(const int& vertex);		                                     //������� ������� ������� � ������� ������
	int GetAmountEdges();                                                        //������� ���������� ����� �����
	void InsertVertex(const int& vertex);					                     //�������� �������
	void InsertVertex();
	int InsertEdge(const int& vertex1, const int& vertex2, int weigth);			 //�������� ����� � ����������� weight ����� vertex1 � vertex2
	void Print();		                                                         //����� ������� ��������� � �������
	void drawGraph();                                                            //���������� �����
};
extern Graph graph;		                                                         //������ ������ Graph

int** Make_TSPMatrix();	                                                         //��������� �������, ������� 0 �� -1
int* Find_Min(int* line, int n);	                                             //����� ������������ �������� � ������
void Print_Matrix(int** matrix);		                                         //����� �������
int** Reduct_Matrix(int** oldmatrix);		                                     //�������� �������
int** Rebuild_Matrix(int** oldmatrix);
void Print_Result();                                                             //����� ����������
///
////GL//////////////\
��������� ������
///
void drawButton1();
void drawButton2();
void drawButton3();
void drawButton4();
void drawButton5();
void drawButton6();
///
//�������� �����
///
void drawCircle(int x, int y, int R);                                            //��������� ����� 
void drawBorderedCircle(int x, int y, int R);                                    //��������� �����, ��� �������� �� ����
void drawText(int nom, int x1, int y1);                                          //��������� ����� � ��������� ����������
void drawLine(int text, int x0, int y0, int x1, int y1);                         //��������� �����
void drawLine2(int text1, int text2, int x0, int y0, int x1, int y1);
void drawVertex(int n);                                                          //��������� ������� 

void makeGraph();                                                                //�������� �����
void InsertEdge();																 //���������� �����
void setCoords(int i, int n);                                                    //��������� ��������� ��� ������ �����

bool Salesman_Check(int** matrix);                                               //�������� �� ����������� ������� ������ ������������
int Circle_Check(int x, int y);                                                  //�������� �������� ������� �� �������
void Button_Check(int x, int y);                                                 //�������� �������� ������� �� ������

void mouseMove(int x, int y);                                                    //������������ ��������� ����
void mouseClick(int button, int state, int x, int y);                            //����������� ������� �� ����
void reshape(int w, int h);                                                      //���������������
void display();                                                                  //����� �����������