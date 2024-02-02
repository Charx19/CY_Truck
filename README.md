# CY_Truck
This project was made by HICHAM, ABHISHEK, and CHARLES

Our project was to simulate that we ran a transportation company. Our goal was to take the data of all the drivers and trips made throughout the last few years and make accurate histograms to visualize the data.
We made 5 different types of options : 
  - d<1/2>: to filter by the number of trips made per the top 10 drivers or the total distance per the top 10 drivers
  - L: The total distance per driver ID
  - T: The top 10 most traveled cities 
  - S: The minimal, maximal, et average distance per trip 

Each of these options will send back a histogram (d1,d2, L, and T ) and S will send a graph

# Help

To know exactly how to execute and make our project work : here is the "help"

```
"Syntax : (bash) main.sh [argument]<option>"
"This is the syntax will be using for this code"

"--help: to print this message out  " 
"Exemple : bash main.sh --help"

"-i:  the argument needed to execute our project with the correct data document"
" do not forget the data/data.csv to be able to find the correct document"
"Example: bash main.sh -i data/data.csv"

"-d<1|2> These 2 options are to print out a histogram with a list of drivers the top 10 drivers depending on these two factors: the number of trips or total distance"
"d1, this option is for the number of trips made per driver, they will be in order from the biggest amount to the smallest "                                                                                
"d2, this option takes the total distance per driver, they will be in order from the biggest amount to the smallest  "
"Exemple : bash main.sh -i data/data.csv -d2"

"-l: this option takes the total distance of each driver, and prints them out from smallest to biggest. The values are linked to the driver's ID number"
"Exemple : bash main.sh -i data/data.csv -l"

For the next two options, you will need to use the makefile: "make -C progc" before executing them
"-t: this option takes the most traveled cities and prints them in alphabetical order, the top 10 cities will appear on the histogram"
"Exemple : bash main.sh -i data/data.csv -t"

"-s: this option takes the minimum, maximal, and average distance per trip "
"Exemple : bash main.sh -i data/data.csv -s"
```
   
