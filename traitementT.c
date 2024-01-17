void traitementCSV(FILE *fichier) {
    
printf("teste de debugage\n");

    Ville *racine = NULL;
    DonneesCSV donnees;
    while (!feof(fichier)) {
      
        fscanf(fichier, "%d, %d, %s, %s, %d, %s",
               &donnees.routeID, &donnees.stepID, donnees.townA, donnees.townB, &donnees.distance, donnees.driverName);

        racine = inserer(racine, donnees.townA, donnees.routeID, 1);
        racine = inserer(racine, donnees.townB, donnees.routeID, 0); 
    }

  if (racine != NULL) {
        VilleInfo infos[100];
        int index = 0;
        collecteInfos(racine, infos, &index);
        qsort(infos, index, sizeof(VilleInfo), comparaisonVilles);
        afficherTop10(infos, index);
    
printf("teste de debugage\n");
libererMemoire(racine);
    }    
}
