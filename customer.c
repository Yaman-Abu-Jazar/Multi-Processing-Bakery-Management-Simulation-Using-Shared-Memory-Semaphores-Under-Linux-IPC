#include "header.h"
#include "constants.h"
#include "functions.h"

struct sembuf acquire = {0, -1, SEM_UNDO},
              release = {0, 1, SEM_UNDO};

int semid_cash, shmid_customer;
mem_customer mem_cs;
customer c;
void alarm_handler(int signal);
int id;

//printf("\033[1;31mThis is red text\033[0m\n");

int main(int argc, char** argv){
    sleep(5);
    readFile("arguments.txt");

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
        perror("Cash Sem Creation -- customer\n");
        exit(7);
    }
    id = atoi(argv[1]);
    c.id = atoi(argv[1]);
    c.frustrated = 0;
    c.leave = 0;
    c.complained = 0;
    c.done = 0;
    c.invoice = 0;
    srand(time(NULL) ^ getpid());
    // create the orders of the customer.
    c.order = (int) (1 + rand() % 6);
    c.quantity = (int) (1 + rand() % 3);

    if(signal(SIGALRM, alarm_handler) == SIG_ERR){
        perror("Alarm Signal Error -- Customer.\n");
        exit(7);
    }
    alarm(10);

    acquire.sem_num = 0;

    if(semop(semid_cash, &acquire, 1) == -1){
        perror("Acquire Error -- semop -- customer\n");
        exit(7);
    }
    mem_cs->c[id] = c;
    mem_cs->c[id].done = 1;

    sleep(3);

    if(rand() % 10 > 8){
        mem_cs->c[id].complained = 1;
        complain = 1;
        printf("\033[1;31mCustomer %d.I have comlained.\033[0m\n", getpid());
        // the customer is not satisfied. he complained.
    }
    if(complain == 1){
        if(rand() % 10 > 8){
            mem_cs->c[id].leave = 1;
            complain = 0;
        }
    }

    release.sem_num = 1;

    if(semop(semid_cash, &release, 1) == -1){
        perror("Release Error -- semop -- customer\n");
        exit(7);
    }

    // printf("\033[1;31mThe Customer # %d has left and he is happy.\033[0m\n", c.id);
    return 0;
}

void alarm_handler(int signal){
    if(c.done == 0){
        mem_cs->c[id].frustrated = 1;
        mem_cs->c[id].leave = 1;
        printf("\033[1;31mCustomer with ID %d has left the bakery. (frustrated)\033[0m\n", c.id);
    }
}