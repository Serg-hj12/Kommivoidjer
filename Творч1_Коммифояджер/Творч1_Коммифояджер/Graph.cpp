#include "Graph.h"
using namespace std;
int WinW = 1080;
int WinH = 1080;

bool* Mouse_Vert = new bool[maxSize];
bool standView = false;
bool Vert_Move = false;

vector<pair<int, int>> Path;
vector<int> NewPath;
int R = 50;

int Moving_Vertex;
int CursorX;
int CursorY;

vertCoord vertC[maxSize + 1];
Graph graph;
int Button_Flag;



Graph::Graph()
{
	for (int i = 0; i < maxSize; i++)
	{
		for (int j = 0; j < maxSize; j++)
		{
			adjMatrix[i][j] = 0;
		}
		Mouse_Vert[i] = false;
	}
}

Graph::~Graph()
{ }

int Graph::GetVertPos(const int& vertex)
{
	for (size_t i = 0; i < vertList.size(); i++)
	{
		if (vertList[i] == vertex)
			return i;
	}
	return -1;
}

bool Graph::IsEmpty()
{
	if (vertList.size() != 0)
		return false;
	else
		return true;
}

bool Graph::IsFull()
{
	return (vertList.size() == maxSize);
}

void Graph::InsertVertex(const int& vertex)
{
	if (!IsFull())
		vertList.push_back(vertex);
	else
	{
		cout << "Граф уже заполнен. Невозможно добавить новую вершину " << endl;
		return;
	}
}

int Graph::InsertEdge(const int& vertex1, const int& vertex2, int weight)
{
	if (weight < 1)
	{
		cout << "\nДанная величина веса некорректна\n";
		return -1;
	}
	if (GetVertPos(vertex1) != (-1) && GetVertPos(vertex2) != (-1))						//если вершины есть в графе
	{
		int vertPos1 = GetVertPos(vertex1);												//находим позиции вершин
		int vertPos2 = GetVertPos(vertex2);
		if (adjMatrix[vertPos1][vertPos2] != 0)											//если между ними уже есть ребро
		{
			cout << "Ребро между вершинами уже есть" << endl;
			return -1;
		}
		else																			//иначе добавляем ребро
		{
			adjMatrix[vertPos1][vertPos2] = weight;
		}
	}
	else
	{
		cout << "Обеих вершин (или одной из них) нет в графе " << endl;
		return -1;
	}
	return 0;
}

void Graph::Print()
{
	if (!IsEmpty())
	{
		cout << "Матрица смежности графа: " << endl;
		for (int i = 0; i < vertList.size(); i++)
		{
			cout << vertList[i] << " ";
			for (int j = 0; j < vertList.size(); j++)
				cout << setw(4) << adjMatrix[i][j];
			cout << endl;
		}
	}
	else
		cout << "Граф пуст " << endl;
}


int Graph::GetAmountEdges()
{
	int edges = 0;
	for (int i = 0; i < vertList.size(); i++)
		for (int j = 0; j < vertList.size(); j++)
			if (adjMatrix[i][j] > 0)
				edges++;
	return edges;
}

void Graph::drawGraph()
{
	int n = graph.GetAmountVerts();
	for (int i = 0; i < n; i++)
	{
		if (!standView)
			setCoords(i, n);
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int a = adjMatrix[i][j];
			int b = adjMatrix[j][i];
			if (a != 0 && b == 0)
				drawLine(a, vertC[i].x, vertC[i].y, vertC[j].x, vertC[j].y);
			if (b != 0 && a != 0 && i < j)
				drawLine2(a, b, vertC[j].x, vertC[j].y, vertC[i].x, vertC[i].y);
		}
	}
	drawVertex(n);
	glutPostRedisplay();
}

int** Make_TSPMatrix()
{
	int n = graph.GetAmountVerts();
	int** matrix = new int*[n];
	for (int i = 0; i < n; i++)
		matrix[i] = new int[n];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int elem = graph.getAdjMatrixElem(i, j);
			if (elem == 0 or i == j)
				matrix[i][j] = -1;
			else
				matrix[i][j] = elem;
		}
	}
	cout << "Матрица с расстояниями между вершинами: \n";
	Print_Matrix(matrix);
	return matrix;
}

int* Find_Min(int* line, int n)
{
	int min = 1000000;
	for (int j = 0; j < n; j++)
		if (line[j] >= 0 && line[j] < min)
			min = line[j];
	for (int j = 0; j < n; j++)
		if (line[j] >= 0)
			line[j] -= min;
	return line;
}

