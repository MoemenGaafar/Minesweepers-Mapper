#include <iostream>
#include <sstream>
#include <cmath>
#include "CMUgraphicsLib/CMUgraphics.h"
#include <fstream>
using namespace std;

const int NUMBER_OF_ROWS = 19;
const int NUMBER_OF_COLUMNS = 19;
const int CELL_LENGTH = 25;
const int SIDELENGTH = 19 * 25;
const int TABLE_LENGTH = 200;
string lightBlue = "images\\LightBlue.jpg", darkBlue = "images\\DarkBlue.jpg";

enum MineType {
	PLACEHOLDER,
	SURFACE,
	BURIED
};

void DrawMap(window* pWind, int** map) {
	//Draws the map using the data in the 2D array 
	//Legend: White is free, light blue is surface mine, darke blue is buried mine

	//Painting each coordinate with the proper color
	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
			switch (map[i][j]) {
			case 0: break;
			case 1: pWind->DrawImage(lightBlue, j * CELL_LENGTH, i * CELL_LENGTH, CELL_LENGTH, CELL_LENGTH); break;
			case 2:pWind->DrawImage(darkBlue, j * CELL_LENGTH, i * CELL_LENGTH, CELL_LENGTH, CELL_LENGTH); break;
			}
		}
	}

	//Drawing the borders of the map
	pWind->SetPen(BLUE, 3);
	pWind->DrawLine(0, 0, 0, SIDELENGTH);
	pWind->DrawLine(0, 0, SIDELENGTH, 0);
	pWind->DrawLine(0, SIDELENGTH, SIDELENGTH, SIDELENGTH);
	pWind->DrawLine(SIDELENGTH, 0, SIDELENGTH, SIDELENGTH);

	//Dicing the map into the proper number of coordinates
	pWind->SetPen(BLACK, 1);
	for (int i = 1; i < NUMBER_OF_ROWS; i++) {
		pWind->DrawLine(0, i * CELL_LENGTH, SIDELENGTH, i * CELL_LENGTH);
	}
	for (int j = 1; j < NUMBER_OF_COLUMNS; j++) {
		pWind->DrawLine(j * CELL_LENGTH, 0, j * CELL_LENGTH, SIDELENGTH);
	}

	//Printing the X and Y axes gradings
	string xCoordinate, YCoordinate;
	for (int i = 0; i < NUMBER_OF_ROWS; i++) {
		xCoordinate = (char)(i + 65);
		pWind->DrawString((i + 0.5)*CELL_LENGTH - 2, SIDELENGTH + 5,xCoordinate);
	}
	for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
		YCoordinate = to_string(j + 1);
		pWind->DrawString(SIDELENGTH + 5, SIDELENGTH - (j + 0.5) * CELL_LENGTH - 2, YCoordinate);
	}
}


void DrawTable(window* pWind, int** map, int tableNum, int numRows) {
	//Draws a table next to the map. tableNum specifies the order of the table, enumarates starting from 1. numRows is its number of rows.

	pWind->SetPen(BLUE, 3);
	int startX = SIDELENGTH + 4 * CELL_LENGTH + (tableNum - 1)*(TABLE_LENGTH + 2*CELL_LENGTH);
	pWind->DrawLine(startX, 0, startX + TABLE_LENGTH,0);
	pWind->DrawLine(startX, CELL_LENGTH, startX + TABLE_LENGTH, CELL_LENGTH);
	pWind->DrawLine(startX, 0, startX, (numRows+1)*CELL_LENGTH);
	pWind->DrawLine(startX + TABLE_LENGTH, 0, startX + TABLE_LENGTH, (numRows + 1) * CELL_LENGTH);
	pWind->DrawLine(startX, (numRows + 1) * CELL_LENGTH, startX + TABLE_LENGTH, (numRows + 1) * CELL_LENGTH);
	pWind->DrawLine(startX + TABLE_LENGTH / 2, CELL_LENGTH, startX + TABLE_LENGTH / 2, (numRows + 1) * CELL_LENGTH);

	pWind->SetPen(BLUE, 1);
	for (int i = 1; i <= numRows; i++) {
		pWind->DrawLine(startX, i*CELL_LENGTH, startX + TABLE_LENGTH, i * CELL_LENGTH);
	}
}

