
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

//creare structura pentru Lista Dubla 
struct ListaDubla {
	Nod* first; 
	Nod* last;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

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

void afisareListaMasiniDeLaInceput(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void afisareListaMasiniDeLaSfarsit(ListaDubla lista) {
	Nod* p = lista.last;
	while (p) {
		afisareMasina(p->masina);
		p = p->prev;
	}
} 
void adaugaMasinaInLista(ListaDubla* lista,  Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->masina = masinaNoua;
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

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = lista->first;
	nou->prev = NULL;
	if(lista->first != NULL) {
		lista->first->prev = nou;
	}
	else {
		lista->last = nou;
	}
	lista->first = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;
	while (!feof(f)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
	Nod* p = lista->first;
	while (p != NULL) {
		Nod* aux = p;
		p = p->next;
		free(aux->masina.model);
		free(aux->masina.numeSofer);
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
			suma += p->masina.pret;
			p = p->next;
		}

		return suma / lista.nrNoduri;

	}
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	if (lista->first == NULL) {
		return;
	}
	Nod* p = lista->first;
	while (p && p->masina.id != id) {
		p = p->next;
	}
	if (p == NULL) {
		return;
	}
	if (p->prev != NULL) {
		lista->first = p->prev;
		if (lista->first) {
			lista->first->prev=NULL;
		}
	}
	else {
		lista->first = p->next;
	}
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	if (lista.first) {
		Nod* max = lista.first;
		Nod* p = lista.first->next;
		while (p) {
			if (p->masina.pret > max->masina.pret) {
				max = p;
			}
			p = p->next;
		}
		char* numeSofer = malloc(strlen(max->masina.numeSofer) + 1);
		strcpy_s(numeSofer, strlen(max->masina.numeSofer) + 1, max->masina.numeSofer);
		return numeSofer;
	}
	else {
		return NULL;
	}
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	printf("\n\n\n Afisare de la sfarsit:\n");
	afisareListaMasiniDeLaSfarsit(lista);

	printf("\n\n\n Pret mediu: %.2f\n", calculeazaPretMediu(lista));

	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	printf("Soferul cu cea mai scumpa masina este: %s\n", numeSofer);
	if(numeSofer!=NULL) {
		free(numeSofer);
	}


	dezalocareLDMasini(&lista);
	afisareListaMasiniDeLaInceput(lista);

	return 0;
}