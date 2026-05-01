#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

Masina initMasina(int id, int nrUsi, float pret, const char* model, 
	const char* numeSofer, unsigned char serie) {
	Masina m;
	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	m.serie = serie;
	m.model = (char*)malloc(strlen(model) + 1);
	strcpy(m.model, model);
	m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
	strcpy(m.numeSofer, numeSofer);

	return m;
}

int calculDiferentaInaltimi(Nod* rad) {
	if (rad != NULL) {
		return calculeazaInaltimeArbore(rad->st) - calculeazaInaltimeArbore(rad->dr);
	}
	else {
		return 0;
	}
}

void rotireLaStanga(Nod** rad) {
	Nod* aux = (*rad)->dr;
	(*rad)->dr = aux->st;
	aux->st = (*rad);
	(*rad) = aux;
}

void rotireLaDreapta(Nod** rad) {
	Nod* aux = (*rad)->st;
	(*rad)->st = aux->dr;
	aux->dr = (*rad);
	(*rad) = aux;
}

void adaugaMasinaInArbore(Nod** rad, Masina masinaNoua) {
	if (*rad == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = masinaNoua;
		nod->dr = NULL;
		nod->st = NULL;
		*rad = nod;
	}
	else {
		if ((*rad)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&((*rad)->st), masinaNoua);
		}
		if ((*rad)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&((*rad)->dr), masinaNoua);
		}
	}

	int diferentaInaltimi = calculDiferentaInaltimi(*rad);
	if (diferentaInaltimi == 2) {
		//dezechilibru pe partea stanga
		if (calculDiferentaInaltimi((*rad)->st) == -1) {
			rotireLaStanga(&(*rad)->st);
		}
		rotireLaDreapta(rad);
	}

	if (diferentaInaltimi == -2) {
		//dezechilibru pe partea dreapta
		if (calculDiferentaInaltimi((*rad)->dr) == 1) {
			rotireLaDreapta(&(*rad)->dr);
		}
		rotireLaStanga(rad);
	}
}

void afisareMasiniDinArbore(Nod* rad) {
	if (rad) {
		afisareMasiniDinArbore(rad->st);
		afisareMasina(rad->info);
		afisareMasiniDinArbore(rad->dr);
	}
}

void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareMasina(rad->info);
		afisarePreordine(rad->st);
		afisarePreordine(rad->dr);
	}
}

void dezalocareArboreDeMasini(Nod** rad) {
	if (*rad) {
		dezalocareArboreDeMasini(&(*rad)->st);
		dezalocareArboreDeMasini(&(*rad)->dr);
		free((*rad)->info.model);
		free((*rad)->info.numeSofer);
		free(*rad);
		*rad = NULL;
	}
}

Masina getMasinaByID(Nod* rad, int id) {
	Masina m;
	m.id = -1;
	if (rad) {
		if (rad->info.id == id) {
			m = rad->info;
			m.model = malloc(sizeof(char) * (strlen(rad->info.model) + 1));
			strcpy(m.model, rad->info.model);
			m.numeSofer = malloc(sizeof(char) * (strlen(rad->info.numeSofer) + 1));
			strcpy(m.numeSofer, rad->info.numeSofer);
		}
		if (id < rad->info.id) {
			m = getMasinaByID(rad->st, id);
		}
		if (id > rad->info.id) {
			m = getMasinaByID(rad->dr, id);
		}
	}
	return m;
}

int determinaNumarNoduri(Nod* rad) {
	if (rad) {
		return determinaNumarNoduri(rad->st) +
			+determinaNumarNoduri(rad->dr) + 1;
	}
	return 0;
}

int maxim(int a, int b) {
	return (a > b ? a : b);
}

int calculeazaInaltimeArbore(Nod* rad) {
	if (rad) {
		return maxim(calculeazaInaltimeArbore(rad->st)
			, calculeazaInaltimeArbore(rad->dr)) + 1;
	}
	return 0;
}

float calculeazaPretTotal(Nod* rad) {
	if (rad) {
		return rad->info.pret +
			+calculeazaPretTotal(rad->st) +
			+calculeazaPretTotal(rad->dr);

	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* rad, const char* numeSofer) {
	if (rad) {
		float suma = calculeazaPretulMasinilorUnuiSofer(rad->st, numeSofer) +
			+calculeazaPretulMasinilorUnuiSofer(rad->dr, numeSofer);
		if (strcmp(rad->info.numeSofer, numeSofer) == 0) {
			suma += rad->info.pret;
		}
		return suma;
	}
	return 0;
}

int main() {
	Nod* rad = NULL;
	adaugaMasinaInArbore(&rad, initMasina(1, 4, 5000, "Lexus", "Ion", 'L'));
	adaugaMasinaInArbore(&rad, initMasina(2, 4, 7000, "Skoda", "Costel", 'S'));
	adaugaMasinaInArbore(&rad, initMasina(3, 4, 10000, "BMW", "Ciki", 'B'));
	adaugaMasinaInArbore(&rad, initMasina(4, 4, 9000, "Audi", "Vasile", 'C'));
	adaugaMasinaInArbore(&rad, initMasina(5, 4, 8000, "Dacia", "Mirel", 'T'));
	afisarePreordine(rad);

	printf("Masina cautata:");
	afisareMasina(getMasinaByID(rad, 4));

	printf("Numar noduri:%d\n", determinaNumarNoduri(rad));
	printf("Inaltime arbore:%d\n", calculeazaInaltimeArbore(rad));
	printf("Pret total:%.2f\n", calculeazaPretTotal(rad));
	printf("Suma preturilor masinilor unui sofer:%.2f\n",
		calculeazaPretulMasinilorUnuiSofer(rad, "Ciki"));

	dezalocareArboreDeMasini(&rad);
	return 0;
}