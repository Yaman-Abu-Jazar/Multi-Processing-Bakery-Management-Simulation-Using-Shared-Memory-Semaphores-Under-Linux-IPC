#include "header.h"
#include "constants.h"
#include "functions.h"

pid_t ppid;
int shmid, semid, shmid_ch, semid_ch;
SHM memory;
CHM mem_ch;

struct sembuf acquire = {0, -1, SEM_UNDO},
              release = {0, 1, SEM_UNDO};

struct sembuf acquire_ch = {0, -1, SEM_UNDO}, 
              release_ch = {0,  1, SEM_UNDO};

int main(int argc, char** argv){
    readFile("arguments.txt");

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

    if((shmid_ch = shmget(CHEF_MEM_KEY, sizeof(CHM), 0777)) != -1){
        if((mem_ch = (CHM)shmat(shmid_ch, NULL, 0)) == (CHM)-1){
            perror("Shared Memory Attachment -- Chef -- Error.\n");
            exit(1);
        }
    } else {
        perror("Shared Memory Creation Error.\n");
        exit(1);
    }

    if((semid_ch = semget(CHEF_SEM_KEY, 2, IPC_CREAT | 0777)) == -1){
        perror("Semaphore Creation Error.\n");
        exit(2);
    }

    int *jobs = malloc(strlen(argv[1]) * sizeof(int));

    for(int i = 0 ; i < strlen(argv[1]) ; i++){
        jobs[i] = argv[1][i] - '0';
    }

    int number_of_jobs, flag;
    //int i = 0;
    while(1){
        //printf("%d\n", ++i);
        // take the ingredients from the shared memory
        acquire.sem_num = CONSUM_INGREDIENTS;
        
        if(semop(semid, &acquire, 1) == -1){
            perror("semop -- chef -- acquire.\n");
            exit(-1);
        }

        // printf("wheat %d -- yeast %d -- butter %d -- milk %d -- sweet items %d.\n", memory->wheat, memory->yeast, memory->butter, memory->milk, memory->sweet_items);

        if(strlen(argv[1]) == 1){
            number_of_jobs = 1;
            if(jobs[0] == 1){
                flag = 1;
                while(memory->wheat < 10 || memory->yeast < 10 || memory->milk < 10 || memory->butter < 10 || memory->salt < 10 || memory->sugar < 10){
                    //sleep(1); // wait
                }
                if(mem_ch->prepared_paste < NUMBER_OF_PASTE){
                    memory->wheat -= 10;
                    memory->yeast -= 10;
                    memory->butter -= 10;
                    memory->milk -= 10;
                    memory->salt -= 10;
                    memory->sugar -= 10;
                }
            } else if(jobs[0] == 2 || jobs[0] == 4 ){
                if(jobs[0] == 2)
                    flag = 2;
                else 
                    flag = 4;
                while(memory->wheat < 1 || memory->yeast < 1 || memory->milk < 1 || memory->butter < 1 || memory->sweet_items < 1 || memory->sugar < 1){
                    //sleep(1); // wait
                }
                if(mem_ch->prepared_cake < NUMBER_OF_CAKE_FLAVORS && flag == 2){
                    memory->sugar--;
                    memory->butter--;
                    memory->milk--;
                    memory->wheat--;
                    memory->yeast--;
                    memory->sweet_items--;
                }
                if(mem_ch->prepared_sweet < NUMBER_OF_SWEET_FLAVORS && flag == 4){
                    memory->sugar--;
                    memory->butter--;
                    memory->milk--;
                    memory->wheat--;
                    memory->yeast--;
                    memory->sweet_items--;
                }
            } else if(jobs[0] == 5 || jobs[0] == 6){
                while(memory->sugar < 1 || memory->salt < 1 || memory->butter < 1 || memory->milk < 1 || memory->sweet_items < 1){
                    //sleep(1); // wait 
                }
                if(jobs[0] == 5){
                    flag = 5;
                    if(mem_ch->prepared_sweet_patisseries < NUMBER_OF_SWEET_PATISSERIES){
                        memory->sugar--;
                        memory->sweet_items--;
                        memory->butter--;
                        memory->milk--;
                    }
                } else {
                    flag = 6;
                    if(mem_ch->prepared_savory_patisseries < NUMBER_OF_SAVORY_PATISSERIES){
                        memory->salt--;
                        memory->butter--;
                        memory->milk--;
                    }
                }
            } else if(jobs[0] == 3){
                flag = 3;
                while(memory->cheese < 1 || memory->salami < 1 ){
                    // sleep(1); // wait 
                }
                if(memory->sandwiches_ready < NUMBER_OF_SANDWICHES){
                    memory->cheese--;
                    memory->salami--;
                }
            }
        } else if(strlen(argv[1]) == 2){
            number_of_jobs = 2;
            if(jobs[0] == 1 && jobs[1] == 6){
                while(memory->wheat < 10 || memory->yeast < 10 || memory->milk < 11 || memory->butter < 11 || memory->salt < 11 || memory->sugar < 10){
                    // sleep(1);
                }
                if(mem_ch->prepared_savory_patisseries < NUMBER_OF_SAVORY_PATISSERIES){
                    memory->butter -= 1;
                    memory->milk -= 1;
                    memory->salt -= 1;
                }
                if(mem_ch->prepared_paste < NUMBER_OF_PASTE){
                    memory->wheat -= 10;
                    memory->yeast -= 10;
                    memory->butter -= 10;
                    memory->milk -= 10;
                    memory->salt -= 10;
                    memory->sugar -= 10;
                }
                flag = 16;
            } else if(jobs[0] == 2 && jobs[1] == 4){
                flag = 24;
                while(memory->wheat < 2 || memory->yeast < 2 || memory->milk < 2 || memory->butter < 2 || memory->sweet_items < 2 || memory->sugar < 2){
                    // sleep(1); // wait
                }
                if(mem_ch->prepared_cake < NUMBER_OF_CAKE_FLAVORS){
                    memory->wheat -= 1;
                    memory->butter -= 1;
                    memory->yeast -= 1;
                    memory->milk -= 1;
                    memory->sugar -= 1;
                    memory->sweet_items -= 1;
                }
                if(mem_ch->prepared_sweet < NUMBER_OF_SWEET_FLAVORS){
                    memory->wheat -= 1;
                    memory->butter -= 1;
                    memory->yeast -= 1;
                    memory->milk -= 1;
                    memory->sugar -= 1;
                    memory->sweet_items -= 1;
                }
            } else if(jobs[0] == 3 && jobs[1] == 5){
                while(memory->sugar < 1 || memory->cheese < 1 || memory->butter < 1 || memory->milk < 1 || memory->sweet_items < 1 || memory->salami < 1){
                    // sleep(1); // wait
                }
                if(memory->sandwiches_ready < NUMBER_OF_SANDWICHES){
                    memory->cheese--;
                    memory->salami--;
                }
                if(mem_ch->prepared_sweet_patisseries < NUMBER_OF_SWEET_PATISSERIES){
                    memory->sugar--;
                    memory->sweet_items--;
                    memory->butter--;
                    memory->milk--;
                }
                flag = 35;
            } else if(jobs[0] == 5 && jobs[1] == 6){
                while(memory->sugar < 1 || memory->salt < 1 || memory->butter < 2 || memory->milk < 2 || memory->sweet_items < 1){
                    // sleep(1); // wait
                }
                if(mem_ch->prepared_sweet_patisseries < NUMBER_OF_SWEET_PATISSERIES){
                    memory->sugar--;
                    memory->sweet_items--;
                    memory->butter--;
                    memory->milk--;
                }
                if(mem_ch->prepared_savory_patisseries < NUMBER_OF_SAVORY_PATISSERIES){
                    memory->salt--;
                    memory->butter--;
                    memory->milk--;
                }
                flag = 56;
            }
        } else if(strlen(argv[1]) == 3){
            number_of_jobs = 3;
            if(jobs[0] == 1 && jobs[1] == 3 && jobs[2] == 5){
                while(memory->wheat < 10 || memory->yeast < 10 || memory->milk < 11 || memory->butter < 11 || memory->salt < 10 || memory->sugar < 11 || memory->sweet_items < 1 || memory->cheese < 1 || memory->salami < 1){
                    // sleep(1); // wait
                }
                if(mem_ch->prepared_paste < NUMBER_OF_PASTE){
                    memory->wheat -= 10;
                    memory->yeast -= 10;
                    memory->butter -= 10;
                    memory->milk -= 10;
                    memory->salt -= 10;
                    memory->sugar -= 10;
                }
                if(mem_ch->prepared_sweet_patisseries < NUMBER_OF_SWEET_PATISSERIES){
                    memory->sugar--;
                    memory->sweet_items--;
                    memory->butter--;
                    memory->milk--;
                }
                if(memory->sandwiches_ready < NUMBER_OF_SANDWICHES){
                    memory->cheese--;
                    memory->salami--;
                }
                flag = 135;
            } else if(jobs[0] == 2 && jobs[1] == 4 && jobs[2] == 6){
                while(memory->wheat < 2 || memory->yeast < 2 || memory->milk < 3 || memory->butter < 3 || memory->salt < 2 || memory->sugar < 2 || memory->sweet_items < 2){
                    // sleep(1); // wait
                }
                if(mem_ch->prepared_cake < NUMBER_OF_CAKE_FLAVORS){
                    memory->wheat -= 1;
                    memory->yeast -= 1;
                    memory->butter -= 1;
                    memory->milk -= 1;
                    memory->salt -= 1;
                    memory->sugar -= 1;
                    memory->sweet_items -= 1;
                }
                if(mem_ch->prepared_sweet < NUMBER_OF_SWEET_FLAVORS){
                    memory->wheat -= 1;
                    memory->yeast -= 1;
                    memory->butter -= 1;
                    memory->milk -= 1;
                    memory->sugar -= 1;
                    memory->sweet_items -= 1;
                }
                if(mem_ch->prepared_savory_patisseries < NUMBER_OF_SAVORY_PATISSERIES){
                    memory->butter -= 1;
                    memory->milk -= 1;
                    memory->salt -= 1;
                }
                flag = 246;
            }
        } else if(strlen(argv[1]) == 6){
            number_of_jobs = 6;
            while(memory->wheat < 12 || memory->yeast < 12 || memory->milk < 14 || memory->butter < 14 || memory->salt < 11 || memory->sugar < 13 || memory->sweet_items < 3 || memory->cheese < 1 || memory->salami < 1){
                // sleep(1); // wait
            }
            if(memory->sandwiches_ready < NUMBER_OF_SANDWICHES){
                memory->cheese--;
                memory->salami--;
            }
            if(mem_ch->prepared_savory_patisseries < NUMBER_OF_SAVORY_PATISSERIES){
                memory->butter -= 1;
                memory->milk -= 1;
                memory->salt -= 1;
            }
            if(mem_ch->prepared_sweet < NUMBER_OF_SWEET_FLAVORS){
                memory->wheat -= 1;
                memory->yeast -= 1;
                memory->butter -= 1;
                memory->milk -= 1;
                memory->sugar -= 1;
                memory->sweet_items -= 1;
            }
            if(mem_ch->prepared_cake < NUMBER_OF_CAKE_FLAVORS){
                memory->wheat -= 1;
                memory->yeast -= 1;
                memory->butter -= 1;
                memory->milk -= 1;
                memory->sugar -= 1;
                memory->sweet_items -= 1;
            }
            if(mem_ch->prepared_sweet_patisseries < NUMBER_OF_SWEET_PATISSERIES){
                memory->sugar--;
                memory->sweet_items--;
                memory->butter--;
                memory->milk--;
            }
            if(mem_ch->prepared_paste < NUMBER_OF_PASTE){
                    memory->wheat -= 10;
                    memory->yeast -= 10;
                    memory->butter -= 10;
                    memory->milk -= 10;
                    memory->salt -= 10;
                    memory->sugar -= 10;
                }
            flag = 123456;
        }

        release.sem_num = CONSUM_INGREDIENTS;

        if(semop(semid, &release, 1) == -1){
            perror("semop -- chef -- release.\n");
            exit(-1);
        }

        if((flag == 1 || contains_digit(flag, 1)) && mem_ch->prepared_paste < NUMBER_OF_PASTE){
            sleep(PASTE_TIME);
            acquire_ch.sem_num = PRODUCED_ITEMS_CHEF;

            if(semop(semid_ch, &acquire_ch, 1) == -1){
                perror("semop -- Chef -- acquire_ch.\n");
                exit(-1);
            }

            mem_ch->prepared_paste += 10;
            if(mem_ch->prepared_paste > NUMBER_OF_PASTE)
                mem_ch->prepared_paste = NUMBER_OF_PASTE;
            printf("\033[1;34mchef #%d .I have done the paste.\033[0m\n", getpid());

            release_ch.sem_num = PRODUCED_ITEMS_CHEF;

            if(semop(semid_ch, &release_ch, 1) == -1){
                perror("semop -- chef -- release_ch.\n");
                exit(-1);
            }
        }

        if((flag == 5 || flag == 6 || flag == 56 || flag == 16 || flag == 123456 || flag == 35 || flag == 135 || flag == 246)){
            acquire_ch.sem_num = CONSUME_ITEMS_CHEF;

            if(semop(semid_ch, &acquire_ch, 1) == -1){
                perror("semop -- Chef -- acquire_ch.\n");
                exit(-1);
            }

            if((flag == 5 || contains_digit(flag, 5)) && memory->sweet_patisseries_ready < NUMBER_OF_SWEET_PATISSERIES){
                while(mem_ch->prepared_paste < 1){

                }
                mem_ch->prepared_paste--;
            }

            if((flag == 6 || contains_digit(flag, 6)) && memory->savory_patisseries_ready < NUMBER_OF_SAVORY_PATISSERIES){
                while(mem_ch->prepared_paste < 1){

                }
                mem_ch->prepared_paste--;
            }

            release_ch.sem_num = CONSUME_ITEMS_CHEF;

            if(semop(semid_ch, &release_ch, 1) == -1){
                perror("semop -- chef -- release_ch.\n");
                exit(-1);
            }
        }

        if((flag == 3 || contains_digit(flag, 3)) && memory->sandwiches_ready < NUMBER_OF_SANDWICHES){
            acquire.sem_num = CONSUM_BREAD;

            if(semop(semid, &acquire, 1) == -1){
                perror("semop -- Chef -- acquire.\n");
                exit(-1);
            }

            while(memory->breads_ready < 1){

            }
            
            memory->breads_ready--;

            release.sem_num = CONSUM_BREAD;

            if(semop(semid, &release, 1) == -1){
                perror("semop -- chef -- release.\n");
                exit(-1);
            }
        }

        if((flag == 2 || contains_digit(flag, 2)) && memory->cakes_ready < NUMBER_OF_CAKE_FLAVORS){
            sleep(CAKE_PREPARE);
            acquire_ch.sem_num = PRODUCED_ITEMS_CHEF;

            if(semop(semid_ch, &acquire_ch, 1) == -1){
                perror("semop -- Chef -- acquire_ch.\n");
                exit(-1);
            }

            printf("\033[1;34mchef #%d .I have done the cake.\033[0m\n", getpid());
            mem_ch->prepared_cake++;

            release_ch.sem_num = PRODUCED_ITEMS_CHEF;

            if(semop(semid_ch, &release_ch, 1) == -1){
                perror("semop -- chef -- release_ch.\n");
                exit(-1);
            }
        }if((flag == 3 || contains_digit(flag, 3)) && memory->sandwiches_ready < NUMBER_OF_SANDWICHES){
            sleep(SANDWICH_TIME);
            acquire.sem_num = PRODUCE_CHEF;

            if(semop(semid, &acquire, 1) == -1){
                perror("semop -- Chef -- acquire.\n");
                exit(-1);
            }

            printf("\033[1;34mchef #%d .I have done the sandwich.\033[0m\n", getpid());
            memory->sandwiches_ready++;
            if(memory->sandwiches_ready > NUMBER_OF_SANDWICHES)
                memory->sandwiches_ready = NUMBER_OF_SANDWICHES;

            release.sem_num = PRODUCE_CHEF;

            if(semop(semid, &release, 1) == -1){
                perror("semop -- chef -- release.\n");
                exit(-1);
            }
        }if((flag == 4 || contains_digit(flag, 4)) && memory->sweets_ready < NUMBER_OF_SWEET_FLAVORS){
            sleep(SWEET_PREPARE);
            acquire_ch.sem_num = PRODUCED_ITEMS_CHEF;

            if(semop(semid_ch, &acquire_ch, 1) == -1){
                perror("semop -- Chef -- acquire_ch.\n");
                exit(-1);
            }

            printf("\033[1;34mchef #%d .I have done the sweet.\033[0m\n", getpid());
            mem_ch->prepared_sweet++;

            release_ch.sem_num = PRODUCED_ITEMS_CHEF;

            if(semop(semid_ch, &release_ch, 1) == -1){
                perror("semop -- chef -- release_ch.\n");
                exit(-1);
            }
        } if((flag == 5 || contains_digit(flag, 5)) && memory->sweet_patisseries_ready < NUMBER_OF_SWEET_PATISSERIES){
            sleep(SWEET_P_PREPARE);
            acquire_ch.sem_num = PRODUCED_ITEMS_CHEF;

            if(semop(semid_ch, &acquire_ch, 1) == -1){
                perror("semop -- Chef -- acquire_ch.\n");
                exit(-1);
            }

            printf("\033[1;34mchef #%d .I have done the sweet patisseries.\033[0m\n", getpid());
            mem_ch->prepared_sweet_patisseries++;

            release_ch.sem_num = PRODUCED_ITEMS_CHEF;

            if(semop(semid_ch, &release_ch, 1) == -1){
                perror("semop -- chef -- release_ch.\n");
                exit(-1);
            }
        } if((flag == 6 || contains_digit(flag, 6)) && memory->savory_patisseries_ready < NUMBER_OF_SAVORY_PATISSERIES){
            sleep(SAVORY_P_PREPARE);
            acquire_ch.sem_num = PRODUCED_ITEMS_CHEF;

            if(semop(semid_ch, &acquire_ch, 1) == -1){
                perror("semop -- Chef -- acquire_ch.\n");
                exit(-1);
            }

            printf("\033[1;34mchef #%d .I have done the sweet patisseries.\033[0m\n", getpid());
            mem_ch->prepared_savory_patisseries++;

            release_ch.sem_num = PRODUCED_ITEMS_CHEF;

            if(semop(semid_ch, &release_ch, 1) == -1){
                perror("semop -- chef -- release_ch.\n");
                exit(-1);
            }
        }
    }


    return 0;
}