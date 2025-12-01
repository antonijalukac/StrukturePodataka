#define _CRT_SECURE_NO_WARNINGS
#define SUCCESS 0
#define ERROR_FILE_NOT_OPENED -1
#define ERROR_MEMORY -2

#include<stdio.h>
#include<stdlib.h>

typedef struct poly* Position;
typedef struct poly {
	int koef;
	int pow;
	Position next;
	
}Poly;

Position createNew(int koef, int pow);
int insertSorted(Position head, Position new);
int readPoly(const char* filename, Position head);
int printPoly(Position head);
int addPoly(Position p1, Position p2, Position result);
int multiplyPoly(Position p1, Position p2, Position result);

int main() {
	Poly head1 = { 0,0,NULL };
	Poly head2 = { 0,0,NULL };
	Poly sum = { 0,0,NULL };
	Poly product = { 0,0,NULL };

	readPoly("poly1.txt", &head1);
	readPoly("poly2.txt", &head2);

	printf("P1=");
	printPoly(&head1);

	printf("P2=");
	printPoly(&head2);

	addPoly(&head1, &head2, &sum);
	printf("zbroj=");
	printPoly(&sum);

	multiplyPoly(&head1, &head2, &product);
	printf("umnozak=");
	printPoly(&product);

	return 0;
}
Position createNew(int koef, int pow) {
	Position new = malloc(sizeof(Poly));
	if (!new) return NULL;

	new->koef = koef;
	new->pow = pow;
	new->next = NULL;
	return new;


}
int insertSorted(Position head, Position new) {
	if (!head || !new) return -1;
	Position prev = head;
	Position temp = head->next;

	if (temp == NULL || temp->pow < new->pow) {
		new->next = temp;
		prev->next = new;
		return 0;
	}

	while (temp != NULL && temp->pow > new->pow) {
		prev = temp;
		temp = temp->next;
	}

	if (temp != NULL && temp->pow == new->pow) {
		temp->koef += new->koef;
		free(new);
		if (temp->koef == 0) {
			prev->next = temp->next;
			free(temp);
		}
		
		return 0;

	}
	new->next = temp;
	prev->next = new;
	return 0;
}

int readPoly(const char* filename, Position head) {
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("greska:Ne mogu otvoriti datoteku.\n");
		return ERROR_FILE_NOT_OPENED;
	}
	int koef, pow;

	while (fscanf(fp, "%d %d", &koef, &pow) == 2) {
		Position newEl = createNew(koef, pow);
		if (!newEl) {
			fclose(fp);
			return ERROR_MEMORY;
		}
		insertSorted(head, newEl);
	}
	fclose(fp);
	return SUCCESS;
}
int printPoly(Position head) {
	Position P = head->next;

	
	if (!P) {
		printf("0\n");
		return 0;
	}
	int first = 1;
	while (P != NULL) {
		if (!first && P->koef > 0)printf("+");
		printf("%dx^%d", P->koef, P->pow);
		first = 0;
		P = P->next;
	}
	printf("\n");
	return 0;

}
int addPoly(Position p1, Position p2, Position result) {
	Position a = p1->next;
	Position b = p2->next;

	while (a != NULL) {
		insertSorted(result, createNew(a->koef, a->pow));
		a = a->next;
	}
	while (b != NULL) {
		insertSorted(result, createNew(b->koef, b->pow));
		b = b->next;
	}
	return 0;
}
int multiplyPoly(Position p1, Position p2, Position result) {
	Position a = p1->next;

	while (a != NULL) {
		Position b = p2->next;

		while (b != NULL) {
			int newKoef = a->koef * b->koef;
			int newPow = a->pow + b->pow;

			insertSorted(result, createNew(newKoef, newPow));

			b = b->next;
		}
		a = a->next;
	}
	return 0;

}