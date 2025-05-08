#ifndef DRINK_H
#define DRINK_H

#define NAZIV_MAX_DUZINA 50

typedef struct {
    char naziv[NAZIV_MAX_DUZINA];
    double kolicina;
    double cijena;
} Pice;

/* Funkcije za unos, ispis, azuriranje, brisanje, spremanje i ucitavanje pica */
void dodaj_pice(Pice **pica, int *broj);
void ispisi_pica(const Pice * const pica, int broj);
void azuriraj_pice(Pice * const pica, int broj);
void obrisi_pice(Pice **pica, int *broj);
void spremi_u_datoteku(const Pice * const pica, int broj);
void ucitaj_iz_datoteke(Pice **pica, int *broj);
void sortiraj_pica(Pice * const pica, int broj, int kriterij);
void pretrazi_pica(const Pice * const pica, int broj);

/* Pomoæne funkcije */
int potvrda_za_akciju(const char *poruka);
int meni_izbor(void);
void sigurni_input(char *buffer, size_t velicina);
void replace_comma_with_dot(char *str);

#endif // DRINK_H
