#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#define SIZE 250
	#define BIGSIZE 10000


	// Structure to represent a city
	typedef struct City {
		char Name[50];
		int Road;
		int Departure;
		struct City* left;
		struct City* right;
		int height; // Height fo AVL balancing
		int idIdx;
		int id[BIGSIZE];
	} City;

	// Function to calculate the height of a node in AVL tree
	int height(City* node) {
		if (node == NULL)
			return 0;
		return node->height;
	}

	// Function to find the maximum height between two values
	int max(int a, int b) {
		return (a > b) ? a : b;
	}

	// Function to create a new city node
	City* CreateCityNode(char CityName[]) {
		City* NewCity = (City*)malloc(sizeof(City));
		size_t size = sizeof(int) * BIGSIZE;
		strcpy(NewCity->Name, CityName);
		NewCity->Road = 1;
		NewCity->Departure = 0;
		NewCity->left = NULL;
		NewCity->right = NULL;
		NewCity->height = 1;
		NewCity->idIdx = 0;
		memset(NewCity->id, 0, size);
		return NewCity;
	}

	// Function to make a simple rotation to the right
	City* RotateRight(City* y) {
		City* x = y->left;
		City* T2 = x->right;

		x->right = y;
		y->left = T2;

		// Update heights
		y->height = max(height(y->left), height(y->right)) + 1;
		x->height = max(height(x->left), height(x->right)) + 1;

		return x;
	}

	// Function to make a simple rotation to the left
	City* RotateLeft(City* x) {
		City* y = x->right;
		City* T2 = y->left;

		y->left = x;
		x->right = T2;

		// Update heights
		x->height = max(height(x->left), height(x->right)) + 1;
		y->height = max(height(y->left), height(y->right)) + 1;

		return y;
	}

	// Function to get the balance factor of a node
	int getBalance(City* node) {
		if (node == NULL)
			return 0;
		return height(node->left) - height(node->right);
	}

	// Function to browse the tree alphabetically and display statistics for the top 10 cities
	void DisplayID(City* root) {
		
		if (root != NULL) {
			
			
			for(int i = 0; i < root->idIdx; i++)
			{
				if(i==0)
					printf("%s :\n",root->Name);
				printf("-%d\n", root->id[i]);
				if(root->id[i]==0)
					printf("\n");
			}
		}
	}


	// Function to insert a city into the AVL tree
	City* InsertAVL(City* root, char CityName[], int idroad, int isDeparture) {
		if (root == NULL)
		{
			City* node = CreateCityNode(CityName);
			if(isDeparture == 1)
			{
				node->id[0] = idroad;
				node->idIdx++;
			}
			return node;
		}
		
		if (strcmp(CityName, root->Name) < 0)
			root->left = InsertAVL(root->left, CityName, idroad, isDeparture);
		else if (strcmp(CityName, root->Name) > 0)
			root->right = InsertAVL(root->right, CityName, idroad, isDeparture);
		else {
			// City already exists, update stats
			int condition = 0;
			for(int i = 0; i < root->idIdx; i++){
				if(root->id[i] == idroad){
					condition = 1;
				}
			}
			
			if(condition == 0){
				root->Road++;
				root->id[root->idIdx] = idroad;
				root->idIdx++;
			}
			
			if(isDeparture == 1)
			{
				root->Departure++;
			}
			return root;
		}

		// Update the height of the current node
		root->height = 1 + max(height(root->left), height(root->right));

		// Get the balance factor for this node
		int Balance = getBalance(root);

		// Case of balance on the left
		if (Balance > 1 && strcmp(CityName, root->left->Name) < 0)
			return RotateRight(root);

		// Case of balance on the right
		if (Balance < -1 && strcmp(CityName, root->right->Name) > 0)
			return RotateLeft(root);

		// Case of left-right balance
		if (Balance > 1 && strcmp(CityName, root->left->Name) > 0) {
			root->left = RotateLeft(root->left);
			return RotateRight(root);
		}

		// Case of right-left balance
		if (Balance < -1 && strcmp(CityName, root->right->Name) < 0) {
			root->right = RotateRight(root->right);
			return RotateLeft(root);
		}

		return root;
	}

	// Function to browse the tree alphabetically and display statistics for the top 10 cities
	void DisplayCityStat(City* root, int count, char NameList[10][SIZE], int RouteList[], int DepartureList[]) {
		int condition = 0;
		if (root != NULL) {
			DisplayCityStat(root->left, count, NameList, RouteList, DepartureList);
			
			if(RouteList[count] < root->Road){
				for(int i=0; i<count; i++){
					if(strcmp(NameList[i], root->Name) == 0){
						condition = 1;
					}
				}
				if(condition == 0){
					strcpy(NameList[count], root->Name);
					RouteList[count] = root->Road;
					DepartureList[count] = root->Departure;
				}
			}

			DisplayCityStat(root->right, count, NameList, RouteList, DepartureList);
		}
	}


	void DisplayCityCallStat(City* root){
		
		char NameList[10][SIZE];
		int RouteList[10];
		int DepartureList[10];
		
		for (int i=0; i<10; i++){
			strcpy(NameList[i], root->Name);
			RouteList[i]=root->Road;
			DisplayCityStat(root, i, NameList, RouteList, DepartureList);
			printf("%s -Road %d -Départs %d\n", NameList[i], RouteList[i], DepartureList[i]);
		}
	}


	// Function to free the memory used by the AVL tree
	void FreeMemory(City* root) {
		if (root != NULL) {
			FreeMemory(root->left);
			FreeMemory(root->right);
			free(root);
		}
	}




	char** LineToTab(char* l, char* type)
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
		

		// count the number of elements to extract
		while (*tmp)
		{
		   if (';' == *tmp)
			   {
				   count++;
				   last_comma = tmp;
			   }
			   tmp++;
		}

		// adds space for the token
		if(last_comma < (l + strlen(l) - 1))
		{
			count ++;
		}

		count++;
		res = malloc(sizeof(char*) * count);
		if(res == NULL)
		{
			  printf("Error allocating memory");
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

		// Function to browse the tree and take the name of the cities with the most trips
	void getCity(City* root, int count, char NameList[10][SIZE], int RouteList[]){
		int condition = 0;
		if (root != NULL) {
			getCity(root->left, count, NameList, RouteList);
			
			if(RouteList[count] < root->Road){
				for(int i = 0; i < count; i++){
					if(strcmp(NameList[i], root->Name) == 0){
						condition = 1;
					}
				}
				if(condition == 0){
					strcpy(NameList[count], root->Name);
					RouteList[count] = root->Road;
				}
			}

			getCity(root->right, count, NameList, RouteList);
		}
	}
	
	void CreateFile(FILE* File, City* root, char Name[10][SIZE]) 
	{
		if(root != NULL)
		{
			CreateFile(File, root->left, Name);
			
			for(int i = 0; i < 10; i++){
				if(!strcmp(Name[i], root->Name))
				{
					fprintf(File, "%s;%d;%d\n", root->Name, root->Road, root->Departure);
				}						
			}
			
			CreateFile(File, root->right, Name);			
		}
		
	}
	
	int CreateCallFile(City* root)
	{
		// Open file in write mode ("w")
		FILE *File = fopen("T.txt", "w");

		// Check if the file could be opened successfully
		if (File == NULL) {
			printf("error opening file.\n");
			return 0;
		}
		
		// Search for cities to write in the file
		char NameList[10][SIZE];
		int RouteList[10];
		
		for(int i = 0; i < 10; i++)
		{
			strcpy(NameList[i], root->Name);
			RouteList[i] = root->Road;
			getCity(root, i, NameList, RouteList);
		}

		// Write the cities to the file
		fprintf(File, "City;Total;Departures\n");
		CreateFile(File, root, NameList);
			
		// Close file
		fclose(File);

		return 1;
		
	}
	
	// Main function to analyze paths from a file
	void CityTreatment(const char* FileName) {
			// Root of the AVL tree
		City* root = NULL;
		City* node = NULL;

			// Processing file data
		FILE * File = fopen(FileName,"r");
		if (File == NULL){
			printf("No input file of this name");
			exit(2);
		}


		int idroad, idstep, distance;
		char CityA[SIZE], CityB[SIZE], DriverName[SIZE];
		char* type;
		char ligne[SIZE];
		char** lineTab = NULL;
		fgets(ligne, sizeof(ligne), File);
        // we read each line and separate them by their ';' before storing important information
		while(fgets(ligne, sizeof(ligne), File) != NULL){

			lineTab = LineToTab(ligne, type);
			if (sscanf(lineTab[0], "%d", &idroad) != 1) {
				printf("Conversion error 1\n");
				exit(1);
			}
			if (sscanf(lineTab[1], "%d", &idstep) != 1) {
				printf("Conversion error 2\n");
				exit(1);
			}
			strcpy(CityA, lineTab[2]);
			strcpy(CityB, lineTab[3]);
			if (sscanf(lineTab[4], "%d", &distance) != 1) {
				printf("Conversion error 3\n");
				exit(1);
			}
			strcpy(DriverName, lineTab[5]);


			// Update statistics for cityA and cityB
			if(idstep == 1)
				root = InsertAVL(root, CityA, idroad, 1);
			else
				root = InsertAVL(root, CityA, idroad, 0);
			root = InsertAVL(root, CityB, idroad, 0);
			free(lineTab);
		}

		fclose(File);
		// Creating the file
		CreateCallFile(root);
		// Free the memory used by the AVL tree
		FreeMemory(root);
	}

	int main() {
		CityTreatment("data.csv");
		return 0;
	}