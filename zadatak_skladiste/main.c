#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "drink.h"

int main() {
    Pice *pica = NULL;
    int broj = 0;
    int izbor;

    ucitajIzDatoteke(&pica, &broj);

    do {
        printf("\n1. Dodaj pice\n2. Ispisi pica\n3. Azuriraj pice\n4. Obrisi pice\n5. Sortiraj pica\n6. Spremi i izadi\n");
        printf("Unesite svoj izbor: ");
        scanf("%d", &izbor);
        getchar();

        switch (izbor) {
            case 1:
                dodajPice(&pica, &broj);
                break;
            case 2:
                ispisiPica(pica, broj);
                break;
            case 3:
                azurirajPice(pica, broj);
                break;
            case 4:
                obrisiPice(&pica, &broj);
                break;
            case 5:
                printf("Sortiraj po:\n1. Cijeni\n2. Kolicini\n");
                int izborSortiranja;
                scanf("%d", &izborSortiranja);
                getchar();
                sortirajPica(pica, broj, izborSortiranja == 1);
                break;
            case 6:
                spremiUDatoteku(pica, broj);
                free(pica);
                printf("Izlazim...\n");
                break;
            default:
                printf("Odabrali ste nepostojecu mogucnost! Pokusajte ponovo.\n");
        }
    } while (izbor != 6);

    return 0;
}
