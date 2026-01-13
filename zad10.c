#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

// Struktura za grad
typedef struct City {
    char name[MAX_LENGTH];
    int population;
    struct City* next; // Vezana lista gradova
} City;

// Struktura za državu
typedef struct Country {
    char name[MAX_LENGTH];
    City* cityList; // Vezana lista gradova 
    struct Country* next; // Vezana lista država
} Country;

// Funkcija za unos gradova u vezanu listu
City* insertCity(City* cityList, char* name, int population) {
    City* newCity = (City*)malloc(sizeof(City));
    if (!newCity) return cityList;

    strcpy(newCity->name, name);
    newCity->population = population;
    newCity->next = NULL;

    if (cityList == NULL || cityList->population > population || (cityList->population == population && strcmp(cityList->name, name) > 0)) {
        newCity->next = cityList;
        return newCity;
    }

    City* temp = cityList;
    while (temp->next && (temp->next->population < population || (temp->next->population == population && strcmp(temp->next->name, name) < 0))) {
        temp = temp->next;
    }

    newCity->next = temp->next;
    temp->next = newCity;

    return cityList;
}

// Funkcija za unos države u vezanu listu
Country* insertCountry(Country* countryList, char* name) {
    Country* newCountry = (Country*)malloc(sizeof(Country));
    if (!newCountry) return countryList;

    strcpy(newCountry->name, name);
    newCountry->cityList = NULL;
    newCountry->next = NULL;

    if (countryList == NULL || strcmp(countryList->name, name) > 0) {
        newCountry->next = countryList;
        return newCountry;
    }

    Country* temp = countryList;
    while (temp->next && strcmp(temp->next->name, name) < 0) {
        temp = temp->next;
    }

    newCountry->next = temp->next;
    temp->next = newCountry;

    return countryList;
}

// Funkcija za ispis gradova u državi
void printCities(City* cityList) {
    while (cityList) {
        printf("%s, %d\n", cityList->name, cityList->population);
        cityList = cityList->next;
    }
}

// Funkcija za ispis svih država
void printCountries(Country* countryList) {
    while (countryList) {
        printf("Država: %s\n", countryList->name);
        printCities(countryList->cityList);
        printf("\n");
        countryList = countryList->next;
    }
}

// Funkcija za pretragu gradova u državi prema broju stanovnika
void searchCities(Country* countryList, char* countryName, int minPopulation) {
    while (countryList) {
        if (strcmp(countryList->name, countryName) == 0) {
            printf("Gradovi u %s s brojem stanovnika većim od %d:\n", countryList->name, minPopulation);
            City* cityList = countryList->cityList;
            while (cityList) {
                if (cityList->population > minPopulation) {
                    printf("%s, %d\n", cityList->name, cityList->population);
                }
                cityList = cityList->next;
            }
            return;
        }
        countryList = countryList->next;
    }
    printf("Država nije pronađena.\n");
}

// Funkcija za čitanje i učitavanje gradova iz datoteke
City* loadCities(City* cityList, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Greška pri otvaranju datoteke %s\n", filename);
        return cityList;
    }

    char cityName[MAX_LENGTH];
    int population;

    while (fscanf(file, "%s %d", cityName, &population) == 2) {
        cityList = insertCity(cityList, cityName, population);
    }

    fclose(file);
    return cityList;
}

int main() {
    Country* countryList = NULL;
    FILE* file = fopen("drzave.txt.txt", "r");
    if (!file) {
        printf("Greška pri otvaranju datoteke drzave.txt\n");
        return 1;
    }

    char countryName[MAX_LENGTH];
    char cityFileName[MAX_LENGTH];

    // Čitanje država i gradova
    while (fscanf(file, "%s %s", countryName, cityFileName) == 2) {
        // Unos države u vezanu listu
        countryList = insertCountry(countryList, countryName);

        // Pronalazak države u listi i učitavanje gradova iz datoteke
        Country* tempCountry = countryList;
        while (tempCountry) {
            if (strcmp(tempCountry->name, countryName) == 0) {
                tempCountry->cityList = loadCities(tempCountry->cityList, cityFileName);
                break;
            }
            tempCountry = tempCountry->next;
        }
    }

    fclose(file);

    // Ispis svih država i gradova
    printCountries(countryList);

    // Pretraga gradova po broju stanovnika
    char searchCountry[MAX_LENGTH];
    int minPopulation;
    printf("Unesite naziv države za pretragu: ");
    scanf("%s", searchCountry);
    printf("Unesite minimalan broj stanovnika: ");
    scanf("%d", &minPopulation);

    searchCities(countryList, searchCountry, minPopulation);

    return 0;
}

