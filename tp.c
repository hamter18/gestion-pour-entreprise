#include "tp.h"


Article lireArt(FILE *fe)
{
	Article a;
	fscanf(fe,"%s",a.idArticle);	
	fscanf(fe,"%f%d%*c",&a.prixU,&a.quantEnStock);
	fgets(a.denomination,40,fe);
	a.denomination[strlen(a.denomination)-1]='\0';
	return a;
}


int  chargementArt(Article *tArt[], int max)
{
	FILE *fe;
	Article a;
	int i=0;
	fe=fopen("articles.txt","r");
        if(fe==NULL)
        {
		printf("Un problème est survenu lors de l'ouverture du fichier en lecture\n");
		return -1;
        }
	a=lireArt(fe);
	while(!feof(fe))
		{
			if(i==max)
				{
					printf("Le tableau est plein, veuillez l'agrandir\n");
					return -2;
				}
			tArt[i]=(Article*) malloc (sizeof(Article));
			if(tArt[i]==NULL)
			{	printf("Un problème de malloc est survenu\n");
				return -1;
			}
			*tArt[i]=a;
			i=i+1;
			a=lireArt(fe);
		}
	fclose(fe);
	return i;
}


void afficherArt(Article a)
{
	printf("%s\t\t|\t%.2f\t|\t%d\t\t|\t%s\n", a.idArticle,a.prixU,a.quantEnStock,a.denomination);
}


void afficherTArt(Article *tArt[],int max)
{
	int i;
	system("clear");
	printf("\t\t-----------------------Tableau d'articles : ----------------------- \n");
	printf("Identifiant\t\tPrix\t\tQuantité\t\tDénomination\n");
	for(i=0;i<max;i++)
		afficherArt(*tArt[i]);
	printf("\t\t\t------------------------------------------\n");
}


int rechDicoArticle(char *IDarticle, Article *tArt[], int nbArt, int *trouve)
{
	int inf=0, sup=nbArt-1, m;
	while(inf<=sup)
		{
			m=(inf+sup)/2;
			if(strcmp(IDarticle,tArt[m]->idArticle)==0)
				{
					*trouve=1;
					return m;
				}
			if(strcmp(IDarticle,tArt[m]->idArticle)<0)
				sup=m-1;
			else inf=m+1;
		}
	*trouve=0;
	return inf;
}

void triDicoArticle(Article *tArt[], int nbArt)
{
	Article **R,**S;
	if(nbArt==1)
		return;
	R=(Article**)malloc((nbArt/2)*sizeof(Article*));
	S=(Article**)malloc((nbArt-nbArt/2)*sizeof(Article*));
	if(R==NULL || S==NULL)
		{
			printf("Problème malloc \n");
			exit(1);
		}
	copierArticle(tArt, 0, nbArt/2,R);
	copierArticle(tArt,nbArt/2,nbArt,S);
	triDicoArticle(R,nbArt/2);
	triDicoArticle(S,nbArt-nbArt/2);
	fusionArticle(R,nbArt/2,S,nbArt-nbArt/2,tArt);
	free(R);
	free(S);
}

void copierArticle(Article *tArt[],int i, int j, Article *R[])
{
	int k=0;
	while(i<j)
		{
			R[k]=tArt[i];
			i=i+1;
			k=k+1;
		}
}

void fusionArticle(Article *R[], int n,Article *S[], int m,Article *tArt[])
{
	int i=0,j=0,k=0;
	while(i<n && j<m)
		{
			if(strcmp(R[i]->idArticle,S[j]->idArticle)<0)
				{
					tArt[k]=R[i];
					i=i+1;
					k=k+1;
				}
			else 
				{
					tArt[k]=S[j];
					j=j+1;
					k=k+1;
				}

		}
	while(i<n)
		{
			tArt[k]=R[i];
			i=i+1;
			k=k+1;
		}
	while(j<m)
		{
			tArt[k]=S[j];
			j=j+1;
			k=k+1;
		}
}


	

int supprimerArticle(Article *tArt[], int nb)
{
	char confirm, IDarticle[8];
	int i,r, trouve;
	Article *art;
	afficherTArt(tArt,nb);
	printf("Tapez l'identifiant de l'article que vous souhaitez supprimer \n");
	scanf("%s%*c",IDarticle);
	r=rechDicoArticle(IDarticle,tArt, nb, &trouve);
	if(trouve==0)
	{
		printf("L'identifiant de l'article n'existe pas !\n");
		return -1;
	}
	else 
	{	
		afficherTArt(tArt,nb);
		printf("Voulez-vous vraiment supprimer cet identifiant ?(o/n)\n");
		scanf("%c",&confirm);
		if(confirm=='o')
		{	
			art=tArt[r];			
			for(i=r;i<nb-1;i++)
				tArt[i]=tArt[i+1];
			nb=nb-1;
			printf("L'identifiant du client a été supprimé\n");
			return nb;
		}
		else 
			printf("La demande de suppression a été abandonnée\n");
	}		
}

