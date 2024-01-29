#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE 250
#define GRANDETAILLE 10000
#define IDDIFFERENT 100000


// Structure pour représenter une Route
typedef struct Route {
	int id;
	float max;
	float min;
	float somme;
	int n;
	float moyenne;
	struct Route* gauche;
	struct Route* droite;
	int hauteur; // Hauteur pour l'équilibrage AVL
} Route;

// Fonction pour calculer la hauteur d'un nœud dans l'arbre AVL
int hauteur(Route* node) {
	if (node == NULL)
		return 0;
	return node->hauteur;
}

// Fonction pour trouver la hauteur maximale entre deux valeurs
int max(int a, int b) {
	return (a > b) ? a : b;
}

// Fonction pour créer un nouveau nœud de Route
Route* creerNoeudRoute(int idRoute, float distance) {
	Route* nouvelleRoute = (Route*)malloc(sizeof(Route));
	
	nouvelleRoute->id = idRoute;
	nouvelleRoute->min = distance;
	nouvelleRoute->max = distance;
	nouvelleRoute->somme = distance;
	nouvelleRoute->n = 1;
	nouvelleRoute->moyenne = 0;
	
	nouvelleRoute->gauche = NULL;
	nouvelleRoute->droite = NULL;
	nouvelleRoute->hauteur = 1;
	return nouvelleRoute;
}

// Fonction pour faire une rotation simple à droite
Route* rotationDroite(Route* y) {
	Route* x = y->gauche;
	Route* T2 = x->droite;

	x->droite = y;
	y->gauche = T2;

	// Mettre à jour les hauteurs
	y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;
	x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;

	return x;
}

// Fonction pour faire une rotation simple à gauche
Route* rotationGauche(Route* x) {
	Route* y = x->droite;
	Route* T2 = y->gauche;

	y->gauche = x;
	x->droite = T2;

	// Mettre à jour les hauteurs
	x->hauteur = max(hauteur(x->gauche), hauteur(x->droite)) + 1;
	y->hauteur = max(hauteur(y->gauche), hauteur(y->droite)) + 1;

	return y;
}

// Fonction pour obtenir le facteur d'équilibre d'un nœud
int getEquilibre(Route* node) {
	if (node == NULL)
		return 0;
	return hauteur(node->gauche) - hauteur(node->droite);
}


// Fonction pour insérer une Route dans l'arbre AVL
Route* insererRoute(Route* racine, int idRoute, float distance) {
	if (racine == NULL)
	{
		Route* noeud = creerNoeudRoute(idRoute, distance);
		return noeud;
	}
	
	if (idRoute < racine->id)
		racine->gauche = insererRoute(racine->gauche, idRoute, distance);
	else if (idRoute > racine->id)
		racine->droite = insererRoute(racine->droite, idRoute, distance);
	else {
		// La Route existe déjà, mettre à jour les statistiques
		
		racine->somme += distance;
		racine->n++;
		
		if(distance > racine->max)
			racine->max = distance;
		
		if(distance < racine->min)
			racine->min = distance;
		
		return racine;
	}

	// Mettre à jour la hauteur du nœud courant
	racine->hauteur = 1 + max(hauteur(racine->gauche), hauteur(racine->droite));

	// Obtenir le facteur d'équilibre pour ce nœud
	int equilibre = getEquilibre(racine);

	// Cas de l'équilibre à gauche-gauche
	if (equilibre > 1 && idRoute < racine->gauche->id)
		return rotationDroite(racine);

	// Cas de l'équilibre à droite-droite
	if (equilibre < -1 && idRoute > racine->droite->id)
		return rotationGauche(racine);

	// Cas de l'équilibre à gauche-droite
	if (equilibre > 1 && idRoute > racine->gauche->id) {
		racine->gauche = rotationGauche(racine->gauche);
		return rotationDroite(racine);
	}

	// Cas de l'équilibre à droite-gauche
	if (equilibre < -1 && idRoute < racine->droite->id) {
		racine->droite = rotationDroite(racine->droite);
		return rotationGauche(racine);
	}

	return racine;
}


void CalculeMoyenne(Route* racine)
{
	if(racine != NULL)
	{
		CalculeMoyenne(racine->gauche);
		racine->moyenne = racine->somme / racine->n;
		CalculeMoyenne(racine->droite);
	}
}


// Fonction pour parcourir l'arbre en ordre alphabétique et afficher les statistiques des 10 premières Routes
void afficherStatsRoutes(Route* racine, int compte, int ListeDeId[], float ListeDeMax[], float ListeDeMin[], float ListeDeMoyenne[]) {
	int condition = 0;
	if (racine != NULL) {
		afficherStatsRoutes(racine->gauche, compte, ListeDeId, ListeDeMax, ListeDeMin, ListeDeMoyenne);
		
		if(ListeDeMax[compte] < racine->max){
			for(int i=0; i<compte; i++){
				if(racine->id == ListeDeId[i]){
					condition = 1;
				}
			}
			
			if(condition == 0){
				ListeDeId[compte] = racine->id;
				ListeDeMax[compte] = racine->max;
				ListeDeMin[compte] = racine->min;
				ListeDeMoyenne[compte] = racine->moyenne;
			}
		}

		afficherStatsRoutes(racine->droite, compte, ListeDeId, ListeDeMax, ListeDeMin, ListeDeMoyenne);
	}
}


void afficherstatsRoutesappel(Route* racine){
	
	int ListeDeId[IDDIFFERENT];
	float ListeDeMin[IDDIFFERENT];
	float ListeDeMax[IDDIFFERENT] = { 0 };
	float ListeDeMoyenne[IDDIFFERENT];
	
	for (int i=0; i<IDDIFFERENT; i++){
		ListeDeId[i] = racine->id;
		ListeDeMin[i] = racine->min;
		ListeDeMax[i] = racine->max;
		ListeDeMoyenne[i] = racine->moyenne;
		afficherStatsRoutes(racine, i, ListeDeId, ListeDeMin, ListeDeMax, ListeDeMoyenne);
		printf("%d -Max %f -Min %f -Moyenne %f\n", ListeDeId[i], ListeDeMin[i], ListeDeMax[i], ListeDeMoyenne[i]);
	}
}


// Fonction pour libérer la mémoire utilisée par l'arbre AVL
void libererMemoire(Route* racine) {
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
		printf("erreur");   
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


	Route* racine = NULL;

		// Traitement des données du fichier
	FILE * fichier = fopen(nomFichier,"r");
	if (fichier == NULL){
		printf("pas de fichier d'entree de ce nom");
		exit(2);
	}


	int idRoute, idEtape;
	float distance;
	char VilleA[TAILLE], VilleB[TAILLE], nomConducteur[TAILLE];
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
		strcpy(VilleA, ligneTab[2]);
		strcpy(VilleB, ligneTab[3]);
		if (sscanf(ligneTab[4], "%f", &distance) != 1) {
			printf("Erreur de conversion. La chaîne n'est pas un nombre valide3.\n");
			exit(1);
		}

		strcpy(nomConducteur, ligneTab[5]);


		// Mettre à jour les statistiques pour la Route			
		racine = insererRoute(racine, idRoute, distance);
	}

	fclose(fichier);
		
	CalculeMoyenne(racine);

	//afficherstatRoute
	afficherstatsRoutesappel(racine);
	
	// Libérer la mémoire utilisée par l'arbre AVL
	libererMemoire(racine);
}

int main() {
	analyserTrajets("data.csv");
	return 0;
}