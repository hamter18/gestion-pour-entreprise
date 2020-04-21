#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct
	{
		char idArticle[6];
		float prixU;
		int quantEnStock;
		char denomination[40];
	}Article;

typedef struct
	{
		char idClient[10];
		int codePostal;
		char civilite[4];
		char prenom[20];
		char nom[20];
		char ville[30];
	}Client;

typedef struct
	{
		char idCommande[6];
		char idArticle[6];
		char idClient[10];
		int quantNonLiv;
	}Commande;
	
typedef struct maillon
	{
		Commande com;
		struct maillon * suiv;
	} Maillon,* Liste;



Article lireArt(FILE *fe);
int chargementArt(Article *tArt[], int max);
void afficherArt(Article a);
void afficherTArt(Article *tArt[],int max);

int rechDicoArticle(char *IDarticle, Article *tArt[], int nbArt, int *trouve);

void triDicoArticle(Article *tArt[], int nbArt);
void copierArticle(Article *tArt[],int i, int j, Article *R[]);
void fusionArticle(Article *R[], int n,Article *S[], int m,Article *tArt[]);

int supprimerArticle(Article *tArt[], int nb);

void formationIDarticle( char designation[], char Idarticle[], Article *tArt[],int nbArt);
int infoArticle (Article *tArt[], int nbArt);
int ajouterArticle( char IDarticle[], char designation[], float prix, int quantite, Article *tArt[], int nbArt);

void saisiePourModifArticle(Article *tArt[], int nbArt);
int modifDenominationArt( Article *tArt[], int nbArt, int i);
int modifQuantiteArt(int quantite, Article *tArt[], int nbArt, int i);
int modifPrixArt(float prix, Article *tArt[], int nbArt, int i);

void afficheArtRuptStock(Article *tArt[], int nbArt);

void sauvegardeArt(FILE *fe,Article *tArt[], int max);

Client lireClient(FILE *fe);
Client* chargementCli(FILE *fe,int *tmax, int *nb);

void afficherCli(Client c);
void afficheTabCli(Client tabC[], int tmax);
void afficheTabCliCom(Client tabC[], int tmax,Liste l);

int rechDich(char *idClient, Client *tabC, int nbcli, int *trouve);
void triDicoClient(Client tabC[], int nbcli);
void copierClient(Client tabC[],int i, int j, Client R[]);
void fusionClient(Client R[], int n,Client S[], int m,Client tabC[]);

void formationIDclient( char nom[], char prenom[], char IDclient[], Client tabC[],int nbcli);
int infoClient(Client tabC[],int nbcli, int tmax);
int ajouterClient( char civilite[], char nom[], char prenom[], char ville[], char IDclient[],int codePostal, Client tabC[], int nbcli, int tmax);

int supprimerClient(Client tabC[], int nb);

void saisiePourModifClient(Client tabC[], int nb);
int mofificationClient(char IDclient[], char newVille[],int newCodePostal, Client tabC[], int nb);

void sauvegardeC(FILE *fe,Client tabC[], int nb);

Commande lireCommande(FILE *flot);
Liste remplirListe(void);

Liste listeVide(void);
Liste ajouter(Liste l,Commande c);
Liste ajouter(Liste l,Commande c);
Liste ajouterEnQueue(Liste l,Commande c);

Liste supprimerEnTete(Liste l);
Liste supprimer(Liste l,char idCommande[]);

int longeurListe(Liste l);

void afficheCommande(Liste l,FILE *fe);
Commande nouvelleCommande(void);

Liste rechercherComClient(Liste list,Client c);

int rechDichCommande(char *idCommande,Liste l, int nb, int *trouve);
void formationIDCommande( char IDarticle[], char IDclient[], char IDCommande[], int nb);
void saisieCommandeClient(Article *tArt[], int nbArt, Client tabC[], int nb, Liste l);
int miseAjour(Article *tArt[], int nbArt, char IDcommande[], char IDarticle[], char IDclient[],int quantiteArt, int r, Liste l);

void supprimerCommandeEnCours(Liste l);

void sauvegarderCom(Liste l,FILE *fe);

void supprimeBlanc(char source[], char dest[]);
void partieGauche(char source[], char dest[], int p);
void assemblerChaines( char source1[], char source2[], char dest[]);


void afficherMenu(void);
int choixMenu(void);

void afficherMenu2(void);
int choixMenu2(void);

void afficherMenu3(void);
int choixMenu3(void);

void afficherMenu4(void);
int choixMenu4(void);