void formationIDarticle( char designation[], char IDarticle[], Article *tArt[],int nbArt)
{
	int i, trouve, l;
	char num[2];
	strcpy(IDarticle,designation);
	supprimeBlanc(IDarticle, IDarticle);
	partieGauche(IDarticle, IDarticle,5);
	i=rechDicoArticle(IDarticle, tArt, nbArt, &trouve);
	while(trouve==1)
	{	
		printf("L'identifiant crée existe déjà. Veuillez saisir un numéro complémentaire : \n");
		scanf("%s",num);
		strcat(IDarticle,num);

		i=rechDicoArticle(IDarticle, tArt, nbArt, &trouve);
		
	}
}


int infoArticle (Article *tArt[], int nbArt)
{
	char IDarticle[6], designation[40], get;
	int quantite, trouve, i, r;
	float prix;
	printf("Vous avez demandé la création d'un nouvel article. \n\n ");
	printf("Veuillez taper le prix de ce nouvel article : \n");
	scanf("%f%*c",&prix);
	printf("Veuillez taper la désignation de l'article (en majuscule) :  \n");
	fgets(designation,40,stdin);
	designation[strlen(designation)-1]='\0';
	printf("Veuillez indiquer en quelle quantité cet article est disponible : \n");
	scanf("%d",&quantite);
	
	printf("\t\tLa saisie de vos informations est terminée ! \n\n");
	
	formationIDarticle( designation, IDarticle, tArt, nbArt);
	
	get=getchar();
	printf("\t\tVotre identifiant est %s . \n",IDarticle);
	
	get=getchar();
	
	r=ajouterArticle(IDarticle, designation, prix, quantite, tArt, nbArt);
	if(r<0)
	{	printf("Une erreur est survenue lors de la création de l'indentifiant .\n");
		if(r==-3)
			printf("La demande de création d'un nouvelle identifiant a été abandonnée. \n\n");
			
		return nbArt;
	}
	
	else 
	{
		system("clear");
		afficherTArt(tArt,nbArt+1);
		printf("Le nouvel article vient d'être ajouté ! \n");
		return nbArt+1;
	}
		
}

int ajouterArticle( char IDarticle[], char designation[], float prix, int quantite, Article *tArt[], int nbArt)
{
	int i, j, trouve;
	char confirm;
	i=rechDicoArticle(IDarticle, tArt, nbArt, &trouve);
	afficherTArt(tArt,nbArt);
	printf("Voulez-vous vraiment créer cet identifiant (o/n) ? \n");
	scanf("%c%*c",&confirm);
	if(confirm=='n')
	
		return -3;
	
	if(confirm=='o')
	{	

		if(200==i)
		{
			printf("Le tableau est plein ! \n");
			return -2;	
		}

		else
		{
			for(j=nbArt-1;j>=i;j--)
				tArt[j+1]=tArt[j];
			tArt[i]=(Article *) malloc(sizeof(Article));
			if(tArt[i]==NULL)
				{
					printf("Problème malloc\n");
					return -3;
				}

			strcpy(tArt[i]->idArticle,IDarticle);
			strcpy(tArt[i]->denomination,designation);
			tArt[i]->prixU=prix;
			tArt[i]->quantEnStock=quantite;
			return 1;
		}
			
	}	
}

void saisiePourModifArticle(Article *tArt[], int nbArt)
{
	char IDarticle[6], ouiNon, c;
	int quantite, trouve, i, d, q, p;
	float prix;
	
	afficherTArt(tArt,nbArt);
	printf("Veuillez saisir l'identifiant de l'article dont vous souhaitez modifier les données : \n");
	scanf("%s%*c",IDarticle);
	i=rechDicoArticle(IDarticle, tArt, nbArt, &trouve);
	while(trouve==0)
	{
		printf("L'identifiant que vous avez saisie n'existe pas.\n Veuillez ressaisir un identifiant d'article valide : \n");
		scanf("%s%*c",IDarticle);
		i=rechDicoArticle(IDarticle, tArt, nbArt, &trouve);
	}
	afficherArt(*tArt[i]);
	printf("Voulez-vous modifier la désignation de l'article (o/n) ? \n");
	scanf("%c%*c",&ouiNon);
	c=getchar();
	if(ouiNon=='n')	
		printf("Vous avez refusé de modifier la désignation de l'article. \n");
	else
		d=modifDenominationArt( tArt, nbArt, i);
	printf("Désirez-vous modifier la quantité disponible de cet article (o/n) ? \n");
	scanf("%c%*c",&ouiNon);
	if(ouiNon=='o')
	{	
		printf("Quelle est la nouvelle quantité qui est disponible pour cet article ? \n");
		scanf("%d%*c",&quantite);
		q=modifQuantiteArt(quantite, tArt, nbArt, i);
	}
	else
		printf("Vous avez refusé de modifier la quantité disponible de l'article.  \n");
	printf("Désirez-vous modifier le prix d'un article (o/n) ? \n");
	scanf("%c%*c",&ouiNon);
	c=getchar();
	if(ouiNon=='o')
	{	
		printf("Quel est le nouveau prix pour cet article ? \n");
		scanf("%f%*c",&prix);
		p=modifPrixArt(prix, tArt, nbArt, i);
	}
	else
		printf("Vous avez refusé de modifier le prix de l'article. \n");
	printf("Votre saisie a été enregistrée. \n");
	
	afficherTArt(tArt,nbArt);
	
	if(d==-1 || q==-1 || p==-1)
		printf("Un problème est survenu lors de la modification . \n");
	else
	{
		system("clear");
		afficherTArt(tArt,nbArt);
		printf("La modification a été réalisée avec succès ! \n");
	}
}
		
