#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Veličina hash tablice
#define TABLE_SIZE 11

// Struktura za grad
typedef struct City {
    char name[100];
    int population;
    struct City* left;  
    struct City* right; 
} City;

// Struktura za državu
typedef struct Country {
    char name[100];
    City* cities;  // BST gradova
    struct Country* next;  // Vezana lista država
} Country;

// Hash tablica
Country* hash_table[TABLE_SIZE];

// Funkcija za izračunavanje hasha na temelju ASCII vrijednosti prvih pet slova države
int hash_function(const char* country_name) {
    int sum = 0;
    for (int i = 0; i < 5 && country_name[i] != '\0'; i++) {
        sum += country_name[i];
    }
    return sum % TABLE_SIZE;
}

// Funkcija za umetanje grada u BST
City* insert_city(City* cities, const char* name, int population) {
    City* new_city = (City*)malloc(sizeof(City));
    strcpy(new_city->name, name);
    new_city->population = population;
    new_city->left = new_city->right = NULL;

    // Umetanje u BST gradova (sortirano prema broju stanovnika, zatim imenu grada)
    if (cities == NULL) {
        return new_city;
    }

    City* current = cities; //za kretanje kroz stablo
    City* parent = NULL; //pamti roditelja cvora gdje cemo umetnuti novi grad
    while (current != NULL) {
        parent = current;
        if (population < current->population ||
            (population == current->population && strcmp(name, current->name) < 0)) {
            current = current->left; //idemo u lijevo podstablo
        }
        else {
            current = current->right;
        }
    }

    if (population < parent->population ||
        (population == parent->population && strcmp(name, parent->name) < 0)) {
        parent->left = new_city;
    }
    else {
        parent->right = new_city;
    }

    return cities;
}

// Funkcija za umetanje države u vezanu listu
Country* insert_country(Country* head, const char* country_name) {
    Country* new_country = (Country*)malloc(sizeof(Country));
    strcpy(new_country->name, country_name);
    new_country->cities = NULL;
    new_country->next = NULL;

    // Umetanje države u vezanu listu (sortirano po imenu države)
    if (head == NULL || strcmp(head->name, country_name) > 0) {
        new_country->next = head;
        return new_country;
    }

    Country* current = head;
    while (current->next != NULL && strcmp(current->next->name, country_name) < 0) {
        current = current->next;
    }

    new_country->next = current->next;
    current->next = new_country;

    return head;
}

// Funkcija za umetanje države u hash tablicu
Country* insert_into_hash_table(Country* hash_table[], const char* country_name, const char* city_name, int population) {
    int index = hash_function(country_name);

    // Prvo pokušavamo pronaći postojeću državu u vezi s tim ključem (indeks tablice)
    Country* current_country = hash_table[index];

    // Ako država već postoji, samo umetnemo grad u BST
    while (current_country != NULL) {
        if (strcmp(current_country->name, country_name) == 0) {
            current_country->cities = insert_city(current_country->cities, city_name, population);
            return hash_table[index];  // Vraćamo tablicu nakon umetanja
        }
        current_country = current_country->next;
    }

    // Ako država ne postoji, umetnemo novu državu u vezanu listu
    current_country = insert_country(hash_table[index], country_name);

    // Nakon što smo umetnuli državu, umetnemo grad u BST
    current_country->cities = insert_city(current_country->cities, city_name, population);

    // Pohranjivanje nove države u hash tablicu
    hash_table[index] = current_country;

    return hash_table[index];
}

// Funkcija za ispis svih gradova u BST
void print_cities(City* cities) {
    if (cities != NULL) {
        print_cities(cities->left);
        printf("%s, Population: %d\n", cities->name, cities->population);
        print_cities(cities->right);
    }
}

// Funkcija za ispis svih država i njihovih gradova
void print_hash_table(Country* hash_table[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {  //prolazi kroz hash tablicu
        Country* current_country = hash_table[i]; //prolazak kroz vezanu listu
        while (current_country != NULL) {
            printf("Country: %s\n", current_country->name);
            print_cities(current_country->cities);
            printf("\n");
            current_country = current_country->next;
        }
    }
}

// Funkcija za učitavanje podataka iz datoteka
Country* load_data(Country* hash_table[], const char* countries_file) {
    FILE* file = fopen(countries_file, "r");
    if (file == NULL) {
        printf("Error opening the countries file!\n");
        return hash_table;
    }

    char country_name[100], cities_file[100];
    while (fscanf(file, "%s %s", country_name, cities_file) != EOF) {
        FILE* cities_file_ptr = fopen(cities_file, "r");
        if (cities_file_ptr == NULL) {
            printf("Error opening the cities file!\n");
            continue;
        }

        char city_name[100];
        int population;
        while (fscanf(cities_file_ptr, "%s %d", city_name, &population) != EOF) {
            insert_into_hash_table(hash_table, country_name, city_name, population);
        }

        fclose(cities_file_ptr);
    }

    fclose(file);
    return hash_table;
}

int main() {
    // Inicijalizacija hash tablice
    for (int i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }

    // Učitavanje podataka
    load_data(hash_table, "countries.txt");

    // Ispisivanje sadržaja hash tablice
    print_hash_table(hash_table);

    return 0;
}