void Print_Matrix(int** matrix)
{
	int n = graph.GetAmountVerts();
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << setw(4) << matrix[i][j];
		cout << endl;
	}
}

int** Reduct_Matrix(int** oldmatrix)
{
	int** matrix = oldmatrix;
	int n = graph.GetAmountVerts();
	for (int i = 0; i < n; i++)
		matrix[i] = Find_Min(matrix[i], n);
	for (int i = 0; i < n; i++)
	{
		int min = 1000000;
		for (int j = 0; j < n; j++)
		{
			if (matrix[j][i] >= 0 && matrix[j][i] < min)
				min = matrix[j][i];
		}
		for (int j = 0; j < n; j++)
		{
			if (matrix[j][i] >= 0)
				matrix[j][i] -= min;
		}
	}
	return matrix;
}

int** Rebuild_Matrix(int** oldmatrix)
{
	int n = graph.GetAmountVerts();
	int** matrix = Reduct_Matrix(oldmatrix);
	int max = -1;
	int line, column;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (matrix[i][j] == 0)
			{
				int minLine = 1000000;
				int minColumn = 1000000;
				for (int k = 0; k < n; k++)
				{
					if (matrix[i][k] != -1 && k != j && matrix[i][k] < minLine)
						minLine = matrix[i][k];
				}
				for (int k = 0; k < n; k++)
				{
					if (matrix[k][j] != -1 && k != i && matrix[k][j] < minColumn)
						minColumn = matrix[k][j];
				}
				if (max < minColumn + minLine)
				{
					max = minColumn + minLine;
					line = i;
					column = j;
				}
			}
		}
	}

	pair<int, int> p;
	p.first = line + 1;
	p.second = column + 1;
	Path.push_back(p);

	matrix[line][column] = -1;
	matrix[column][line] = -1;

	for (int i = 0; i < n; i++)
	{
		matrix[line][i] = -1;
		matrix[i][column] = -1;
	}
	cout << endl;
	cout << "\nПромежуточные отрезки путей: ";
	for (int i = 0; i < Path.size(); i++)
		cout << Path[i].first << " -> " << Path[i].second << "; ";
	cout << endl;
	return matrix;
}

void Print_Result()
{
	int second = Path[0].second;
	int i = 2;
	NewPath.push_back(Path[0].first);
	NewPath.push_back(Path[0].second);
	while (i != graph.GetAmountVerts() + 1)
		for (int j = 1; j < graph.GetAmountVerts(); j++)
			if (Path[j].first == second)
			{
				second = Path[j].second;
				NewPath.push_back(second);
				i++;
			}
	cout << "Ответ: ";
	for (int i = 0; i < NewPath.size(); i++)
	{
		cout << NewPath[i];
		if (i != NewPath.size() - 1)
			cout << " -> ";
	}
	int sum = 0;
	for (int i = 0; i < Path.size(); i++)
	{
		int line = Path[i].first - 1;
		int column = Path[i].second - 1;
		sum += graph.getAdjMatrixElem(line, column);
	}
	cout << "\nКратчайшее растояние между вершинами = " << sum << endl;;
}
///
//кнопки
///
void drawButton1()
{
	if (Button_Flag == 1)
		glColor3f(1.0, 0.7, 0.0);
	else
		glColor3f(0.0, 0.3, 1.0);
	glBegin(GL_QUADS);
	glVertex2i(50, WinH - 20);
	glVertex2i(WinW / 7, WinH - 20);
	if (Button_Flag == 1)
		glColor3f(0.9, 0.5, 1.0);
	else
		glColor3f(0.0, 0.7, 1.0);
	glVertex2i(WinW / 7, WinH - WinH / 7);
	glVertex2i(50, WinH - WinH / 7);
	glEnd();
	glColor3f(0.1, 0.1, 0.1);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, WinH - 20);
	glVertex2i(50, WinH - WinH / 7);
	glVertex2i(WinW / 7, WinH - WinH / 7);
	glVertex2i(WinW / 7, WinH - 20);
	glEnd();
	string name = "Make new Graph";
	glRasterPos2i(WinW / 21, 0.92 * WinH);
	for (int i = 0; i < name.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
}