int modifDenominationArt(Article *tArt[], int nbArt, int i)
{
	char confirm,c, designation[40];
	printf("Quelle est la nouvelle dénomination de l'article ? \n");
	fgets(designation,40,stdin);
	designation[strlen(designation)-1]='\0';
	c=getchar();
	printf("Voulez-vous vraiment modifier la désignation de l'article (o/n) ?\n");
	scanf("%c%*c",&confirm);
	c=getchar();
	if(confirm=='n')
	{	
		printf("La demande de changement de la dénomination de l'article a été abandonnée.\n");
		return -1;
	}
	else 
	{
		strcpy(tArt[i]->denomination,designation);
		return 2;
	}
}

int modifQuantiteArt(int quantite, Article *tArt[], int nbArt, int i)
{
	char confirm;
	printf("Voulez-vous vraiment modifier la quantité de l'article (o/n) ?\n");
	scanf("%c%*c",&confirm);
	if(confirm=='o')
	{
		tArt[i]->quantEnStock=quantite;
		return 2;
	}
	else 
	{
	printf("La demande du changement de la quantité de l'article a été abandonnée.\n");
	return -1;	
	}
}

int modifPrixArt(float prix, Article *tArt[], int nbArt, int i)
{
	char confirm;
	printf("Voulez-vous vraiment modifier le prix de l'article (o/n) ?\n");
	scanf("%c%*c",&confirm);
	if(confirm=='o')
	{
		tArt[i]->prixU=prix;
		return 2;
	}
	else 
	{
	printf("La demande du changement de prix de l'article a été abandonnée.\n");
	return -1;	
	}
}


void afficheArtRuptStock(Article *tArt[], int nbArt)
{
	int i;
	system("clear");
	printf("\t\t-----------------------Tableau d'articles en rupture de stock : ----------------------- \n");
	printf("Identifiant\t\tPrix\t\tQuantité\t\tDénomination\n");
	for(i=0;i<nbArt;i++)
		if((tArt[i]->quantEnStock)<=0)
			afficherArt(*tArt[i]);
	printf("\t\t\t------------------------------------------\n");
}

void sauvegardeArt(FILE *fe,Article *tArt[], int max)
{
	int i;
	fe=fopen("articles.txt","w");
	for(i=0;i<max;i++)
	{
		fprintf(fe,"%s\t%.2f\t%d\t%s\n",tArt[i]->idArticle,tArt[i]->prixU,tArt[i]->quantEnStock,tArt[i]->denomination);
	}
	fclose(fe);
}

Client lireClient(FILE *fe)
{
	Client c;
	fscanf(fe,"%s",c.idClient);	
	fscanf(fe,"%d%s%*c",&c.codePostal,c.civilite);
	fgets(c.prenom,20,fe);
	c.prenom[strlen(c.prenom)-1]='\0';
	fgets(c.nom,20,fe);
	c.nom[strlen(c.nom)-1]='\0';
	fgets(c.ville,30,fe);	
	c.ville[strlen(c.ville)-1]='\0';
	return c;
}


Client* chargementCli(FILE *fe,int *tmax, int *nb)
{
	int i=0;
	Client *tabC,*aux;
	*tmax=5;
	Client c;

	fe=fopen("clients.txt","r");
	tabC=(Client*)malloc(*tmax*sizeof(Client));
	if(tabC==NULL)
		{
			printf("Problème malloc\n");
			return NULL;
		}
	c=lireClient(fe);
	while(!feof(fe))
	{
		if(i==*tmax)
			{
				*tmax=*tmax+5;
				aux=(Client*)realloc(tabC,*tmax*sizeof(Client));
				if(aux==NULL)
					{
						printf("Problème realloc\n");
						return NULL;
					}
				tabC=aux;
			}
		tabC[i]=c;
		i=i+1;
		c=lireClient(fe);
	}
	*nb=i;
	return tabC;
}


