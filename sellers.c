#include "header.h"
#include "constants.h"
#include "functions.h"

struct sembuf acquire = {0, -1, SEM_UNDO},
              release = {0, 1, SEM_UNDO};

int semid_cash, shmid, shmid_customer, semid;
pid_t ppid;
SHM memory;
mem_customer mem_cs;

int main(int argc, char** argv){
    sleep(15);

    readFile("arguments.txt");

    int flag = atoi(argv[1]);
    ppid = getppid();
    if((shmid = shmget((key_t)ppid, sizeof(memory), 0777)) != -1){
        if((memory = (SHM)shmat(shmid, NULL, 0)) == (SHM)-1){
            perror("Shared Memory Attachment -- Chef -- Error.\n");
            exit(1);
        }
    } else {
        perror("Shared Memory Creation Error.\n");
        exit(1);
    }

    if((semid = semget((key_t)ppid, 2, IPC_CREAT | 0777)) == -1){
        perror("Semaphore Creation Error.\n");
        exit(2);
    }

    if((shmid_customer = shmget(CUSTOMER_SHM_KEY, sizeof(mem_cs), 0777)) != -1){
        if((mem_cs = (mem_customer)shmat(shmid_customer, NULL, 0)) == (mem_customer)-1){
            perror("Shared Memory Attachment -- Chef -- Error.\n");
            exit(1);
        }
    } else {
        perror("Shared Memory Creation Error.\n");
        exit(1);
    }

    if ( (semid_cash = semget(CASH_SEM_KEY, 0, 0)) == -1 ) {
        perror("Cash Sem Creation -- seller\n");
        exit(7);
    }
    int counter = 0;
    if(flag == 1){
            counter = 1;
    } else {
        counter = 2;
    }
    int price;
    int profit = 0;
    while(1){
        acquire.sem_num = 1;

        if(semop(semid_cash, &acquire, 1) == -1){
            perror("Acquire Error -- semop -- seller.\n");
            exit(7);
        }

        if(mem_cs->c[counter].frustrated != 1 && mem_cs->c[counter].leave != 1){
            if(mem_cs->c[counter].order == 1){
                if(memory->breads_ready >= mem_cs->c[counter].quantity){
                    price = PRICE_OF_BREAD;
                    memory->breads_ready -= mem_cs->c[counter].quantity;
                    printf("\033[95mSeller %d. I have sold %d bread.\033[0m\n", getpid(), mem_cs->c[counter].quantity);
                } else {
                    memory->missing_items_requests++;
                    printf("\033[95mSeller %d.Missed Bread.\033[0m\n", getpid());
                    price = 0;
                }
            } else if(mem_cs->c[counter].order == 2){
                if(memory->cakes_ready >= mem_cs->c[counter].quantity){
                    price = PRICE_OF_CAKE;
                    memory->cakes_ready -= mem_cs->c[counter].quantity;
                    printf("\033[95mSeller %d. I have sold %d cake.\033[0m\n", getpid(), mem_cs->c[counter].quantity);
                } else{
                    memory->missing_items_requests++;
                    printf("\033[95mSeller %d.Missed Cake.\033[0m\n", getpid());
                    price = 0;
                }
                
            } else if(mem_cs->c[counter].order == 3){
                if(memory->sandwiches_ready >= mem_cs->c[counter].quantity){
                    price = PRICE_OF_SANDWICH;
                    memory->sandwiches_ready -= mem_cs->c[counter].quantity;
                    printf("\033[95mSeller %d. I have sold %d sandwish.\033[0m\n", getpid(), mem_cs->c[counter].quantity);
                } else{
                    memory->missing_items_requests++;
                    printf("\033[95mSeller %d.Missed Sandwish.\033[0m\n", getpid());
                    price = 0;
                }  
            } else if(mem_cs->c[counter].order == 4){
                if(memory->sweets_ready >= mem_cs->c[counter].quantity){
                    memory->sweets_ready -= mem_cs->c[counter].quantity;
                    printf("\033[95mSeller %d. I have sold %d sweet.\033[0m\n", getpid(), mem_cs->c[counter].quantity);
                    price = PRICE_OF_SWEET;
                } else {
                    memory->missing_items_requests++;
                    printf("\033[95mSeller %d.Missed Sweet.\033[0m\n", getpid());
                    price = 0;
                }
            } else if(mem_cs->c[counter].order == 5){
                if(memory->sweet_patisseries_ready >= mem_cs->c[counter].quantity){
                    price = PRICE_OF_SWEET_PATESSERIES;
                    memory->sweet_patisseries_ready -= mem_cs->c[counter].quantity;
                    printf("\033[95mSeller %d. I have sold %d sweet patisserie.\033[0m\n", getpid(), mem_cs->c[counter].quantity);
                } else {
                    memory->missing_items_requests++;
                    printf("\033[95mSeller %d.Missed Sweet Patisserie.\033[0m\n", getpid());
                    price = 0;
                }
            } else if(mem_cs->c[counter].order == 6){
                if(memory->savory_patisseries_ready >= mem_cs->c[counter].quantity){
                    price = PRICE_OF_SAVORY_PATESSERIES;
                    memory->savory_patisseries_ready -= mem_cs->c[counter].quantity;
                    printf("\033[95mSeller %d. I have sold %d savory patisserie.\033[0m\n", getpid(), mem_cs->c[counter].quantity);
                } else {
                    price = 0;
                    memory->missing_items_requests++;
                    printf("\033[95mSeller %d.Missed Savory Patisserie.\033[0m\n", getpid());
                }
            }
            
            mem_cs->c[counter].invoice = price * mem_cs->c[counter].quantity;
            if(mem_cs->c[counter].complained == 1){
                profit += 0; // reimburse the unhappy customer by giving free order
                memory->complained_customers++;
            }else {
                profit += mem_cs->c[counter].invoice;
                memory->total_profit += profit;
            }
        } else {
            memory->frustrated_customers++;
        }

        counter += 2;


        release.sem_num = 0;

        if(semop(semid_cash, &release, 1) == -1){
            perror("Release Error -- semop -- seller.\n");
            exit(7);
        }

    }

    

    return 0;
}