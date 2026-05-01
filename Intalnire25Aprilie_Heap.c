//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//struct StructuraMasina {
//	int id;
//	int nrUsi;
//	float pret;
//	char* model;
//	char* numeSofer;
//	unsigned char serie;
//};
//typedef struct StructuraMasina Masina;
//
//struct Heap {
//	int lungime;
//	Masina* masini;
//	int nrElemente;
//};
//typedef struct Heap Heap;
//
//Masina citireMasinaDinFisier(FILE* file) {
//	char buffer[100];
//	char sep[3] = ",\n";
//	fgets(buffer, 100, file);
//	char* aux;
//	Masina m1;
//	aux = strtok(buffer, sep);
//	m1.id = atoi(aux);
//	m1.nrUsi = atoi(strtok(NULL, sep));
//	m1.pret = atof(strtok(NULL, sep));
//	aux = strtok(NULL, sep);
//	m1.model = malloc(strlen(aux) + 1);
//	strcpy_s(m1.model, strlen(aux) + 1, aux);
//
//	aux = strtok(NULL, sep);
//	m1.numeSofer = malloc(strlen(aux) + 1);
//	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
//
//	m1.serie = *strtok(NULL, sep);
//	return m1;
//}
//
//void afisareMasina(Masina masina) {
//	printf("Id: %d\n", masina.id);
//	printf("Nr. usi : %d\n", masina.nrUsi);
//	printf("Pret: %.2f\n", masina.pret);
//	printf("Model: %s\n", masina.model);
//	printf("Nume sofer: %s\n", masina.numeSofer);
//	printf("Serie: %c\n\n", masina.serie);
//}
//
//Heap initializareHeap(int lungime) {
//	Heap heap;
//	heap.lungime = lungime;
//	heap.masini = malloc(sizeof(Masina) * lungime);
//	heap.nrElemente = 0;
//	return heap;
//}
//
//void filtreazaHeap(Heap heap, int pozitieNod) {
//	int pozSt = 2 * pozitieNod + 1;
//	int pozDr = 2 * pozitieNod + 2;
//	int pozMin = pozitieNod;
//	if (pozSt <heap.nrElemente && heap.masini[pozMin].pret > heap.masini[pozSt].pret) {
//		pozMin = pozSt;
//	}
//	if (pozDr <heap.nrElemente && heap.masini[pozMin].pret > heap.masini[pozDr].pret) {
//		pozMin = pozDr;
//	}
//
//	if (pozMin != pozitieNod) {
//		Masina aux = heap.masini[pozMin];
//		heap.masini[pozMin] = heap.masini[pozitieNod];
//		heap.masini[pozitieNod] = aux;
//		if (pozMin < (heap.nrElemente - 2) / 2) {
//			filtreazaHeap(heap, pozMin);
//		}
//	}
//}
//
//Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
//	Heap heap = initializareHeap(10);
//	heap.nrElemente = 0;
//	FILE* f = fopen(numeFisier, "r");
//	if (f) {
//		while (!feof(f)) {
//			heap.masini[heap.nrElemente++] = citireMasinaDinFisier(f);
//		}
//	}
//	fclose(f);
//	for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--) {
//		filtreazaHeap(heap, i);
//	}
//
//	return heap;
//}
//
//void afisareHeap(Heap heap) {
//	for (int i = 0; i < heap.nrElemente; i++) {
//		afisareMasina(heap.masini[i]);
//	}
//}
//
//void afiseazaHeapAscuns(Heap heap) {
//	for (int i = heap.nrElemente; i < heap.lungime; i++) {
//		afisareMasina(heap.masini[i]);
//	}
//}
//
//Masina extrageMasina(Heap* heap) {
//	Masina masina;
//	masina.id = -1;
//	if (heap->nrElemente > 0) {
//		 masina = heap->masini[0];
//		heap->masini[0] = heap->masini[heap->nrElemente - 1];
//		heap->masini[heap->nrElemente - 1] = masina;
//		heap->nrElemente--;
//		for (int i = (heap->nrElemente - 2) / 2; i >= 0; i--) {
//			filtreazaHeap(*heap, i);
//		}
//	}
//	return masina;
//}
//
//
//void dezalocareHeap(Heap* heap) {
//	for (int i = 0; i < heap->lungime; i++) {
//		free(heap->masini[i].model);
//		free(heap->masini[i].numeSofer);
//	}
//	free(heap->masini);
//	heap->masini = NULL;
//	heap->lungime = 0;
//	heap->nrElemente = 0;
//}
//
//char* getNextCarModel(Heap heap) {
//	for (int i = 0; i < 6; i++) {
//		Masina m = extrageMasina(&heap);
//	}
//	if (heap.nrElemente > 0) {
//		return heap.masini[0].model;
//	}
//	else {
//		return "";
//	}
//}
//
//int main() {
//
//	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
//	afisareHeap(heap);
//
//	//3 threaduri
//	//fiecare proceseaza 2 operatii/masini
//	//care este modelul urmatoarei masini ce trebuie analizata
//	//dupa procesarea unica a celor 3 threaduri
//
//
//
//
//	printf("Extrageri:\n");
//	afisareMasina(extrageMasina(&heap));
//	afisareMasina(extrageMasina(&heap));
//	afisareMasina(extrageMasina(&heap));
//	afisareMasina(extrageMasina(&heap));
//
//	printf("Heap ramas vizibil:\n");
//	afisareHeap(heap);
//
//	printf("Elemente ascunse:\n");
//	afiseazaHeapAscuns(heap);
//
//
//	dezalocareHeap(&heap);
//	return 0;
//}