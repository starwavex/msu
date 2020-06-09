#include "v8.h"

/**
* CheckPoints - принимает на вход указатели на две точки.
* Проверяет принадлежность этих двух точек одной и той же компоненте.
* Если так, то возвращает 1, иначе возвращает 0.
* Если один из указателей нулевой, то возвращает EOF.
*/
int CheckPoints(struct SPoint *point1, struct SPoint *point2) {
	if ((!point1) || (!point2)) {
		return EOF;
	}
	int _xabs = abs(point1->x - point2->x);
	int _yabs = abs(point1->y - point2->y);
	if (_xabs >= _yabs) {
		return (_xabs <= 1);
	}
	else {
		return (_yabs <= 1);
	}
}

/**
* CreateList - принимает на вход адрес указателя на массив точек, а также имя файла с набором точек.
* Вычисляет количество точек в файле.
* Заполняет массив этим набором точек.
* В память под входным указателем записывает указатель на начало этого массива.
* Возвращает количество точек (натуральное число) в случае успеха.
* При ошибке возвращает 0.
*/
unsigned CreateList(struct SPoint** _points, const char* _filename) {
	
	FILE* f = fopen(_filename, "r");
	if (!f) {
		return 0;
	}

	unsigned card = 0;
	unsigned char buf = 0;
	while (fscanf(f, "%c", &buf) == 1) {
		if (buf == '\n') {
			++card;
		}
	}
	
	if (card == 0) {
		fclose(f);
		return 0;
	}
	

	struct SPoint* points = (struct SPoint*)malloc(card * sizeof(struct SPoint));
	if (!points) {
		fclose(f);
		return 0;
	}

	rewind(f);
	for (unsigned i = 0; i < card; ++i) {
		if (fscanf(f, "%d", &(points + i)->x) != 1) {
			fclose(f);
			return 0;
		}
		if (fgetc(f) != ' ') {
			fclose(f);
			return 0;
		}
		if (fscanf(f, "%d", &(points + i)->y) != 1) {
			fclose(f);
			return 0;
		}
		if (fgetc(f) != '\n') {
			fclose(f);
			return 0;
		}
		(points + i)->pointComponent = NULL;
	}
	fclose(f);

	*_points = points;
	return card;
}

/**
* SaveList - принимает на вход адрес компоненты, кол-во компонент, а также имя файла.
* Заполняет файл всеми компонентами.
* Возвращает 0 в случае успеха.
* При ошибке возвращает EOF.
*/
int SaveList(struct SComponent* _component, unsigned _card, const char* _filename) {
	if (!_component || !_filename) {
		return EOF;
	}
	FILE* f = fopen(_filename, "w");
	if (!f) {
		return EOF;
	}

	for (unsigned i = 0; i < _card; ++i) {
		for (unsigned j = 0; j < _component->numberOfPoints; ++j) {
			fprintf(f, "%d %d\n", (_component->points)[j]->x, (_component->points)[j]->y);
		}
		fprintf(f, "\n");
		_component = _component->nextComponent;
	}
	return 0;
}

/**
* CheckList - принимает на вход массив точек и кол-во точек.
* Проверяет, не повторяется ли какая-либо точка.
* Возвращает 0 в случае отсутствия повторов.
* Иначе возвращает EOF.
*/
int CheckList(struct SPoint* _points, unsigned _card) {
	if (!_points) {
		return EOF;
	}
	for (unsigned i = 0; i < _card; ++i) {
		for (unsigned j = i + 1; j < _card; ++j) {
			if ((_points + i)->x == (_points + j)->x && (_points + i)->y == (_points + j)->y) {
				return EOF;
			}
		}
	}
	return 0;
}

/**
* PrintList - принимает на вход адрес компоненты и кол-во компонент.
* Печатает все компоненты в окно консоли.
*/
void PrintList(struct SComponent* _component, unsigned _card) {
	if (!_component || _card == 0) {
		return;
	}
	for (unsigned i = 0; i < _card; ++i) {
		for (unsigned j = 0; j < _component->numberOfPoints; ++j) {
			printf("%d %d\n", (_component->points)[j]->x, (_component->points)[j]->y);
		}
		printf("\n");
		_component = _component->nextComponent;
	}
	return;
}