void drawButton2()
{
	if (Button_Flag == 2)
		glColor3f(1.0, 0.7, 0.0);
	else
		glColor3f(0.0, 0.3, 1.0);
	glBegin(GL_QUADS);
	glVertex2i(50, WinH - WinH / 7 - 20);
	glVertex2i(WinW / 7, WinH - WinH / 7 - 20);
	if (Button_Flag == 2)
		glColor3f(0.9, 0.5, 1.0);
	else
		glColor3f(0.0, 0.7, 1.0);
	glVertex2i(WinW / 7, WinH - 2 * (WinH / 7));
	glVertex2i(50, WinH - 2 * (WinH / 7));
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, WinH - WinH / 7 - 20);
	glVertex2i(50, WinH - 2 * (WinH / 7));
	glVertex2i(WinW / 7, WinH - 2 * (WinH / 7));
	glVertex2i(WinW / 7, WinH - WinH / 7 - 20);
	glEnd();
	string name = "Find shortest way";
	glRasterPos2i(WinW / 19, 0.77 * WinH);
	for (int i = 0; i < name.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
}

void drawButton3()
{
	if (Button_Flag == 3)
		glColor3f(1.0, 0.7, 0.0);
	else
		glColor3f(0.0, 0.7, 1.0);
	glBegin(GL_QUADS);
	glVertex2i(50, WinH - 2 * (WinH / 7) - 20);
	glVertex2i(50, WinH - 3 * (WinH / 7));
	if (Button_Flag == 3)
		glColor3f(0.9, 0.5, 1.0);
	else
		glColor3f(0.0, 0.3, 1.0);
	glVertex2i(WinW / 7, WinH - 3 * (WinH / 7));
	glVertex2i(WinW / 7, WinH - 2 * (WinH / 7) - 20);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, WinH - 2 * (WinH / 7) - 20);
	glVertex2i(50, WinH - 3 * (WinH / 7));
	glVertex2i(WinW / 7, WinH - 3 * (WinH / 7));
	glVertex2i(WinW / 7, WinH - 2 * (WinH / 7) - 20);
	glEnd();
	string name = "Output matrix";
	string name1 = "in console";
	glRasterPos2i(WinW / 16.5, 0.63 * WinH);
	for (int i = 0; i < name.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);
	glRasterPos2i(WinW / 16, (0.63 * WinH) - 15);
	for (int i = 0; i < name1.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name1[i]);
}

void drawButton4()
{
	if (Button_Flag == 4)
		glColor3f(1.0, 0.7, 0.0);
	else
		glColor3f(0.0, 0.7, 1.0);
	glBegin(GL_QUADS);
	glVertex2i(50, WinH - 3 * (WinH / 7) - 20);
	glVertex2i(50, WinH - 4 * (WinH / 7));
	if (Button_Flag == 4)
		glColor3f(0.9, 0.5, 1.0);
	else
		glColor3f(0.0, 0.3, 1.0);
	glVertex2i(WinW / 7, WinH - 4 * (WinH / 7));
	glVertex2i(WinW / 7, WinH - 3 * (WinH / 7) - 20);
	glEnd();
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2i(50, WinH - 3 * (WinH / 7) - 20);
	glVertex2i(50, WinH - 4 * (WinH / 7));
	glVertex2i(WinW / 7, WinH - 4 * (WinH / 7));
	glVertex2i(WinW / 7, WinH - 3 * (WinH / 7) - 20);
	glEnd();
	string name = "Clear the console";
	glRasterPos2i(WinW / 16, 0.48 * WinH);
	for (int i = 0; i < name.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, name[i]);

}

void drawCircle(int x, int y, int R)
{
	glColor4f(0.85, 0.75, 0.89, 0.9);
	float x1, y1;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	glColor3f(0.5, 0.5, 0.5);
	for (int i = 0; i < 361; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y1 = R * cos(theta) + y;
		x1 = R * sin(theta) + x;;
		glVertex2f(x1, y1);
	}
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	float x2, y2;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y2 = R * cos(theta) + y;
		x2 = R * sin(theta) + x;
		glVertex2f(x2, y2);
	}
	glEnd();
}

void drawBorderedCircle(int x, int y, int R)
{
	glColor3f(0.3, 0.4, 0.4);
	float x1, y1;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	glColor3f(0.0, 0.9, 0.9);
	for (int i = 0; i < 361; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y1 = R * cos(theta) + y;
		x1 = R * sin(theta) + x;;
		glVertex2f(x1, y1);
	}
	glEnd();

	glColor3f(0.0, 0.4, 0.0);
	float x2, y2;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++)
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(360);
		y2 = R * cos(theta) + y;
		x2 = R * sin(theta) + x;
		glVertex2f(x2, y2);
	}
	glEnd();
}

