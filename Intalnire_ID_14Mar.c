#include<stdio.h>
#include<malloc.h>

typedef struct Test Test;
typedef struct Nod Nod;

struct Test
{
	char* materie;
	int nrStudenti;
	float medie;
};

struct Nod {
	Test info;
	Nod* next;
};

Test initTest(const char* materie, int nrStudenti,
	float medie) {
	Test t;
	t.nrStudenti = nrStudenti;
	t.medie = medie;
	t.materie = (char*)malloc(sizeof(char)
		* (strlen(materie) + 1));
	strcpy(t.materie, materie);

	return t;
}

void afisareTest(Test t) {
	printf("\nLa testul de la materia %s au participat %d studenti, iar media finala a fost %.2f!",
		t.materie, t.nrStudenti, t.medie);
}

void push(Nod** varf, Test t) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = *varf;
	nou->info = t;
	*varf = nou;
}

Test pop(Nod** varf) {
	if ((*varf) == NULL) {
		return initTest(NULL, 0, 0);
	}
	Test t;
	t = (*varf)->info;
	Nod* aux = *varf;
	*varf = (*varf)->next;
	free(aux);

	return t;
}

void put(Nod** coada, Test t) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = t;

	if (*coada == NULL) {
		*coada = nou;
	}
	else
	{
		Nod* aux = *coada;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}

int main() {
	Nod* stiva = NULL;
	push(&stiva, initTest("SDD", 60, 2.5));
	push(&stiva, initTest("POO", 75, 6.5));
	push(&stiva, initTest("Java", 48, 5.5));

	Test t;
	printf("\nTraversare stiva:");
	while (stiva != NULL) {
		t = pop(&stiva);
		afisareTest(t);
		free(t.materie);
	}

	Nod* coada = NULL;
	put(&coada, initTest("SDD", 60, 2.5));
	put(&coada, initTest("POO", 75, 6.5));
	put(&coada, initTest("Java", 48, 5.5));

	printf("\nTraversare coada:");
	while (coada != NULL) {
		t = pop(&coada);
		afisareTest(t);
		free(t.materie);
	}

	return 0;
}