/**
* LinkPoints - принимает на вход массив точек и число точек.
* Создаёт одну компоненту по критерию связности.
* Возвращает 0 в случае успеха, иначе - ненулевое число.
*/
int LinkPoints(struct SPoint* _points, unsigned _card) {
	if (!_points || _card == 0) {
		return EOF;
	}
	struct SPoint* origin = NULL;
	for (unsigned i = 0; i < _card; ++i) {
		if ((_points + i)->pointComponent == NULL) {
			origin = _points + i;
			i = _card;
		}
	}
	if (!origin) {
		return 1;
	}

	struct SComponent* component = (struct SComponent*)malloc(sizeof(struct SComponent));
	if (!component) {
		return EOF;
	}
	component->numberOfPoints = 1;
	origin->pointComponent = component;
	
	int criteria = EOF;
	for (unsigned i = 0; i < _card; ++i) {
		if ((_points + i)->pointComponent == component) {
			for (unsigned j = 0; j < _card; ++j) {
				criteria = CheckPoints(_points + i, _points + j);
				if (criteria == EOF) {
					free(component);
					return EOF;
				}
				if (criteria == 1 && (_points + j)->pointComponent != component) {
					(_points + j)->pointComponent = component;
					component->numberOfPoints += 1;
				}
			}
			
		}
	}
	if (component->numberOfPoints == 0) {
		free(component);
		return EOF;
	}

	struct SPoint** cpoints = (struct SPoint**)malloc(component->numberOfPoints * sizeof(struct SPoint*));
	if (!cpoints) {
		free(component);
		return EOF;
	}
	for (unsigned i = 0, j = 0; i < _card; ++i) {
		if ((_points + i)->pointComponent == component) {
			*(cpoints + j) = _points + i;
			++j;
		}
	}
	component->points = cpoints;
	component->nextComponent = component;
	return 0;
}

/**
* LinkComponents - принимает на вход массив точек и число точек.
* Объединяет компоненты в циклический список.
* Возвращает натуральное число компонент, в случае ошибки возвращает 0.
*/
unsigned LinkComponents(struct SPoint* _points, unsigned _card) {
	if (!_points || _card == 0) {
		return 0;
	}
	unsigned numberOfComponents = 0;
	int linked = 0;
	while (linked == 0) {
		linked = LinkPoints(_points, _card);
		if (linked == 0) {
			++numberOfComponents;
		}
	}
	if (linked == EOF || numberOfComponents == 0) {
		return 0;
	}
	struct SComponent** index = (struct SComponent**)calloc(numberOfComponents, sizeof(struct SComponent));
	if (!index) {
		return 0;
	}

	index[0] = _points->pointComponent;
	for (unsigned i = 0, j = 1; i < _card && j < numberOfComponents; ++i) {
		linked = 1;
		for (unsigned k = 0; k < numberOfComponents && index[k] != NULL; ++k) {
			if ((_points + i)->pointComponent == index[k]) {
				linked = 0;
			}
		}
		if (linked) {
			index[j] = (_points + i)->pointComponent;
			index[j - 1]->nextComponent = index[j];
			++j;
		}
	}
	index[numberOfComponents - 1]->nextComponent = index[0];
	free(index);
	return numberOfComponents;
}

/**
* GetPoint - без входных параметров.
* Предлагает ввести точку с клавиатуры.
* Возвращает адрес точки в случае успеха, иначе - NULL.
*/
struct SPoint* GetPoint(void) {
	struct SPoint* point = (struct SPoint*)malloc(sizeof(struct SPoint));
	if (!point) {
		return NULL;
	}
	fprintf(stdout, "Enter new point coords via space (x y): ");
	if (scanf("%d %d", &(point->x), &(point->y)) != 2) {
		free(point);
		return NULL;
	}
	point->pointComponent = NULL;
	return point;
}

/**
* GetPoints - принимает на вход адрес для записи кол-ва новых точек.
* Заполняет новыми точками массив.
* Возвращает массив указателей в случае успеха. Иначе - NULL.
*/
struct SPoint** GetPoints(unsigned* _card) {
	if (!_card) {
		return NULL;
	}
	fprintf(stdout, "Enter the number of points you want to add: ");
	if (fscanf(stdin, "%u", _card) != 1) {
		return NULL;
	}
	struct SPoint** newps = (struct SPoint**)calloc(*_card, sizeof(struct SPoint*));
	if (!newps) {
		return NULL;
	}
	for (unsigned i = 0; i < *_card; ++i) {
		fflush(stdin);
		newps[i] = GetPoint();
		if (!newps[i]) {
			
			*_card = 0;
			FreeNewps(newps, i);
			return NULL;
		}
	}
	return newps;
}

