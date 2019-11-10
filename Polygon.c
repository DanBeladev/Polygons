#define _CRT_SECURE_NO_WARNINGS
#include "Polygon.h"

void(*polygon_methods[])(polygon*) = { add_polygon , print_polygon ,print_perimeter, print_diagonals, print_area };

void add_polygon( polygon * polygonToAdd)
{
	struct Node* node = malloc(sizeof(struct Node));
	node->next = NULL;
	node->poly = polygonToAdd;

	if (IsFirstPolygon(polygons)) {
		polygons->head = polygons->tail = node;
	}
	else
	{
		polygons->tail->next = node;
		polygons->tail = node;
	}
}

void print_polygon(polygon * polygonToPrint)
{
	int i;
	printf("%s ", GetEnumTypeInText(polygonToPrint));
	for (i = 0; i < polygonToPrint->type; i++) {
		printf("{%d, %d} ", polygonToPrint->vertices[i].x, polygonToPrint->vertices[i].y);
	}
	printf("\n");
}

void print_perimeter( polygon * polygonToPrint)
{
	int i;
	double perimeter = 0.0;
	int len = polygonToPrint->type;
	for (i= 0; i < len; i++) {
		perimeter += getDistance(polygonToPrint->vertices[i], polygonToPrint->vertices[(i + 1) % len]);
	}
	printf("perimeter = %.1f\n", perimeter);
}

void print_diagonals( polygon * polygonToPrint)
{
	int i,j;
	vertices* verts = polygonToPrint->vertices;
	double diagonal = 0;
	int numOfVertices = polygonToPrint->type;
	for ( i= 2; i < numOfVertices - 1; i++)
	{
		double distance = getDistance(verts[0], verts[i]);
		diagonal += distance;
	}

	for ( i = 1; i < numOfVertices - 2; i++)
	{
		for (j = i + 2; j < numOfVertices; j++)
		{
			double distance = getDistance(verts[i], verts[j]);
			diagonal += distance;
		}
	}
	printf("diagonals = %.1f\n", diagonal);
}

void print_area( polygon * polygonToPrint)
{
	int i;
	double area = 0.0;
	int numOfVertices = polygonToPrint->type;
	int j = numOfVertices - 1;
	for (i = 0; i < numOfVertices; i++)
	{
		double xj = (double)polygonToPrint->vertices[j].x;
		double xi = (double)polygonToPrint->vertices[i].x;
		double yj = (double)polygonToPrint->vertices[j].y;
		double yi = (double)polygonToPrint->vertices[i].y;

		area += (double)((xj + xi) * (yj - yi));
		j = i;
	}
	area = abs((int)area);
	area /= 2.0;
	printf("area = %.1f\n", area);
}

void Run()
{
	initList();
	bool lastCommand = false;
	long long int command;
	while (!lastCommand)
	{
		scanf("%llx", &command);

		HandleCommand(command);

		lastCommand = IsLastCommand(command);
	}
}

