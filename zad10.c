#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct Country* next;  // Pokazivač na sljedeću državu u listi
} Country;

// Funkcija za umetanje grada u BST (sortirano prema broju stanovnika, zatim nazivu)
City* insert_city(City* cities, const char* name, int population) {
    City* new_city = (City*)malloc(sizeof(City));
    strcpy(new_city->name, name);
    new_city->population = population;
    new_city->left = new_city->right = NULL;

    // Umetanje grada u BST
    if (cities == NULL) {
        return new_city;
    }

    City* current = cities;
    City* parent = NULL;
    while (current != NULL) {
        parent = current;
        if (population < current->population ||
            (population == current->population && strcmp(name, current->name) < 0)) {
            current = current->left;
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

// Funkcija za umetanje države u sortiranu vezanu listu
Country* insert_country(Country* countries, const char* name) {
    Country* new_country = (Country*)malloc(sizeof(Country));
    strcpy(new_country->name, name);
    new_country->cities = NULL;
    new_country->next = NULL;

    // Umetanje države u sortiranu vezanu listu
    if (countries == NULL || strcmp(countries->name, name) > 0) {
        new_country->next = countries;
        countries = new_country;
    }
    else {
        Country* current = countries;
        while (current->next != NULL && strcmp(current->next->name, name) < 0) {
            current = current->next;
        }
        new_country->next = current->next;
        current->next = new_country;
    }

    return countries;
}

// Funkcija za ispis gradova u BST
void print_cities(City* cities) {
    if (cities != NULL) {
        print_cities(cities->left);
        printf("%s, Broj stanovnika: %d\n", cities->name, cities->population);
        print_cities(cities->right);
    }
}

// Funkcija za ispis svih država i njihovih gradova
void print_countries(Country* countries) {
    while (countries != NULL) {
        printf("drzava: %s\n", countries->name);
        print_cities(countries->cities);
        printf("\n");
        countries = countries->next;
    }
}

// Funkcija za učitavanje podataka iz datoteka
Country* load_data(Country* countries) {
    FILE* file = fopen("countries.txt", "r");
    if (file == NULL) {
        printf("greska u otvaranju datoteke!\n");
        return countries;
    }

    char country_name[100], cities_file[100];
    while (fscanf(file, "%s %s", country_name, cities_file) != EOF) {
        countries = insert_country(countries, country_name);

        FILE* cities_file_ptr = fopen(cities_file, "r");
        if (cities_file_ptr == NULL) {
            printf("greska u otvaranju datoteke!\n");
            continue;
        }

        char city_name[100];
        int population;
        Country* current_country = countries;
        while (current_country != NULL && strcmp(current_country->name, country_name) != 0) {
            current_country = current_country->next;
        }

        if (current_country != NULL) {
            while (fscanf(cities_file_ptr, "%s %d", city_name, &population) != EOF) {
                current_country->cities = insert_city(current_country->cities, city_name, population);
            }
        }

        fclose(cities_file_ptr);
    }

    fclose(file);
    return countries;
}

// Funkcija za pretragu gradova u državi sa više stanovnika od unesenog broja
void search_cities_by_population(Country* countries, const char* country_name, int min_population) {
    Country* current_country = countries;

    // Pronaći državu
    while (current_country != NULL && strcmp(current_country->name, country_name) != 0) {
        current_country = current_country->next;
    }

    if (current_country == NULL) {
        printf("Country not found!\n");
        return;
    }

    printf("Cities in %s with population greater than %d:\n", country_name, min_population);
    City* current_city = current_country->cities;
    // Pretraga gradova u BST
    while (current_city != NULL) {
        if (current_city->population > min_population) {
            printf("%s, Population: %d\n", current_city->name, current_city->population);
        }
        if (current_city->left != NULL && current_city->population > min_population) {
            current_city = current_city->left;
        }
        else if (current_city->right != NULL && current_city->population > min_population) {
            current_city = current_city->right;
        }
        else {
            break;
        }
    }
}


int main() {
    Country* countries = NULL;

    // Učitavanje podataka
    countries = load_data(countries);

    // Ispisivanje država i njihovih gradova
    print_countries(countries);

    // Pretraga gradova
    char country_name[100];
    int min_population;
    printf("unesite ime drzave: ");
    scanf("%s", country_name);
    printf("unesite minimalan broj stanovnika: ");
    scanf("%d", &min_population);

    search_cities_by_population(countries, country_name, min_population);

    return 0;
}
