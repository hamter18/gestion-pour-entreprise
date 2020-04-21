#include "tp.h"

void test(void)
{
        FILE *fe;
        Article *tArt[200], a;
        Client *tabC, cl;
        Commande com;
        int num, nbArt, tmax, nb, choix, r, trouve, tmax1;
        char ch, ouiNon, c;
        Liste l;
	nbArt=chargementArt(tArt, 200);
	triDicoArticle(tArt, nbArt);
	tabC=chargementCli(fe,&tmax,&nb);
	triDicoClient(tabC,nb);
	l=remplirListe();
	num=choixMenu();
	while(num!=9)
	{
		switch(num)
		{
			case 1: 
				
				choix=choixMenu2();
				while(choix!=9)
				{
					switch(choix)
					{
						case 1:
							afficherTArt(tArt,nbArt);
							c=getchar();
							break;
						case 2: 
							nbArt=supprimerArticle(tArt, nbArt);
							afficherTArt(tArt,nbArt);
							printf("Le nombre restant d'articles est %d\n",nbArt);
							c=getchar();
							break;
						case 3:
							nbArt = infoArticle(tArt, nbArt);
							c=getchar();
							break;	
						case 4: 
							saisiePourModifArticle(tArt, nbArt);
							c=getchar();
							break;
						case 5 :
							afficheArtRuptStock(tArt,nbArt);
							c=getchar();
							break;
					}
					c=getchar();
					choix=choixMenu2();
				}
				break;
			case 2: 
				choix=choixMenu3();
				while(choix!=9)
				{
					switch(choix)
					{
						case 1: afficheTabCli(tabC,nb);
							c=getchar();
							break;
						case 2: 
							nb=infoClient( tabC, nb, tmax);
							c=getchar();
							break;
						case 3: 
							nb=supprimerClient(tabC, nb);
							printf("Le nombre de clients restant est %d \n", nb);
							c=getchar();
							break;
						case 4: 
							saisiePourModifClient(tabC, nb);
							c=getchar();
							break;

					}
					c=getchar();
					choix=choixMenu3();
				}
				break;

			case 3: 
				choix=choixMenu4();
				while(choix!=9)
				{
					switch(choix)
					{
						case 1:
							afficheCommande(l,stdout); /*stdout --> pour afficher à l'écran*/
							c=getchar();
							break;
						case 2: 
							saisieCommandeClient(tArt, nbArt, tabC, nb, l);
							c=getchar();
							break;
						case 3:
							supprimerCommandeEnCours(l);
							c=getchar();
							break;
						case 4:
							afficheTabCliCom(tabC,nb,l);
							c=getchar();
							break;
					}
					c=getchar();
					choix=choixMenu4();
				}
				break;	
				
		}
		c=getchar();
		num=choixMenu();

	}
sauvegardeArt(fe, tArt, nbArt);
//sauvegardeC(fe,tabC,nb);
sauvegarderCom(l,fe);
}


int main()
{
	test();
	return 0;
}

