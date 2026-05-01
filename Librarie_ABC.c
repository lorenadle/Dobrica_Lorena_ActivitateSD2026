#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraLibrarie {
	int id;
	int nrBucati;
	float pret;
	char* numeCarte;
	char* numeAutor;
	unsigned char categorie;
};
typedef struct StructuraLibrarie Librarie;

struct Nod {
	Librarie info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

Librarie citireLibrarieDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Librarie m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrBucati = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.numeCarte = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeCarte, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeAutor = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeAutor, strlen(aux) + 1, aux);

	m1.categorie = *strtok(NULL, sep);
	return m1;
}

void afisareLibrarie(Librarie Librarie) {
	printf("Id: %d\n", Librarie.id);
	printf("Nr. bucati : %d\n", Librarie.nrBucati);
	printf("Pret: %.2f\n", Librarie.pret);
	printf("Nume Carte: %s\n", Librarie.numeCarte);
	printf("Nume Autor: %s\n", Librarie.numeAutor);
	printf("Categorie: %c\n\n", Librarie.categorie);
}


void adaugaLibrarieInArbore(Nod** rad, Librarie LibrarieNoua) {
	if (*rad == NULL) {
		Nod* nod = malloc(sizeof(Nod));
		nod->info = LibrarieNoua;
		nod->dr = NULL;
		nod->st = NULL;
		*rad = nod;
	}
	else {
		if ((*rad)->info.id > LibrarieNoua.id) {
			adaugaLibrarieInArbore(&((*rad)->st), LibrarieNoua);
		}
		if ((*rad)->info.id < LibrarieNoua.id) {
			adaugaLibrarieInArbore(&((*rad)->dr), LibrarieNoua);
		}
	}

}

Nod* citireArboreDeLibrariiDinFisier(const char* numeFisier) {
	Nod* rad = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Librarie m = citireLibrarieDinFisier(f);
			adaugaLibrarieInArbore(&rad, m);
		}
	}
	fclose(f);
	return rad;
}

void afisareLibrariiDinArbore(Nod* rad) {
	if (rad) {
		afisareLibrariiDinArbore(rad->st);
		afisareLibrarie(rad->info);
		afisareLibrariiDinArbore(rad->dr);
	}
}

void afisarePreordine(Nod* rad) {
	if (rad) {
		afisareLibrarie(rad->info);
		afisarePreordine(rad->st);
		afisarePreordine(rad->dr);
	}
}

void dezalocareArboreDeLibrarii(Nod** rad) {
	if (*rad) {
		dezalocareArboreDeLibrarii(&(*rad)->st);
		dezalocareArboreDeLibrarii(&(*rad)->dr);
		free((*rad)->info.numeCarte);
		free((*rad)->info.numeAutor);
		free(*rad);
		*rad = NULL;
	}
}

Librarie getLibrarieByID(Nod* rad, int id) {
	Librarie m;
	m.id = -1;
	if (rad) {
		if (rad->info.id == id) {
			m = rad->info;
			m.numeCarte = malloc(sizeof(char) * (strlen(rad->info.numeCarte) + 1));
			strcpy(m.numeCarte, rad->info.numeCarte);
			m.numeAutor = malloc(sizeof(char) * (strlen(rad->info.numeAutor) + 1));
			strcpy(m.numeAutor, rad->info.numeAutor);
		}
		if (id < rad->info.id) {
			m = getLibrarieByID(rad->st, id);
		}
		if (id > rad->info.id) {
			m = getLibrarieByID(rad->dr, id);
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

float calculeazaPretulCartilorUnuiAutor(Nod* rad, const char* numeAutor) {
	if (rad) {
		float suma = calculeazaPretulCartilorUnuiAutor(rad->st, numeAutor) +
			+calculeazaPretulCartilorUnuiAutor(rad->dr, numeAutor);
		if (strcmp(rad->info.numeAutor, numeAutor) == 0) {
			suma += rad->info.pret;
		}
		return suma;
	}
	return 0;
}

int main() {
	Nod* rad = citireArboreDeLibrariiDinFisier("Librarii.txt");
	afisarePreordine(rad);

	printf("Librarie cautata:");
	afisareLibrarie(getLibrarieByID(rad, 10));

	printf("Numar noduri:%d\n", determinaNumarNoduri(rad));
	printf("Inaltime arbore:%d\n", calculeazaInaltimeArbore(rad));
	printf("Pret total:%.2f\n", calculeazaPretTotal(rad));
	printf("Suma preturilor cartilor unui autor:%.2f\n",
		calculeazaPretulCartilorUnuiAutor(rad, "Gheorghe"));

	dezalocareArboreDeLibrarii(&rad);
	return 0;
}