void afficherCli(Client c)
{
	printf("%s\t\t%s\t\t\t%s\t\t\t%s\t\t\t%d\t\t\t%s\n",c.idClient,c.civilite,c.nom,c.prenom,c.codePostal,c.ville);
}


void afficheTabCli(Client tabC[], int tmax)
{
	int i;
	system("clear");
	printf("\t\t--------------------------Tableau des clients : -----------------\n");
	printf("Identifiant\t\tCivilité\t\tNom\t\t\tPrénom\t\t\tCode Postal\t\t\tVille\n");
	for(i=0;i<tmax;i++)
		afficherCli(tabC[i]);
	printf("\t\t\t\t\t-------------------------------------------\n");
}





void afficheTabCliCom(Client tabC[], int tmax, Liste list)
{
	int i,r;
	Liste l;
	system("clear");
	printf("\t\t\t\t--------------------------Tableau des clients + commandes : -----------------\n");
	for(i=0;i<tmax;i++)
	{	
		afficherCli(tabC[i]);
		printf("\n");
		l=list;
		while(l!=NULL)
		{
			l=rechercherComClient(l,tabC[i]);
			if(l!=NULL)
			{
				printf("IdCommande\t\tIdArticle\t\tIdClient\t\t\tQuantité\n");
				printf("%s\t\t|\t%s\t\t|\t%s\t\t|\t%d\n\n\n",(l->com).idCommande,(l->com).idArticle,(l->com).idClient,(l->com).quantNonLiv);
				l=l->suiv;
			}
		}
		list=list->suiv;
		printf("\t\t\t\t\t-------------------------------------------\n");
		
	}
	printf("\t\t\t\t\t-------------------------------------------\n");
}

int rechDich(char *idClient, Client *tabC, int nbcli, int *trouve)
{
	int inf=0, sup=nbcli-1, m;
	while(inf<=sup)
		{
			m=(inf+sup)/2;
			if(strcmp(idClient,tabC[m].idClient)==0)
				{
					*trouve=1;
					return m;
				}
			if(strcmp(idClient,tabC[m].idClient)<0)
				sup=m-1;
			else inf=m+1;
		}
	*trouve=0;
	return inf;
}

void triDicoClient(Client tabC[], int nbcli)
{
	Client *R,*S;
	if(nbcli==1)
		return;
	R=(Client*)malloc((nbcli/2)*sizeof(Client));
	S=(Client*)malloc((nbcli-nbcli/2)*sizeof(Client));
	if(R==NULL || S==NULL)
		{
			printf("Problème malloc \n");
			return;
		}
	copierClient(tabC, 0, nbcli/2,R);
	copierClient(tabC,nbcli/2,nbcli,S);
	triDicoClient(R,nbcli/2);
	triDicoClient(S,nbcli-nbcli/2);
	fusionClient(R,nbcli/2,S,nbcli-nbcli/2,tabC);
	free(R);
	free(S);
}

void copierClient(Client tabC[],int i, int j, Client R[])
{
	int k=0;
	while(i<j)
		{
			R[k]=tabC[i];
			i=i+1;
			k=k+1;
		}
}

void fusionClient(Client R[], int n,Client S[], int m,Client tabC[])
{
	int i=0,j=0,k=0;
	while(i<n && j<m)
		{
			if(strcmp(R[i].idClient,S[j].idClient)<0)
				{
					tabC[k]=R[i];
					i=i+1;
					k=k+1;
				}
			else 
				{
					tabC[k]=S[j];
					j=j+1;
					k=k+1;
				}

		}
	while(i<n)
		{
			tabC[k]=R[i];
			i=i+1;
			k=k+1;
		}
	while(j<m)
		{
			tabC[k]=S[j];
			j=j+1;
			k=k+1;
		}
}

void formationIDclient( char nom[], char prenom[], char IDclient[], Client tabC[],int nbcli)
{
	int i, trouve,l;
	char num[2];
	supprimeBlanc(nom, nom);
	supprimeBlanc(prenom, prenom);
	partieGauche(nom, nom, 4);
	partieGauche(prenom, prenom, 5);
	assemblerChaines(nom, prenom,IDclient);
	i=rechDich(IDclient, tabC, nbcli, &trouve);
	while(trouve==1)
	{	
		printf("L'identifiant créé existe déjà. Veuillez saisir un chiffre complémentaire : \n");
		scanf("%s",num);
		strcat(IDclient,num);
		i=rechDich(IDclient, tabC, nbcli, &trouve);
	}

}


