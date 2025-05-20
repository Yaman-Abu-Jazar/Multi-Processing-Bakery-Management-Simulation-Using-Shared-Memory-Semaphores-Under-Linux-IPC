#ifndef CONSTANTS
#define CONSTANTS

#define PI 3.14159265358979323846f

#define NUMBER_OF_OVENS 3
#define NUMBER_OF_CHEF_TEAMS 6
#define NUMBER_OF_BAKER_TEAMS 3
#define CHEF_MEM_KEY 0x1234
#define CHEF_SEM_KEY 0x1357
#define OVEN_SEM_KEY 0x2468
#define NUMBER_OF_PASTE 50
#define QUITE_PERIOD_MAX 8
#define QUITE_PERIOD_MIN 1
#define NUMBER_OF_CUSTOMER_MAX 100
#define CASH_SEM_KEY 0x9753
#define CUSTOMER_SHM_KEY 0x8642
#define NUMBER_OF_CASHES 2

// Time to prepare and bake products (in sec)
#define BREAD_TIME 10
#define CAKE_TIME 10
#define SWEET_TIME 10
#define SANDWICH_TIME 5
#define PASTE_TIME 10
#define SWEET_P_TIME 5
#define SAVORY_P_TIME 5
#define SAVORY_P_PREPARE 5
#define SWEET_P_PREPARE 5
#define CAKE_PREPARE 5
#define SWEET_PREPARE 5

// sleeping time for supply chain employees (in sec)
#define SUPPLY_CHAIN_SLEEP 30

static int NUMBER_OF_BREAD_CATEGORIES = 5;
static int NUMBER_OF_BREADS = 10;
static int NUMBER_OF_SANDWICHES = 5;
static int NUMBER_OF_CAKE_FLAVORS = 5;
static int NUMBER_OF_SWEET_FLAVORS = 5;
static int NUMBER_OF_SWEET_PATISSERIES = 10;
static int NUMBER_OF_SAVORY_PATISSERIES = 10;
static int NUMBER_OF_CHEFS = 5;
static int NUMBER_OF_BAKERS = 5;
static int NUMBER_OF_SELLERS = 2;
static int NUMBER_OF_SUPPLY_CHAIN_EMPLOYEES = 5;
static int PRICE_OF_CAKE = 50;
static int PRICE_OF_SANDWICH = 20;
static int PRICE_OF_SWEET_PATESSERIES = 15;
static int PRICE_OF_SAVORY_PATESSERIES = 15;
static int PRICE_OF_SWEET = 25;
static int PRICE_OF_BREAD = 5;
static int TOTAL_TIME_SIMULATION = 10;
static int DESIRED_DAILY_PROFIT = 1000;
static int NUMBER_OF_WHEAT_MAX = 50;
static int NUMBER_OF_WHEAT_MIN = 5;
static int NUMBER_OF_YEAST_MAX = 50;
static int NUMBER_OF_YEAST_MIN = 5;
static int NUMBER_OF_BUTTER_MAX = 50;
static int NUMBER_OF_BUTTER_MIN = 5;
static int NUMBER_OF_CHEESE_MAX = 30;
static int NUMBER_OF_CHEESE_MIN = 3;
static int NUMBER_OF_SALAMI_MAX = 30;
static int NUMBER_OF_SALAMI_MIN = 3;
static int NUMBER_OF_MILK_MAX = 50;
static int NUMBER_OF_MILK_MIN = 5;
static int NUMBER_OF_SUGAR_MAX = 50;
static int NUMBER_OF_SUGAR_MIN = 5;
static int NUMBER_OF_SALT_MAX = 50;
static int NUMBER_OF_SALT_MIN = 5;
static int NUMBER_OF_SWEET_ITEMS_MAX = 30;
static int NUMBER_OF_SWEET_ITEMS_MIN = 3;
static int MAX_NUM_OF_FRUSTRATED_CUSTOMER = 10;
static int MAX_NUM_OF_CUSTOMERS_THAT_COMPLAINED = 10;
static int MAX_NUM_OF_CUSTOMERS_THAT_REQUESTED_MISSING_ITEMS = 10;

static int complain = 0;


union semun {
    int val;
    struct semid_ds * buf;
    ushort * array;
};

enum ORDERS {BREAD = 1, CAKE, SANDWICH, SWEET, SWEER_PATISSERIE, SAVORY_PATISSERIES};

typedef struct customer_info{
    int id;
    enum ORDERS order;
    int quantity;
    int frustrated;
    int complained;
    int invoice;
    int done;
    int leave;
} customer;

struct customer_memory{
    customer c[NUMBER_OF_CUSTOMER_MAX];
};

typedef struct customer_memory* mem_customer;

struct Bakery_Shared_Memory {
    int wheat;
    int yeast;
    int butter;
    int cheese;
    int milk;
    int sugar;
    int salt;
    int sweet_items;
    int salami;

    int breads_ready;
    int cakes_ready;
    int sandwiches_ready;
    int sweet_patisseries_ready;
    int savory_patisseries_ready;
    int sweets_ready;

    int total_profit;

    int frustrated_customers;
    int complained_customers;
    int missing_items_requests;

    int simulation_time_passed;
};

typedef struct Bakery_Shared_Memory* SHM;

struct chef_memory {
    int prepared_paste;
    int prepared_cake;
    int prepared_sweet;
    int prepared_sweet_patisseries;
    int prepared_savory_patisseries;
};

typedef struct chef_memory * CHM;

// constants for shared memory
enum {BUY_INGREDIENTS = 0, CONSUM_INGREDIENTS, PRODUCE_CHEF, CONSUM_BREAD, PRODUCE_BAKER}; 

// chefs jobs
enum {PREPARE_PASTE = 1, PREPARE_CAKES, PREPARE_SANDWICHES, PREPARE_SWEETS, PREPARE_SWEET_PATISSERIES, PREPARE_SAVORY_PATISSERIES};

enum {PRODUCED_ITEMS_CHEF = 0, CONSUME_ITEMS_CHEF, CONSUME_CAKE_SWEET, CONSUME_PATISSERIES, CONSUME_PASTE};

enum {ACQUIRE_OVEN, RELEASE_OVEN};

#endif