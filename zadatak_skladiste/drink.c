#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include "drink.h"

/* File utility functions */
static int provjeri_datoteku(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

static long dohvati_velicinu_datoteke(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return -1;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);
    return size;
}

/* Input validation utilities */
static void ocisti_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void sigurni_input(char* buffer, size_t velicina) {
    if (fgets(buffer, (int)velicina, stdin)) {
        buffer[strcspn(buffer, "\n")] = '\0';
    } else {
        ocisti_buffer();
        buffer[0] = '\0';
    }
}

void to_lowercase(char* str) {
    for (; *str; ++str) {
        *str = (char)tolower((unsigned char)*str);
    }
}

void replace_comma_with_dot(char* str) {
    for (; *str; ++str) {
        if (*str == ',') *str = '.';
    }
}

int meni_izbor(void) {
    char buffer[16];
    sigurni_input(buffer, sizeof(buffer));
    return atoi(buffer);
}

/* Drink operations */
void dodaj_pice(Pice** pica, int* broj) {
    Pice* temp = realloc(*pica, (*broj + 1) * sizeof(Pice));
    if (!temp) {
        perror("Greska kod alokacije memorije");
        return;
    }
    *pica = temp;

    printf("Unesite naziv pica: ");
    sigurni_input((*pica)[*broj].naziv, NAZIV_MAX_DUZINA);
    to_lowercase((*pica)[*broj].naziv);

    printf("Unesite kolicinu: ");
    char buffer[64];
    sigurni_input(buffer, sizeof(buffer));
    replace_comma_with_dot(buffer);
    (*pica)[*broj].kolicina = strtod(buffer, NULL);

    printf("Unesite cijenu: ");
    sigurni_input(buffer, sizeof(buffer));
    replace_comma_with_dot(buffer);
    (*pica)[*broj].cijena = strtod(buffer, NULL);

    (*broj)++;
    printf("Pice uspjesno dodano.\n");
}

void ispisi_pica(const Pice* pica, int broj) {
    if (!broj) {
        printf("Nema dostupnih pica za ispis.\n");
        return;
    }

    printf("\nPopis pica:\n");
    for (int i = 0; i < broj; i++) {
        printf("%d. Naziv: %s, Kolicina: %.2lf, Cijena: %.2lf\n",
               i+1, pica[i].naziv, pica[i].kolicina, pica[i].cijena);
    }
}

/* File operations */
void spremi_u_datoteku(const Pice* pica, int broj) {
    FILE* file = fopen(DATOTEKA, "a");
    if (!file) {
        perror("Ne mogu otvoriti datoteku za spremanje");
        return;
    }

    fseek(file, 0, SEEK_END);

    for (int i = 0; i < broj; i++) {
        if (fprintf(file, "%s %.2lf %.2lf\n", pica[i].naziv, pica[i].kolicina, pica[i].cijena) < 0) {
            perror("Greska kod pisanja u datoteku");
            break;
        }
    }

    if (fclose(file) != 0) {
        perror("Greska kod zatvaranja datoteke");
    } else {
        printf("Podaci uspjesno spremljeni u datoteku '%s'.\n", DATOTEKA);
    }
}

void ucitaj_iz_datoteke(Pice** pica, int* broj) {
    if (!provjeri_datoteku(DATOTEKA)) {
        fprintf(stderr, "Datoteka '%s' ne postoji.\n", DATOTEKA);
        return;
    }

    FILE* file = fopen(DATOTEKA, "r");
    if (!file) {
        perror("Greska prilikom otvaranja datoteke");
        return;
    }

    fseek(file, 0, SEEK_SET);

    Pice temp;
    int temp_broj = 0;
    Pice* temp_pica = NULL;
    int neispravni_unosi = 0;

    while (fscanf(file, "%49s %lf %lf", temp.naziv, &temp.kolicina, &temp.cijena) == 3) {
        to_lowercase(temp.naziv);
        Pice* novi_niz = realloc(temp_pica, (temp_broj + 1) * sizeof(Pice));
        if (!novi_niz) {
            perror("Greska kod alokacije memorije");
            free(temp_pica);
            fclose(file);
            return;
        }
        temp_pica = novi_niz;
        temp_pica[temp_broj++] = temp;
    }

    if (!feof(file) && ferror(file)) {
        perror("Greska prilikom citanja datoteke");
    }

    if (fclose(file) != 0) {
        perror("Greska kod zatvaranja datoteke");
    }

    if (*pica) {
        free(*pica);
    }
    *pica = temp_pica;
    *broj = temp_broj;

    printf("Ucitano %d pica iz datoteke '%s'.\n", temp_broj, DATOTEKA);
}

