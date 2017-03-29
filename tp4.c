#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tp4.h"
#define MAX 50

Benevole *nouveauBen(char *nom, char *prenom, int CIN, char sexe, int annee){
    Benevole *nouv = malloc(sizeof(Benevole));
    strcpy(nouv->nom, nom);
    strcpy(nouv->prenom, prenom);
    nouv->CIN = CIN;
    nouv->sexe = sexe;
    nouv->annee = annee;
    nouv->suivant = NULL;
    return nouv;
}

ListBenevoles *nouvelleListe(){
    ListBenevoles *liste = malloc(sizeof(ListBenevoles));
    liste->premier = NULL;
    liste->nbBenevoles = 0;
    return liste;
}

Tranche *nouvelleTranche(int borneSup){
    Tranche *nouv = malloc(sizeof(Tranche));
    nouv->borneSup = borneSup;
    ListBenevoles *liste = nouvelleListe();
    nouv->listeB = liste;
    nouv->pere = NULL;
    nouv->filsG = NULL;
    nouv->filsD = NULL;
    return nouv;
}



Tranche *ajoutTranche(Tranche *racine , int borneSup){
    //Création de la nouvelle tranche
    Tranche *nouv = nouvelleTranche(borneSup);

    //
    Tranche *parcours = racine;

    if (racine==NULL){
        racine = nouv;
        return nouv;
    }
    else{
            while (parcours !=NULL){
                    if (parcours->borneSup==borneSup){
                        free(nouv);
                        return NULL;
                    }
                    else if (parcours->borneSup >  borneSup){
                        if (parcours->filsG==NULL){
                            parcours->filsG=nouv;
                            nouv->pere=parcours;
                            return nouv;
                        } else parcours = parcours->filsG;
                    }
                    else if (parcours->borneSup < borneSup){
                        if (parcours->filsD==NULL){
                            parcours->filsD=nouv;
                            nouv->pere=parcours;
                            return nouv;
                        } else parcours = parcours->filsD;
                    }
            }
    }
    return nouv;
}

Benevole *insererBen(Tranche *racine , Benevole *benevole){
    int age=anneeActuelle() - benevole->annee;
    int borne;

    if(age%5==0)
        borne = age;
    else
        borne = 5*(age/5)+5;

    printf("age = %d, borne = %d\n\n", age, borne );
    if (age<0)
        return NULL;
    if (racine==NULL)
        return NULL;
    Tranche *parcours = racine;
    while (parcours!=NULL){
        if (age >= parcours->borneSup-4 && age <= parcours->borneSup){
            Benevole *parcoursB = parcours->listeB->premier;

            if (parcours->listeB->premier == NULL){
                parcours->listeB->premier = benevole;
                parcours->listeB->nbBenevoles++;
                return benevole;
            }
            else if (benevole->annee > parcours->listeB->premier->annee){
                benevole->suivant = parcours->listeB->premier;
                parcours->listeB->premier = benevole;
                parcours->listeB->nbBenevoles++;
                return benevole;
            }
            else {
                while (parcoursB!=NULL){

                    if (parcoursB->annee == benevole->annee && parcoursB->CIN == benevole->CIN){
                            printf("\n Le benevole est deja enregistre");
                            return NULL;
                    }

                    else if(parcoursB->suivant==NULL){
                        parcoursB->suivant = benevole ;
                        parcours->listeB->nbBenevoles++;
                        return benevole;
                    }
                    else if (parcoursB->annee <= benevole->annee){
                        parcoursB = parcoursB->suivant;
                    }


                    else if (parcoursB->annee > benevole->annee){
                        benevole->suivant = parcoursB->suivant ;
                        parcoursB->suivant= benevole;
                        parcours->listeB->nbBenevoles++;
                        return benevole;
                    }
                }
            }
        } else if(age < parcours->borneSup-4){
            if (parcours->filsG==NULL){
                ajoutTranche(parcours, borne);
            }
            parcours = parcours->filsG;
        } else if(age > parcours->borneSup){
            if (parcours->filsD==NULL){
                ajoutTranche(parcours, borne);
            }
            parcours = parcours->filsD;
        }
    }

}

