
#include "header.h"
#include "constants.h"
#include "functions.h"

void ctrl_c_handler(int signal);
void alarm_handler(int signal);

pid_t *ch_id, *b_id, *s_id, *sch_id;
int shmid, semid, shmid_ch, semid_ch, semid_oven, msqid, semid_cash, shmid_cs;
SHM memory;
CHM ch_m;
mem_customer cs_m;
int customer_counter;
pid_t drawer_id;
pid_t customer_id[NUMBER_OF_CUSTOMER_MAX];

int main(int argc, char** argv){

    readFile("arguments.txt");

    ch_id = malloc(NUMBER_OF_CHEFS * sizeof(pid_t));
    sch_id = malloc(NUMBER_OF_SUPPLY_CHAIN_EMPLOYEES * sizeof(pid_t));
    b_id = malloc(NUMBER_OF_BAKERS * sizeof(pid_t));
    s_id = malloc(NUMBER_OF_SELLERS * sizeof(pid_t));

    // declare variables
    pid_t pid = getpid();
    union semun arg;
    static ushort start_val[5] = {1, 1, 1, 1, 1};
    static ushort start_val_chef[5] = {1, 1, 1, 1, 1};
    static ushort start_val_oven[2] = {NUMBER_OF_OVENS, 0};
    static ushort start_val_cash[2] = {NUMBER_OF_CASHES, 0};

    // create the shared memory of shared items
    if((shmid = shmget((key_t)pid, sizeof(SHM), IPC_CREAT | 0777)) != -1){
        if((memory = (SHM)shmat(shmid, NULL, 0)) == (SHM)-1){
            perror("Shared Memory Attachment Parent Error.\n");
            exit(1);
        }
    } else {
        perror("Shared Memory Creation Error.\n");
        exit(1);
    }

    // create the shared memory between chefs and bakers
    if((shmid_ch = shmget(CHEF_MEM_KEY, sizeof(CHM), IPC_CREAT | 0777)) != -1){
        if((ch_m = (CHM)shmat(shmid_ch, NULL, 0)) == (CHM)-1){
            perror("Shared Chef Memory Attachment -- Parent -- Error.\n");
            exit(1);
        }
    } else {
        perror("Shared Chef Memory Creation -- Parent -- Error.\n");
        exit(1);
    }

    // create semaphore to protect access to shared items
    if((semid = semget((key_t)pid, 5, IPC_CREAT | 0777)) != -1){
        arg.array = start_val;
        if(semctl(semid, 0, SETALL, arg) == -1){
            perror("Semaphore Attachment Parent Error.\n");
            exit(2);
        }
    } else {
        perror("Semaphore Creation Error.\n");
        exit(2);
    }

    // create a semaphore to protect access to the chef's products
    if((semid_ch = semget(CHEF_SEM_KEY, 5, IPC_CREAT | 0777)) != -1){
        arg.array = start_val_chef;
        if(semctl(semid_ch, 0, SETALL, arg) == -1){
            perror("Semaphore Attachment Parent Error.\n");
            exit(2);
        }
    } else {
        perror("Semaphore Chef Creation Error.\n");
        exit(2);
    }

    // Create the semaphore of ovens
    if((semid_oven = semget(OVEN_SEM_KEY, 2, IPC_CREAT | 0777)) != -1){
        arg.array = start_val_oven;
        if(semctl(semid_oven, 0, SETALL, arg) == -1){
            perror("Semaphore Attachment Parent Error.\n");
            exit(2);
        }
    } else {
        perror("Semaphore Oven Creation Error.\n");
        exit(2);
    }

    // Create the semaphore of chash
    if((semid_cash = semget(CASH_SEM_KEY, 2, IPC_CREAT | 0777)) != -1){
        arg.array = start_val_cash;
        if(semctl(semid_cash, 0, SETALL, arg) == -1){
            perror("Semaphore Attachment Parent Error.\n");
            exit(2);
        }
    } else {
        perror("Semaphore Cash Creation Error.\n");
        exit(2);
    }

    //create a shared memory between customers and sellers
    if((shmid_cs = shmget(CUSTOMER_SHM_KEY, sizeof(mem_customer), IPC_CREAT | 0777)) != -1){
        if((cs_m = (mem_customer)shmat(shmid_ch, NULL, 0)) == (mem_customer)-1){
            perror("Shared customer Memory Attachment -- Parent -- Error.\n");
            exit(1);
        }
    } else {
        perror("Shared customer Memory Creation -- Parent -- Error.\n");
        exit(1);
    }

    // create the supply chain employees (supply chain processes)
    for(int i = 0 ; i < NUMBER_OF_SUPPLY_CHAIN_EMPLOYEES ; i++){
        if((sch_id[i] = fork()) == -1){
            perror("fork -- supply chain -- parent.\n");
            exit(3);
        }
        if(sch_id[i] == 0){
            execlp("./supply_chain", "supply_chain", NULL);
            perror("Supply Chain Exec Error.\n");
            exit(4);
        } else {
            // do nothing just continue 
        }
    }

    // Create the Chefs Processes and adjust their jobs according to their number
    int j;
    char k[12];
    for(int i = 0 ; i < NUMBER_OF_CHEFS ; i++){
        if((ch_id[i] = fork()) == -1){
            perror("fork -- chef -- parent.\n");
            exit(3);
        } else {
            if(NUMBER_OF_CHEFS >= NUMBER_OF_CHEF_TEAMS){
                j = (i % NUMBER_OF_CHEF_TEAMS) + 1;
            }
            // number of chefs < number of chef teams
            else if(NUMBER_OF_CHEFS == 5){
                j = (i % NUMBER_OF_CHEF_TEAMS) + 1;
                if(j == 5){
                    j += 51;
                }
            } else if(NUMBER_OF_CHEFS == 4){
                j = (i % NUMBER_OF_CHEF_TEAMS) + 1;
                if(j == 2){
                    j += 22;
                } else if(j == 4){
                    j += 52;
                }
            } else if(NUMBER_OF_CHEFS == 3){
                j = (i % NUMBER_OF_CHEF_TEAMS) + 1;
                if(j == 1){
                    j += 15;
                } else if(j == 2){
                    j += 22;
                } else if(j == 3){
                    j += 32;
                }
            } else if(NUMBER_OF_CHEFS == 2){
                j = (i % NUMBER_OF_CHEF_TEAMS) + 1;
                if(j == 1){
                    j += 134;
                } else{
                    j += 244;
                }
            } else if(NUMBER_OF_CHEFS == 1) {
                j = 123456;
            }
            sprintf(k, "%d", j);
        }
        if(ch_id[i] == 0){
            execlp("./chefs", "chefs", k, NULL);
            perror("Chefs Exec Error.\n");
            exit(4);
        } else {
            // do nothing just continue 
        }
    }

    // creating bakers processes and adjust their positions relative to ovens according to their number
    for(int i = 0 ; i < NUMBER_OF_BAKERS ; i++){
        if((b_id[i] = fork()) == -1){
            perror("fork -- baker -- parent.\n");
            exit(3);
        } else {
            if(NUMBER_OF_BAKERS >= NUMBER_OF_BAKER_TEAMS){
                j = (i % NUMBER_OF_BAKER_TEAMS) + 1;
            } else if(NUMBER_OF_BAKERS == 2){
                j = (i % NUMBER_OF_BAKER_TEAMS) + 1;
                if(j == 1){
                    j += 11;
                } else {
                    j += 1;
                }
            } else if(NUMBER_OF_BAKERS == 1){
                j = 123;
            }
            sprintf(k, "%d", j);
        }
        if(b_id[i] == 0){
            execlp("./bakers", "baker", k, NULL);
            perror("Baker Exec Error.\n");
            exit(4);
        } else {
            // do nothing just continue 
        }
    }

    for(int i = 0 ; i < NUMBER_OF_SELLERS ; i++){
        if((s_id[i] = fork()) == -1){
            perror("fork -- seller -- parent.\n");
            exit(3);
        }
        if(s_id[i] == 0){
            j = i + 1;
            sprintf(k, "%d", j);
            execlp("./sellers", "seller", k, NULL);
            perror("Seller Exec Error.\n");
            exit(4);
        } else {
            // do nothing just continue 
        }
    }

    if(signal(SIGINT, ctrl_c_handler) == SIG_ERR){
        perror("Control C Catchable Error.\n");
        exit(-1);
    }

    printf("Parent %d.All Employees have started their jobs.\n", getpid());

    if(signal(SIGALRM, alarm_handler) == SIG_ERR){
        perror("Control C Catchable Error.\n");
        exit(-1);
    }

    alarm(TOTAL_TIME_SIMULATION * 60);
    memory->simulation_time_passed = TOTAL_TIME_SIMULATION * 60;
    usleep(500000);
    // create the drawer process
    if((drawer_id = fork()) == -1){
        perror("Fork drawer process -- Error.\n");
        exit(9);
    }
    if(drawer_id == 0){
        execlp("./drawer", "drawer", NULL);
        perror("Execlp drawer Error.\n");
        exit(9);
    }
    customer_counter = 0;
    int quite_period;
    char c[12];
    srand(time(NULL));
    while(1){
        if((customer_id[customer_counter++] = fork()) == -1){
            perror("customer fork error.\n");
            exit(6);
        }
        if(customer_id[customer_counter-1] == 0){
            sprintf(c, "%d", customer_counter);
            execlp("./customer", "customer", c, NULL);
            perror("customer execlp error.\n");
            exit(6);
        } 

        // Stop Conditions
        if(memory->total_profit >= DESIRED_DAILY_PROFIT || memory->missing_items_requests >= MAX_NUM_OF_CUSTOMERS_THAT_REQUESTED_MISSING_ITEMS || memory->frustrated_customers >= MAX_NUM_OF_FRUSTRATED_CUSTOMER || memory->complained_customers >= MAX_NUM_OF_CUSTOMERS_THAT_COMPLAINED){
            ctrl_c_handler(SIGINT);
        }
        // sleep until fork another customer
        quite_period = (int) (QUITE_PERIOD_MIN + (rand() % (QUITE_PERIOD_MAX - QUITE_PERIOD_MIN)));
        sleep(quite_period);
    }


    return 0;
}

