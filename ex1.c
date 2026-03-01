#include<stdio.h>
#include<malloc.h>

typedef struct Carte Carte;

struct Carte {
	int id;
	int anAparitie;
	char* autor;
	char* nume;
};
struct Carte initializare(int id, int anAparitie,  const char* autor, const char* nume) {
	struct Carte s;
	s.id = id;
	s.anAparitie = anAparitie;
	s.autor = (char*)malloc(sizeof(char) * (strlen(autor) + 1));
	strcpy(s.autor, autor);
	s.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(s.nume, nume);
	return s;
}

Carte copiaza(Carte d) {
	return initializare(d.id, d.anAparitie,  d.autor, d.nume);
}

void afisare(struct Carte s) {
	printf("%d. Cartea %s scrisa de autorul %s are  anAparitie %d.\n",
		s.id, s.nume, s.autor, s.anAparitie);
}

void afisareVector(struct Carte* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(*(vector + i));
	}
}

struct Carte* copiazaPrimeleNElemente(Carte* vector, int nrElemente, int nrElementeCopiate) {
	if (nrElementeCopiate < nrElemente) {
		struct Carte* vectorNou = malloc(sizeof(Carte) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++) {
			vectorNou[i] = copiaza(vector[i]);
		}
		return vectorNou;
	}
	else {
		return NULL;
	}
}

void dezalocare(struct Carte** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].nume);
	}
	free(*vector);
	*nrElemente = 0;
	*vector = NULL;
}

void copiazaAnumiteElemente(struct Carte* vector, char nrElemente, float prag, struct Carte** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].anAparitie < prag) {
			(*dimensiune)++;
		}
	}
	*vectorNou = malloc(sizeof(Carte) * (*dimensiune));
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].anAparitie < prag) {
			(*vectorNou)[*dimensiune] = copiaza(vector[i]);
			(*dimensiune)++;
		}
	}
}

struct Carte getPrimulElementConditionat(struct Carte* vector, int nrElemente, const char* numeCautat) {
	Carte s;
	s.id = -1;
	s.autor = NULL;
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(numeCautat, vector[i].nume) == 0) {
			s = copiaza(vector[i]);
		}
	}
	return s;
}



int main() {
	struct Carte Carte = initializare(1, 1965, "Marin Preda", "Morometii");
	afisare(Carte);

	int nrCarti = 3;
	Carte* vector = (Carte*)malloc(sizeof(Carte) * nrCarti);
	vector[0] = initializare(2, 1985, "Ion Luca Caragiale", "O scrisoare pierduta");
	vector[1] = initializare(3, 1950, "Ion Creanga", "Amintiri din copilarie");
	vector[2] = initializare(4, 1970, "Ioan Slavici", "Moara cu noroc");
	

	afisareVector(vector, nrCarti);
	printf("\n\n");
	int nrCopiate = 2;
	Carte* scurt = copiazaPrimeleNElemente(vector, nrCarti, nrCopiate);

	afisareVector(scurt, nrCopiate);
	dezalocare(&scurt, &nrCopiate);
	Carte* filtrat = NULL;
	int dimensiune = 0;
	copiazaAnumiteElemente(vector, nrCarti, 50, &filtrat, &dimensiune);
	printf("\nVector filtrat:\n");
	afisareVector(filtrat, dimensiune);
	dezalocare(&filtrat, &dimensiune);

	Carte CarteCautata = getPrimulElementConditionat(vector, nrCarti, "Morometii");
	printf("\nCarte cautata:\n");
	afisare(CarteCautata);
	if (CarteCautata.id != -1) {
		free(CarteCautata.nume);
	}
	free(Carte.nume);
	dezalocare(&vector, &nrCarti);
	return 0;
}