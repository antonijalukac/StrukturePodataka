#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX 256

typedef struct postfix* Position;

typedef struct postfix {
	double number;
	Position next;

}Postfix;

char* readPostfix();
Position push(Position head, double x);
Position pop(Position head, double *x);
double calculatePostfix(char* expression);

int main() {
	char* expression = readPostfix();
	if (expression == NULL) {
		return -1;
	}

	double result = calculatePostfix(expression);
	printf("rezultat je :%.2f\n", result);

	free(expression);


	return 0;
}
char* readPostfix() {
	FILE* fp = fopen("postfix.txt", "r");
	if (fp == NULL) {
		printf("greška pri otvaranju datoteke.\n");
		return NULL;
	}
	char* buffer = malloc(MAX * sizeof(char));
	if (buffer == NULL) {
		printf("greška u alokaciji memorije,\n");
		fclose(fp);
		return NULL;
	}
	if (fgets(buffer, MAX, fp) == NULL) {
		printf("greska\n");
		free(buffer);
		fclose(fp);
		return NULL;
	}
	fclose(fp);

	return buffer;


}
Position push(Position head, double x) {
	Position new = malloc(sizeof(Postfix));
	if (new == NULL) {
		printf("greska u alokaciji memorije.\n");
		return head;
	}
	
	new->number = x;
	new->next = head;
return new;
}
Position pop(Position head,double *x) {
	if (head == NULL) {
		printf("greska,stog je prazan.\n");
		return NULL;
	}
	Position temp = head; //privremeni pokaziva?
	*x = temp->number; //uzimamo vrijednost
	head = temp->next; //pomaknemo vrh
	free(temp); //oslobodimo

	return head; //vracamo novi vrh

}




double calculatePostfix(char* expression) {
	Position stack = NULL;
	char* token = strtok(expression, " ");
	double a, b, result;

	while (token != NULL)
	{
		if (isdigit(token[0]) || (token[0] ==' - ' && isdigit(token[1])))
			stack = push(stack, atof(token));

		else {
			stack = pop(stack, &b);
			stack = pop(stack, &a);

			if (token[0] == '+') result = a + b;
			else if (token[0] == '-') result = a - b;
			else if (token[0] == '*') result = a * b;
			else if (token[0] == '/') result = a / b;


			stack = push(stack, result);
		}
		token = strtok(NULL, " ");

		
	

	}
	stack = pop(stack, &result);
	double temp;
	while (stack != NULL) {
		stack = pop(stack, &temp);
	}
	return result;
}