#pragma once
#include <stdio.h>
#include <stdlib.h>

struct Student {
	char Name[256];
	int Group;
	int School;
};

unsigned CountStudents(const char* filename);
/*
Counts the number of strings in 'filename'
Returns this number as unsigned integer.
*/

int SortStudents(struct Student** s, unsigned Ns);
/*
For student [i] looks for student [j > i] with the same school, but not with the same group.
Then, looks for student [k > i] with the group of student [i].
If found, swaps the groups of student [k] and student [j].
Returns 0 if success, EOF in case of any error.
*/

int Input1(FILE* f, struct Student* s);
int Output1(FILE* f, struct Student* s);
int Input(char* filename, struct Student** s, unsigned Ns);
int Output(char* filename, struct Student** s, unsigned Ns);
int test1(void);