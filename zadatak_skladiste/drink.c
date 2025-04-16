#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "drink.h"

void dodajPice(Pice **pica, int *broj) {
    *pica = realloc(*pica, (*broj + 1) * sizeof(Pice));
    if (*pica == NULL) {
        perror("Neuspjela alokacija memorije");
        exit(EXIT_FAILURE);
    }

    printf("Unesite naziv pica: ");
    fgets((*pica)[*broj].naziv, sizeof((*pica)[*broj].naziv), stdin);
    (*pica)[*broj].naziv[strcspn((*pica)[*broj].naziv, "\n")] = 0;

    printf("Unesite kolicinu: ");
    scanf("%f", &(*pica)[*broj].kolicina);
    getchar();

    printf("Unesite cijenu: ");
    scanf("%f", &(*pica)[*broj].cijena);
    getchar();

    (*broj)++;
}

void ispisiPica(Pice *pica, int broj) {
    printf("\nPopis pica:\n");
    for (int i = 0; i < broj; i++) {
        printf("Naziv: %s, Kolicina: %.2f, Cijena: %.2f\n", pica[i].naziv, pica[i].kolicina, pica[i].cijena);
    }
}

void azurirajPice(Pice *pica, int broj) {
    char naziv[50];
    printf("Unesite naziv pica za azuriranje: ");
    fgets(naziv, sizeof(naziv), stdin);
    naziv[strcspn(naziv, "\n")] = 0;

    for (int i = 0; i < broj; i++) {
        if (strcmp(pica[i].naziv, naziv) == 0) {
            printf("Unesite novu kolicinu: ");
            scanf("%f", &pica[i].kolicina);
            getchar();
            printf("Unesite novu cijenu: ");
            scanf("%f", &pica[i].cijena);
            getchar();
            return;
        }
    }
    printf("Pice nije pronadeno!\n");
}

void obrisiPice(Pice **pica, int *broj) {
    char naziv[50];
    printf("Unesite naziv pica za brisanje: ");
    fgets(naziv, sizeof(naziv), stdin);
    naziv[strcspn(naziv, "\n")] = 0;

    for (int i = 0; i < *broj; i++) {
        if (strcmp((*pica)[i].naziv, naziv) == 0) {
            for (int j = i; j < *broj - 1; j++) {
                (*pica)[j] = (*pica)[j + 1];
            }
            *pica = realloc(*pica, (*broj - 1) * sizeof(Pice));
            (*broj)--;
            return;
        }
    }
    printf("Pice nije pronadeno!\n");
}

void sortirajPica(Pice *pica, int broj, int sortirajPoCijeni) {
    for (int i = 0; i < broj - 1; i++) {
        for (int j = 0; j < broj - i - 1; j++) {
            int trebaZamijeniti = 0;
            if (sortirajPoCijeni) {

                if (pica[j].cijena > pica[j + 1].cijena) {
                    trebaZamijeniti = 1;
                }
            } else {

                if (pica[j].kolicina > pica[j + 1].kolicina) {
                    trebaZamijeniti = 1;
                }
            }
            if (trebaZamijeniti) {

                Pice temp = pica[j];
                pica[j] = pica[j + 1];
                pica[j + 1] = temp;
            }
        }
    }
    printf("Pica su uspjesno sortirana.\n");
}

void spremiUDatoteku(Pice *pica, int broj) {
    FILE *file = fopen("pica.txt", "w");
    if (file == NULL) {
        perror("Ne mogu otvoriti datoteku za spremanje");
        return;
    }
    for (int i = 0; i < broj; i++) {
        fprintf(file, "%s %.2f %.2f\n", pica[i].naziv, pica[i].kolicina, pica[i].cijena);
    }
    fclose(file);
    printf("Podaci su uspjesno spremljeni u datoteku 'pica.txt'.\n");
}

void ucitajIzDatoteke(Pice **pica, int *broj) {
    FILE *file = fopen("pica.txt", "r");
    if (file == NULL) {
        printf("Datoteka ne postoji ili se ne moze otvoriti. Pocetni podaci nisu ucitani.\n");
        return;
    }

    while (!feof(file)) {
        *pica = realloc(*pica, (*broj + 1) * sizeof(Pice));
        if (fscanf(file, "%49s %f %f", (*pica)[*broj].naziv, &(*pica)[*broj].kolicina, &(*pica)[*broj].cijena) == 3) {
            (*broj)++;
        }
    }
    fclose(file);
    printf("Podaci su uspjesno ucitani iz datoteke 'pica.txt'.\n");
}
