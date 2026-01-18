#define _CRT_SECURE_NO_WARNINGS
#define _FAILED_MEMORY_ALLOCATION NULL

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Dir {
	char name[50];
	struct Dir* parent;
	struct Dir* child;
	struct Dir* sibling;

}Dir;

Dir* createDir(char* name, Dir* parent);
int md(Dir* current);
Dir* cd_dir(Dir* curent);
Dir* cd_back(Dir* current);
int dir(Dir* current);

int main() {
	int choice = 0;
	Dir* root = createDir("C:", NULL);
	Dir* current = root;

	while (choice != 5) {
		printf("Trenutni direktorij:%s\n", current->name);
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
			current = cd_dir(current);
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
Dir* createDir(char* name, Dir* parent) {
	Dir* newDir = (Dir*)malloc(sizeof(Dir));
	if (newDir == NULL) return NULL;

	strcpy(newDir->name, name);
	newDir->parent = parent;
	newDir->child = NULL;
	newDir->sibling = NULL;

	return newDir;

}

int md(Dir* current) {
	char name[50];
	Dir* temp;

	printf("ime direktorija:\n");
	scanf("%s", name);

	if (current->child == NULL) {
		current->child = createDir(name, current);
		return current->child != NULL;
	}
	temp = current->child;
	while (temp->sibling != NULL)
		temp = temp->sibling;


	temp->sibling = createDir(name, current);
	return temp->sibling != NULL;

}

Dir* cd_dir(Dir* current) {
	char name[50];
	Dir* temp;

	printf("ime direktorija:\n");
	scanf("%s", name);

	temp = current->child;

	while (temp != NULL) {
		if (strcmp(temp->name, name) == 0) {
			return temp;
		}
		temp = temp->sibling;
	}
	printf("direktorij ne postoji.\n");
	return current;

}

Dir* cd_back(Dir* current) {
	if (current->parent != NULL) return current->parent;
	return current;

}
int dir(Dir* current) {
	Dir* temp = current->child;

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