/* Update, delete and search functions */
void azuriraj_pice(Pice* pica, int broj) {
    if (!broj) {
        printf("Nema pica za azuriranje.\n");
        return;
    }

    char naziv[NAZIV_MAX_DUZINA];
    printf("Unesite naziv pica za azuriranje: ");
    sigurni_input(naziv, sizeof(naziv));
    to_lowercase(naziv);

    for (int i = 0; i < broj; i++) {
        if (strcmp(pica[i].naziv, naziv) == 0) {
            printf("\nTrenutni podaci:\nNaziv: %s, Kolicina: %.2lf, Cijena: %.2lf\n",
                  pica[i].naziv, pica[i].kolicina, pica[i].cijena);

            printf("Azuriraj:\n1. Kolicinu\n2. Cijenu\nOdabir: ");
            int opcija = meni_izbor();

            char buffer[64];
            if (opcija == 1) {
                printf("Nova kolicina: ");
                sigurni_input(buffer, sizeof(buffer));
                replace_comma_with_dot(buffer);
                pica[i].kolicina = strtod(buffer, NULL);
                printf("Kolicina azurirana.\n");
            } else if (opcija == 2) {
                printf("Nova cijena: ");
                sigurni_input(buffer, sizeof(buffer));
                replace_comma_with_dot(buffer);
                pica[i].cijena = strtod(buffer, NULL);
                printf("Cijena azurirana.\n");
            } else {
                printf("Neispravan odabir.\n");
            }
            return;
        }
    }
    printf("Pice nije pronadeno!\n");
}

void obrisi_pice(Pice** pica, int* broj) {
    if (!(*broj)) {
        printf("Nema pica za brisanje.\n");
        return;
    }

    char naziv[NAZIV_MAX_DUZINA];
    printf("Unesite naziv pica za brisanje: ");
    sigurni_input(naziv, sizeof(naziv));
    to_lowercase(naziv);

    for (int i = 0; i < *broj; i++) {
        if (strcmp((*pica)[i].naziv, naziv) == 0) {
            for (int j = i; j < *broj - 1; j++) {
                (*pica)[j] = (*pica)[j + 1];
            }

            Pice* temp = (*broj > 1) ? realloc(*pica, (*broj - 1) * sizeof(Pice)) : NULL;
            if (*broj > 1 && !temp) {
                perror("Greska kod smanjivanja memorije");
                return;
            }
            *pica = (*broj > 1) ? temp : NULL;
            (*broj)--;
            printf("Pice uspjesno obrisano.\n");
            return;
        }
    }
    printf("Pice nije pronadeno!\n");
}

/* Sorting functions */
static int usporedi_po_imenu(const void* a, const void* b) {
    return strcmp(((const Pice*)a)->naziv, ((const Pice*)b)->naziv);
}

