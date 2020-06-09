#include "v8.h"

int main(void) {
	struct SPoint* points, ** newps;
	unsigned numberOfPoints, numberOfComponents, N_newp;

	if (Autotest()) {
		return EOF;
	}
	else {
		fprintf(stdout, "SELF-TEST OK!\n");
	}

	numberOfPoints = CreateList(&points, "points.txt");
	if (!numberOfPoints) {
		fprintf(stderr, "Error: invalid file!\n");
		return EOF;
	}
	if (CheckList(points, numberOfPoints)) {
		free(points);
		fprintf(stderr, "Error: point is repeated in your file!\n");
		return EOF;
	}

	numberOfComponents = LinkComponents(points, numberOfPoints);
	if (!numberOfComponents) {
		fprintf(stderr, "Error linking components!\n");
		free(points);
		return EOF;
	}

	struct SComponent* component = points->pointComponent;
	if (!component) {
		fprintf(stderr, "Very strange error...\n");
		free(points);
		return EOF;
	}

	PrintList(component, numberOfComponents);	// optional.
	
	newps = GetPoints(&N_newp);
	if (!newps) {
		fprintf(stderr, "Error: your input is invalid!\n");
		free(points);
		return EOF;
	}

	component = AddPoints(component, &numberOfComponents, newps, N_newp);
	if (!component) {
		fprintf(stderr, "Error adding the new point!\n");
		numberOfComponents = FreeComponents(component, numberOfComponents);
		free(points);
		return EOF;
	}
	fprintf(stdout, "%u point(s) added successfully!\n-----------------\n", N_newp);
	PrintList(component, numberOfComponents);	// optional.

	if (SaveList(component, numberOfComponents, "out.txt")) {
		fprintf(stderr, "Error saving the list!\n");
		FreeNewps(newps, N_newp);
		free(points);
		return EOF;
	}

	if (numberOfComponents != FreeComponents(component, numberOfComponents)) {
		fprintf(stderr, "Error freeing memory!\n");
		FreeNewps(newps, N_newp);
		free(points);
		return EOF;
	}
	FreeNewps(newps, N_newp);
	free(points);
	return 0;
}