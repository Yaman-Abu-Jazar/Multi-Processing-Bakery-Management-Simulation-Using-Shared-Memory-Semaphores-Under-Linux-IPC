
#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"

// file to read the arguments text file
void readFile(char* filename){
    char line[200];
    char label[100];

    FILE *file;

    file = fopen(filename, "r");

    if (file == NULL){
        perror("The file not exist\n");
        exit(-2);
    }


    char separator[] = "=";

    while(fgets(line, sizeof(line), file) != NULL){

        char *str = strtok(line, separator);
        strncpy(label, str, sizeof(label));
        str = strtok(NULL, separator);


        if (strcmp(label, "NUMBER_OF_BREAD_CATEGORIES") == 0) {
            NUMBER_OF_BREAD_CATEGORIES = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_BREADS") == 0) {
            NUMBER_OF_BREADS = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SANDWICHES") == 0) {
            NUMBER_OF_SANDWICHES = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_CAKE_FLAVORS") == 0) {
            NUMBER_OF_CAKE_FLAVORS = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SWEET_FLAVORS") == 0) {
            NUMBER_OF_SWEET_FLAVORS = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SWEET_PATISSERIES") == 0) {
            NUMBER_OF_SWEET_PATISSERIES = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SAVORY_PATISSERIES") == 0) {
            NUMBER_OF_SAVORY_PATISSERIES = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_CHEFS") == 0) {
            NUMBER_OF_CHEFS = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_BAKERS") == 0) {
            NUMBER_OF_BAKERS = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SUPPLY_CHAIN_EMPLOYEES") == 0) {
            NUMBER_OF_SUPPLY_CHAIN_EMPLOYEES = atoi(str);
        }
        if (strcmp(label, "PRICE_OF_CAKE") == 0) {
            PRICE_OF_CAKE = atoi(str);
        }
        if (strcmp(label, "PRICE_OF_SANDWICH") == 0) {
            PRICE_OF_SANDWICH = atoi(str);
        }
        if (strcmp(label, "PRICE_OF_SWEET_PATESSERIES") == 0) {
            PRICE_OF_SWEET_PATESSERIES = atoi(str);
        }
        if (strcmp(label, "PRICE_OF_SAVORY_PATESSERIES") == 0) {
            PRICE_OF_SAVORY_PATESSERIES = atoi(str);
        }
        if (strcmp(label, "PRICE_OF_SWEET") == 0) {
            PRICE_OF_SWEET = atoi(str);
        }
        if (strcmp(label, "PRICE_OF_BREAD") == 0) {
            PRICE_OF_BREAD = atoi(str);
        }
        if (strcmp(label, "TOTAL_TIME_SIMULATION") == 0) {
            TOTAL_TIME_SIMULATION = atoi(str);
        }
        if (strcmp(label, "DESIRED_DAILY_PROFIT") == 0) {
            DESIRED_DAILY_PROFIT = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_WHEAT_MAX") == 0) {
            NUMBER_OF_WHEAT_MAX = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_WHEAT_MIN") == 0) {
            NUMBER_OF_WHEAT_MIN = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_YEAST_MIN") == 0) {
            NUMBER_OF_YEAST_MIN = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_YEAST_MAX") == 0) {
            NUMBER_OF_YEAST_MAX = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_BUTTER_MAX") == 0) {
            NUMBER_OF_BUTTER_MAX = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_BUTTER_MIN") == 0) {
            NUMBER_OF_BUTTER_MIN = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_CHEESE_MIN") == 0) {
            NUMBER_OF_CHEESE_MIN = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_CHEESE_MAX") == 0) {
            NUMBER_OF_CHEESE_MAX = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SALAMI_MAX") == 0) {
            NUMBER_OF_SALAMI_MAX = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SALAMI_MIN") == 0) {
            NUMBER_OF_SALAMI_MIN = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_MILK_MAX") == 0) {
            NUMBER_OF_MILK_MAX = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_MILK_MIN") == 0) {
            NUMBER_OF_MILK_MIN = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SUGAR_MAX") == 0) {
            NUMBER_OF_SUGAR_MAX = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SUGAR_MIN") == 0) {
            NUMBER_OF_SUGAR_MIN = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SALT_MAX") == 0) {
            NUMBER_OF_SALT_MAX = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SALT_MIN") == 0) {
            NUMBER_OF_SALT_MIN = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SWEET_ITEMS_MAX") == 0) {
            NUMBER_OF_SWEET_ITEMS_MAX = atoi(str);
        }
        if (strcmp(label, "NUMBER_OF_SWEET_ITEMS_MIN") == 0) {
            NUMBER_OF_SWEET_ITEMS_MIN = atoi(str);
        }
        if (strcmp(label, "MAX_NUM_OF_FRUSTRATED_CUSTOMER") == 0) {
            MAX_NUM_OF_FRUSTRATED_CUSTOMER = atoi(str);
        }
        if (strcmp(label, "MAX_NUM_OF_CUSTOMERS_THAT_COMPLAINED") == 0) {
            MAX_NUM_OF_CUSTOMERS_THAT_COMPLAINED = atoi(str);
        }
        if (strcmp(label, "MAX_NUM_OF_CUSTOMERS_THAT_REQUESTED_MISSING_ITEMS") == 0) {
            MAX_NUM_OF_CUSTOMERS_THAT_REQUESTED_MISSING_ITEMS = atoi(str);
        }
    }

    fclose(file);
}


// function to check if a specific digit exists in a specific number
int contains_digit(int number, int digit) {
    if (digit < 0 || digit > 9) return 0; // invalid digit

    number = number < 0 ? -number : number; // handle negative numbers

    while (number > 0) {
        if (number % 10 == digit)
            return 1;
        number /= 10;
    }

    return 0;
}

#endif