int infoClient(Client tabC[],int nbcli, int tmax)
{
	char civilite[4], nom[20], prenom[20], ville[30], IDclient[10], get;
	int codePostal, trouve, i, r;
	printf("Vous avez demandé la création d'un nouvel utilisateur \n\n");
	printf("Veuillez taper votre civilité : (Mr/ Mme) \n ");
	scanf("%s%*c",civilite);
	printf("Veuillez taper votre nom (minuscule) : \n");
	fgets(nom,20,stdin);
	nom[strlen(nom)-1]='\0';
	printf("Veuillez taper votre prénom (minuscule) : \n");
	fgets(prenom,20,stdin);
	prenom[strlen(prenom)-1]='\0';
	printf("Veuillez taper votre code postal : \n");
	scanf("%d%*c",&codePostal);
	printf("Veuillez taper le nom de votre ville : \n");
	fgets(ville,30,stdin);	
	ville[strlen(ville)-1]='\0';

	printf("\t\tLa saisie de vos informations est terminée ! \n\n");
	
	formationIDclient(nom, prenom, IDclient, tabC, nbcli);
	get=getchar();
	printf("\t\tVotre identifiant est %s . \n",IDclient);
	get=getchar();

	r=ajouterClient(civilite, nom, prenom, ville, IDclient, codePostal, tabC, nbcli, tmax);
	if(r<0)
	{	printf("Une erreur est survenue lors de la création de l'indentifiant .\n");
		if(r==-3)
			printf("La demande de création d'un nouvel identifiant a été abandonnée. \n\n");
			
		return -1;
	}
	
	else 
	{
		system("clear");
		nbcli=nbcli+1;
		afficheTabCli(tabC, nbcli);
		printf("Le nouveau nombre de clients est %d.\n",nbcli);
		printf("Le nouveau client vient d'être ajouté ! \n");
	}

	return nbcli;	
}



int ajouterClient( char civilite[], char nom[], char prenom[], char ville[], char IDclient[],int codePostal, Client tabC[], int nbcli, int tmax)
{
	int i, j, trouve;
	char confirm;
	i=rechDich(IDclient, tabC, nbcli, &trouve);
	//i=position a inserer retourner par rechercheIDclient , valeur retourner par adresse && n représente le nombre d'élément du tableau
	afficheTabCli(tabC, nbcli);
	printf("Voulez-vous vraiment créer l'identifiant  %s (o/n) ?\n",IDclient);
	scanf("%c",&confirm);
	if(confirm=='n')
	
		return -3;
	
	if(confirm=='o')
	{	

		if(nbcli+1==tmax)
		{
			printf("Le tableau est plein ! \n");
			return -2;	
		}

		
		
		for(j=nbcli-1;j>=i;j--)
		
			tabC[j+1]=tabC[j];


		
		strcpy(tabC[i].civilite,civilite);
		strcpy(tabC[i].nom,nom);
		strcpy(tabC[i].prenom,prenom);
		strcpy(tabC[i].ville,ville);
		strcpy(tabC[i].idClient,IDclient);
		tabC[i].codePostal=codePostal;
		return 1;
	
			
	}	
}



int supprimerClient(Client tabC[], int nb)
{
	char confirm, IDclient[10];
	int i,r, trouve;
	Client aux;
	afficheTabCli(tabC, nb);
	printf("Tapez l'identifiant du client que vous souhaitez supprimer :\n");
	scanf("%s%*c",IDclient);
	r=rechDich(IDclient, tabC, nb, &trouve);
	if(trouve==0)
		printf("L'identifiant du client n'existe pas !\n");
	else 
	{	
		afficheTabCli(tabC, nb);
		printf("Voulez-vous vraiment supprimer cet identifiant (o/n) ?\n");
		scanf("%c",&confirm);
		if(confirm=='o')
		{	
			aux=tabC[r];			
			for(i=r;i<nb-1;i++)
				tabC[i]=tabC[i+1];
			{
				nb=nb-1;
				afficheTabCli(tabC, nb);
				printf("L'identifiant du client a été supprimé\n");
			}
		}
		else 
			printf("La demande de suppression a été abandonnée\n");
	}
	return nb;		
}

void saisiePourModifClient(Client tabC[], int nb)
{
	char IDclient[10], newVille[30], v;
	int  newCodePostal;
	afficheTabCli(tabC, nb);
	printf("Veuillez saisir l'identifiant du client dont vous souhaitez modifier l'adresse : \n");
	scanf("%s%*c",IDclient);
	printf("Veuillez saisir la nouvelle ville où habite le client : \n");
	fgets(newVille,30,stdin);
	newVille[strlen(newVille)-1]='\0';
	printf("Veuillez saisir le nouveau code postal : \n");
	scanf("%d%*c",&newCodePostal);
	v=mofificationClient(IDclient, newVille, newCodePostal, tabC, nb);
	if(v==1)
	{
		system("clear");
		afficheTabCli(tabC, nb);
		printf("La modification a été réalisée avec succès ! \n");
	}
	else
		printf("Un problème est survenu lors de la modification . \n");
}