Benevole *chercherBen(Tranche *racine ,int CIN, int annee){
    int age = anneeActuelle() - annee ;
    Tranche *parcours = racine;

    while(parcours!=NULL){
        if (age <= parcours->borneSup && age >= parcours->borneSup-4){
            Benevole *personne = parcours->listeB->premier ;
            while (personne!=NULL){
                if (personne->CIN == CIN && personne->annee == annee)
                    return personne;
                personne = personne->suivant;

            }
            if (personne==NULL)
                return NULL;
        }
        else if (age < parcours->borneSup-4 ){
            parcours = parcours->filsG;
        }
        else if (age > parcours->borneSup){
            parcours = parcours->filsD;
        }
    }
    if (parcours == NULL)
        return NULL;
}

int supprimerBen(Tranche *racine , int CIN, int annee){
    int age = anneeActuelle() - annee ;
    int res;
    Tranche *parcours = racine;

    while(parcours!=NULL){
        if (age <= parcours->borneSup && age >= parcours->borneSup-4){
            Benevole *personne = parcours->listeB->premier, *pPrecedent=NULL ;

            while (personne!=NULL){
                if (personne->CIN == CIN && personne->annee == annee){
                    if (pPrecedent==NULL){
                        parcours->listeB->premier = personne->suivant;
                        personne->suivant = NULL;
                    }
                    else{
                        pPrecedent->suivant = personne->suivant ;
                        personne->suivant = NULL;
                    }
                    free(personne);
                    parcours->listeB->nbBenevoles--;
                    if (parcours->listeB->nbBenevoles==0){
                        if (racine->borneSup!=parcours->borneSup){
                            res = supprimerTranche(racine, parcours->borneSup);
                            if (res==0)
                                printf("La tranche a ete supprimee");
                            else
                                printf("Erreur, la tranche n'a pas pu etre supprimee");
                            return 0;
                            }
                        else{
                            printf("On ne peut pas supprimer la racine");
                            return 0;
                        }
                    }
                    return 0;
                }
                pPrecedent = personne ;
                personne = personne->suivant;

            }
            if (personne==NULL)
                return 1;
        }
        else if (age < parcours->borneSup-4 ){
            parcours = parcours->filsG;
        }
        else if (age > parcours->borneSup){
            parcours = parcours->filsD;
        }
    }
    if (parcours == NULL)
        return 1;
}

int supprimerTranche(Tranche *racine , int borneSup){  //supprimer racine, racine ne garde pas NULL
    int res;
    Tranche *parcours = racine;
    if(parcours == NULL){
        printf("La tranche est inexistante");
        return 1;
    }

    while (parcours!=NULL){
        if (parcours->borneSup == borneSup){
            if (parcours->listeB->premier!=NULL){
                Benevole *bCourant = parcours->listeB->premier, *bPrecedent = NULL;

                while(parcours->listeB->nbBenevoles!=0){ //supprimer tous les benevoles
                    bPrecedent = bCourant ;
                    bCourant = bCourant->suivant;
                    bPrecedent->suivant = NULL;
                    free(bPrecedent);
                    parcours->listeB->nbBenevoles--;
                }
                parcours->listeB->premier = NULL;
            }

            if (parcours->listeB->nbBenevoles == 0){
                //suppression de la tranche

                    if (parcours->filsG == NULL && parcours->filsD==NULL){ //Si pas de fils
                            if (parcours->borneSup < parcours->pere->borneSup){
                            parcours->pere->filsG = NULL;
                            free(parcours->listeB);
                            free(parcours);
                            return 0;
                        }
                        else{
                            parcours->pere->filsD = NULL;
                            free(parcours->listeB);
                            free(parcours);
                            return 0;
                        }
                    }
                    else if (parcours->filsG == NULL){
                        parcours->pere->filsD = parcours->filsD;
                        parcours->filsD->pere = parcours->pere;
                        free(parcours->listeB);
                        free(parcours);
                        return 0;
                    }
                    else if (parcours->filsD == NULL){
                        parcours->pere->filsG = parcours->filsG;
                        parcours->filsG->pere = parcours->pere;
                        free(parcours->listeB);
                        free(parcours);
                        return 0;
                    }
                    else{   //si il a un fils gauche et un fils droit
                        Tranche *nTrch = successeur(parcours);  //nTrch est le successeur de la tranche parcours
                        int bS = nTrch->borneSup;
                        ListBenevoles *tmp = nTrch->listeB;

                        res = supprimerTranche(racine, nTrch->borneSup);
                        parcours->borneSup = bS;
                        parcours->listeB = tmp;
                        if (res == 0)
                            return 0;
                        else
                            return 1;
                    }
                }
        }

        else if (parcours->borneSup > borneSup)
            parcours = parcours->filsG ;

        else if (parcours->borneSup < borneSup)
            parcours = parcours->filsD;

    }

}

