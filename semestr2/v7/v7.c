#include "v7.h"

unsigned CountStudents(const char* filename) {
	int students = 0;
	int buf = (int)'\0';
	FILE* f = fopen(filename, "r");
	if (!f) {
		return 0;
	}
	do {
		buf = fgetc(f);
		if (buf == '\n') {
			++students;
		}
	} while (buf != EOF);
	return students;
}

int Input1(FILE* f, struct Student* s) {
	// Check if file pointer or struct is not null
	if ((!f) || (!s)) {
		return EOF;
	}
	unsigned char buf = 0;
	unsigned spacecounter = 0;
	for (unsigned i = 0; (i < 256) && (spacecounter < 2); ++i) {
		if (fscanf(f, "%c", &buf) != 1) {
			return EOF;
		}	// Checking for END OF FILE
		if (buf == ' ') {
			++spacecounter;
		}
		((*s).Name)[i] = buf;
	}

	if (fscanf(f, "%d", &((*s).Group)) != 1) {
		return EOF;
	}	// Write group to s.Group or return error.

	if (fscanf(f, "%c", &buf) != 1) {
		return EOF;
	}
	else {
		if (buf != ' ') {
			return EOF;
		}
	}	// Checking for valid formatting, pt.1

	if (fscanf(f, "%d", &((*s).School)) != 1) {
		return EOF;
	}	// Write group to s.Group or return error.

	if (fscanf(f, "%c", &buf) != 1) {
		return EOF;
	}
	else {
		if (buf != '\n') {
			return EOF;
		}
		else {
			return 0;
		}
	}	// Checking for valid formatting, pt.2
}

int Output1(FILE* f, struct Student* s) {
	// Check if file pointer or struct is not null
	if ((!f) || (!s)) {
		return EOF;
	}
	if (fprintf(f, "%s%d %d\n", (*s).Name, (*s).Group, (*s).School) == -1) {
		return EOF;
	}
	return 0;
}

int Input(char* filename, struct Student** s, unsigned Ns) {
	if (!s) {
		return EOF;
	}

	FILE* f = fopen(filename, "r");
	if (!f) {
		return EOF;
	}

	for (unsigned i = 0; i < Ns; ++i) {
		if (Input1(f, (*s)+i) == EOF) {
			fclose(f);
			return EOF;
		}
	}
	fclose(f);
	return 0;
}

int Output(char* filename, struct Student** s, unsigned Ns) {
	if (!s) {
		return EOF;
	}

	FILE* f = fopen(filename, "w");
	if (!f) {
		return EOF;
	}

	for (unsigned i = 0; i < Ns; ++i) {
		if (Output1(f, *s + i) == EOF) {
			fclose(f);
			return EOF;
		}
	}

	fclose(f);
	return 0;
}

int SortStudents(struct Student** s, unsigned Ns) {
	struct Student bufi, bufj;
	if ((s == NULL) || (Ns <= 0)) {
		return EOF;
	}
	for (unsigned i = 0; i < Ns - 1; ++i) {
		bufi = *(*s + i);
		for (unsigned j = i + 1; j < Ns; j++) {
			bufj = *(*s + j);
			if ((bufj.School == bufi.School) && (bufj.Group != bufi.Group)) {
				for (unsigned k = i + 1; k < Ns; k++) {
					if ((*(*s + k)).Group == bufi.Group) {
						(*(*s + k)).Group = bufj.Group;
						(*(*s + j)).Group = bufi.Group;
					}
				}
			}
		}
	}
	return 0;
}

int test1(void) {
	struct Student* s = (struct Student*)calloc(3, sizeof(struct Student));
	if (!s) {
		return EOF;
	}
	const char name[4] = { 'a',' ','b',' ' };
	for (unsigned i = 0; i < 4; ++i) {
		(s[0]).Name[i] = name[i];
		(s[1]).Name[i] = name[i];
		(s[2]).Name[i] = name[i];
	}
	s[0].Group = 1; s[0].School = 1;
	s[1].Group = 2; s[1].School = 1;
	s[2].Group = 1; s[2].School = 3;
	int rslt = SortStudents(&s, 3);
	if (s[0].Group != s[1].Group) {
		free(s);
		return EOF;
	}
	free(s);
	return rslt;
}