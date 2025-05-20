#include "header.h"
#include "constants.h"
#include "functions.h"

pid_t ppid;
int shmid, semid, shmid_ch, semid_ch, semid_ba;
SHM memory;
CHM mem_ch;

struct sembuf acquire = {0, -1, SEM_UNDO},
              release = {0, 1, SEM_UNDO};

struct sembuf acquire_ch = {0, -1, SEM_UNDO}, 
              release_ch = {0,  1, SEM_UNDO};

struct sembuf acquire_ba = {0, -1, SEM_UNDO}, 
              release_ba = {0,  1, SEM_UNDO};

int main(int argc, char** argv){
    readFile("arguments.txt");
    ppid = getppid();

    if((shmid = shmget((key_t)ppid, sizeof(memory), 0777)) != -1){
        if((memory = (SHM)shmat(shmid, NULL, 0)) == (SHM)-1){
            perror("Shared Memory Attachment Baker Error.\n");
            exit(1);
        }
    } else {
        perror("Shared Memory Creation Error.\n");
        exit(1);
    }

    if((shmid_ch = shmget(CHEF_MEM_KEY, sizeof(CHM), 0777)) != -1){
        if((mem_ch = (CHM)shmat(shmid_ch, NULL, 0)) == (CHM)-1){
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

    if((semid_ch = semget(CHEF_SEM_KEY, 2, IPC_CREAT | 0777)) == -1){
        perror("Semaphore Creation Error.\n");
        exit(2);
    }

    if((semid_ba = semget(OVEN_SEM_KEY, 2, IPC_CREAT | 0777)) == -1){
        perror("Semaphore Creation Error.\n");
        exit(2);
    }

    int oven = atoi(argv[1]);

    while(1){
        if(strlen(argv[1]) == 1){
            if(oven == 1){ // bake cakes and sweets
                // take the prepared items from the chef
                acquire_ch.sem_num = CONSUME_CAKE_SWEET;

                if(semop(semid_ch, &acquire_ch, 1) == -1){
                    perror("semop -- baker -- acquire_ch.\n");
                    exit(-1);
                }

                while(mem_ch->prepared_cake < 1 || mem_ch->prepared_sweet < 1){
                    // just wait until get ready
                }
                if(memory->cakes_ready < NUMBER_OF_CAKE_FLAVORS)
                    mem_ch->prepared_cake--;
                if(memory->sweets_ready < NUMBER_OF_SWEET_FLAVORS)
                    mem_ch->prepared_sweet--;

                release_ch.sem_num = CONSUME_CAKE_SWEET;

                if(semop(semid_ch, &release_ch, 1) == -1){
                    perror("semop -- baker -- release_ch.\n");
                    exit(-1);
                }
                // reserve the oven and put the item in it
                acquire_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &acquire_ba, 1) == -1){
                    perror("semop -- baker -- acquire_ba.\n");
                    exit(-1);
                }

                sleep(CAKE_TIME);


                release_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &release_ba, 1) == -1){
                    perror("semop -- baker -- release_ba.\n");
                    exit(-1);
                }
                // exit the item from the oven and add it to ready products
                acquire.sem_num = PRODUCE_BAKER;

                if(semop(semid, &acquire, 1) == -1){
                    perror("semop -- baker -- acquire.\n");
                    exit(-1);
                }

                if(memory->cakes_ready < NUMBER_OF_CAKE_FLAVORS){
                    memory->cakes_ready++;
                    printf("\033[1;32mBaker %d . I have baked the cake\033[0m\n", getpid());
                }
                   
                if(memory->sweets_ready < NUMBER_OF_SWEET_FLAVORS){
                    memory->sweets_ready++;
                    printf("\033[1;32mBaker %d . I have baked the sweet\033[0m\n", getpid());
                }
                    

                release.sem_num = PRODUCE_BAKER;

                if(semop(semid, &release, 1) == -1){
                    perror("semop -- baker -- release.\n");
                    exit(-1);
                }

            } else if(oven == 2){ // bake cakes and sweets
            // take the prepared items from the chef
                acquire_ch.sem_num = CONSUME_PATISSERIES;

                if(semop(semid_ch, &acquire_ch, 1) == -1){
                    perror("semop -- baker -- acquire_ch.\n");
                    exit(-1);
                }

                while(mem_ch->prepared_savory_patisseries < 1 || mem_ch->prepared_sweet_patisseries < 1){
                    // just wait until get ready
                }

                if(memory->sweet_patisseries_ready < NUMBER_OF_SWEET_PATISSERIES)
                    mem_ch->prepared_sweet_patisseries--;
                if(memory->savory_patisseries_ready < NUMBER_OF_SAVORY_PATISSERIES)
                    mem_ch->prepared_savory_patisseries--;
               

                release_ch.sem_num = CONSUME_PATISSERIES;

                if(semop(semid_ch, &release_ch, 1) == -1){
                    perror("semop -- baker -- release_ch.\n");
                    exit(-1);
                }
                // reserve the oven and put the item in it
                acquire_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &acquire_ba, 1) == -1){
                    perror("semop -- baker -- acquire_ba.\n");
                    exit(-1);
                }

                sleep(SWEET_P_TIME);

                release_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &release_ba, 1) == -1){
                    perror("semop -- baker -- release_ba.\n");
                    exit(-1);
                }
                // exit the item from the oven and add it to ready products
                acquire.sem_num = PRODUCE_BAKER;

                if(semop(semid, &acquire, 1) == -1){
                    perror("semop -- baker -- acquire.\n");
                    exit(-1);
                }

                if(memory->sweet_patisseries_ready < NUMBER_OF_SWEET_PATISSERIES){
                    printf("\033[1;32mBaker %d . I have baked the sweet patisseries\033[0m\n", getpid());
                    memory->sweet_patisseries_ready++;
                }
                   
                if(memory->savory_patisseries_ready < NUMBER_OF_SAVORY_PATISSERIES){
                    printf("\033[1;32mBaker %d . I have baked the savory patisseries\033[0m\n", getpid());
                    memory->savory_patisseries_ready++;
                }

                release.sem_num = PRODUCE_BAKER;

                if(semop(semid, &release, 1) == -1){
                    perror("semop -- baker -- release.\n");
                    exit(-1);
                }

            } else if(oven == 3){ // bake cakes and sweets
            // take the prepared items from the chef
                acquire_ch.sem_num = CONSUME_PASTE;

                if(semop(semid_ch, &acquire_ch, 1) == -1){
                    perror("semop -- baker -- acquire_ch.\n");
                    exit(-1);
                }

                while(mem_ch->prepared_paste < 5){
                    // just wait until get ready
                }

                if(memory->breads_ready < NUMBER_OF_BREADS)
                    mem_ch->prepared_paste -= 5;

                release_ch.sem_num = CONSUME_PASTE;

                if(semop(semid_ch, &release_ch, 1) == -1){
                    perror("semop -- baker -- release_ch.\n");
                    exit(-1);
                }
                // reserve the oven and put the item in it
                acquire_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &acquire_ba, 1) == -1){
                    perror("semop -- baker -- acquire_ba.\n");
                    exit(-1);
                }

                sleep(BREAD_TIME);


                release_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &release_ba, 1) == -1){
                    perror("semop -- baker -- release_ba.\n");
                    exit(-1);
                }
                // exit the item from the oven and add it to ready products
                acquire.sem_num = PRODUCE_BAKER;

                if(semop(semid, &acquire, 1) == -1){
                    perror("semop -- baker -- acquire.\n");
                    exit(-1);
                }

                if(memory->breads_ready < NUMBER_OF_BREADS){
                    printf("\033[1;32mBaker %d . I have baked the bread\033[0m\n", getpid());
                    memory->breads_ready += 5;
                }
                if(memory->breads_ready > NUMBER_OF_BREADS)
                    memory->breads_ready = NUMBER_OF_BREADS;

                release.sem_num = PRODUCE_BAKER;

                if(semop(semid, &release, 1) == -1){
                    perror("semop -- baker -- release.\n");
                    exit(-1);
                }

            }
        } else if(strlen(argv[1]) == 2){
            if(oven == 12){
                acquire_ch.sem_num = CONSUME_CAKE_SWEET;

                if(semop(semid_ch, &acquire_ch, 1) == -1){
                    perror("semop -- baker -- acquire_ch.\n");
                    exit(-1);
                }


                while(mem_ch->prepared_cake < 1 || mem_ch->prepared_sweet < 1){
                    // just wait until get ready
                }

                if(memory->cakes_ready < NUMBER_OF_CAKE_FLAVORS)
                    mem_ch->prepared_cake--;
                if(memory->sweets_ready < NUMBER_OF_SWEET_FLAVORS)
                    mem_ch->prepared_sweet--;

                release_ch.sem_num = CONSUME_CAKE_SWEET;

                if(semop(semid_ch, &release_ch, 1) == -1){
                    perror("semop -- baker -- release_ch.\n");
                    exit(-1);
                }

                acquire_ch.sem_num = CONSUME_PATISSERIES;

                if(semop(semid_ch, &acquire_ch, 1) == -1){
                    perror("semop -- baker -- acquire_ch.\n");
                    exit(-1);
                }

                while(mem_ch->prepared_savory_patisseries < 1 || mem_ch->prepared_sweet_patisseries < 1){
                    // just wait until get ready
                }

                if(memory->sweet_patisseries_ready < NUMBER_OF_SWEET_PATISSERIES)
                    mem_ch->prepared_sweet_patisseries--;
                if(memory->savory_patisseries_ready < NUMBER_OF_SAVORY_PATISSERIES)
                    mem_ch->prepared_savory_patisseries--;

                release_ch.sem_num = CONSUME_PATISSERIES;

                if(semop(semid_ch, &release_ch, 1) == -1){
                    perror("semop -- baker -- release_ch.\n");
                    exit(-1);
                }
                // reserve the oven and put the first item in it
                acquire_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &acquire_ba, 1) == -1){
                    perror("semop -- baker -- acquire_ba.\n");
                    exit(-1);
                }

                sleep(CAKE_TIME);

                release_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &release_ba, 1) == -1){
                    perror("semop -- baker -- release_ba.\n");
                    exit(-1);
                }
                // exit the item from the oven and add it to ready products
                acquire.sem_num = PRODUCE_BAKER;

                if(semop(semid, &acquire, 1) == -1){
                    perror("semop -- baker -- acquire.\n");
                    exit(-1);
                }

                if(memory->cakes_ready < NUMBER_OF_CAKE_FLAVORS){
                    memory->cakes_ready++;
                    printf("\033[1;32mBaker %d . I have baked the cake\033[0m\n", getpid());
                }
                   
                if(memory->sweets_ready < NUMBER_OF_SWEET_FLAVORS){
                    memory->sweets_ready++;
                    printf("\033[1;32mBaker %d . I have baked the sweet\033[0m\n", getpid());
                }

                release.sem_num = PRODUCE_BAKER;

                if(semop(semid, &release, 1) == -1){
                    perror("semop -- baker -- release.\n");
                    exit(-1);
                }


                // reserve the oven and put the second item in it
                acquire_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &acquire_ba, 1) == -1){
                    perror("semop -- baker -- acquire_ba.\n");
                    exit(-1);
                }

                sleep(SWEET_P_TIME);


                release_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &release_ba, 1) == -1){
                    perror("semop -- baker -- release_ba.\n");
                    exit(-1);
                }
                // exit the item from the oven and add it to ready products
                acquire.sem_num = PRODUCE_BAKER;

                if(semop(semid, &acquire, 1) == -1){
                    perror("semop -- baker -- acquire.\n");
                    exit(-1);
                }

                if(memory->sweet_patisseries_ready < NUMBER_OF_SWEET_PATISSERIES){
                    printf("\033[1;32mBaker %d . I have baked the sweet patisseries\033[0m\n", getpid());
                    memory->sweet_patisseries_ready++;
                }
                   
                if(memory->savory_patisseries_ready < NUMBER_OF_SAVORY_PATISSERIES){
                    printf("\033[1;32mBaker %d . I have baked the savory patisseries\033[0m\n", getpid());
                    memory->savory_patisseries_ready++;
                }

                release.sem_num = PRODUCE_BAKER;

                if(semop(semid, &release, 1) == -1){
                    perror("semop -- baker -- release.\n");
                    exit(-1);
                }
            }
        } else if(strlen(argv[1]) == 3){
            if(oven == 123){

                acquire_ch.sem_num = CONSUME_PASTE;

                if(semop(semid_ch, &acquire_ch, 1) == -1){
                    perror("semop -- baker -- acquire_ch.\n");
                    exit(-1);
                }

                while(mem_ch->prepared_paste < 5){
                    // just wait until get ready
                }

                if(memory->breads_ready < NUMBER_OF_BREADS)
                    mem_ch->prepared_paste -= 5;

                release_ch.sem_num = CONSUME_PASTE;

                if(semop(semid_ch, &release_ch, 1) == -1){
                    perror("semop -- baker -- release_ch.\n");
                    exit(-1);
                }

                // reserve the oven and put the item in it
                acquire_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &acquire_ba, 1) == -1){
                    perror("semop -- baker -- acquire_ba.\n");
                    exit(-1);
                }

                sleep(BREAD_TIME);

                release_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &release_ba, 1) == -1){
                    perror("semop -- baker -- release_ba.\n");
                    exit(-1);
                }
                // exit the item from the oven and add it to ready products
                acquire.sem_num = PRODUCE_BAKER;

                if(semop(semid, &acquire, 1) == -1){
                    perror("semop -- baker -- acquire.\n");
                    exit(-1);
                }

                if(memory->breads_ready < NUMBER_OF_BREADS){
                    printf("\033[1;32mBaker %d . I have baked the bread\033[0m\n", getpid());
                    memory->breads_ready += 5;
                }
                if(memory->breads_ready > NUMBER_OF_BREADS)
                    memory->breads_ready = NUMBER_OF_BREADS;

                release.sem_num = PRODUCE_BAKER;

                if(semop(semid, &release, 1) == -1){
                    perror("semop -- baker -- release.\n");
                    exit(-1);
                }


                acquire_ch.sem_num = CONSUME_CAKE_SWEET;

                if(semop(semid_ch, &acquire_ch, 1) == -1){
                    perror("semop -- baker -- acquire_ch.\n");
                    exit(-1);
                }

                while(mem_ch->prepared_cake < 1 || mem_ch->prepared_sweet < 1){
                    // just wait until get ready
                }

                if(memory->cakes_ready < NUMBER_OF_CAKE_FLAVORS)
                    mem_ch->prepared_cake--;
                if(memory->sweets_ready < NUMBER_OF_SWEET_FLAVORS)
                    mem_ch->prepared_sweet--;

                release_ch.sem_num = CONSUME_CAKE_SWEET;

                if(semop(semid_ch, &release_ch, 1) == -1){
                    perror("semop -- baker -- release_ch.\n");
                    exit(-1);
                }

                acquire_ch.sem_num = CONSUME_PATISSERIES;

                if(semop(semid_ch, &acquire_ch, 1) == -1){
                    perror("semop -- baker -- acquire_ch.\n");
                    exit(-1);
                }

                while(mem_ch->prepared_savory_patisseries < 1 || mem_ch->prepared_sweet_patisseries < 1){
                    // just wait until get ready
                }

                if(memory->sweet_patisseries_ready < NUMBER_OF_SWEET_PATISSERIES)
                    mem_ch->prepared_sweet_patisseries--;
                if(memory->savory_patisseries_ready < NUMBER_OF_SAVORY_PATISSERIES)
                    mem_ch->prepared_savory_patisseries--;

                release_ch.sem_num = CONSUME_PATISSERIES;

                if(semop(semid_ch, &release_ch, 1) == -1){
                    perror("semop -- baker -- release_ch.\n");
                    exit(-1);
                }

                // reserve the oven and put the first item in it
                acquire_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &acquire_ba, 1) == -1){
                    perror("semop -- baker -- acquire_ba.\n");
                    exit(-1);
                }

                sleep(SWEET_TIME);

                release_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &release_ba, 1) == -1){
                    perror("semop -- baker -- release_ba.\n");
                    exit(-1);
                }
                // exit the item from the oven and add it to ready products
                acquire.sem_num = PRODUCE_BAKER;

                if(semop(semid, &acquire, 1) == -1){
                    perror("semop -- baker -- acquire.\n");
                    exit(-1);
                }

                if(memory->cakes_ready < NUMBER_OF_CAKE_FLAVORS){
                    memory->cakes_ready++;
                    printf("\033[1;32mBaker %d . I have baked the cake\033[0m\n", getpid());
                }
                   
                if(memory->sweets_ready < NUMBER_OF_SWEET_FLAVORS){
                    memory->sweets_ready++;
                    printf("\033[1;32mBaker %d . I have baked the sweet\033[0m\n", getpid());
                }

                release.sem_num = PRODUCE_BAKER;

                if(semop(semid, &release, 1) == -1){
                    perror("semop -- baker -- release.\n");
                    exit(-1);
                }


                // reserve the oven and put the second item in it
                acquire_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &acquire_ba, 1) == -1){
                    perror("semop -- baker -- acquire_ba.\n");
                    exit(-1);
                }

                sleep(SAVORY_P_TIME);


                release_ba.sem_num = ACQUIRE_OVEN;

                if(semop(semid_ba, &release_ba, 1) == -1){
                    perror("semop -- baker -- release_ba.\n");
                    exit(-1);
                }
                // exit the item from the oven and add it to ready products
                acquire.sem_num = PRODUCE_BAKER;

                if(semop(semid, &acquire, 1) == -1){
                    perror("semop -- baker -- acquire.\n");
                    exit(-1);
                }

                if(memory->sweet_patisseries_ready < NUMBER_OF_SWEET_PATISSERIES){
                    printf("\033[1;32mBaker %d . I have baked the sweet patisseries\033[0m\n", getpid());
                    memory->sweet_patisseries_ready++;
                }
                   
                if(memory->savory_patisseries_ready < NUMBER_OF_SAVORY_PATISSERIES){
                    printf("\033[1;32mBaker %d . I have baked the savory patisseries\033[0m\n", getpid());
                    memory->savory_patisseries_ready++;
                }

                release.sem_num = PRODUCE_BAKER;

                if(semop(semid, &release, 1) == -1){
                    perror("semop -- baker -- release.\n");
                    exit(-1);
                }


            }

        }


    }

    return 0;
}