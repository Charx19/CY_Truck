#!/bin/bash


help()
{
echo "Ce code est pour filter et afficher des données triés d'une société de transport, en dépendant du type d'option que vous allez selectionner "
	echo
	echo "Syntaxe : (bash) main.sh [argument]<option>"
	echo "Argument :"
	echo
	echo "--help     afficher ce texte " 
	echo "Exemple : bash main.sh --help"
	echo
	echo "-f [file]     sélection le fichier data.csv(cette commandes est essencielle)"
	echo "Exemple : bash main.sh -f data.csv"
	echo
	echo "-d<1|2>     ceci créer un fichier avec la liste des conducteurs et le nombre de trajet différents qu'ils ont effectués"
	echo " d1 , c'est pour trier par ordre décroissant le nombre de trajet, ce sera les 10 premiers conducteurs"                                                                                
	echo "                                                                                 3) d2 , c'est pour trier par ordre décroissant le nombre de trajet, ce sera les 10 premiers conducteurs"
	echo "Exemple : bash main.sh -f data.csv -d2"
	echo "-l      créer un fichier avec les 10 plus grandes distances parcours par les conducteurs, ordonnez par numéro d'identifiant de trajet croissant"
	echo "Exemple : bash main.sh -f data.csv -l"
	echo " -t , créer un fichier qui aura les 10 villes les plus parcours en ordre alphabétique"
	echo "Exemple : bash main.sh -f data.csv -t"
	echo " -s , on récupère les distances minimales, maximales et moyennes"
	echo "Exemple : bash main.sh -f data.csv -s"
	echo "--avl    choisir le trie avec un avl, le plus rapide"
	echo "Exemple : bash main.sh -f data.csv -s --avl"
	echo
	echo "--abr    choisir le trie avec un abr, le moins rapide"
	echo "Exemple : bash main.sh -f data.csv -t --abr"	

}

if [[$1 == '--help']]; then
	help
	exit
fi









