#ifndef TP4_H_INCLUDED
#define TP4_H_INCLUDED

typedef struct benevole{
    char nom[20];
    char prenom[20];
    char sexe;
    int annee;
    int CIN;
    struct benevole *suivant;
}Benevole;

typedef struct ListBenevoles{
    Benevole *premier;
    int nbBenevoles;
}ListBenevoles;

typedef struct tranche{
    ListBenevoles *listeB;
    int borneSup;
    struct tranche *pere;
    struct tranche *filsG;
    struct tranche *filsD;
}Tranche;

typedef struct Element{
    Tranche *tranche;
    struct Element *suivant;
}Element;

typedef struct Pile{
    int taille;
    Element *tete;
}Pile;

Benevole *nouveauBen(char *nom, char *prenom , int CIN, char sexe, int annee);
Tranche *nouvelleTranche(int borneSup);
ListBenevoles *nouvelleListe();
Tranche *ajoutTranche(Tranche *racine , int borneSup);
Benevole *insererBen(Tranche *racine , Benevole *benevole);
Benevole *chercherBen(Tranche *racine ,int CIN, int annee);
int supprimerBen(Tranche *racine , int CIN, int annee);
int supprimerTranche(Tranche *racine , int borneSup);
Tranche *successeur(Tranche *trch);
ListBenevoles *BenDhonneur(Tranche *racine);
int actualiser(Tranche *racine);
int totalBenTranche(Tranche *racine, int borneSup);
int totalBen(Tranche *racine);
float pourcentageTranche(Tranche *racine, int borneSup);
void afficherTranche(Tranche *racine, int borneSup);
void afficherArbre(Tranche *racine);
void supprimerArbre(Tranche *racine);

//struct Pile
Pile *creerPile();
void empiler(Tranche *trch, Pile *p);
Tranche *depiler(Pile *p);
int pileVide(Pile *p);
//

int anneeActuelle();



#endif // TP4_H_INCLUDED
