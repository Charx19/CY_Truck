#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#define TAILLE 250
	#define GRANDETAILLE 10000


	// Structure pour représenter une ville
	typedef struct Ville {
		char nom[50];
		int trajetsTotals;
		int departs;
		struct Ville* gauche;
		struct Ville* droite;
		int hauteur; // Hauteur pour l'équilibrage AVL
		int idIdx;
		int id[GRANDETAILLE];
	} Ville;

	// Fonction pour calculer la hauteur d'un nœud dans l'arbre AVL
	int hauteur(Ville* node) {
		if (node == NULL)
			return 0;
		return node->hauteur;
	}

	// Fonction pour trouver la hauteur maximale entre deux valeurs
	int max(int a, int b) {
		return (a > b) ? a : b;
	}

	// Fonction pour créer un nouveau nœud de ville
	Ville* creerNoeudVille(char nomVille[]) {
		Ville* nouvelleVille = (Ville*)malloc(sizeof(Ville));
		size_t size = sizeof(int) * GRANDETAILLE;
		strcpy(nouvelleVille->nom, nomVille);
		nouvelleVille->trajetsTotals = 1;
		nouvelleVille->departs = 0;
		nouvelleVille->gauche = NULL;
		nouvelleVille->droite = NULL;
		nouvelleVille->hauteur = 1;
		nouvelleVille->idIdx = 0;
		memset(nouvelleVille->id, 0, size);
		return nouvelleVille;
	}

	// Fonction pour faire une rotation simple à droite
	Ville* rotationDroite(Ville* y) {
		Ville* x = y->gauche;
		Ville* T2 = x->droite;

		x->droite = y;
		y->gauche = T2;

		// Mettre à jour les hauteurs
		y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
		x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;

		return x;
	}

	// Fonction pour faire une rotation simple à gauche
	Ville* rotationGauche(Ville* x) {
		Ville* y = x->droite;
		Ville* T2 = y->gauche;

		y->gauche = x;
		x->droite = T2;

		// Mettre à jour les hauteurs
		x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
		y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;

		return y;
	}

	// Fonction pour obtenir le facteur d'équilibre d'un nœud
	int getEquilibre(Ville* node) {
		if (node == NULL)
			return 0;
		return hauteur(node->gauche) - hauteur(node->droite);
	}

	// Fonction pour parcourir l'arbre en ordre alphabétique et afficher les statistiques des 10 premières villes
	void afficherID(Ville* racine) {
		
		if (racine != NULL) {
			
			
			for(int i = 0; i < racine->idIdx; i++)
			{
				if(i==0)
					printf("%s :\n",racine->nom);
				printf("-%d\n", racine->id[i]);
				if(racine->id[i]==0)
					printf("\n");
			}
		}
	}


	// Fonction pour insérer une ville dans l'arbre AVL
	Ville* insererVille(Ville* racine, char nomVille[], int idroute, int isDepart) {
		if (racine == NULL)
		{
			Ville* noeud = creerNoeudVille(nomVille);
			if(isDepart == 1)
			{
				noeud->id[0] = idroute;
				noeud->idIdx++;
			}
			return noeud;
		}
		
		if (strcmp(nomVille, racine->nom) < 0)
			racine->gauche = insererVille(racine->gauche, nomVille, idroute, isDepart);
		else if (strcmp(nomVille, racine->nom) > 0)
			racine->droite = insererVille(racine->droite, nomVille, idroute, isDepart);
		else {
			// La ville existe déjà, mettre à jour les statistiques
			int condition = 0;
			for(int i = 0; i < racine->idIdx; i++){
				if(racine->id[i] == idroute){
					condition = 1;
				}
			}
			
			if(condition == 0){
				racine->trajetsTotals++;
				racine->id[racine->idIdx] = idroute;
				racine->idIdx++;
			}
			
			if(isDepart == 1)
			{
				racine->departs++;
			}
			return racine;
		}

		// Mettre à jour la hauteur du nœud courant
		racine->hauteur = 1 + max(hauteur(racine->gauche), hauteur(racine->droite));

		// Obtenir le facteur d'équilibre pour ce nœud
		int equilibre = getEquilibre(racine);

		// Cas de l'équilibre à gauche-gauche
		if (equilibre > 1 && strcmp(nomVille, racine->gauche->nom) < 0)
			return rotationDroite(racine);

		// Cas de l'équilibre à droite-droite
		if (equilibre < -1 && strcmp(nomVille, racine->droite->nom) > 0)
			return rotationGauche(racine);

		// Cas de l'équilibre à gauche-droite
		if (equilibre > 1 && strcmp(nomVille, racine->gauche->nom) > 0) {
			racine->gauche = rotationGauche(racine->gauche);
			return rotationDroite(racine);
		}

		// Cas de l'équilibre à droite-gauche
		if (equilibre < -1 && strcmp(nomVille, racine->droite->nom) < 0) {
			racine->droite = rotationDroite(racine->droite);
			return rotationGauche(racine);
		}

		return racine;
	}

	// Fonction pour parcourir l'arbre en ordre alphabétique et afficher les statistiques des 10 premières villes
	void afficherStatsVilles(Ville* racine, int compte, char ListeDeNom[10][TAILLE], int ListeDeTrajets[], int ListeDeDeparts[]) {
		int condition = 0;
		if (racine != NULL) {
			afficherStatsVilles(racine->gauche, compte, ListeDeNom, ListeDeTrajets, ListeDeDeparts);
			
			if(ListeDeTrajets[compte] < racine->trajetsTotals){
				for(int i=0; i<compte; i++){
					if(strcmp(ListeDeNom[i], racine->nom) == 0){
						condition = 1;
					}
				}
				if(condition == 0){
					strcpy(ListeDeNom[compte], racine->nom);
					ListeDeTrajets[compte] = racine->trajetsTotals;
					ListeDeDeparts[compte] = racine->departs;
				}
			}

			afficherStatsVilles(racine->droite, compte, ListeDeNom, ListeDeTrajets, ListeDeDeparts);
		}
	}


	void afficherstatsvillesappel(Ville* racine){
		
		char ListeDeNom[10][TAILLE];
		int ListeDeTrajets[10];
		int ListeDeDeparts[10];
		
		for (int i=0; i<10; i++){
			strcpy(ListeDeNom[i], racine->nom);
			ListeDeTrajets[i]=racine->trajetsTotals;
			afficherStatsVilles(racine, i, ListeDeNom, ListeDeTrajets, ListeDeDeparts);
			printf("%s -TrajetsTotals %d -Départs %d\n", ListeDeNom[i], ListeDeTrajets[i], ListeDeDeparts[i]);
		}
	}


	// Fonction pour libérer la mémoire utilisée par l'arbre AVL
	void libererMemoire(Ville* racine) {
		if (racine != NULL) {
			libererMemoire(racine->gauche);
			libererMemoire(racine->droite);
			free(racine);
		}
	}




	char** ligneToTab(char* l, char* type)
	{

		char c;
		char* s = &c;
		char** res = &s;
		int count = 0;
		char* tmp = l;
		char* last_comma = 0;
		char delim[3];
		delim[0] = ';';
		delim[1] = 0;
		

		// compte le nombre d'elements a extraire
		while (*tmp)
		{
		   if (';' == *tmp)
			   {
				   count++;
				   last_comma = tmp;
			   }
			   tmp++;
		}

		// ajoute de l'espace pour le token
		if(last_comma < (l + strlen(l) - 1))
		{
			count ++;
		}

		count++;
		res = malloc(sizeof(char*) * count);
		if(res == NULL)
		{
			  printf("erreur");   //// METS TON MESSAGE DERREUR ICI
		}

		int idx  = 0;
		char* token = strtok(l, delim);

		while (token)
		{
			*(res + idx) = strdup(token);
			token = strtok(0, delim);
			idx++;
		}
		*(res + idx) = 0;

		return(res);
	}

	// Fonction principale pour analyser les trajets à partir d'un fichier
	void analyserTrajets(const char* nomFichier) {
			// Racine de l'arbre AVL

		Ville* racine = NULL;
		Ville* noeud = NULL;

			// Traitement des données du fichier
		FILE * fichier = fopen(nomFichier,"r");
		if (fichier == NULL){
			printf("pas de fichier d'entree de ce nom");
			exit(2);
		}


		int idRoute, idEtape, distance;
		char villeA[TAILLE], villeB[TAILLE], nomConducteur[TAILLE];
		char* type;
		char ligne[TAILLE];
		char** ligneTab = NULL;
		fgets(ligne, sizeof(ligne), fichier);
		while(fgets(ligne, sizeof(ligne), fichier) != NULL) // on lit chaque ligne
		{
			// et on les sépares par leur ';' avant de stocker les informations importantes
			ligneTab = ligneToTab(ligne, type);
			if (sscanf(ligneTab[0], "%d", &idRoute) != 1) {
				printf("Erreur de conversion. La chaîne n'est pas un nombre valide1.\n");
				exit(1);
			}
			if (sscanf(ligneTab[1], "%d", &idEtape) != 1) {
				printf("Erreur de conversion. La chaîne n'est pas un nombre valide2.\n");
				exit(1);
			}
			strcpy(villeA, ligneTab[2]);
			strcpy(villeB, ligneTab[3]);
			if (sscanf(ligneTab[4], "%d", &distance) != 1) {
				printf("Erreur de conversion. La chaîne n'est pas un nombre valide3.\n");
				exit(1);
			}
			strcpy(nomConducteur, ligneTab[5]);


			// Mettre à jour les statistiques pour la villeA et villeB
			if(idEtape == 1)
				racine = insererVille(racine, villeA, idRoute, 1);
			else
				racine = insererVille(racine, villeA, idRoute, 0);
			racine = insererVille(racine, villeB, idRoute, 0);
		}

		fclose(fichier);
		// Parcours de l'arbre en ordre alphabétique et affichage des statistiques des 10 premières villes
		printf("Les 10 villes les plus traversées (par ordre alphabétique):\n");
		//afficherstatville
		afficherstatsvillesappel(racine);
		
		
		
		// Libérer la mémoire utilisée par l'arbre AVL
		libererMemoire(racine);
	}

	int main() {
		analyserTrajets("data.csv");
		return 0;
	}