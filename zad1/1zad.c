#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

typedef struct student { //struktura
	char ime[30];
	char prezime[30];
	int bodovi;
}Student;

Student* alocirajMemoriju(int brStud) {  //pokazivac na strukturu student,funkcija za alokaciju memorije prima jedan argument int i vraca pokazivac na tip Student

	Student* studenti=(Student*)malloc(brStud * sizeof(Student));
	if (studenti == NULL) {
		printf("greska pri alokaciji memorije.\n");
		return NULL;
	}
	return studenti;
}

int otvoriDatoteku() { //funkcija za otvaranje datoteke i brojanje redaka 
	FILE* fp; 
	fp = fopen("popis.txt", "r");
	if (fp == NULL) { //ako datoteka ne postoji ili ako se ne moze otvorit
		printf("greska pri otvaranju datoteke.\n");
		return -1;
	}
	int brojac = 0;
	char buffer[100]; //niz znakova za privremeno spremanje 

	while (fgets(buffer, sizeof(buffer), fp) != NULL) { //fgets cita red po red iz datoteke
		brojac++;
	}
	fclose(fp);
	return brojac;

		
}
float maxBodovi(Student *studenti,int brojStudenata){ //funkcija pomocu koje racunamo najveci broj bodova
	float max_bodovi = 0.0;

	for (int i = 0;i < brojStudenata;i++) {
		if (studenti[i].bodovi > max_bodovi) {
			max_bodovi = studenti[i].bodovi;
		}
	}

	return max_bodovi;

}
int ispis(Student* studenti, int brojStudenata, int max_bodovi) {
	printf("Ime:\t,Prezime:\t,Aposlutni:\t,Relativni:\n");
	
	for (int i = 0;i < brojStudenata;i++) {
		float relativniBroj = ((float)studenti[i].bodovi / max_bodovi) * 100; //formula za racunanje relativnog broja bodova
		printf("%-5s\t %-10s\t %d\t\t%.2f\n",  //%-10s znaci rezerviraj 10 znakova prostora za ispis,minus je da se poravna ulijevo
			studenti[i].ime,
			studenti[i].prezime,
			studenti[i].bodovi,
			relativniBroj);

	}


	return brojStudenata;
}


int main() {

	int brojStudenata = otvoriDatoteku(); //pozivamo funkciju za otvaranje datoteke i opet osiguravamo da se vrati greska ako ne uspije otvaranje

	if (brojStudenata <= 0) {
		printf("Datoteka je prazna.\n");
		return 1;
	}

	Student* studenti = alocirajMemoriju(brojStudenata); //pozivamo funkciju za alokaciju memorije
	if (studenti == NULL) {
		return 1;
	}

	FILE* fp = fopen("popis.txt", "r");
	if (fp ==NULL) {
		printf("Greska pri otvaranju datoteke.\n");
		return 1;
	}

	for (int i = 0;i < brojStudenata;i++) { //ovde citamo podatke iz datoteke 
		fscanf(fp, "%s %s %d",
			studenti[i].ime,
			studenti[i].prezime,
			&studenti[i].bodovi);
	}
	fclose(fp);

	float max_bodovi = maxBodovi(studenti, brojStudenata);  //poziv funkcije za racnunanje najveceg broja bodova

	ispis(studenti, brojStudenata, max_bodovi); //ispisujemo podatke

	free(studenti);




	return 0;
}