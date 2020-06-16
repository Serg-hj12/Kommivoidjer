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

const int maxSize = 20;                      //максимальное количество вершин
extern int WinW;
extern int WinH;
struct vertCoord		                     //структура координат вершин
{
	int x, y;
};

class Graph
{
private:
	vector<int> vertList;														 //вектор вершин
	int adjMatrix[maxSize][maxSize];											 //матрица смежности
public:
	Graph();																	 //конструктор 
	~Graph();																	 //деструктор 
	bool IsEmpty();																 //проверка на пустоту
	bool IsFull();																 //проверка на полноту

	int getVertText(int i) { return vertList[i]; }                               //вернуть значение вершины
	vector<int> Shop() { return  vertList; }                                     //вернуть вектор вершин
	int getAdjMatrixElem(int i, int j) { return adjMatrix[i][j]; }               //вернуть элемент матрицы смежности
	void setEdgeZero(int i, int j) { adjMatrix[i][j] = 0; adjMatrix[j][i] = 0; } //занулить элемент матрицы (дл€  оммиво€жера)
	int GetAmountVerts() { return vertList.size(); }                             //вернуть количество вершин графа

	int GetVertPos(const int& vertex);		                                     //вернуть позицию вершины в векторе вершин
	int GetAmountEdges();                                                        //вернуть количество ребер графа
	void InsertVertex(const int& vertex);					                     //
	int InsertEdge(const int& vertex1, const int& vertex2, int weigth);	     //добавить ребро с рассто€нием weight между vertex1 и vertex2
	void Print();		                                                         //вывод матрицы смежности в консоль
	void drawGraph();                                                            //рисоавание графа
};
extern Graph graph;		                                                         //объект класса Graph

int** Make_TSPMatrix();	                                                         //создаетс€ матрица, замен€€ 0 на -1
int* Find_Min(int* line, int n);	                                             //поиск минимального элемента в строке
void Print_Matrix(int** matrix);		                                         //вывод матрицы
int** Reduct_Matrix(int** oldmatrix);		                                     //редукци€ матрицы
int** Rebuild_Matrix(int** oldmatrix);
void Print_Result();                                                             //вывод результата
///
////GL//////////////\
рисование кнопок
///
void drawButton1();
void drawButton2();
void drawButton3();
void drawButton4();
///
//элементы графа
///
void drawCircle(int x, int y, int R);                                            //рисование круга 
void drawBorderedCircle(int x, int y, int R);                                    //рисование круга, при указание на него
void drawText(int nom, int x1, int y1);                                          //рисование ребра с указанием рассто€ни€
void drawLine(int text, int x0, int y0, int x1, int y1);                         //рисование ребер
void drawLine2(int text1, int text2, int x0, int y0, int x1, int y1);
void drawVertex(int n);                                                          //рисование вершины 

void makeGraph();                                                                //создание графа
void setCoords(int i, int n);                                                    //установка координат дл€ вершин графа

bool Salesman_Check(int** matrix);                                               //проверка на возможность решени€ задачч  оммиво€жера
int Circle_Check(int x, int y);                                                  //проверка указани€ курсора на вершину
void Button_Check(int x, int y);                                                 //проверка указани€ курсора на кнопку

void mouseMove(int x, int y);                                                    //отслеживание координат мыши
void mouseClick(int button, int state, int x, int y);                            //регистраци€ нажатий на мышь
void reshape(int w, int h);                                                      //масштабирование
void display();                                                                  //вывод изображени€