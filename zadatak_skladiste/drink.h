
#ifndef DRINK_H
#define DRINK_H

typedef struct {
    char naziv[50];
    float kolicina;
    float cijena;
} Pice;

void dodajPice(Pice **pica, int *broj);
void ispisiPica(Pice *pica, int broj);
void azurirajPice(Pice *pica, int broj);
void obrisiPice(Pice **pica, int *broj);
void sortirajPica(Pice *pica, int broj, int sortirajPoCijeni);
void spremiUDatoteku(Pice *pica, int broj);
void ucitajIzDatoteke(Pice **pica, int *broj);

#endif
