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

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
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

}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	Nod* rad = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			adaugaMasinaInArbore(&rad, m);
		}
	}
	fclose(f);
	return rad;
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
	Nod* rad = citireArboreDeMasiniDinFisier("masini.txt");
	afisarePreordine(rad);

	printf("Masina cautata:");
	afisareMasina(getMasinaByID(rad, 10));

	printf("Numar noduri:%d\n", determinaNumarNoduri(rad));
	printf("Inaltime arbore:%d\n", calculeazaInaltimeArbore(rad));
	printf("Pret total:%.2f\n", calculeazaPretTotal(rad));
	printf("Suma preturilor masinilor unui sofer:%.2f\n",
		calculeazaPretulMasinilorUnuiSofer(rad, "Gheorghe"));

	dezalocareArboreDeMasini(&rad);
	return 0;
}