Tranche *successeur(Tranche *trch){
    Tranche *parcours = trch;
    if (parcours->filsD!=NULL){
        parcours = parcours->filsD;
        while(parcours->filsG!=NULL)
            parcours = parcours->filsG;
        return parcours;
    }
    else{
        Tranche *ptr = parcours->pere;
        while (ptr!=NULL && parcours==ptr->filsD){
            parcours = ptr;
            ptr = ptr->pere;
        }
        return ptr;
    }
}

ListBenevoles *BenDhonneur(Tranche *racine){
    Tranche *parcours = racine ;
    ListBenevoles *nouvListe = malloc(sizeof(ListBenevoles));
    int anneeMax=3000;
    if (racine==NULL){
        printf("Pas d'arbre\n");
        return NULL;
    }

    while (parcours->filsD!=NULL){      //parcours prend la tranche la plus vieille
        parcours = parcours->filsD;
    }

    while (parcours->listeB->nbBenevoles == 0){  //on remonte dans l'ABR s'il n'y a pas de bénévoles
        parcours = parcours->pere;
        if (parcours==NULL){
            printf("Pas de benevole dans l'arbre");
            return NULL;
        }
    }

    ListBenevoles *pListe = parcours->listeB;
    Benevole *pCourant = pListe->premier;

    while (pCourant!=NULL){             //on cherche le plus vieux
        anneeMax = pCourant->annee;
        pCourant=pCourant->suivant;
    }

    pCourant = pListe->premier;         //on repart du début de la liste

    while (pCourant->annee!=anneeMax){
        pCourant = pCourant->suivant;
    }
    nouvListe->premier = pCourant;
    return nouvListe;
}

Pile *creerPile(){
    Pile *p = malloc(sizeof(Pile));
    p->taille = 0;
    p->tete = NULL;
    return p;
}

void empiler(Tranche *trch, Pile *p){
    Element *empile = malloc(sizeof(Element));
    empile->tranche = trch;
    if (trch==NULL)
        return;
    else{
        if (p->tete==NULL){
            p->tete = empile;
            empile->suivant = NULL;
            p->taille++;
        }
        else{

            empile->suivant = p->tete;
            p->tete = empile;
            p->taille++;
        }
    }
}

int pileVide(Pile *p){
    if (p->tete==NULL)
        return 1;
    else
        return 0;
}

Tranche *depiler(Pile *p){
    if (!pileVide(p)){
        Tranche *aDepiler = p->tete->tranche;
        Element *ptr = p->tete;
        if(p->taille==0)
            p->tete = NULL;
        else
            p->tete = p->tete->suivant;
        free(ptr);         // NE PAS OUBLIER DE LIBERER L'ESPACE MEMOIRE
        p->taille--;
        return aDepiler;
    }
    return NULL;
}


int actualiser(Tranche *racine){

    int nombre=0;
    if (racine!=NULL){
        Pile *p = creerPile();
        empiler(racine, p);
        while(!pileVide(p)){
            Tranche *t = depiler(p);
            Benevole *parcours = t->listeB->premier, *pPrecedent = NULL, *pSauv = NULL;
            while(parcours!=NULL){
                if (anneeActuelle()-parcours->annee > t->borneSup){ //on choisit de ne pas déplacer tous ceux dont l'age est supérieur car sinon compliquer pour trier dans la nouvelle liste
                    if (pPrecedent==NULL){
                        t->listeB->premier = parcours->suivant;
                        pSauv = parcours->suivant;
                        parcours->suivant = NULL;
                    }
                    else{
                        pPrecedent->suivant = parcours->suivant ;
                        pSauv = parcours->suivant;
                        parcours->suivant = NULL;
                    }
                    t->listeB->nbBenevoles--;       //car on incrémente le nombre de bénévole dans insererBen
                    /*if (t->listeB->nbBenevoles==0){
                            if (racine->borneSup == t->borneSup)
                                printf("On ne peut pas supprimer la racine\n");
                            else
                                supprimerTranche(racine, t->borneSup);
                    }*/
                    insererBen(racine, parcours);
                    //insererBen(racine, ptr);
                    parcours = pSauv;
                    nombre++;
                }
                else {
                    pPrecedent = parcours;
                    parcours = parcours->suivant;
                }
            }
            if (t->filsG != NULL)
                empiler(t->filsG, p);
            if (t->filsD != NULL)
                empiler(t->filsD, p);
        }
        free(p);
        return nombre;
    }
    return nombre;
}

