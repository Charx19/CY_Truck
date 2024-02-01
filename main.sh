#!/usr/bin/env bash

help() {
    echo "This code is to filter and print out with histograms the data from a Transport document, depending on which type of filter selected "
	echo
	echo "Syntax : (bash) main.sh [argument]<option>"
	echo "This is the syntax will be using for this code"
	echo
	echo "--help: to print this message out  " 
	echo "Exemple : bash main.sh --help"
	echo
	echo "-i:  the argument needed to execute our project with the correct data document"
 	echo " do not forget the data/data.csv to be able to find the correct document"
	echo "Example: bash main.sh -i data/data.csv"
	echo
	echo "-d<1|2> These 2 options are to print out a histogram with a list of drivers the top 10 drivers depending on these two factors: the number of trips or total distance"
	echo "d1, this option is for the number of trips made per driver, they will be in order from the biggest amount to the smallest "                                                                                
	echo "d2, this option takes the total distance per driver, they will be in order from the biggest amount to the smallest  "
	echo "Exemple : bash main.sh -i data/data.csv -d2"
    echo
	echo "-l: this option takes the total distance of each driver, and prints them out from smallest to biggest. The values are linked to the driver's ID number"
	echo "Exemple : bash main.sh -i data/data.csv -l"
    echo
	echo "-t: this option takes the most traveled cities and prints them in alphabetical order, the top 10 cities will appear on the histogram"
	echo "Exemple : bash main.sh -i data/data.csv -t"
    echo
	echo "-s: this option takes the minimum, maximal and average distance per trip "
	echo "Exemple : bash main.sh -i data/data.csv -s"
  
  	
}

#######################################
# Check if a variable exists
# Arguments :
#   The variable to check
# Returns :
#   0 if the variable exists, 0 if it doesn't
#######################################
isset() {
    if ! [ -z "$1" ]; then
        return 0 # The variable exists
    else
        return 1 # The variable doesn't exist
    fi
}

#######################################
# Check if a folder exists from the root directory, if not, create it
# Arguments :
#   The name of the folder to check
# Returns :
#   0 if the folder exists, 1 if it doesn't
#######################################
check_folder(){
    local folder_name="$1"
    if [ -d "$(dirname "$0")/$folder_name" ]; then
        return 0
    else
        echo "Creating $folder_name folder..."
        mkdir -p "$folder_name" # Create the folder if its doesn't exist
        return 1
    fi
}

#######################################
# Check if the argument for an option is empty
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
    echo "Missing argument -i, see --help for information"
    exit 1
fi

# Check if the input file exist
if ! [ -f "$input" ]; then
    echo "$input doesn't exist !"
    exit 1
fi

# Check if an option has been chosen
if ! "$option_defined";  then
    echo "Option isn't defined !, see --help for information"
    exit 1
fi

# Check if the C program exists
executable_path="progc/main"

# If it doesn't, compile it with makefile
if ! [ -x "$executable_path" ]; then
    echo "The C executable '$executable_path' is either not present or not executable."
    echo "Compiling C program..."
    make -C "progc"
fi

# Check the existence of folders
if check_folder temp; then
    echo "Deleting temp contents..."
    rm -rf "$(dirname "$0")/temp"/* # Remove the contents of temp
fi

check_folder images

# Option -d1
if isset "$d1"; then
    start=$(date +%s) # Records the start time
    # if (!ids[$6,$1]++) : Associative array to check if the Route ID hasn't been gotten already. If no, then add to the count
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