int mofificationClient(char IDclient[], char newVille[], int newCodePostal, Client tabC[], int nb)
{
	Client c;
	int i, trouve;
	char confirm;
	i=rechDich(IDclient, tabC, nb, &trouve);
	if(trouve==0)
		printf("L'identifiant du client n'existe pas !\n");
	else 
	{	
		afficheTabCli(tabC, nb);
		printf("Voulez-vous vraiment modifier l'adresse de cet identifiant (o/n) ?\n");
		scanf("%c",&confirm);
		if(confirm=='o')
		{	
			tabC[i].codePostal=newCodePostal;
			strcpy(tabC[i].ville,newVille);
			return 1;
		}
		else 
		{
		printf("La demande de changement d'adresse a été abandonnée\n");
		return -1;	
		}
	}
}

void sauvegardeC(FILE *fe,Client tabC[], int nb)
{
	int i;
	fe=fopen("clients.txt","w");
	for(i=0;i<nb;i++)
	{
		fprintf(fe,"%s\t%d\t%s\t%s\t%s\t%s\n",tabC[i].idClient,tabC[i].codePostal,tabC[i].civilite,tabC[i].prenom,tabC[i].nom,tabC[i].ville);
	}
	fclose(fe);
}


Commande lireCommande(FILE *fe)
{
	Commande c;
	fscanf(fe,"%s%s%s%d%*c",c.idCommande,c.idArticle,c.idClient,&c.quantNonLiv);
	return c;
}

Liste remplirListe(void)
{
	FILE * fe;
	Liste l;
	Commande c;
	fe=fopen("lignesCommandesEnCours.txt","r");
	if(fe==NULL)
	{
		printf("Un problème est survenu lors de l'ouverture du fichier en lecture\n");
		return NULL;
	}
	l=listeVide();
	c=lireCommande(fe);
	while(!feof(fe))
	{
		l= ajouter(l,c);
		c=lireCommande(fe);
	}
	fclose(fe);
	return l;
}


Liste listeVide(void)
{
	return NULL;
}

Liste ajouterEnTete(Liste l,Commande c)
{
	Maillon *m;
	m=(Maillon*)malloc(sizeof(Maillon));
	if(m==NULL)
	{
		printf("Problème malloc\n");
		exit(1);
	}
	m->com=c;
	m->suiv=l;
	return m;
}	

Liste ajouter(Liste l,Commande c)
{
	if(l==NULL)
		return ajouterEnTete(l,c);
	if(strcmp(c.idCommande,(l->com).idCommande)<0)  
		return ajouterEnTete(l,c);
	if(strcmp(c.idCommande,(l->com).idCommande )==0)
		return l;
	l->suiv=ajouter(l->suiv,c);
	return l;
}

Liste ajouterEnQueue(Liste l,Commande c)
{
	if(l==NULL)
		return ajouterEnTete(l,c);
	l->suiv=ajouterEnQueue(l->suiv,c);
	return l;
}

Liste supprimerEnTete(Liste l)
{
	Maillon *aux;
	if(l==NULL)
	{	printf("L'opération est interdite !");
		exit(1);
	}
	aux=l;
	l=l->suiv;
	free(aux);
	return l;
}

Liste supprimer(Liste l,char idCommande[])
{
	if(l==NULL)
		return l;
	if(strcmp(idCommande,(l->com).idCommande)==0)
		return supprimerEnTete(l);
	l->suiv=supprimer(l->suiv,idCommande);
	return l;	
}

int longeurListe(Liste l)
{
	if(l==NULL)
		return 0;
	return longeurListe(l->suiv)+1;
}


void afficheCommande(Liste l,FILE *fe)
{
	int lon;
	lon=longeurListe(l);
	system("clear");
	printf("\t-------------------Tableau des commandes en cours : -----------------\n");
	printf("IdCommande\t\tIdArticle\t\tIdClient\t\t\tQuantité\n");
	while(l!=NULL)
	{
		fprintf(fe,"%s\t\t|\t%s\t\t|\t%s\t\t|\t%d\n",(l->com).idCommande,(l->com).idArticle,(l->com).idClient,(l->com).quantNonLiv);
		l=l->suiv;
	}
	printf("\t-------------------------------------------------------------\n");
	printf("Le nombre de commandes en cours est %d.",lon);
	
}


Liste rechercherComClient(Liste l, Client c)
{
	while(l!=NULL)
	{	if(strcmp(c.idClient,(l->com).idClient)==0)
			return l;
		l=l->suiv;
	}
	return NULL;
}

