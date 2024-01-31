#!/usr/bin/env bash

help() {
    echo "Ce code est pour filter et afficher des données triés d'une société de transport, en dépendant du type d'option que vous allez selectionner "
	echo
	echo "Syntaxe : (bash) main.sh [argument]<option>"
	echo "Arguments :"
	echo
	echo "--help: afficher ce texte " 
	echo "Exemple : bash main.sh --help"
	echo
	echo "-i  sélectionne le fichier data.csv(cette commandes est essencielle)"
	echo "Exemple : bash main.sh -i data.csv"
	echo
	echo "-d<1|2> ceci créer un fichier avec la liste des conducteurs et le nombre de trajet différents qu'ils ont effectués"
	echo "d1, c'est pour trier par ordre décroissant le nombre de trajet, ce sera les 10 premiers conducteurs"                                                                                
	echo "d2, c'est pour trier par ordre décroissant le nombre de trajet, ce sera les 10 premiers conducteurs"
	echo "Exemple : bash main.sh -i data.csv -d2"
    echo
	echo "-l: créer un fichier avec les 10 plus grandes distances parcours par les conducteurs, ordonnez par numéro d'identifiant de trajet croissant"
	echo "Exemple : bash main.sh -i data/data.csv -l"
    echo
	echo "-t: créer un fichier qui aura les 10 villes les plus parcours en ordre alphabétique"
	echo "Exemple : bash main.sh -i data/data.csv -t"
    echo
	echo "-s: on récupère les distances minimales, maximales et moyennes"
	echo "Exemple : bash main.sh -i data/data.csv -s"
    echo
	echo "--avl: choisir le trie avec un avl, le plus rapide"
	echo "Exemple : bash main.sh -i data/data.csv -s --avl"
  	
}

#######################################
# Check if a variable exist
# Arguments :
#   The variable to check
# Returns :
#   0 if the variable exist, 0 if its doesn't
#######################################
isset() {
    if ! [ -z "$1" ]; then
        return 0 # The variable exist
    else
        return 1 # The variable doesn't exist
    fi
}

#######################################
# Check if a folder exist from the root directory, if not, create it
# Arguments :
#   The name of folder to check
# Returns :
#   0 if the folder exist, 1 if its doesn't
#######################################
check_folder(){
    local folder_name="$1"
    if [ -d "$(dirname "$0")/$folder_name" ]; then
        return 0
    else
        mkdir -p "$folder_name" # Create the folder if its doesn't exist
        return 1
    fi
}

#######################################
# Check if the argument for a option is empty
# Arguments :
#   The option
#   The arguments to check
#######################################
check_empty(){
    if [ -z "$2" ]; then
        echo "$1 option should not be empty"
    fi
}

#######################################
# Record time
# Arguments :
#   The string to add for the echo
#   The start time
#######################################
show_elapsed_second(){
    end=$(date +%s) # Records the end time
    local elapsed_seconds=$(( end - $2 )) # Difference between the end and start times

    echo "$1$elapsed_seconds seconds"
}

#######################################

option_defined=false

while [ $# -gt 0 ] ; 
do
    case $1 in
        --help)
            help
            exit
            ;;

        -i)
            check_empty "-i" "$2"
            input="$2"
            shift
            ;;

        -o)
            check_empty "-o" "$2"
            output="$2"
            shift
            ;;

        -d1)
            d1=true
            option_defined=true
            ;;

        -d2)
            d2=true
            option_defined=true
            ;;
        
        -l)
            l=true
            option_defined=true
            ;;
        -t)
            t=true
            option_defined=true
            ;;
        -s)
            s=true
            option_defined=true
            ;;

    esac
    shift
done

# Check if the input has been set
if ! isset "$input"; then
    echo "Missing argument -i"
    exit 1
fi

# Check if the input file exist
if ! [ -f "$input" ]; then
    echo "$input doesn't exist !"
    exit 1
fi

# Check if a option has been choosen
if ! "$option_defined";  then
    echo "option isn't defined !"
    exit 1
fi

# Check if C program exist
executable_path="progc/main"

if ! [ -x "$executable_path" ]; then
    echo "The C executable '$executable_path' is either not present or not executable."
    make -C "progc"
fi

# Check the existence of folders
if check_folder temp; then
    rm -rf "$(dirname "$0")/temp"/* # Remove the contents of temp
fi

check_folder images

# Option -d1
if isset "$d1"; then
    start=$(date +%s) # Records the start time
    # if (!ids[$6,$1]++) : Associative array to check if the Route ID hasn't been get already. If no, then add to the count
    # substr(driver, 1, length(driver)-1) = Remove the newline that is at the end of the name
    # sort -t';' -k2 -nr = Order by biggest number of routes first
    # head -n 10 = Keep the top 10 drivers
    awk -F ';' 'NR > 1 && !ids[$6,$1]++ {
        count[$6]++
    } 
    END {
        for (driver in count) {
            printf "%s;%d\n", substr(driver, 1, length(driver)-1), count[driver]
        }
    }' "$input" | sort -t';' -k2 -nr | head -n 10 > "temp/data_d1.dat"
    show_elapsed_second "Option -d1 has taken : " "$start"
    gnuplot -c "progc/gnuplots/d.gnu" "temp/data_d1.dat" "d1" "Nb routes" "NB ROUTES" "250"
    convert -rotate 90 "images/img_d1.png" "images/img_d1.png" 
fi

# Option -d2
if isset "$d2"; then
    start=$(date +%s) # Records the start time
    # substr(driver, 1, length(driver)-1) = Remove the newline that is at the end of the name
    # sort -t';' -k2 -nr = Order by biggest distance first
    # head -n 10 = Keep the top 10 biggest distance
    awk -F ';' 'NR > 1 {
        count[$6]+=$5
    } 
    END {
        for (driver in count) {
            printf "%s;%f\n", substr(driver, 1, length(driver)-1), count[driver]
        }
    }' "$input" | sort -t';' -k2 -nr | head -n 10 > "temp/data_d2.dat"
    show_elapsed_second "Option -d2 has taken : " "$start"
    gnuplot -c "progc/gnuplots/d.gnu" "temp/data_d2.dat" "d2" "Distance" "DISTANCE (Km)" "160000"
    convert -rotate 90 "images/img_d2.png" "images/img_d2.png"
fi

if isset "$l"; then
    start=$(date +%s) # Records the start time
    # sort -t';' -k2 -nr = Order by biggest distance first
    # head -n 10 = Keep the top 10 biggest distance
    # sort -n = Order by Route ID
    awk -F ';' 'NR > 1 {
        count[$1]+=$5
    } 
    END {
        for (i in count) {
            printf "%d;%f\n", i, count[i]
        }
    }' "$input" | sort -t';' -k2 -nr | head -n 10 | sort -nr > "temp/data_l.dat"
    show_elapsed_second "Option -l has taken : " "$start"
    gnuplot -c "progc/gnuplots/l.gnu" "temp/data_l.dat"
fi

if isset "$t"; then
    start=$(date +%s) # Records the start time
    "$executable_path" "$input" "temp/data_t.dat" "-t"
    show_elapsed_second "Option -t has taken : " "$start"
    gnuplot -c "progc/gnuplots/t.gnu" "temp/data_t.dat"
fi

if isset "$s"; then
    start=$(date +%s) # Records the start time
    "$executable_path" "$input" "temp/data_s.dat" "-s"
    show_elapsed_second "Option -s has taken : " "$start"
    gnuplot -c "progc/gnuplots/s.gnu" "temp/data_s.dat"
fi