/**
* FreeNewps - освобождает массив новых точек и указателей на них.
*/
void FreeNewps(struct SPoint** _newps, unsigned _card) {
	if (!_newps) {
		return;
	}
	for (unsigned i = 0; i < _card; ++i) {
		free(_newps[i]);
	}
	free(_newps);
	return;
}

/**
* LookPoint - принимает на вход адрес компоненты, кол-во компонент и адрес точки.
* Проверяет, есть ли эта точка среди всех компонент.
* Возвращает 0 в случае отсутствия, 1 в случае обнаружения, EOF в случае ошибки.
*/
int LookPoint(struct SComponent* _component, unsigned _card, struct SPoint* _point) {
	if (!_component || !_point) {
		return EOF;
	}
	for (unsigned i = 0; i < _card; ++i) {
		for (unsigned j = 0; j < _component->numberOfPoints; ++j) {
			if ((_component->points)[j]->x == _point->x && (_component->points)[j]->y == _point->y) {
				return 1;
			}
		}
		_component = _component->nextComponent;
	}
	return 0;
}

/**
* AddPoint - принимает на вход адрес компоненты, кол-во компонент и адрес точки.
* Создаёт новую компоненту, состоящую из этой точки.
* При наличии слипающихся компонент объединяет их в одну.
* Слипающихся с этой точкой компонент может быть не более 4 (по геометрическим соображениям).
* Возвращает адрес новой компоненты в случае успеха, NULL в случае ошибки.
*/
struct SComponent* AddPoint(struct SComponent* _component, unsigned* _card, struct SPoint* _point) {
	if (!_component || !_point || !_card) {
		return NULL;
	}
	struct SComponent* linked[4] = { NULL, NULL, NULL, NULL };
	struct SComponent* origin = _component;
	for (unsigned k = 0, i = 0; i < 4 && k < *_card; _component = _component->nextComponent) {
		for (unsigned j = 0; j < _component->numberOfPoints; ++j) {
			if (CheckPoints(_point, (_component->points)[j]) == 1) {
				linked[i] = _component;
				j = _component->numberOfPoints;
				++i;
			}
		}
		++k;
	}
	++(*_card);
	unsigned new_numberOfPoints = 1;
	for (unsigned i = 0; i < 4; ++i) {
		if (linked[i] != NULL) {
			--(*_card);
			new_numberOfPoints += linked[i]->numberOfPoints;
		}
	}

	struct SComponent* new_component = (struct SComponent*)malloc(sizeof(struct SComponent));
	if (!new_component) {
		return NULL;
	}
	new_component->numberOfPoints = new_numberOfPoints;
	_point->pointComponent = new_component;
	if (new_numberOfPoints == 1) {
		struct SPoint** new_points = (struct SPoint**)malloc(sizeof(struct SPoint*));
		if (!new_points) {
			return NULL;
		}
		*new_points = _point;
		new_component->points = new_points;
		
		new_component->nextComponent = _component->nextComponent;
		_component->nextComponent = new_component;
		return new_component;
	}
	else {
		struct SPoint** new_points = (struct SPoint**)malloc(new_numberOfPoints * sizeof(struct SPoint*));
		new_points[0] = _point;
		_component = linked[0];
		for (unsigned i = 1, j = 0, k = 0; i < new_numberOfPoints && k < 4; ++i) {
			if (_component->numberOfPoints <= j) {
				j = 0;
				++k;
				_component = linked[k];
			}
			new_points[i] = (_component->points)[j];
			new_points[i]->pointComponent = _component;
			++j;
		}
		_component = origin;
		for (unsigned i = 0; /*i < *_card && */ i < 4 && linked[i] != NULL; _component = _component->nextComponent) {
			if (_component->nextComponent == linked[i]) {
				_component->nextComponent = linked[i]->nextComponent;
				++i;
			}
		}
		new_component->points = new_points;
		new_component->nextComponent = _component->nextComponent;
		_component->nextComponent = new_component;
		return new_component;
	}
}

