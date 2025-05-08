#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "drink.h"

#define DATOTEKA "pica.txt"

int main(void) {
    Pice *pica = NULL;
    int broj = 0;
    int izbor;

    ucitaj_iz_datoteke(&pica, &broj);
    if (pica == NULL && broj > 0) {
        fprintf(stderr, "Greska pri ucitavanju podataka. Prekidam program.\n");
        return EXIT_FAILURE;
    }

    do {
        printf("\n--- Izbornik ---\n");
        printf("1. Dodaj pice\n");
        printf("2. Ispisi pica\n");
        printf("3. Azuriraj pice\n");
        printf("4. Obrisi pice\n");
        printf("5. Sortiraj pica\n");
        printf("6. Pretrazi pica\n");
        printf("7. Spremi i izadi\n");
        izbor = meni_izbor();

        switch (izbor) {
            case 1:
                dodaj_pice(&pica, &broj);
                break;
            case 2:
                ispisi_pica(pica, broj);
                break;
            case 3:
                if (broj > 0) {
                    printf("\nTrenutni popis pica:\n");
                    ispisi_pica(pica, broj);
                    azuriraj_pice(pica, broj);
                } else {
                    printf("Nema pica za azuriranje.\n");
                }
                break;
            case 4:
                if (broj > 0) {
                    printf("\nTrenutni popis pica:\n");
                    ispisi_pica(pica, broj);
                    obrisi_pice(&pica, &broj);
                } else {
                    printf("Nema pica za brisanje.\n");
                }
                break;
            case 5: {
                printf("Sortiraj po:\n1. Imenu\n2. Cijeni\n3. Kolicini\nUnesite izbor: ");
                int izbor_sortiranja = meni_izbor();
                if (izbor_sortiranja >= 1 && izbor_sortiranja <= 3) {
                    sortiraj_pica(pica, broj, izbor_sortiranja);
                } else {
                    printf("Neispravan izbor sortiranja!\n");
                }
                break;
            }
            case 6: {
                int nastavi_pretrazivanje = 1;
                while (nastavi_pretrazivanje) {
                    pretrazi_pica(pica, broj);
                    if (!potvrda_za_akciju("zelite li nastaviti pretrazivati?")) {
                        nastavi_pretrazivanje = 0;
                    }
                }
                break;
            }
            case 7:
                if (potvrda_za_akciju("Jeste li sigurni da zelite izaci iz programa?")) {
                    spremi_u_datoteku(pica, broj);
                    free(pica);
                    pica = NULL;
                    printf("Izlazim...\n");
                    izbor = 7; // izlaz
                } else {
                    printf("Izlazak otkazan.\n");
                    izbor = 0; // reset, nastavlja se rad
                }
                break;
            default:
                printf("Odabrali ste nepostojecu opciju! Pokusajte ponovo.\n");
        }
    } while (izbor != 7);

    return 0;
}
