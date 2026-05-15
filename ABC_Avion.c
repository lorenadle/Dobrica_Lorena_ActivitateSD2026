
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Avion {
	int id;
	char* model;
	int nrLocuri;
	int nrLocuriOcupate;
	float pretBilete;
};
typedef struct Avion Avion;

struct Nod {
	Avion info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

Avion citireAvionDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Avion a1;
	aux = strtok(buffer, sep);
	a1.id = atoi(aux);
	aux = strtok(NULL, sep);
	a1.model = malloc(strlen(aux) + 1);
	strcpy_s(a1.model, strlen(aux) + 1, aux);
	a1.nrLocuri = atoi(strtok(NULL, sep));
	a1.nrLocuriOcupate = atoi(strtok(NULL, sep));
	a1.pretBilete = atof(strtok(NULL, sep));
	return a1;
}

void afisareAvion(Avion Avion) {
	printf("Id: %d\n", Avion.id);
	printf("Model: %s\n", Avion.model);
	printf("Nr. locuri : %d\n", Avion.nrLocuri);
	printf("Nr. locuri ocupate: %d\n", Avion.nrLocuriOcupate);
	printf("Pret: %.2f\n", Avion.pretBilete);
}


void adaugaAvionInArbore(Nod** rad, Avion AvionNou) {
	if (*rad == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = AvionNou;
		nod->dr = NULL;
		nod->st = NULL;
		*rad = nod;
	}
	else {
		if ((*rad)->info.id > AvionNou.id) {
			adaugaAvionInArbore(&((*rad)->st), AvionNou);
		}
		if ((*rad)->info.id < AvionNou.id) {
			adaugaAvionInArbore(&((*rad)->dr), AvionNou);
		}
	}

}

Nod* citireArboreDeAvioaneDinFisier(const char* numeFisier) {
	Nod* rad = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Avion m = citireAvionDinFisier(f);
			adaugaAvionInArbore(&rad, m);
		}
	}
	fclose(f);
	return rad;
}

void afisareAvionDinArbore(Nod* rad) {
	if (rad) {
		afisareAvionDinArbore(rad->st);
		afisareAvion(rad->info);
		afisareAvionDinArbore(rad->dr);
	}
}

void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareAvion(rad->info);
		afisarePreordine(rad->st);
		afisarePreordine(rad->dr);
	}
}

void dezalocareArboreDeAvioane(Nod** rad) {
	if (*rad) {
		dezalocareArboreDeAvioane(&(*rad)->st);
		dezalocareArboreDeAvioane(&(*rad)->dr);
		free((*rad)->info.model);
		free(*rad);
		*rad = NULL;
	}
}

Avion getAvionByID(Nod* rad, int id) {
	Avion a;
	a.id = -1;
	if (rad) {
		if (rad->info.id == id) {
			a = rad->info;
			a.model = malloc(sizeof(char) * (strlen(rad->info.model) + 1));
			strcpy(a.model, rad->info.model);
			
		}
		if (id < rad->info.id) {
			a = getAvionByID(rad->st, id);
		}
		if (id > rad->info.id) {
			a = getAvionByID(rad->dr, id);
		}
	}
	return a;
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
		return rad->info.pretBilete +
			+calculeazaPretTotal(rad->st) +
			+calculeazaPretTotal(rad->dr);

	}
	return 0;
}

float calculeazaPretulBiletelorUnuiAvion(Nod* rad, const char* model) {
	if (rad) {
		float suma = calculeazaPretulBiletelorUnuiAvion(rad->st, model) +
			+calculeazaPretulBiletelorUnuiAvion(rad->dr, model);
		if (strcmp(rad->info.model, model) == 0) {
			suma += rad->info.pretBilete;
		}
		return suma;
	}
	return 0;
}

int main() {
	Nod* rad = citireArboreDeAvioaneDinFisier("Avioane.txt");
	afisarePreordine(rad);

	printf("Avion:");
	afisareAvion(getAvionByID(rad, 10));
	

	printf("Numar noduri:%d\n", determinaNumarNoduri(rad));
	printf("Inaltime arbore:%d\n", calculeazaInaltimeArbore(rad));
	printf("Pret total:%.2f\n", calculeazaPretTotal(rad));
	printf("Suma biletelor:%.2f\n",
		calculeazaPretulBiletelorUnuiAvion(rad, "A330"));

	dezalocareArboreDeAvioane(&rad);
	return 0;
}