void drawText(int text, int x1, int y1)
{
	glColor3f(0.0, 0.0, 0.0);
	GLvoid* font = GLUT_BITMAP_HELVETICA_18;
	string s = to_string(text);
	glRasterPos2i(x1 - 5, y1 - 5);
	for (size_t j = 0; j < s.length(); j++)
		glutBitmapCharacter(font, s[j]);
}

void drawLine(int text, int x0, int y0, int x1, int y1) 
{
	glColor3i(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(x0, y0);
	glVertex2i(x1, y1);
	glEnd();
	drawText(text, (x0 + x1) / 2 + 10, (y0 + y1) / 2 + 10);

	float vx = x0 - x1;
	float vy = y0 - y1;
	float s = 1.0f / sqrt(vx * vx + vy * vy);
	vx *= s;
	vy *= s;
	x1 = x1 + R * vx;
	y1 = y1 + R * vy;

	glColor3i(0, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(x1, y1);
	glVertex2f(x1 + 10 * (vx + vy), y1 + 10 * (vy - vx));
	glVertex2f(x1 + 10 * (vx - vy), y1 + 10 * (vy + vx));
	glEnd();
}

void drawLine2(int text1, int text2, int x0, int y0, int x1, int y1)
{
	glColor3i(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2i(x0, y0);
	glVertex2i(x1, y1);
	glEnd();

	float vx = x0 - x1;
	float vy = y0 - y1;
	float s = 1.0f / sqrt(vx * vx + vy * vy);
	vx *= s;
	vy *= s;
	int x = (x0 + x1) / 2;
	int y = (y0 + y1) / 2;

	drawText(text1, x - 16 * (vx - vy), y - 16 * (vy + vx));
	drawText(text2, x + 16 * (vx - vy), y + 16 * (vy + vx));

	glColor3i(0, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x + 10 * (vx + vy), y + 10 * (vy - vx));
	glVertex2f(x + 10 * (vx - vy), y + 10 * (vy + vx));
	glVertex2f(x, y);
	glVertex2f(x - 10 * (vx + vy), y - 10 * (vy - vx));
	glVertex2f(x - 10 * (vx - vy), y - 10 * (vy + vx));
	glEnd();
}

void drawVertex(int n)
{
	for (int i = 0; i < n; i++)
	{
		if (Mouse_Vert[i])
			drawBorderedCircle(vertC[i].x, vertC[i].y, R);
		else
			drawCircle(vertC[i].x, vertC[i].y, R);
		drawText(i + 1, vertC[i].x, vertC[i].y);
	}

}

void setCoords(int i, int n)
{
	int tem, val;
	int x0 = WinW / 2;
	int y0 = WinH / 2;
	if (WinW > WinH)
	{
		val = 5 * (WinH / 13) / n;
		tem = WinH / 2 - val - 10;
	}
	else
	{
		val = 5 * (WinW / 13) / n;
		tem = WinW / 2 - val - 10;
	}
	float theta = 2.0f * 3.1415926f * i / n;
	int y1 = tem * cos(theta) + y0;
	int x1 = tem * sin(theta) + x0;
	vertC[i].x = x1;
	vertC[i].y = y1;
}

void makeGraph()
{
	standView = false;
	int amountVerts, amountEdges, sourceVertex, targetVertex, edgeWeight;
	cout << "Введите количество вершин: "; cin >> amountVerts;
	cout << "Введите количество дорог(ребер) между вершинами: "; cin >> amountEdges;
	for (int i = 1; i <= amountVerts; i++) {

		graph.InsertVertex(i);
	}
	int i = 0;
	while (i < amountEdges)
	{
		cout << "Начальная вершина: "; cin >> sourceVertex;
		cout << "Конечный вершина: "; cin >> targetVertex;
		cout << "Расстояние между вершинами: "; cin >> edgeWeight;
		int a = graph.InsertEdge(sourceVertex, targetVertex, edgeWeight);
		if (a == 0)
			i++;
	}
	cout << endl;
	graph.Print();
}

bool Salesman_Check(int** matrix)
{
	if (graph.IsEmpty())
		return false;
	for (int i = 0; i < graph.GetAmountVerts(); i++)
	{
		int cnt = 0;
		for (int j = 0; j < graph.GetAmountVerts(); j++)
		{
			if (matrix[i][j] > 0 && j != i)
				cnt++;
		}
		if (cnt < 1)
			return false;
	}
	return true;
}

int Circle_Check(int x, int y)
{
	for (int i = 0; i < graph.GetAmountVerts(); i++)
		if (pow(x - vertC[i].x, 2) + pow(y - vertC[i].y, 2) <= pow(R, 2))
			return i;
	return -1;
}

void Button_Check(int x, int y)
{
	if (x > 50 && x < WinW / 7 && y < (WinH - 20) && y >(WinH - WinH / 7))
		Button_Flag = 1;
	else if (x > 50 && x < WinW / 7 && y < (WinH - WinH / 7 - 20) && y > WinH - 2 * (WinH / 7))
		Button_Flag = 2;
	else if (x > 50 && x < WinW / 7 && y < WinH - 2 * (WinH / 7) - 20 && y > WinH - 3 * (WinH / 7))
		Button_Flag = 3;
	else if (x > 50 && x < WinW / 7 && y > WinH - 4 * (WinH / 7) && y < WinH - 3 * (WinH / 7) - 20)
		Button_Flag = 4;
	else if (x > 50 && x < WinW / 7 && y > WinH - 5 * (WinH / 7) && y < WinH - 4 * (WinH / 7) - 20)
		Button_Flag = 5;
	else if (x > 50 && x < WinW / 7 && y > WinH - 6 * (WinH / 7) && y < WinH - 5 * (WinH / 7) - 20)
		Button_Flag = 6;
	else if (x > 50 && x < WinW / 7 && y > 10 && y < WinH - 6 * (WinH / 7) - 20)
		Button_Flag = 7;
	else
		Button_Flag = 0;
}

void mouseMove(int x, int y)
{
	y = WinH - y;
	CursorX = x;
	CursorY = y;
	int i = Circle_Check(x, y);
	if (i != -1)
		Mouse_Vert[i] = true;
	else
		for (int j = 0; j < graph.GetAmountVerts(); j++)
			Mouse_Vert[j] = false;
	if (Vert_Move)
	{
		vertC[Moving_Vertex].x = CursorX;
		vertC[Moving_Vertex].y = CursorY;
	}
	Button_Check(x, y);
	glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y)
{
	int j = Circle_Check(x, WinH - y);
	if (Vert_Move)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			Vert_Move = false;
			return;
		}
	}

	if (j != -1)
	{
		standView = true;
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			Vert_Move = true;
			Moving_Vertex = j;
			return;
		}
	}

	if (button == 3 || button == 4)
	{
		standView = true;
		if (button == 3)
		{
			if (R != 300)
				R++;
		}
		else if (button == 4)
		{
			if (R != 15)
				R--;
		}
	}

	if (x >= 50 and x <= (WinW / 7) and y >= 20 and y <= (WinH / 7))
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			NewPath.clear();
			Path.clear();
			Graph New;
			graph = New;
			makeGraph();
			return;

		}
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
			standView = false;
			return;
		}
	}
	if (x >= 50 and x <= (WinW / 7) and y >= ((WinH / 7) + 20) and y <= 2 * (WinH / 7))
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			NewPath.clear();
			Path.clear();
			cout << "\nЗадача Коммивояжера:\n";
			int** matrix = Make_TSPMatrix();
			bool checker = Salesman_Check(matrix);
			if (!checker)
			{
				cout << "\nЗадача Коммивояжера для данного графа некорректна\n\n";
				return;
			}
			int n = graph.GetAmountVerts();
			while (Path.size() < n)
				matrix = Rebuild_Matrix(matrix);
			cout << endl;
			Print_Result();
			return;
		}
	}
	if (x >= 50 and x <= (WinW / 7) and y >= (2 * (WinH / 7) + 20) and y <= 3 * (WinH / 7))
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			graph.Print();
			return;
		}
	}
	if (x >= 50 && x <= WinW / 7 && y <= 4 * (WinH / 7) && y >= 3 * (WinH / 7) + 20)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			system("cls");
		}

	}
}

void reshape(int w, int h)
{
	WinW = w;
	WinH = h;
	glViewport(0, 0, (GLsizei)WinW, (GLsizei)WinH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, (GLdouble)WinW, 0, (GLdouble)WinH);
	glutPostRedisplay();
}

void display()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WinW, 0, WinH);
	glViewport(0, 0, WinW, WinH);
	glClearColor(0.7, 1.0, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	drawButton1();
	drawButton2();
	drawButton3();
	drawButton4();
	graph.drawGraph();
	glutSwapBuffers();
}