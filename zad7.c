#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct dir {
	char name[50];
	struct dir* child;
	struct dir* parent;
	struct dir* sibling;

}dir;

dir* createDir(char* name, dir* parent);
int md(dir* current);
dir* cd_back(dir* current);
int Dir(dir* current);

int main() {
	int choice = 0;
	dir* root = createDir("C:", NULL);
	dir* current = root;

	while (choice != 5) {
		printf("\nTrenutni direktorij: %s\n", current->name);
		printf("1 - md\n");
		printf("2 - cd dir\n");
		printf("3 - cd..\n");
		printf("4 - dir\n");
		printf("5 - exit\n");
		printf("Izbor: ");
		scanf("%d", &choice);

		if (choice == 1) {
			md(current);
		}
		else if (choice == 2) {
			current = cd(current);
		}
		else if (choice == 3) {
			current = cd_back(current);
		}
		else if (choice == 4) {
			dir(current);
		}
		else if (choice == 5) {
			printf("Izlaz iz programa.\n");
		}
		else {
			printf("Neispravan izbor!\n");
		}
	}

	return 0;
}

dir* createDir(char* name, dir* parent) {
	dir* newDir = malloc(sizeof(dir));
	strcpy(newDir->name, name);
	newDir->child = NULL;
	newDir->sibling = NULL;
	newDir->parent = parent;

	return newDir;

}

int md(dir* current) {
	char name[50];
	dir* temp;

	printf("Ime direktorija:\n");
	scanf("%s", name);

	temp = current->child;

	if (temp == NULL) {
		current->child = createDir(name, current);
		return 1;
	}
	while (temp->sibling != NULL) 
		temp = temp->sibling;


	temp->sibling = createDir(name, current);	
	return 1;
	
}

dir* cd_back(dir* current) {
	if (current->parent != NULL)
		return current->parent;
	return current;
}

int Dir(dir* current) {
	dir* temp = current->child;

	if (temp == NULL) {
		printf("direktorij je prazan.\n");
		return 0;
	}
	while (temp != NULL) {
		printf("%s\n", temp->name);
		temp = temp->sibling;
	}
	return 1;
}

