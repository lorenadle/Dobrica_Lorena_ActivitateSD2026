#include<stdio.h>
#include<malloc.h>

typedef struct Carte Carte;
typedef struct Nod Nod;
typedef struct HashTable HashTable;

struct Carte
{
	char* nume;
	int nrPagini;
	float pret;
};

struct Nod {
	Carte info;
	Nod* next;
};

struct HashTable
{
	int dimensiune;
	Nod** vector;
};

Carte initCarte(const char* nume, int nrPagini,
	float pret) {
	Carte t;
	t.nrPagini = nrPagini;
	t.pret = pret;
	t.nume = (char*)malloc(sizeof(char)
		* (strlen(nume) + 1));
	strcpy(t.nume, nume);

	return t;
}

void afisareCarte(Carte t) {
	printf("\Cartea cu numele %s are %d pagini, iar pretul este %.2f!",
		t.nume, t.nrPagini, t.pret);
}

void afisareListaCarte(Nod* cap) {
	while (cap != NULL) {
		afisareCarte(cap->info);
		cap = cap->next;
	}
}

void inserareLaSfarsit(Nod** cap, Carte t) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = t;

	if (*cap == NULL) {
		*cap = nou;
	}
	else
	{
		Nod* aux = *cap;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}

HashTable initHashTable(int size) {
	HashTable tabela;
	tabela.dimensiune = size;
	tabela.vector = (Nod**)malloc(sizeof(Nod*) * size);
	for (int i = 0; i < size; i++) {
		tabela.vector[i] = NULL;
	}

	return tabela;
}

int hash(int dim, int nrPag) {
	return nrPag % dim;
}

void inserareHashTable(HashTable tabela, Carte t) {
	if (tabela.dimensiune > 0) {
		int pozitie = hash(tabela.dimensiune, t.nrPagini);
		if (pozitie >= 0 && pozitie < tabela.dimensiune) {
			inserareLaSfarsit(&(tabela.vector[pozitie]), t);
		}
	}
}

void afisareHashTable(HashTable tabela) {
	for (int i = 0; i < tabela.dimensiune; i++) {
		printf("\nPozitie: %d", i);
		afisareListaCartee(tabela.vector[i]);
	}
}

int calculNumarPaginiPrimaLista(HashTable tabela) {
	int nrStud = 0;
	Nod* capLista0 = tabela.vector[0];
	while (capLista0 != NULL) {
		nrStud += capLista0->info.nrPagini;
		capLista0 = capLista0->next;
	}

	return nrStud;
}

void dezalocareLista(Nod** cap) {
	while ((*cap) != NULL) {
		free((*cap)->info.nume);
		Nod* copie = *cap;
		*cap = (*cap)->next;
		free(copie);
	}
}

void dezalocareHashTable(HashTable tabela) {
	for (int i = 0; i < tabela.dimensiune; i++)
	{
		dezalocareLista(&(tabela.vector[i]));
	}
	free(tabela.vector);
}

int main() {
	HashTable tabela = initHashTable(4);
	inserareHashTable(tabela, initCarte("ION", 250, 20.5));
	inserareHashTable(tabela, initCarte("MOROMETII", 375, 45.2));
	inserareHashTable(tabela, initCarte("BALTAGUL", 223, 32.2));

	afisareHashTable(tabela);

	printf("\nNumarul cartilor din prima lista a tabelei este: %d!",
		calculNumarPaginiPrimaLista(tabela));

	dezalocareHashTable(tabela);

	return 0;
}