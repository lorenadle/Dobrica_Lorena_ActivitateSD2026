#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


typedef struct Cafenea {

	unsigned int cod;
	char* nume;
	int nrTipuriCafea;
	float suprafata;

}Cafenea;


typedef struct Nod {

	Cafenea info;
	struct Nod* stanga;
	struct Nod* dreapta;

}Nod;


Cafenea initializareCafenea(unsigned int cod, const char* nume,
	int nrTipuriCafea, float suprafata) {

	Cafenea m;
	m.cod = cod;
	m.nume = (char*)malloc(sizeof(char) * strlen(nume) + 1);
	strcpy(m.nume, nume);
	m.nrTipuriCafea = nrTipuriCafea;
	m.suprafata = suprafata;

	return m;

}

Nod* creareNod(Cafenea m, Nod* stanga, Nod* dreapta) {

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initializareCafenea(m.cod, m.nume,
		m.nrTipuriCafea, m.suprafata);
	nou->stanga = stanga;
	nou->dreapta = dreapta;

	return nou;

}


Nod* inserareArbore(Nod* radacina, Cafenea m) {

	if (radacina) {

		if (m.cod < radacina->info.cod) {

			radacina->stanga = inserareArbore(radacina->stanga, m);
			return radacina;

		}
		else if (m.cod > radacina->info.cod) {

			radacina->dreapta = inserareArbore(radacina->dreapta, m);
			return radacina;

		}
		else {

			return radacina;

		}

	}

	return creareNod(m, NULL, NULL);

}


int maxim(int a, int b) {

	return a > b ? a : b;

}

int nrNiveluri(Nod* radacina) {

	if (radacina) {

		return 1 + maxim(nrNiveluri(radacina->stanga), nrNiveluri(radacina->dreapta));

	}
	else {

		return 0;

	}

}

void afisarePreordine(Nod* radacina) {

	if (radacina) {

		printf("\n%u %s %d %.2f\n", radacina->info.cod,
			radacina->info.nume, radacina->info.nrTipuriCafea,
			radacina->info.suprafata);
		afisarePreordine(radacina->stanga);
		afisarePreordine(radacina->dreapta);

	}

}


void dezalocareArbore(Nod* radacina) {

	Nod* tempStanga = radacina->stanga;
	Nod* tempDreapta = radacina->dreapta;
	free(radacina->info.nume);
	free(radacina);
	dezalocareArbore(radacina->stanga);
	dezalocareArbore(radacina->dreapta);

}


Nod* gasesteMinim(Nod* radacina) {
	while (radacina->stanga) {
		radacina = radacina->stanga;
	}
	return radacina;
}

Nod* stergereNod(Nod* radacina, unsigned int cod) {
	if (!radacina) return NULL;

	if (cod < radacina->info.cod) {
		radacina->stanga = stergereNod(radacina->stanga, cod);
	}
	else if (cod > radacina->info.cod) {
		radacina->dreapta = stergereNod(radacina->dreapta, cod);
	}
	else {
		// Nod gasit
		if (!radacina->stanga && !radacina->dreapta) {

			free(radacina->info.nume);
			free(radacina);

			return NULL;

		}
		else if (!radacina->stanga || !radacina->dreapta) {

			Nod* copil = radacina->stanga ? radacina->stanga : radacina->dreapta;
			free(radacina->info.nume);
			free(radacina);

			return copil;

		}
		else {

			Nod* minimDreapta = gasesteMinim(radacina->dreapta);
			free(radacina->info.nume);
			radacina->info = initializareCafenea(minimDreapta->info.cod,
				minimDreapta->info.nume,
				minimDreapta->info.nrTipuriCafea,
				minimDreapta->info.suprafata);
			radacina->dreapta = stergereNod(radacina->dreapta, minimDreapta->info.cod);

		}
	}

	return radacina;

}


Nod* subarboreInalt(Nod* radacina) {

	if (!radacina) {

		return NULL;

	}

	int inaltimeStanga = nrNiveluri(radacina->stanga);
	int inaltimeDreapta = nrNiveluri(radacina->dreapta);

	if (inaltimeStanga > inaltimeDreapta) {

		return radacina->stanga;

	}
	else if (inaltimeDreapta > inaltimeStanga) {

		return radacina->dreapta;

	}
	else {

		return NULL;

	}

}


int numarNoduriSubordine(Nod* radacina) {

	if (!radacina) {

		return 0;

	}

	int total = 0;

	if (radacina->stanga) {

		total += 1 + numarNoduriSubordine(radacina->stanga);

	}

	if (radacina->dreapta) {

		total += 1 + numarNoduriSubordine(radacina->dreapta);

	}

	return total;

}

Nod* subarboreCuMaiMultiNoduri(Nod* radacina) {

	if (!radacina) {

		return NULL;

	}

	int noduriStanga = numarNoduriSubordine(radacina->stanga);
	int noduriDreapta = numarNoduriSubordine(radacina->dreapta);

	if (noduriStanga > noduriDreapta) {

		return radacina->stanga;

	}
	else if (noduriDreapta > noduriStanga) {

		return radacina->dreapta;

	}
	else {

		return NULL;

	}

}


int main() {

	Cafenea m;
	Nod* radacina = NULL;

	char buffer[100];
	char separator[] = ",";
	char* token;

	FILE* f = fopen("Cafenea.txt", "r");
	while (fgets(buffer, 100, f)) {

		token = strtok(buffer, separator);
		m.cod = atoi(token);

		token = strtok(NULL, separator);
		m.nume = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(m.nume, token);

		token = strtok(NULL, separator);
		m.nrTipuriCafea = atoi(token);

		token = strtok(NULL, separator);
		m.suprafata = atof(token);

		radacina = inserareArbore(radacina, m);
		free(m.nume);

	}

	afisarePreordine(radacina);

	unsigned int codDeSters;
	printf("\nIntrodu codul Cafenelei de sters: ");
	scanf("%u", &codDeSters);
	radacina = stergereNod(radacina, codDeSters);

	printf("\nArbore dupa stergere in preordine:\n");
	afisarePreordine(radacina);

	Nod* subarbore = subarboreInalt(radacina);
	if (subarbore) {

		printf("\nSubarborele cu inaltime mai mare (preordine):\n");
		afisarePreordine(subarbore);

	}
	else {

		printf("\nSubarborii au inaltimi egale sau nu exista.\n");

	}

	printf("\nNumar noduri aflate in subordinea radacinii: %d\n",
		numarNoduriSubordine(radacina));

	return 0;

}