#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct SPoint {
	int x;
	int y;
	struct SComponent* pointComponent;
};
struct SComponent {
	struct SComponent* nextComponent;
	unsigned numberOfPoints;
	struct SPoint** points;
};

unsigned CreateList(struct SPoint** _points, const char* _filename);
int SaveList(struct SComponent* _component, unsigned _card, const char* _filename);
int CheckList(struct SPoint* _points, unsigned _card);
void PrintList(struct SComponent* _component, unsigned _card);

int CheckPoints(struct SPoint* point1, struct SPoint* point2);
int LinkPoints(struct SPoint* _points, unsigned _card);
unsigned LinkComponents(struct SPoint* _points, unsigned _card);
unsigned FreeComponents(struct SComponent* _component, unsigned _card);

int LookPoint(struct SComponent* _component, unsigned _card, struct SPoint* _point);
struct SPoint* GetPoint(void);
struct SPoint** GetPoints(unsigned* _card);
void FreeNewps(struct SPoint** _newps, unsigned _card);
struct SComponent* AddPoint(struct SComponent* _component, unsigned* _card, struct SPoint* _point);
struct SComponent* AddPoints(struct SComponent* _component, unsigned* _card, struct SPoint** _newps, unsigned _N_newps);

int Autotest(void);
