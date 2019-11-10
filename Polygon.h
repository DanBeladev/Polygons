#ifndef POLYGON_H
#define POLYGON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<math.h>
#define BYTE 8

#define ADD_POLYGON 0
#define PRINT_POLYGON 1
#define PRINT_PERIMETER 2
#define PRINT_DIAGONALS 3
#define PRINT_AREA 4


enum POLYGON { HEXAGON = 6, HEPTAGON = 7, OCTAGON = 8 };

typedef struct vertices {
	char x;
	char y;
}vertices;

typedef struct PolygonList
{
	struct Node* head;
	struct Node* tail;
}List;

typedef struct polygon {
	enum POLYGON type;
	struct vertices* vertices;
	// choose your representation for the vertices
}polygon;

typedef struct Node {
	struct polygon* poly;
	struct Node* next;
}Node;List* polygons;void add_polygon( polygon* polygonToAdd);
// add new polygon to the list

void print_polygon( polygon* poltygonToPrint);
// print the type of polygon and its vertices

void print_perimeter( polygon* polygonToPrint);
// calculate and print the perimeter

void print_diagonals( polygon* polygonToPrint);
// calculate and print the length of all diagonals

void print_area( polygon* polygonToPrint);
// calculate and print the area
void Run();

enum POLYGON GetPolygonType(long long  command);

char* GetEnumTypeInText(polygon* poly);

void HandleCommand(long long  command);

polygon* createNewPolygon(long long  command);

void PrintPolygons(char forWho);

void AllocateVertices( polygon * newPolygon);

void initList();

void GetVertices( polygon* poly);

char findForWhoToDoAction(long long  command);

int ToWriteTypeAndVertices(long long  command);

void handleActions(long long  command, char forWho);

int IsNewPolygon(long long  command);

int IsFirstPolygon(List* ls);

bool IsLastCommand(long long  command);

bool ToCalculateAndPrintPerimeter(long long  command);

bool ToCalculateAndPrintArea(long long  command);

void printDetails(polygon* polygon, long long  command);

double getDistance(vertices p1, vertices p2);

bool ToCalculateAndPrintSumOfDiagnosals(long long  command);

void FreeMemory();

#endif // !POLYGON_H

