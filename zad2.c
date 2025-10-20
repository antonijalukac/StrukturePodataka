#define _CRT_SECURE_NO_WARNINGS


#include<stdio.h>
#include<stdlib.h>

typedef struct person* Position; 
typedef struct person {
	char name[50];
	char lastname[50];
	int year;
	Position next;

}Person;


int add(Position head,const char* name,const char* lastname,int year);
int printList(Position head);
int end(Position head,const char* name,const char* lastname,int year);
Position findLastName(Position head,const char* lastname);
int delete(Position head,const char* lastname);


int main() {

	Person head = { .name ="",.lastname ="",.year = 0,.next = NULL}; //inicijalizacija strukture person
	char lastname[50];
	Position found;

	add(&head,"Ana","Lukac",2004);
	add(&head,"Ante","Pavic",2001);
	add(&head,"Ivana","Peric",2006);
	
	printList(&head);
	
	end(&head,"Mate","Grgic",2003);

	printf("nakon dodavanja na kraj:\n");
	printList(&head);

	const char* toFind = "Lukac";
		

	found=findLastName(&head,toFind);

	if (found != NULL) {  
		printf("pronadjena osoba s prezimenom %s.\n", found->lastname);

	}
	else {
		printf("Osoba nije pronadjena.\n");
	}

	const char* toDelete = "Pavic";

	
	if (delete(& head, toDelete) != NULL) {
		printf("Osoba sa perzimenom %s je obrisana.\n", lastname);
	}
	printf("lista nakon brisanja:\n");
	printList(&head);

	return 0;
}
int add(Position head,const char*name,const char*lastname,int year) {
	Position newPerson = (Position)malloc(sizeof(Person)); //alokcija memorije za newPerson
	if (newPerson == NULL) {
		printf("Greska pri alokaciji memorije.\n");
		return -1;
	}

	strcpy(newPerson->name,name);
	strcpy(newPerson->lastname, lastname);
	newPerson->year = year;

	newPerson->next = head->next; //da newPerson->next pokaziva na ono sta je pokazivao head->next,head->next pokazuje na prvi element liste
	head->next = newPerson; //da head->next pokazuje na novog clana

	return 0;
}
int printList(Position head) {

	printf("osobe u listi:\n");
	Position temp = head->next; //pokazivac temp koji pokazuje na prvi pravi element liste

	if (temp == NULL) {
		printf("lista je prazna.\n");
	}


	while (temp != NULL) {
		printf("%s %s %d\n", temp->name, temp->lastname, temp->year);
		temp = temp->next; //prelazi na iduci
	}



	return 0;
}
int end(Position head,const char* name,const char* lastname,int year) {
	Position newPerson = (Position)malloc(sizeof(Person));
	if (newPerson == NULL) {
		printf("greska u alokaciji memorije.\n");
		return -1;
	}

	strcpy(newPerson->name, name);
	strcpy(newPerson->lastname, lastname);
	newPerson->year = year;

	newPerson->next = NULL; //postavljamo pokazivac novog elementa na NULL da bude zadnji
	Position q = head; //krecemo od pocetka liste

	while (q->next != NULL) {
		q = q->next; //prolazimo kroz listu dok q nije zadnji element
	}
	q ->next = newPerson; //zadnji element sada pokazuje na novi

	return 0;
}

Position findLastName(Position head,const char* lastname) {
	
	
	
	
	Position temp = head->next; //pocinje od prve osobe u listi

	while (temp != NULL) {
		if (strcmp(temp->lastname,lastname) == 0) { //usporeduje prezimena u listi sa trazenim prezimenom
			return temp; 
		}
		temp = temp->next; //prelazi na iduceg ako ne nade trazeno
	}

	return NULL;
}

int delete(Position head,const char* lastname) {
	Position q = head; //pokazuje na kao prethodni
	Position temp = q->next; //pokazuje na trnutni koji se provjerava

	while (temp != NULL && strcmp(temp->lastname, lastname) != 0) { //provjerava dok ne dode do kraja
		q = temp; //q ide na trnutni temp
		temp = temp->next; //temp ide na sljedeci
	}


	if (temp != NULL) { //pronadeno za brisanje
		q->next = temp->next; //preskace onaj koji se brise da mu pokazivac bude na iducem
		free(temp); //brise iz memorije
		return 0;
	}
	return -1;
}