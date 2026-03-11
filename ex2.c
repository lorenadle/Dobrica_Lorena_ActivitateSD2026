#include<stdio.h>
#include<stdlib.h>

struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};

struct Telefon initializare(int id, int RAM, const char* producator, float pret, char serie) {
	struct Telefon t;
	t.id = id;
	t.RAM = RAM;
	t.producator = (char*)malloc(sizeof(char) * (strlen(producator)+1));
	strcpy_s(t.producator, strlen(producator) + 1, producator);
	t.pret = pret;
	t.serie = serie;

	return t;
}

void afisare(struct Telefon t) {
	if (t.producator != NULL) {
		printf("%d. Teleonul %s  seria %c are %d GB RAM si costa %5.2f RON.\n",
			t.id, t.producator, t.serie, t.RAM, t.pret);
	}
	else {
		printf("%d. Teleonul  seria %c are %d GB RAM si costa %5.2f RON.\n",
			t.id,  t.serie, t.RAM, t.pret);

	}
}

void modificaPret(struct Telefon* t, float noulPret) {
	if (noulPret > 0) {
		t->pret = noulPret;
	}
}

void dezalocare(struct Telefon *t) {
	if (t->producator != NULL) {
		free(t->producator);
		t->producator = NULL;
	}
}

int main() {
	struct Telefon t;
	t = initializare(1, 256, "Samsung", 2000.5, 'A');
	afisare(t);
	modificaPret(&t, 1000);
	afisare(t);
	dezalocare(&t);
	afisare(t);
	return 0;
}