/**
* AddPoints - принимает на вход адрес компоненты, кол-во компонент, массив адресов новых точек, кол-во новых точек.
* Добавляет каждую новую точку.
* Возвращает адрес компоненты в случае успеха, NULL в случае ошибки.
*/
struct SComponent* AddPoints(struct SComponent* _component, unsigned* _card, struct SPoint** _newps, unsigned _N_newps) {
	if (!_component || !_card || !_newps) {
		return NULL;
	}
	for (unsigned i = 0; i < _N_newps; ++i) {
		if (LookPoint(_component, *_card, _newps[i]) == 0) {
			_newps[i]->pointComponent = AddPoint(_component, _card, _newps[i]);
			if (_newps[i]->pointComponent == NULL) {
				FreeNewps(_newps, _N_newps);
				return NULL;
			}
			_component = _newps[i]->pointComponent;
		}
		if (!_component) {
			FreeNewps(_newps, _N_newps);
			return NULL;
		}
	}
	return _component;
}

/**
* FreeComponents - принимает на вход адрес компоненты и кол-во компонент.
* Уничтожает компоненты в памяти (точнее - наборы указателей на точки).
* Возвращает натуральное число удалённых компонент, 0 в случае ошибки.
*/
unsigned FreeComponents(struct SComponent* _component, unsigned _card) {
	if (!_component) {
		return 0;
	}
	struct SComponent* buf = _component;
	for (unsigned i = 0; i < _card; ++i) {
		free(buf->points);
		buf = _component->nextComponent;
		free(_component);
		_component = buf;
	}
	return _card;
}

/**
* Autotest - автотест функций на пяти точках, которые должны слипнуться в одну область.
* Возвращает 0 в случае успеха, EOF в случае ошибки.
* Печатает имя функции, в которой возникла ошибка.
*/
int Autotest(void) {
	unsigned card = 5;
	struct SPoint* points = (struct SPoint*)malloc(card * sizeof(struct SPoint));
	if (!points) {
		fprintf(stderr, "MEMORY TEST ERROR!\n");
		return EOF;
	}

	points[0].x = 1,  points[0].y = 1;
	points[1].x = -1, points[1].y = 1;
	points[2].x = -1, points[2].y = -1;
	points[3].x = 1,  points[3].y = -1;
	points[4].x = 1,  points[4].y = 1;
	
	if (!CheckList(points, card)) {
		free(points);
		fprintf(stderr, "CheckList() TEST FAILED!\n");
		return EOF;
	}
	points[4].x = 0, points[4].y = 0;
	for (unsigned i = 0; i < 5; ++i) {
		points[i].pointComponent = NULL;
	}

	card = LinkComponents(points, card);
	if (card != 1 || points->pointComponent == NULL || (points+4)->pointComponent->numberOfPoints != 5) {
		free(points);
		fprintf(stderr, "LinkComponents() TEST 1 FAILED!\n");
		return EOF;
	}
	card = FreeComponents(points->pointComponent, card);
	if (card != 1) {
		free(points);
		fprintf(stderr, "FreeComponents() TEST 1 FAILED!\n");
		return EOF;
	}
	for (unsigned i = 0; i < 5; ++i) {
		points[i].pointComponent = NULL;
	}
	card = 4;
	card = LinkComponents(points, card);
	if (card != 4) {
		free(points);
		fprintf(stderr, "LinkComponents() TEST 2 FAILED!\n");
		return EOF;
	}
	struct SComponent* component = points->pointComponent;
	if (LookPoint(component, card, points) != 1 || LookPoint(component, card, points + 4) != 0) {
		free(points);
		fprintf(stderr, "LookPoint() TEST FAILED!\n");
		return EOF;
	}
	component = AddPoint(component, &card, points + 4);
	if (!component || card != 1) {
		free(points);
		fprintf(stderr, "AddPoint() TEST FAILED!\n");
		return EOF;
	}
	card = FreeComponents(component, card);
	if (!card) {
		free(points);
		fprintf(stderr, "FreeComponents() TEST 2 FAILED!\n");
		return EOF;
	}
	free(points);
	return 0;
}