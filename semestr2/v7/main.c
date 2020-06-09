#include "v7.h"

int main(void) {
	if (test1()) {
		printf("Test FAIL.\n");
		return EOF;
	}
	else {
		printf("Test PASSED.\n");
	}

	int rslt = 0;
	unsigned Ns = CountStudents("students.txt");
	struct Student* s = (struct Student*)calloc(Ns,sizeof(struct Student));
	if (!s) {
		return EOF;
	}
	
	rslt = Input("students.txt", &s, Ns);
	if (rslt) {
		printf("Error reading file!\n");
		free(s);
		return -1;
	}

	SortStudents(&s, Ns);

	rslt = Output("out.txt", &s, Ns);
	free(s);

	return rslt;
}