void ctrl_c_handler(int signal){
    
    for(int i = 0 ; i < NUMBER_OF_SUPPLY_CHAIN_EMPLOYEES ; i++){
        kill(sch_id[i], SIGINT);
        printf("Kill Supply Chain Employee.\n");
    }

    for(int i = 0 ; i < NUMBER_OF_BAKERS ; i++){
        kill(b_id[i], SIGINT);
        printf("Kill Baker.\n");
    }

    for(int i = 0 ; i < NUMBER_OF_CHEFS ; i++){
        kill(ch_id[i], SIGINT);
        printf("Kill Chef\n");
    }

    for(int i = 0 ; i < NUMBER_OF_SELLERS ; i++){
        kill(s_id[i], SIGINT);
        printf("Kill Seller\n");
    }

    for(int i = 0 ; i < customer_counter ; i++){
        kill(customer_id[i], SIGINT);
        printf("Kill Seller\n");
    }

    kill(drawer_id, SIGINT);

    printf("wheat %d -- yeast %d -- butter %d -- milk %d -- sweet items %d.\n", memory->wheat, memory->yeast, memory->butter, memory->milk, memory->sweet_items);
    printf("bread %d -- cake %d -- sweet %d -- savory p %d -- sweet p %d -- sandwiches %d paste %d.\n", memory->breads_ready, memory->cakes_ready, memory->sweets_ready, memory->savory_patisseries_ready, memory->sweet_patisseries_ready, memory->sandwiches_ready, ch_m->prepared_paste);
    printf("cake %d -- sweet %d -- sweet p %d -- savory p %d.\n", ch_m->prepared_cake, ch_m->prepared_sweet, ch_m->prepared_sweet_patisseries, ch_m->prepared_savory_patisseries);
    printf("Profit = %d -- frustrated %d -- complained %d -- customer number %d -- missing items %d -- Time Remaining %d\n", memory->total_profit, memory->frustrated_customers, memory->complained_customers, customer_counter, memory->missing_items_requests, memory->simulation_time_passed);
    shmdt(memory);
    shmdt(ch_m);
    shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
    semctl(semid, 0, IPC_RMID, 0);
    shmctl(shmid_ch, IPC_RMID, (struct shmid_ds *) 0);
    semctl(semid_ch, 0, IPC_RMID, 0);
    semctl(semid_oven, 0, IPC_RMID, 0);
    exit(0);
}

void alarm_handler(int signal){
    ctrl_c_handler(SIGINT);
}
