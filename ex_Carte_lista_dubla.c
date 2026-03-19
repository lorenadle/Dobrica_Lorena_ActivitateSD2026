
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct StructuraCarte {
	int id;
	int nrPagini;
	float pret;
	char* numeAutor;
	unsigned char categorie;
};
typedef struct StructuraCarte Carte;


struct Nod {
	Carte Carte;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;


struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

Carte citireCarteDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Carte c1;
	aux = strtok(buffer, sep);
	c1.id = atoi(aux);
	c1.nrPagini = atoi(strtok(NULL, sep));
	c1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	c1.numeAutor = malloc(strlen(aux) + 1);
	strcpy_s(c1.numeAutor, strlen(aux) + 1, aux);
	c1.categorie = *strtok(NULL, sep);
	return c1;
}

void afisareCarte(Carte Carte) {
	printf("Id: %d\n", Carte.id);
	printf("Nr. pagini : %d\n", Carte.nrPagini);
	printf("Pret: %.2f\n", Carte.pret);
	printf("Nume autor: %s\n", Carte.numeAutor);
	printf("Serie: %c\n\n", Carte.categorie);
}

void afisareListaCartiDeLaInceput(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareCarte(p->Carte);
		p = p->next;
	}
}

void afisareListaCartiDeLaSfarsit(ListaDubla lista) {
	Nod* p = lista.last;
	while (p) {
		afisareCarte(p->Carte);
		p = p->prev;
	}
}
void adaugaCarteInLista(ListaDubla* lista, Carte CarteNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->Carte = CarteNoua;
	nou->next = NULL;
	nou->prev = lista->last;
	if (lista->last != NULL) {
		lista->last->next = nou;

	}
	else {
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lista, Carte CarteNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->Carte = CarteNoua;
	nou->next = lista->first;
	nou->prev = NULL;
	if (lista->first != NULL) {
		lista->first->prev = nou;
	}
	else {
		lista->last = nou;
	}
	lista->first = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDCartiDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;
	while (!feof(f)) {
		adaugaCarteInLista(&lista, citireCarteDinFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
	Nod* p = lista->first;
	while (p != NULL) {
		Nod* aux = p;
		p = p->next;
		free(aux->Carte.numeAutor);
		free(aux);
	}
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

float calculeazaPretMediu(ListaDubla lista) {
	if (lista.nrNoduri == 0) {
		return 0;
	}
	else {
		float suma = 0;
		Nod* p = lista.first;
		while (p != NULL) {
			suma += p->Carte.pret;
			p = p->next;
		}

		return suma / lista.nrNoduri;

	}
}

void stergeCarteDupaID(ListaDubla* lista, int id) {
	if (lista->first == NULL) {
		return;
	}
	Nod* p = lista->first;
	while (p && p->Carte.id != id) {
		p = p->next;
	}
	if (p == NULL) {
		return;
	}
	if (p->prev != NULL) {
		lista->first = p->prev;
		if (lista->first) {
			lista->first->prev = NULL;
		}
	}
	else {
		lista->first = p->next;
	}
}

char* getNumeAutorCarteScumpa(ListaDubla lista) {
	if (lista.first) {
		Nod* max = lista.first;
		Nod* p = lista.first->next;
		while (p) {
			if (p->Carte.pret > max->Carte.pret) {
				max = p;
			}
			p = p->next;
		}
		char* numeAutor = malloc(strlen(max->Carte.numeAutor) + 1);
		strcpy_s(numeAutor, strlen(max->Carte.numeAutor) + 1, max->Carte.numeAutor);
		return numeAutor;
	}
	else {
		return NULL;
	}
}

int main() {
	ListaDubla lista = citireLDCartiDinFisier("carti.txt");
	afisareListaCartiDeLaInceput(lista);

	printf("\n\n\n Afisare de la sfarsit:\n");
	afisareListaCartiDeLaSfarsit(lista);

	printf("\n\n\n Pret mediu: %.2f\n", calculeazaPretMediu(lista));

	char* numeAutor = getNumeAutorCarteScumpa(lista);
	printf("Autorul cu cea mai scumpa carte este: %s\n", numeAutor);
	if (numeAutor != NULL) {
		free(numeAutor);
	}


	dezalocareLDCarti(&lista);
	afisareListaCartiDeLaInceput(lista);

	return 0;
}