void formationIDCommande(char IDarticle[], char IDclient[], char IDCommande[], int nb)
{
	int i,trouve;
	char IDcli[10], IDart[6], num[2];
	Liste l;
	strcpy(IDcli,IDclient);
	strcpy(IDart, IDarticle);
	partieGauche(IDart, IDart, 2);
	partieGauche(IDcli, IDcli, 3);
	assemblerChaines(IDart, IDcli,IDCommande);
	i=rechDichCommande(IDCommande,l,nb, &trouve);
	if(trouve==1) 
	{	
		printf("L'identifiant créé existe déjà. Veuillez saisir un chiffre complémentaire : \n");
		scanf("%s",num);
		strcat(IDCommande,num);
		i=rechDichCommande(IDCommande,l,nb, &trouve);
	}

}

int rechDichCommande(char *idCommande,Liste l, int nb, int *trouve)
{
	int inf=0,sup=nb-1,m;
	while(inf<=sup)
		{
			m=(inf+sup)/2;
			if(strcmp(idCommande,(l->com).idCommande)==0)
				{
					*trouve=1;
					return m;
				}
			if(strcmp(idCommande,(l->com).idCommande)<0)
				sup=m-1;
			else inf=m+1;
		}
	*trouve=0;
	return inf;
}



void saisieCommandeClient(Article *tArt[], int nbArt, Client tabC[], int nb, Liste l)
{
	char IDclient[10], IDarticle[6], IDcommande[6],c ;
	int i, r, trouve, quantiteArt, aJour; 
	system("clear");
	afficheTabCli(tabC, nb);
	printf("Veuillez saisir l'identifiant du client qui commande : \n");
	scanf("%s%*c",IDclient);
	i=rechDich(IDclient, tabC, nb, &trouve);
	while(trouve==0)
	{	printf("L'identifiant du client n'existe pas ! \n\n");
		printf("Veuillez saisir l'identifiant du client qui commande : \n");
		scanf("%s%*c",IDclient);
		i=rechDich(IDclient, tabC, nb, &trouve);
	}
	afficherTArt(tArt,nbArt);
	printf("Quel est l'identifiant de l'article que le client a choisi : \n");
	scanf("%s%*c",IDarticle);
	r=rechDicoArticle(IDarticle, tArt, nbArt, &trouve);
	while(trouve==0)
	{	printf("L'identifiant de l'article n'existe pas ! \n\n");
		printf("Quel est l'identifiant de l'article que le client a choisi : \n");
		scanf("%s",IDarticle);
		r=rechDicoArticle(IDarticle, tArt, nbArt, &trouve);
	}
	printf("Veuillez saisir la quantité que vous souhaitez de cet article : \n");
	scanf("%d",&quantiteArt);
	c=getchar();
	printf("La saisie pour le client %s a été effectuée. \n", IDclient);
	
	formationIDCommande(IDarticle, IDclient, IDcommande, nb);
	
	printf("L'identifiant de la commande est %s\n", IDcommande);

	aJour=miseAjour(tArt, nbArt,IDcommande, IDarticle, IDclient, quantiteArt, r, l);
	if(aJour<0)
	{
		printf("Un problème est survenue lors de la mise à jour du stock suite à votre commande : \n");
		return;
	}
	printf("Le stock a bien été mis à jour ! \n");
}




int miseAjour(Article *tArt[], int nbArt, char IDcommande[], char IDarticle[], char IDclient[],int quantiteArt, int r, Liste l)
{
	Commande com;
	char c;
	Liste list;
	strcpy((l->com).idCommande, IDcommande);
	strcpy((l->com).idArticle, IDarticle);
	strcpy((l->com).idClient, IDclient);
	(l->com).quantNonLiv=quantiteArt;
	
	if(quantiteArt>tArt[r]->quantEnStock)
		{
			(l->com).quantNonLiv=tArt[r]->quantEnStock+(quantiteArt-tArt[r]->quantEnStock);
			list=l;
			l=ajouterEnQueue(l,com);
			afficheCommande(l,stdout);
			c=getchar();
		}
	if(quantiteArt<tArt[r]->quantEnStock)
	{
		tArt[r]->quantEnStock=tArt[r]->quantEnStock-quantiteArt;	
	}
	
	afficherTArt(tArt,nbArt);
	return 1; 
}


void supprimerCommandeEnCours(Liste l)
{
	char confirm, IDcommande[6];
	Liste list;
	afficheCommande(l,stdout);
	printf("Tapez l'identifiant de la commande que vous souhaitez supprimer :\n");
	scanf("%s%*c",IDcommande);
	printf("Voulez-vous vraiment supprimer cet commande (o/n) ?\n");
	scanf("%c%*c",&confirm);
	if(confirm=='o')
		{	
			system("clear");
			list=l;
			l=supprimer(l,IDcommande);
		}
	else 
		printf("La demande de suppression a été abandonnée\n");
		
	afficheCommande(l,stdout);
	printf("La commande a été supprimée !\n");
}