void printMines(window* pWind, int** map) {
	//prints the coordinates of the mines on the interface

	int rowTable1 = 0, rowTable2 = 0, startX = 0;
	string xCoordinate, yCoordinate;
	pWind->SetPen(BLACK);
	for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
	   xCoordinate = (char)(j + 65);
	   for (int i = 0; i < NUMBER_OF_ROWS; i++) {
	    	yCoordinate = to_string(NUMBER_OF_ROWS - i);
			switch(map[i][j]) {
			case 0: continue;
			case 1: {
				startX = SIDELENGTH + 4 * CELL_LENGTH;
				pWind->DrawString(startX + TABLE_LENGTH / 8, (rowTable1 + 1) * CELL_LENGTH + CELL_LENGTH / 4, xCoordinate);
				pWind->DrawString(startX + 5 * TABLE_LENGTH / 8, (rowTable1 + 1) * CELL_LENGTH + CELL_LENGTH / 4, yCoordinate);
				rowTable1++;
				break;
			}
			case 2: { 
				startX = SIDELENGTH + 6 * CELL_LENGTH + TABLE_LENGTH;
				pWind->DrawString(startX + TABLE_LENGTH / 8, (rowTable2 + 1) * CELL_LENGTH + CELL_LENGTH / 4, xCoordinate);
				pWind->DrawString(startX + 5 * TABLE_LENGTH / 8, (rowTable2 + 1) * CELL_LENGTH + CELL_LENGTH / 4, yCoordinate);
				rowTable2++;
				break;
			}
			}
		}
	}
}

int main()
{
	    //Create window
      	window* pWind = new window(3 * SIDELENGTH - 100, 1.5*SIDELENGTH, 0, 0);
		pWind->ChangeTitle("LE MAP");

		//Declaration of the 2D array that contains the mine data
		int** map = new int* [NUMBER_OF_ROWS];
		for (int i = 0; i < NUMBER_OF_ROWS; i++) {
			map[i] = new int[NUMBER_OF_COLUMNS];
		}

		//Initializing the array with data from an external text file
		ifstream loadedMap;
		int numSurface = 0, numBuried = 0;
		loadedMap.open("MapInfo.txt");
		if (loadedMap) {
			for (int i = 0; i < NUMBER_OF_ROWS; i++) {
				for (int j = 0; j < NUMBER_OF_COLUMNS; j++) {
					loadedMap >> map[i][j];
					switch (map[i][j]) {
					case 1: numSurface++; break;
					case 2: numBuried++; break;
					}
				}
			}
		}
		else {
			cout << "Error opening MapInfo!";
		}

		//Drawing the map using the loaded data
		DrawMap(pWind, map);
		

		//Printing the mine locations in tables
		DrawTable(pWind, map, 1, numSurface);
		DrawTable(pWind, map, 2, numBuried);
		pWind->SetFont(15, BOLD, BY_NAME, "Arial");
		pWind->SetPen(WHITE);
		pWind->DrawImage(lightBlue, SIDELENGTH + 100, 0, TABLE_LENGTH, CELL_LENGTH);
		pWind->DrawImage(darkBlue, SIDELENGTH + 150 + TABLE_LENGTH, 0, TABLE_LENGTH, CELL_LENGTH); 
		pWind->DrawString(SIDELENGTH + 125, CELL_LENGTH / 4, "Surface Mines found at:");
		pWind->DrawString(SIDELENGTH + TABLE_LENGTH + 175, CELL_LENGTH / 4, "Buried Mines found at:");
		printMines(pWind, map);

		delete pWind;
	}

		



