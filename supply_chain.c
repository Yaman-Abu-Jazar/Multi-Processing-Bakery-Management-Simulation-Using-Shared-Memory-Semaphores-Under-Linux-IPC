#include "header.h"
#include "constants.h"
#include "functions.h"

pid_t ppid;
int shmid, semid;
SHM memory;

struct sembuf acquire = {0, -1, SEM_UNDO},
              release = {0, 1, SEM_UNDO};

void handler(int signal);

int main(int argc, char** argv){
    
    readFile("arguments.txt");
    ppid = getppid();
    if((shmid = shmget((key_t)ppid, sizeof(memory), 0777)) != -1){
        if((memory = (SHM)shmat(shmid, NULL, 0)) == (SHM)-1){
            perror("Shared Memory Attachment Supply Chain Employee Error.\n");
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

    if(signal(SIGUSR1, handler) == SIG_ERR){
        perror("signal handler error -- supply chain employee -- sigset.\n");
        exit(5);
    }

    while(1){
        acquire.sem_num = BUY_INGREDIENTS;

        if ( semop(semid, &acquire, 1) == -1 ) {
            perror("semop -- supply chain employee -- acquire");
            exit(5);
        }
        
        memory->butter += 5;
        if(memory->butter > NUMBER_OF_BUTTER_MAX)
            memory->butter = NUMBER_OF_BUTTER_MAX;
        
        memory->wheat += 5;
        if(memory->wheat > NUMBER_OF_WHEAT_MAX)
            memory->wheat = NUMBER_OF_WHEAT_MAX;

        memory->yeast += 5;
        if(memory->yeast > NUMBER_OF_YEAST_MAX)
            memory->yeast = NUMBER_OF_YEAST_MAX;

        memory->milk += 5;
        if(memory->milk > NUMBER_OF_MILK_MAX)
            memory->milk = NUMBER_OF_MILK_MAX;

        memory->cheese += 3;
        if(memory->cheese > NUMBER_OF_CHEESE_MAX)
            memory->cheese = NUMBER_OF_CHEESE_MAX;
        
        memory->salt += 5;
        if(memory->salt > NUMBER_OF_SALT_MAX)
            memory->salt = NUMBER_OF_SALT_MAX;

        memory->sugar += 5;
        if(memory->sugar > NUMBER_OF_SUGAR_MAX)
            memory->sugar = NUMBER_OF_SUGAR_MAX;

        memory->salami += 3;
        if(memory->salami > NUMBER_OF_SALAMI_MAX)
            memory->salami = NUMBER_OF_SALAMI_MAX;

        memory->sweet_items += 3;
        if(memory->sweet_items > NUMBER_OF_SWEET_ITEMS_MAX)
            memory->sweet_items = NUMBER_OF_SWEET_ITEMS_MAX;
        
        release.sem_num = BUY_INGREDIENTS;

        if ( semop(semid, &release, 1) == -1 ) {
            perror("semop -- supply chain employee -- release");
            exit(5);
        }
        sleep(SUPPLY_CHAIN_SLEEP);
    }


    return 0;
}

void handler(int signal){

}