#define _CRT_SECURE_NO_WARNINGS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Article {
    char name[50];
    int quantity;
    float price;
    struct Article* next;   // pokazuje na sljede?i artikl u vezanoj listi
} Article;

typedef struct Bill {
    int year, month, day;
    Article* articles;      // povezana lista artikala
    struct Bill* next;      // sljede?i ra?un u listi
} Bill;

int parseDate(const char* s, int* y, int* m, int* d);
Bill* loadAll();
float query(Bill* root, const char* name,
int y1, int m1, int d1,
    int y2, int m2, int d2,
    int* totalQty);
int freeBills(Bill* head);


int main() {

    Bill* root = loadAll();

    if (!root) {
        printf("Could not load bills.\n");
        return 0;
    }

    char item[50];
    char d1[11], d2[11];
    int y1, m1, dn1, y2, m2, dn2;
    int qty;

    printf("Item name: ");                  // npr: Bread
    scanf(" %49[^\n]", item);

    printf("Start date (YYYY-MM-DD): ");    // npr: 2023-01-01
    scanf("%10s", d1);
    parseDate(d1, &y1, &m1, &dn1);

    printf("End date (YYYY-MM-DD): ");      // npr: 2023-12-31
    scanf("%10s", d2);
    parseDate(d2, &y2, &m2, &dn2);

    float money = query(root, item, y1, m1, dn1, y2, m2, dn2, &qty);

    // ispis rezultata
    printf("\nItem: %s\n", item);
    printf("Total quantity purchased: %d\n", qty);
    printf("Total money spent: %.2f\n", money);

    freeBills(root); // ?iš?enje memorije

    return 0;
}


// Pretvara "YYYY-MM-DD" u 3 cijela broja

int parseDate(const char* s, int* y, int* m, int* d) {
    return sscanf(s, "%d-%d-%d", y, m, d);
}



// Vra?a negativno ako je prvi datum raniji,
// pozitivno ako je kasniji, 0 ako su isti.
// za sortiranje racuna po datumu
int compareDates(int y1, int m1, int d1, int y2, int m2, int d2) {
    if (y1 != y2) return y1 - y2;
    if (m1 != m2) return m1 - m2;
    return d1 - d2;
}


// Ubacuje artikl u sortiranu listu (po nazivu)
Article* insertArticleSorted(Article* head, Article* node) {

    // ako je lista prazna ili treba biti prvi
    if (head == NULL || strcmp(node->name, head->name) < 0) {
        node->next = head;
        return node;
    }

    Article* cur = head;

    // pronadji mjesto gdje treba ubaciti
    while (cur->next != NULL && strcmp(node->name, cur->next->name) > 0)
        cur = cur->next;

    // ubaci artikl
    node->next = cur->next;
    cur->next = node;

    return head;
}



// Ubacuje ra?un u vezanu listu sortiranu po datumu
Bill* insertBillSorted(Bill* head, Bill* node) {

    // ubacivanje na po?etak
    if (head == NULL ||
        compareDates(node->year, node->month, node->day,
            head->year, head->month, head->day) < 0) {

        node->next = head;
        return node;
    }

    Bill* cur = head;

    // pronadji mjesto gdje treba ubaciti
    while (cur->next != NULL &&
        compareDates(node->year, node->month, node->day,
            cur->next->year, cur->next->month, cur->next->day) > 0)
        cur = cur->next;

    // ubaci ra?un
    node->next = cur->next;
    cur->next = node;

    return head;
}



// U?itava jedan racun iz datotekeiv
Bill* loadBill(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return NULL;

    char line[256];
    fgets(line, sizeof(line), f);    // ?itanje datuma

    Bill* bill = malloc(sizeof(Bill));
    if (!bill) return NULL;

    parseDate(line, &bill->year, &bill->month, &bill->day);

    bill->articles = NULL;
    bill->next = NULL;

    // U?itavanje artikala liniju po liniju
    while (fgets(line, sizeof(line), f)) {
        Article* a = malloc(sizeof(Article));
        if (!a) continue;

        sscanf(line, " %49[^,], %d, %f", a->name, &a->quantity, &a->price);
        a->next = NULL;

        bill->articles = insertArticleSorted(bill->articles, a);
    }

    fclose(f);
    return bill;
}



// U?itava sve racune
Bill* loadAll() {
    FILE* f = fopen("bills.txt", "r");
    if (!f) return NULL;

    Bill* head = NULL;
    char filename[100];

    // ?ita imena fajlova liniju po liniju
    while (fgets(filename, sizeof(filename), f)) {
        filename[strcspn(filename, "\n")] = 0;

        Bill* b = loadBill(filename);
        if (b != NULL)
            head = insertBillSorted(head, b); // sortirano ubacivanje
    }

    fclose(f);
    return head;
}



// Ra?una koliko je novca potrošeno na dati artikl
// u datom vremenskom rasponu iukupnu kolicinu
float query(Bill* root, const char* name,
    int y1, int m1, int d1,
    int y2, int m2, int d2,
    int* totalQty)
{
    float totalMoney = 0;
    *totalQty = 0;

    Bill* b = root;

    // prolazak kroz sve ra?une u zadatom rasponu
    while (b != NULL) {

        // provjera datuma
        if (compareDates(b->year, b->month, b->day, y1, m1, d1) >= 0 &&
            compareDates(b->year, b->month, b->day, y2, m2, d2) <= 0) {

            Article* a = b->articles;

            // prolazak kroz artikle
            while (a != NULL) {
                if (strcmp(a->name, name) == 0) {
                    totalMoney += a->quantity * a->price;
                    *totalQty += a->quantity;
                }
                a = a->next;
            }
        }

        b = b->next;
    }

    return totalMoney;
}


int freeArticles(Article* head) {
    Article* cur = head;
    while (cur != NULL) {
        Article* next = cur->next;
        free(cur);      // osloba?anje jednog artikla
        cur = next;
    }
    return 0;
}

int freeBills(Bill* head) {
    Bill* cur = head;
    while (cur != NULL) {
        Bill* next = cur->next;
        freeArticles(cur->articles); // prvo artikli
        free(cur);                   // onda ra?un
        cur = next;
    }
    return 0;
}


