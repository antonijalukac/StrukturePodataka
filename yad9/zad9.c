#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;


/* a) Funkcija insert */
Node* insert(Node* root, int value) {
    /* Ako je stablo prazno, stvori novi čvor */
    if (root == NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        if (newNode == NULL)
            return NULL;
        newNode->value = value;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    /* Ako je vrijednost manja od trenutne, ide u lijevo podstablo */
    if (value < root->value)
        root->left = insert(root->left, value);
    /* Inače ide u desno podstablo */
    else
        root->right = insert(root->right, value);

    /* Vraća korijen stabla */
    return root;
}

/* b) Funkcija inorderToFile (ispisuje stablo u datoteku) */
void inorderToFile(Node* root, FILE* f) {
    if (root == NULL)
        return;

    inorderToFile(root->left, f);
    fprintf(f, "%d ", root->value);
    inorderToFile(root->right, f);
}

/* c) Funkcija replace (zamjena čvorova sumom njihovih potomaka) */
int replace(Node* root) {
    if (root == NULL)
        return 0;

    /* Rekurzivno izračunaj sume lijevog i desnog podstabla */
    int leftSum = replace(root->left);
    int rightSum = replace(root->right);

    /* Spremi staru vrijednost čvora */
    int oldValue = root->value;

    /* Zamijeni vrijednost čvora sumom potomaka */
    root->value = leftSum + rightSum;

    /* Vrati ukupnu sumu podstabla */
    return root->value + oldValue;
}

/* d) Funkcija za oslobađanje memorije stabla */
void freeTree(Node* root) {
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    Node* root = NULL;

    /* Otvaranje datoteke za ispis */
    FILE* f = fopen("inorder.txt", "w");
    if (!f) {
        printf("Greska pri otvaranju datoteke.\n");
        return 1;
    }

    srand((unsigned)time(NULL));
    int n = 10;   // Broj elemenata za umetanje u stablo

    fprintf(f, "Inorder nakon insert-a:\n");

    /* Generiranje i umetanje slucajnih brojeva u rasponu <10, 90> */
    for (int i = 0; i < n; i++) {
        int x = rand() % 81 + 10;  // [10, 90]
        root = insert(root, x);
    }

    /* Ispis stabla u datoteku */
    inorderToFile(root, f);
    fprintf(f, "\n");

    /* Zamjena vrijednosti čvorova s sumama potomaka */
    replace(root);

    /* Ispis stabla nakon zamjene */
    fprintf(f, "Inorder nakon replace-a:\n");
    inorderToFile(root, f);
    fprintf(f, "\n");

    fclose(f);
    freeTree(root);  // Oslobađanje memorije

    return 0;
}

