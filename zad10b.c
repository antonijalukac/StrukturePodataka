#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktura za grad
typedef struct City {
    char name[100];
    int population;
    struct City* next; // Vezani pokazivač na sljedeći grad
} City;

// Struktura za državu
typedef struct Country {
    char name[100];
    City* cities;  // Sortirana lista gradova prema broju stanovnika i nazivu
    struct Country* left;   // Pokazivač na lijevog sina u BST
    struct Country* right;  // Pokazivač na desnog sina u BST
} Country;

// Funkcija za umetanje grada u sortiranu vezanu listu gradova
City* insert_city(City* cities, const char* name, int population) {
    City* new_city = (City*)malloc(sizeof(City));
    strcpy(new_city->name, name);
    new_city->population = population;
    new_city->next = NULL;

    // Umetanje u sortiranu listu gradova
    if (cities == NULL || cities->population > population ||
        (cities->population == population && strcmp(cities->name, name) > 0)) {
        new_city->next = cities;
        return new_city;
    }

    City* current = cities;
    while (current->next != NULL && (current->next->population < population ||
        (current->next->population == population && strcmp(current->next->name, name) < 0))) {
        current = current->next;
    }

    new_city->next = current->next;
    current->next = new_city;

    return cities;
}

// Funkcija za umetanje države u binarno stablo
Country* insert_country(Country* countries, const char* name) {
    if (countries == NULL) {
        Country* new_country = (Country*)malloc(sizeof(Country));
        strcpy(new_country->name, name);
        new_country->cities = NULL;
        new_country->left = new_country->right = NULL;
        return new_country;
    }

    if (strcmp(name, countries->name) < 0) {
        countries->left = insert_country(countries->left, name);
    }
    else if (strcmp(name, countries->name) > 0) {
        countries->right = insert_country(countries->right, name);
    }

    return countries;
}

// Funkcija za ispis gradova u sortiranoj listi
void print_cities(City* cities) {
    while (cities != NULL) {
        printf("%s, Population: %d\n", cities->name, cities->population);
        cities = cities->next;
    }
}

// Funkcija za ispis svih država i njihovih gradova
void print_countries(Country* countries) {
    if (countries == NULL) {
        return;
    }

    print_countries(countries->left);
    printf("Country: %s\n", countries->name);
    print_cities(countries->cities);
    printf("\n");
    print_countries(countries->right);
}

// Funkcija za učitavanje podataka iz datoteka
Country* load_data(Country* countries) {
    FILE* file = fopen("countries.txt", "r");
    if (file == NULL) {
        printf("Error opening the countries file!\n");
        return countries;
    }

    char country_name[100], cities_file[100];
    while (fscanf(file, "%s %s", country_name, cities_file) != EOF) {
        countries = insert_country(countries, country_name);

        FILE* cities_file_ptr = fopen(cities_file, "r");
        if (cities_file_ptr == NULL) {
            printf("Error opening the cities file!\n");
            continue;
        }

        char city_name[100];
        int population;
        Country* current_country = countries;
        while (current_country != NULL && strcmp(current_country->name, country_name) != 0) {
            current_country = current_country->left;
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

// Funkcija za pretragu gradova u državi sa većim brojem stanovnika od unesenog broja
void search_cities_by_population(Country* countries, const char* country_name, int min_population) {
    Country* current_country = countries;

    // Pronaći državu u BST-u
    while (current_country != NULL && strcmp(current_country->name, country_name) != 0) {
        if (strcmp(country_name, current_country->name) < 0) {
            current_country = current_country->left;
        }
        else {
            current_country = current_country->right;
        }
    }

    if (current_country == NULL) {
        printf("Country not found!\n");
        return;
    }

    printf("Cities in %s with population greater than %d:\n", country_name, min_population);
    City* current_city = current_country->cities;
    while (current_city != NULL) {
        if (current_city->population > min_population) {
            printf("%s, Population: %d\n", current_city->name, current_city->population);
        }
        current_city = current_city->next;
    }
}

// Glavna funkcija
int main() {
    Country* countries = NULL;

    // Učitavanje podataka
    countries = load_data(countries);

    // Ispisivanje država i njihovih gradova
    print_countries(countries);

    // Pretraga gradova
    char country_name[100];
    int min_population;
    printf("Enter the country name to search for cities: ");
    scanf("%s", country_name);
    printf("Enter the minimum population: ");
    scanf("%d", &min_population);

    search_cities_by_population(countries, country_name, min_population);

    return 0;
}