void sauvegarderCom(Liste l,FILE *fe)
{
	fe=fopen("lignesCommandesEnCours.txt","w");
	while(l!= NULL)
	{
		fprintf(fe,"%s\t%s\t%s\t%d\n",(l->com).idCommande,(l->com).idArticle,(l->com).idClient,(l->com).quantNonLiv);
		l=l->suiv;
	}
	fclose(fe);
}

void supprimeBlanc(char source[], char dest[])
{
	int i=0, j=0;
	while(source[i]!='\0')
	{	if(source[i]!=' '&&'\n'&&'\t')
		{	dest[j]=source[i];
			j=j+1;
			i=i+1;
		}
		else
			i=i+1;
	}
	dest[j]='\0';
}

void partieGauche(char source[], char dest[], int p)
{
	int i=0;
	while(source[i]!='\0'&& i<p)
	{	dest[i]=source[i];
		i=i+1;
	}
	dest[i]='\0';
}

void assemblerChaines( char source1[], char source2[], char dest[])
{
	strcpy(dest, source1);
	strcat(dest, source2);
}

void afficherMenu(void)
{
	system("clear");
	printf("\n\n\n\n\t\t\tGestion \n\n");
	printf("\t1\t Travailler sur la liste des articles \n");
	printf("\t2\t Travailler sur la liste des clients \n");
	printf("\t3\t Travailler sur les commandes en cours d'attente\n");
	printf("\n");
	printf("\t9\t Quitter \n\n");
}

int choixMenu(void)
{
	int num;
	afficherMenu();

	printf("\t\t\t\tVeuillez choisir une option : \n");
	scanf("%d%*c", &num);

	while(num<1 || num>3)
	{	if(num==9)
			break;
		printf("choix incorrect, veuillez ressaisir : ");

		afficherMenu();
		printf("\t\t\t\tVeuillez choisir une option : \n");
		scanf("%d%*c", &num);
		return num;
	}
}

void afficherMenu2(void)
{
	system("clear");
	printf("\n\n\n\n\t\t\tGestion \n\n");
	printf("\t1\t Afficher le tableau d'article : \n");
	printf("\t2\t Supprimer un article : \n");
	printf("\t3\t Ajouter un article : \n");
	printf("\t4\t Modifier un article : \n");
	printf("\t5\t Afficher le tableau des articles en rupture de stock :\n");
	printf("\n");
	printf("\t9\t Quitter \n\n");
}

int choixMenu2(void)
{
	int choix;
	afficherMenu2();
	
	printf("\t\tVeuillez choisir une option : \n");
	scanf("%d", &choix);
	
	while(choix<1 || choix>5)
	{	if(choix==9)
			break;
		printf("Votre choix est incorrect, veuillez ressaisir :");
		
		afficherMenu2();
		printf("\t\t\t\tVeuillez choisir une option : \n");
		scanf("%d%*c", &choix);
		return choix;
	}
}


void afficherMenu3(void)
{
	system("clear");
	printf("\n\n\n\n\t\t\tGestion \n\n");
	printf("\t1\t Afficher le tableau de clients : \n");
	printf("\t2\t Ajouter un client : \n");
	printf("\t3\t Supprimer un client : \n");
	printf("\t4\t Modifier l'adresse d'un client : \n");
	printf("\n");
	printf("\t9\t Quitter \n\n");
}

int choixMenu3(void)
{
	int choix;
	afficherMenu3();
	
	printf("\t\tVeuillez choisir une option : \n");
	scanf("%d", &choix);
	
	while(choix<1 || choix>4)
	{	if(choix==9)
			break;
		printf("Votre choix est incorrect, veuillez ressaisir :");
		
		afficherMenu3();
		printf("\t\t\t\tVeuillez choisir une option : \n");
		scanf("%d%*c", &choix);
		return choix;
	}
}

void afficherMenu4(void)
{
system("clear");
	printf("\n\n\n\n\t\t\tGestion \n\n");
	printf("\t1\t Afficher la liste des commandes en cours : \n");
	printf("\t2\t Ajouter une commande en cours \n");
	printf("\t3\t Supprimer une commande en cours \n");
	printf("\t4\t Affichage des commandes en cours clients par clients : \n");
	printf("\n");
	printf("\t9\t Quitter \n\n");
}

int choixMenu4(void)
{
	int choix;
	afficherMenu4();
	
	printf("\t\tVeuillez choisir une option : \n");
	scanf("%d", &choix);
	
	while(choix<1 || choix>4)
	{	if(choix==9)
			break;
		printf("Votre choix est incorrect, veuillez ressaisir :");
		afficherMenu4();
		printf("\t\t\t\tVeuillez choisir une option : \n");
		scanf("%d%*c", &choix);
		return choix;
	}
}



