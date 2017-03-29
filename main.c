#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp4.h"
#define MAX 50

int main()
{
    int choix, sortir=0;
    int bSup, annee, cin, age, res;
    char nom[MAX], prenom[MAX], sexe;

    while (sortir==0){
        printf("\n\n\t\t\t***Menu***\n\n");
        printf("1. Initialiser un arbre\n2. Ajouter une tranche\n3. Ajouter un benevole dans une tranche d age\n4. Afficher les tranches d age d un ABR\n5. Afficher les benevoles d une tranche d age\n6. Supprimer un benevole\n7. Supprimer une tranche\n8. Afficher les benevoles d honneur\n9. Afficher le nombre total de benevoles\n10. Actualiser l ABR\n11. Afficher le pourcentage de benevoles d une tranche d age\n12. Detruire l arbre\n13. Quitter\n\n");
        scanf("%d", &choix);
        switch(choix){

        case 1 :
            printf("\t\t\t***Initialiser un arbre***\n\n");
            printf("Entrez la tranche d'arbre\n");
            scanf("%d", &bSup);
            Tranche *racine = nouvelleTranche(bSup);
            break;

        case 2 :
            printf("\t\t\t***Ajouter une tranche***\n\n");
            printf("Entrez la tranche d'arbre\n");
            scanf("%d", &bSup);
            if (bSup%5==0)
                ajoutTranche(racine, bSup);
            else
                printf("Veuillez entrer un multiple de 5");
            break;

        case 3 :
            printf("\t\t\t***Ajouter un benevole***\n\n");
            printf("Quel est le nom du benevole ?\n");
            scanf("%s", nom);
            printf("Quel est le prenom du benevole ? \n");
            scanf("%s", prenom);
            printf("Quel est le sexe du benevole ? ");
            char c;
            while((c = getchar()) != EOF && c != '\n');
            scanf("%c", &sexe);
            printf("Quel est son annee de naissance ?");
            scanf("%d", &annee);
            printf("Quel est son CIN ?");
            scanf("%d", &cin);
            Benevole *ben = nouveauBen(nom, prenom, cin, sexe, annee);
            Benevole *nBen = insererBen(racine, ben);
            if (nBen!=NULL)
                printf("Le benevole %s %s a bien ete ajoute\n\n", nBen->prenom, nBen->nom);

            break;

        case 4 :
            printf("\t\t\t***Afficher l ABR***\n\n");
            afficherArbre(racine);
            break;

        case 5 :
            printf("\t\t\t***Afficher une tranche d age***\n\n");
            printf("Quelle tranche d age voulez-vous afficher ?\n\n ");
            scanf("%d", &age);
            afficherTranche(racine, age);
            break;

        case 6 :
            printf("\t\t***Supprimer un benevole***\n\n");
            printf("Entrez le CIN du benevole a supprimer\n");
            scanf("%d", &cin);
            printf("Entrez l annee de naissance du benevole a supprimer\n");
            scanf("%d", &annee);
            res = supprimerBen(racine, cin, annee);
            if(res==0)
                printf("Le benevole a bien ete supprime\n");
            else
                printf("Le benevole n a pas pu etre supprime\n");
            break;

        case 7 :
            printf("\t\t***Supprimer une tranche***\n\n");
            printf("Quelle tranche voulez-vous supprimer ? \n");
            scanf("%d", &bSup);
            /*if (bSup==racine->borneSup){
                res = supprimerTranche(racine, bSup);
                racine = NULL;          // Car lorsquon met racine à null dans la fonction ca ne marche pas
            }
            else*/
            if (bSup == racine->borneSup)
                printf("La racine ne peut pas etre supprimee");
            else {
                res = supprimerTranche(racine, bSup);

                if(res==0)
                    printf("\nLa tranche a bien ete supprimee\n");
                else
                    printf("\nLa tranche n a pas pu etre supprimee\n");
            }
            break;

        case 8 :
            printf("\t\t\t***Benevoles d honneur***\n\n");
            ListBenevoles *liste = BenDhonneur(racine);
           /* if (liste->premier==NULL)
                printf("Erreur");
            else {*/
            if (liste == NULL)
                printf("Error\n");
            else {
                Benevole *parcours = liste->premier;

                printf("| %-10s | %-10s | %-4s | %-5s | %-10s |\n", "Nom", "Prenom", "Sexe", "Annee", "CIN");
                while(parcours!=NULL){
                    printf("| %-10s | %-10s | %-4c | %-5d | %-10d |\n", parcours->nom, parcours->prenom, parcours->sexe, parcours->annee, parcours->CIN);
                    parcours = parcours->suivant;
                }
            }
            break;

        case 9 :
            printf("\t\t\t***Nombre total de benevoles***\n\n");
            printf("Le nombre total de benevoles est de %d\n\n", totalBen(racine));
            break;

        case 10 :
            printf("\t\t\t***Actualiser l ABR***\n\n");
            res = actualiser(racine);
            printf("%d personnes ont ete deplacees\n\n", res);
            break;

        case 11 :
            printf("\t\t\t***Pourcentage de benevoles d une tranche d age***\n\n");
            printf("Entrez la tranche d age a etudier\n");
            scanf("%d", &bSup);
            if (pourcentageTranche(racine, bSup)==-1)
                printf("Le calcul n est pas possible\n");
            else
                printf("%f %% des benevoles sont de la tranche d age %d\n  ", pourcentageTranche(racine, bSup), bSup);
            break;

        case 12 :
            printf("\t\t\t***Detruire l arbre***\n\n");
            supprimerArbre(racine);
            free(racine);
            racine = NULL;
            printf("L'arbre a ete supprime");
            break;

        case 13 :
            sortir = 1;
            break;
        }
    }
    return 0;
}
