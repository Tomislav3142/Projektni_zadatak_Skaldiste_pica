#ifndef DRINK_H
#define DRINK_H

#define NAZIV_MAX_DUZINA 50
#define DATOTEKA "pica.txt"

typedef struct {
    char naziv[NAZIV_MAX_DUZINA];
    double kolicina;
    double cijena;
} Pice;

/* Core functions */
void dodaj_pice(Pice **pica, int *broj);
void ispisi_pica(const Pice *pica, int broj);
void azuriraj_pice(Pice *pica, int broj);
void obrisi_pice(Pice **pica, int *broj);
void spremi_u_datoteku(const Pice *pica, int broj);
void ucitaj_iz_datoteke(Pice **pica, int *broj);
void sortiraj_pica(Pice *pica, int broj, int kriterij);
void pretrazi_pica(const Pice *pica, int broj);
void obrisi_datoteku(void);

/* Helper functions */
int potvrda_za_akciju(const char *poruka);
int meni_izbor(void);
void sigurni_input(char *buffer, size_t velicina);
void replace_comma_with_dot(char *str);
void to_lowercase(char *str);

/* File operations */
int provjeri_postojanje_datoteke(const char *filename);
long velicina_datoteke(const char *filename);

#endif
