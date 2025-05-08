#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include "drink.h"


void sigurni_input(char *buffer, size_t velicina) {
    if (fgets(buffer, (int)velicina, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0'; 
    } else {
        clearerr(stdin);
        buffer[0] = '\0';
    }
}


int meni_izbor(void) {
    char buffer[16];
    int izbor = 0;
    sigurni_input(buffer, sizeof(buffer));
    izbor = atoi(buffer);
    return izbor;
}


void replace_comma_with_dot(char *str) {
    for (; *str; str++) {
        if (*str == ',') {
            *str = '.';
        }
    }
}

void dodaj_pice(Pice **pica, int *broj) {
    Pice *temp = realloc(*pica, ((*broj) + 1) * sizeof(Pice));
    if (temp == NULL) {
        perror("Greska kod alokacije memorije");
        return;
    }
    *pica = temp;

    printf("Unesite naziv pica: ");
    sigurni_input((*pica)[*broj].naziv, NAZIV_MAX_DUZINA);

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

void ispisi_pica(const Pice * const pica, int broj) {
    if (broj == 0) {
        printf("Nema dostupnih pica za ispis.\n");
        return;
    }
    printf("\nPopis pica:\n");
    for (int i = 0; i < broj; i++) {
        printf("%d. Naziv: %s, Kolicina: %.2lf, Cijena: %.2lf\n",
               i + 1, pica[i].naziv, pica[i].kolicina, pica[i].cijena);
    }
}

void azuriraj_pice(Pice * const pica, int broj) {
    if (broj == 0) {
        printf("Nema pica za azurirati.\n");
        return;
    }

    char naziv[NAZIV_MAX_DUZINA];
    printf("Unesite naziv pica za azuriranje: ");
    sigurni_input(naziv, sizeof(naziv));

    for (int i = 0; i < broj; i++) {
        if (strcmp(pica[i].naziv, naziv) == 0) {
            printf("Unesite novu kolicinu: ");
            char buffer[64];
            sigurni_input(buffer, sizeof(buffer));
            replace_comma_with_dot(buffer);
            pica[i].kolicina = strtod(buffer, NULL);

            printf("Unesite novu cijenu: ");
            sigurni_input(buffer, sizeof(buffer));
            replace_comma_with_dot(buffer);
            pica[i].cijena = strtod(buffer, NULL);

            printf("Pice je uspjesno azurirano.\n");
            return;
        }
    }
    printf("Pice nije pronadeno!\n");
}

void obrisi_pice(Pice **pica, int *broj) {
    if (*broj == 0) {
        printf("Nema pica za brisanje.\n");
        return;
    }

    char naziv[NAZIV_MAX_DUZINA];
    printf("Unesite naziv pica za brisanje: ");
    sigurni_input(naziv, sizeof(naziv));

    for (int i = 0; i < *broj; i++) {
        if (strcmp((*pica)[i].naziv, naziv) == 0) {
            for (int j = i; j < *broj - 1; j++) {
                (*pica)[j] = (*pica)[j + 1];
            }
            if (*broj - 1 > 0) {
                Pice *temp = realloc(*pica, (*broj - 1) * sizeof(Pice));
                if (temp == NULL) {
                    perror("Greska kod realokacije memorije prilikom brisanja");
                    return;
                }
                *pica = temp;
            } else {
                free(*pica);
                *pica = NULL;
            }
            (*broj)--;
            printf("Pice je uspjesno obrisano.\n");
            return;
        }
    }
    printf("Pice nije pronadeno!\n");
}

void spremi_u_datoteku(const Pice * const pica, int broj) {
    FILE *file = fopen("pica.txt", "w");
    if (file == NULL) {
        perror("Ne mogu otvoriti datoteku za spremanje");
        return;
    }
    for (int i = 0; i < broj; i++) {
        fprintf(file, "%s %.2lf %.2lf\n", pica[i].naziv, pica[i].kolicina, pica[i].cijena);
    }
    if (fclose(file) != 0) {
        perror("Greska kod zatvaranja datoteke");
        return;
    }
    printf("Podaci su uspjesno spremljeni u datoteku 'pica.txt'.\n");
}

void ucitaj_iz_datoteke(Pice **pica, int *broj) {
    FILE *file = fopen("pica.txt", "r");
    if (file == NULL) {
        if (errno == ENOENT) {
            fprintf(stderr, "Datoteka 'pica.txt' ne postoji. Pocetni podaci nisu ucitani.\n");
        } else {
            perror("Greska prilikom otvaranja datoteke za ucitavanje");
        }
        return;
    }

    Pice temp;
    int temp_broj = 0;
    Pice *temp_pica = NULL;
    int neispravni_unosi = 0;

    while (1) {
        int rezultat = fscanf(file, "%49s %lf %lf", temp.naziv, &temp.kolicina, &temp.cijena);
        if (rezultat == EOF) break;
        if (rezultat != 3) {
            fprintf(stderr, "Nepravilan format u datoteci 'pica.txt'. Preskacem ovu liniju.\n");
            while (fgetc(file) != '\n' && !feof(file));
            neispravni_unosi++;
            continue;
        }
        Pice *novi_niz = realloc(temp_pica, (temp_broj + 1) * sizeof(Pice));
        if (novi_niz == NULL) {
            perror("Greska kod realokacije memorije kod ucitavanja datoteke");
            free(temp_pica);
            fclose(file);
            return;
        }
        temp_pica = novi_niz;
        temp_pica[temp_broj] = temp;
        temp_broj++;
    }

    fclose(file);

    if (*pica != NULL) {
        free(*pica);
        *pica = NULL;
        *broj = 0;
    }

    *pica = temp_pica;
    *broj = temp_broj;

    if (neispravni_unosi > 0) {
        fprintf(stderr, "Ucitano je %d ispravnih pica, ali %d neispravnih unosa je preskoceno.\n", temp_broj, neispravni_unosi);
    } else {
        printf("Podaci su uspjesno ucitani iz datoteke 'pica.txt'.\n");
    }
}

int usporedi_po_imenu(const void *a, const void *b) {
    const Pice *pa = (const Pice*)a;
    const Pice *pb = (const Pice*)b;
    return strcmp(pa->naziv, pb->naziv);
}

int usporedi_po_cijeni(const void *a, const void *b) {
    const Pice *pa = (const Pice*)a;
    const Pice *pb = (const Pice*)b;
    if (pa->cijena < pb->cijena) return -1;
    if (pa->cijena > pb->cijena) return 1;
    return 0;
}

int usporedi_po_kolicini(const void *a, const void *b) {
    const Pice *pa = (const Pice*)a;
    const Pice *pb = (const Pice*)b;
    if (pa->kolicina < pb->kolicina) return -1;
    if (pa->kolicina > pb->kolicina) return 1;
    return 0;
}

void sortiraj_pica(Pice * const pica, int broj, int kriterij) {
    if (broj <= 1) {
        printf("Nema dovoljno pica za sortiranje.\n");
        return;
    }
    switch (kriterij) {
        case 1: 
            qsort(pica, broj, sizeof(Pice), usporedi_po_imenu);
            printf("Pica su uspjesno sortirana po imenu.\n");
            break;
        case 2: 
            qsort(pica, broj, sizeof(Pice), usporedi_po_cijeni);
            printf("Pica su uspjesno sortirana po cijeni.\n");
            break;
        case 3: 
            qsort(pica, broj, sizeof(Pice), usporedi_po_kolicini);
            printf("Pica su uspjesno sortirana po kolicini.\n");
            break;
        default:
            printf("Neispravan kriterij sortiranja!\n");
            break;
    }
}

void pretrazi_pica(const Pice * const pica, int broj) {
    if (broj == 0) {
        printf("Nema pica za pretrazivanje.\n");
        return;
    }
    printf("Pretrazivanje pica po:\n1. Imenu\n2. Cijeni\n3. Kolicini\nUnesite izbor: ");
    int kriterij = meni_izbor();

    char naziv_trazi[NAZIV_MAX_DUZINA];
    char buffer[64];
    double broj_trazi, epsilon = 0.01;
    int pronadeno = 0;

    switch (kriterij) {
        case 1: 
            printf("Unesite naziv pica za pretragu: ");
            sigurni_input(naziv_trazi, sizeof(naziv_trazi));
            for (int i = 0; i < broj; i++) {
                if (strcmp(pica[i].naziv, naziv_trazi) == 0) {
                    if (!pronadeno) printf("Rezultati pretrage:\n");
                    printf("%d. Naziv: %s, Kolicina: %.2lf, Cijena: %.2lf\n",
                           i + 1, pica[i].naziv, pica[i].kolicina, pica[i].cijena);
                    pronadeno++;
                }
            }
            break;
        case 2: 
            printf("Unesite cijenu za pretragu: ");
            sigurni_input(buffer, sizeof(buffer));
            replace_comma_with_dot(buffer);
            broj_trazi = strtod(buffer, NULL);
            for (int i = 0; i < broj; i++) {
                if (fabs(pica[i].cijena - broj_trazi) < epsilon) {
                    if (!pronadeno) printf("Rezultati pretrage:\n");
                    printf("%d. Naziv: %s, Kolicina: %.2lf, Cijena: %.2lf\n",
                           i + 1, pica[i].naziv, pica[i].kolicina, pica[i].cijena);
                    pronadeno++;
                }
            }
            break;
        case 3: 
            printf("Unesite kolicinu za pretragu: ");
            sigurni_input(buffer, sizeof(buffer));
            replace_comma_with_dot(buffer);
            broj_trazi = strtod(buffer, NULL);
            for (int i = 0; i < broj; i++) {
                if (fabs(pica[i].kolicina - broj_trazi) < epsilon) {
                    if (!pronadeno) printf("Rezultati pretrage:\n");
                    printf("%d. Naziv: %s, Kolicina: %.2lf, Cijena: %.2lf\n",
                           i + 1, pica[i].naziv, pica[i].kolicina, pica[i].cijena);
                    pronadeno++;
                }
            }
            break;
        default:
            printf("Neispravan izbor za pretragu!\n");
            return;
    }

    if (!pronadeno) {
        printf("Nema rezultata pretrage za zadani kriterij.\n");
    }
}

int potvrda_za_akciju(const char *poruka) {
    char odgovor[8];
    printf("%s (d/n): ", poruka);
    sigurni_input(odgovor, sizeof(odgovor));
    if (tolower((unsigned char)odgovor[0]) == 'd') {
        return 1;
    }
    return 0;
}