double getDistance(vertices p1, vertices p2) {
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

bool IsLastCommand(long long  command) {
	return (command & 0x0000000000000001) == 1;
}

void HandleCommand(long long  command)
{
	if (IsNewPolygon(command))
	{
		polygon* newPolygon = createNewPolygon(command);
		GetVertices(newPolygon);
		(*polygon_methods[ADD_POLYGON])(newPolygon);
	}
	char forWhoToDoAction = findForWhoToDoAction(command);
	handleActions(command, forWhoToDoAction);


}

polygon* createNewPolygon(long long  command) {
	polygon* newPolygon;
	newPolygon = malloc(sizeof(polygon));
	newPolygon->type = GetPolygonType(command);
	AllocateVertices(newPolygon);
	return newPolygon;

}


void GetVertices( polygon* poly) {
	int i, j;
	long long int first;
	long long int second;
	scanf("%llx", &first);
	scanf("%llx", &second);
	char coordinateX;
	char coordinateY;
	//all the first number is coordinates 
	for (i= 0; i < BYTE / 2; i++) {
		coordinateX = first & 0x00000000000000ff;
		coordinateY = (first >> (BYTE)) & 0x00000000000000ff;
		poly->vertices[i].x = coordinateX;
		poly->vertices[i].y = coordinateY;
		first = first >> BYTE * 2;
	}
	//the next two coordinates
	for ( j = 0; j < BYTE / 4; j++) {
		coordinateX = second & 0x00000000000000ff;
		coordinateY = (second >> BYTE) & 0x00000000000000ff;
		poly->vertices[(BYTE / 2) + j].x = coordinateX;
		poly->vertices[(BYTE / 2) + j].y = coordinateY;
		second = second >> BYTE * 2;
	}

	if (poly->type != HEXAGON) {
		coordinateX = second & 0x00000000000000ff;
		coordinateY = (second >> BYTE) & 0x00000000000000ff;
		poly->vertices[BYTE - 2].x = coordinateX;
		poly->vertices[BYTE - 2].y = coordinateY;
		second = second >> BYTE * 2;
		if (poly->type != HEPTAGON) {
			coordinateX = second & 0x00000000000000ff;
			coordinateY = (second >> BYTE) & 0x00000000000000ff;
			poly->vertices[BYTE - 1].x = coordinateX;
			poly->vertices[BYTE - 1].y = coordinateY;
			second = second >> BYTE * 2;
		}

	}

}

char findForWhoToDoAction(long long  command) {
	long long int mask = 0x000000000000000f;

	//char t = (mask & (command >> 10));
	return (char)(mask & (command >> 10));
}

void handleActions(long long   command, char forWho) {

	Node* current = polygons->head;
	if (forWho != 0) {
		while (current != NULL)
		{
			if (current->poly->type == forWho || forWho == 0x0f) {
				printDetails(current->poly, command);
			}
			current = current->next;
		}
	}
	else
	{
		printDetails(polygons->tail->poly, command);
	}


}

void printDetails(polygon* polygon, long long  command) {

	if (ToWriteTypeAndVertices(command)) {
		(*polygon_methods[PRINT_POLYGON])(polygon);
		//PrintPolygons(forWho);
	}
	if (ToCalculateAndPrintPerimeter(command)) {
		(*polygon_methods[PRINT_PERIMETER])(polygon);
	}
	if (ToCalculateAndPrintArea(command)) {
		(*polygon_methods[PRINT_AREA])(polygon);
	}
	if (ToCalculateAndPrintSumOfDiagnosals(command)) {
		(*polygon_methods[PRINT_DIAGONALS])(polygon);
	}
}

int IsNewPolygon(long long  command) {
	return (command >> 1) & 1;
}

int ToWriteTypeAndVertices(long long  command) {
	return (command >> 6) & 0x00000001;
}

void PrintPolygons(char forWho) {
	if (forWho == 0) {
		(*polygon_methods[PRINT_POLYGON])(polygons->tail->poly);
		//print_polygon(polygons->tail->poly);
	}
	else {
		struct Node* current = polygons->head;
		while (current != NULL) {
			if (current->poly->type == forWho) {
				(*polygon_methods[PRINT_POLYGON])(current->poly);
				//print_polygon(current);
			}
			current = current->next;
		}
	}
}

enum POLYGON GetPolygonType(long long  command) {
	long long int mask = 0x000000000000000f;
	char result = (char)mask & (command >> 2);
	enum POLYGON type = result;
	return type;
}

char* GetEnumTypeInText(polygon* poly) {
	switch (poly->type)
	{
	case HEXAGON:return "hexagon";
	case HEPTAGON:return "heptagon";
	case OCTAGON: return "octagon";
	default:
		return "problem with reading type";
		break;
	}
}

void AllocateVertices(struct polygon* newPolygon) {
	if (newPolygon->type == HEXAGON) {
		newPolygon->vertices = malloc(sizeof(struct vertices)*HEXAGON);
	}
	else if (newPolygon->type == HEPTAGON) {
		newPolygon->vertices = malloc(sizeof(struct vertices)*HEPTAGON);
	}
	else if (newPolygon->type == OCTAGON) {
		newPolygon->vertices = malloc(sizeof(struct vertices)*OCTAGON);
	}
}

int IsFirstPolygon(List* ls) {
	return (ls->tail == NULL && ls->head == NULL);
}

void initList() {
	polygons = (List*)malloc(sizeof(List));
	polygons->head = polygons->tail = NULL;
}

bool ToCalculateAndPrintPerimeter(long long  command)
{
	return ((command >> 7) & 1) == 1;
}


bool ToCalculateAndPrintArea(long long  command)
{
	return ((command >> 8) & 1) == 1;
}

bool ToCalculateAndPrintSumOfDiagnosals(long long  command) {
	return ((command >> 9) & 1) == 1;
}

void FreeMemory() {
	Node* head = polygons->head;
	Node* tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		free(tmp->poly->vertices);
		free(tmp->poly);
		free(tmp);
	}

	free(polygons);
}