static int usporedi_po_cijeni_rastuce(const void* a, const void* b) {
    double diff = ((const Pice*)a)->cijena - ((const Pice*)b)->cijena;
    return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

static int usporedi_po_cijeni_opadajuce(const void* a, const void* b) {
    return -usporedi_po_cijeni_rastuce(a, b);
}

static int usporedi_po_kolicini_rastuce(const void* a, const void* b) {
    double diff = ((const Pice*)a)->kolicina - ((const Pice*)b)->kolicina;
    return (diff > 0) ? 1 : ((diff < 0) ? -1 : 0);
}

static int usporedi_po_kolicini_opadajuce(const void* a, const void* b) {
    return -usporedi_po_kolicini_rastuce(a, b);
}

void sortiraj_pica(Pice* pica, int broj, int kriterij) {
    if (broj <= 1) {
        printf("Nema dovoljno pica za sortiranje.\n");
        return;
    }

    int nacin = 1;
    if (kriterij == 2 || kriterij == 3) {
        printf("Nacin sortiranja:\n1. Rastuce\n2. Opadajuce\nOdabir: ");
        nacin = meni_izbor();
        if (nacin != 1 && nacin != 2) {
            printf("Neispravan odabir. Koristim rastuci redoslijed.\n");
            nacin = 1;
        }
    }

    switch (kriterij) {
        case 1:
            qsort(pica, broj, sizeof(Pice), usporedi_po_imenu);
            break;
        case 2:
            qsort(pica, broj, sizeof(Pice),
                 (nacin == 1) ? usporedi_po_cijeni_rastuce : usporedi_po_cijeni_opadajuce);
            break;
        case 3:
            qsort(pica, broj, sizeof(Pice),
                 (nacin == 1) ? usporedi_po_kolicini_rastuce : usporedi_po_kolicini_opadajuce);
            break;
        default:
            printf("Neispravan kriterij sortiranja!\n");
            return;
    }
    printf("Pica uspjesno sortirana.\n");
}

/* Search function */
void pretrazi_pica(const Pice* pica, int broj) {
    if (!broj) {
        printf("Nema pica za pretragu.\n");
        return;
    }

    printf("Pretraga po:\n1. Imenu\n2. Cijeni\n3. Kolicini\nOdabir: ");
    int kriterij = meni_izbor();

    char naziv[NAZIV_MAX_DUZINA];
    char buffer[64];
    double vrijednost;
    int pronadeno = 0;
    const double EPSILON = 0.001;

    switch (kriterij) {
        case 1:
            printf("Unesite naziv: ");
            sigurni_input(naziv, sizeof(naziv));
            to_lowercase(naziv);
            for (int i = 0; i < broj; i++) {
                if (strstr(pica[i].naziv, naziv)) {
                    printf("%d. %s, Kolicina: %.2lf, Cijena: %.2lf\n",
                          i+1, pica[i].naziv, pica[i].kolicina, pica[i].cijena);
                    pronadeno++;
                }
            }
            break;

        case 2:
            printf("Unesite cijenu: ");
            sigurni_input(buffer, sizeof(buffer));
            replace_comma_with_dot(buffer);
            vrijednost = strtod(buffer, NULL);
            for (int i = 0; i < broj; i++) {
                if (fabs(pica[i].cijena - vrijednost) < EPSILON) {
                    printf("%d. %s, Kolicina: %.2lf, Cijena: %.2lf\n",
                          i+1, pica[i].naziv, pica[i].kolicina, pica[i].cijena);
                    pronadeno++;
                }
            }
            break;

        case 3:
            printf("Unesite kolicinu: ");
            sigurni_input(buffer, sizeof(buffer));
            replace_comma_with_dot(buffer);
            vrijednost = strtod(buffer, NULL);
            for (int i = 0; i < broj; i++) {
                if (fabs(pica[i].kolicina - vrijednost) < EPSILON) {
                    printf("%d. %s, Kolicina: %.2lf, Cijena: %.2lf\n",
                          i+1, pica[i].naziv, pica[i].kolicina, pica[i].cijena);
                    pronadeno++;
                }
            }
            break;

        default:
            printf("Neispravan izbor pretrage!\n");
            return;
    }

    if (!pronadeno) {
        printf("Nema rezultata pretrage.\n");
    }
}

/* Confirmation dialog */
int potvrda_za_akciju(const char* poruka) {
    char odgovor[8];
    printf("%s (d/n): ", poruka);
    sigurni_input(odgovor, sizeof(odgovor));
    return tolower(odgovor[0]) == 'd';
}

/* File management */
void obrisi_datoteku(void) {
    if (potvrda_za_akciju("Jeste li sigurni da zelite obrisati datoteku?")) {
        if (remove(DATOTEKA) == 0) {
            printf("Datoteka '%s' obrisana.\n", DATOTEKA);
        } else {
            perror("Greska prilikom brisanja");
        }
    } else {
        printf("Brisanje otkazano.\n");
    }
}