int totalBenTranche(Tranche *racine, int borneSup){ //on suppose que la borneSup existe bien dans l'ABR
    Tranche *parcours = racine;
    if (parcours == NULL)
        return -1;
    while(parcours!=NULL){
        if (parcours->borneSup == borneSup){
            return parcours->listeB->nbBenevoles;
        }
        else if (parcours->borneSup > borneSup){
            parcours = parcours->filsG;
        }
        else if (parcours->borneSup < borneSup){
            parcours = parcours->filsD;
        }
    }
    return -1;
}

int totalBen(Tranche *racine){
    int nombre=0;
    if (racine!=NULL){
        Pile *p = creerPile();
        empiler(racine, p);
        while(!pileVide(p)){
            Tranche *t = depiler(p);
            nombre+= t->listeB->nbBenevoles;
            if (t->filsG != NULL)
                empiler(t->filsG, p);
            if (t->filsD != NULL)
                empiler(t->filsD, p);
        }
        return nombre;
    }
    return nombre;
}

float pourcentageTranche(Tranche *racine, int borneSup){
    float res = 0;
    if (totalBen(racine)!=0 && totalBenTranche(racine, borneSup)!=-1){
    //printf("le nombre total dans cette tranche est %d\n", totalBenTranche(racine, borneSup));
    res = (float) totalBenTranche(racine, borneSup)/totalBen(racine)*100;
    return res;
    }
    else
        printf("Le nombre total de benevole est de 0");
    return 0;
}

void afficherTranche(Tranche *racine, int borneSup){
    Tranche *parcours = racine ;
    while (parcours!=NULL){
        if (parcours->borneSup == borneSup){
            Benevole *parcoursB = parcours->listeB->premier;
            printf("\t \t Liste des benevoles de la tranche d age %d\n\n", borneSup);
            printf("| %-10s | %-10s | %-4s | %-10s | %-10s |\n", "Nom", "Prenom", "Sexe", "Annee", "CIN");
            while(parcoursB!=NULL){
                printf("| %-10s | %-10s | %-4c | %-10d | %-10d |\n", parcoursB->nom, parcoursB->prenom, parcoursB->sexe, parcoursB->annee, parcoursB->CIN);
                parcoursB = parcoursB->suivant;
            }
            return;
        }
        else if (parcours->borneSup > borneSup)
            parcours = parcours->filsG;
        else if (parcours->borneSup < borneSup)
            parcours = parcours->filsD;
    }
    printf("La tranche n a pas ete trouvee\n");
}

void afficherArbre(Tranche *racine){ //parcours infixe (filsG, x, fils D)
    if (racine!=NULL){
        Pile *p = creerPile();
        Tranche *ptr = racine;
        int fin = 0;
        printf("\t\t Liste des tranches d age\n\n");
        while(!fin){
            while(ptr!=NULL){  //ptr pas nul wtf
                empiler(ptr, p);
                ptr = ptr->filsG;
            }
            if(pileVide(p))
                fin = 1;
            else{
                Tranche *t = depiler(p);
                printf("|%-10d|\n", t->borneSup);
                ptr = t->filsD;
            }
        }
    }
    else
        printf("Pas d'arbre");
}

void supprimerArbre(Tranche *racine){
    Tranche *parcours = racine;
    Pile *p = creerPile();
    if (racine==NULL)
        return;
    else{
        empiler(parcours, p);
        while (!pileVide(p)){
            Tranche *t = depiler(p);
            if(t->filsD!=NULL){
                empiler(t->filsD, p);
            }
            if (t->filsG!=NULL){
                empiler(t->filsG, p);
            }
            if (t == racine){
                racine->filsD=NULL;
                racine->filsG=NULL;
                Benevole *bCourant = racine->listeB->premier, *bPrecedent = NULL;
                while (racine->listeB->nbBenevoles!=0){
                    bPrecedent = bCourant ;
                    bCourant = bCourant->suivant;
                    bPrecedent->suivant = NULL;
                    free(bPrecedent);
                    parcours->listeB->nbBenevoles--;
                }
                racine->listeB=NULL;
                free(racine->listeB);
            }
            else{
                supprimerTranche(racine, t->borneSup);
            }
        }
        free(racine);
        return;
    }
}


int anneeActuelle(){
    time_t secondes;
    struct tm instant;
    time(&secondes);
    instant = *localtime(&secondes);
    return instant.tm_year + 1900;
}



