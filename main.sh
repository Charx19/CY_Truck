#!/usr/bin/env bash

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

show_elapsed_second(){
    end=$(date +%s) # Records the end time
    local elapsed_seconds=$(( end - $1 )) # Difference between the end and start times

    echo "Elapsed time: $elapsed_seconds seconds"
}

#######################################

while [ $# -gt 0 ] ; 
do
    case $1 in
        -h)
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
            ;;

        -d2)
            d2=true
            ;;
        
        -l)
            l=true
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

# Check if C program exist
executable_name="progc/main"

if ! [ -x "$executable_name" ]; then
    echo "The C executable '$executable_name' is either not present or not executable."
fi

# Check the existence of folders
if check_folder temp; then
    rm -rf "$(dirname "$0")/temp"/* # Remove the contents of temp
fi

check_folder images

start=$(date +%s) # Records the start time

# Option -d1
if isset "$d1"; then
    awk 'BEGIN{FS=OFS=";"} NR > 1 {count[$6]++} END {for (i in count) print count[i], i}' "$input" | sort -nr | head -n 10 > "temp/filtered.csv"
    gnuplot -c "progc/d.gnu" "temp/filtered.csv" "d1" "NB ROUTES"
    show_elapsed_second "$start"
fi

# Option -d2
if isset "$d2"; then
    awk 'BEGIN{FS=OFS=";"} NR > 1 {count[$6]+=$5} END {for (i in count) print count[i], i}' "$input" | sort -nr | head -n 10 > "temp/filtered.csv"
    gnuplot -c "progc/d.gnu" "temp/filtered.csv" "d2" "DISTANCE (Km)"
    show_elapsed_second "$start"
fi

if isset "$l"; then
    show_elapsed_second "$start"
fi

