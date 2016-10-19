
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

// Structs
typedef struct thread{
  pthread_t thread_id;
  int id;
  int condition, x, y, copy_goal, copy_cycle;
  char name[6], letter;
  
} thread_data;

struct Workspace{
  
  char pos[5][5];
};

struct Shared{
  int condition_t;
  int goal_t;
  int eliminated_t[3]
  int cycle_t;
  int carrot_t[2][2];
  int mtn_t[2];
  char winner_t[6];
  struct Workspace *map;
}shared_t;

pthread_mutex_t timeTravel_signal_mutex;

// Function Headers
void setup_time_seed();
int getRandom(int rangeLow, int rangeHigh);
void *run_API(void *thread);
void runner_signal(thread_data *runner);
void init_data(thread_data *thread);
void print_map();
bool valid_move(char c, int x, int y);
void update_pos(char c, int xn, int yn, int &xo, int &yo);
void rand_pos(int *x, int *y);
void move_mtn();
void init_pos(thread_data *thread);
void create_map();

// Function Delcarations
void setup_time_seed(){
  struct timeval time;
  gettimeofday(&time, NULL);
  srandom((unsigned int) time.tv_usec);
}

// Description: Takes in two ranges and return a random number between that.
int getRandom(int rangeLow, int rangeHigh) {
  double myRand = rand() / (1.0 + RAND_MAX);
  int range = rangeHigh - rangeLow + 1;
  int myRand_scaled = (myRand * range) + rangeLow;
  return myRand_scaled;
} // end getRandom()

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void print_map(){
  int i, j;
  for(i = 0; i < 5; i++){
    for(j = 0; j < 5; j++){
      
    }
  }
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
bool valid_move(char c, int x, int y){

}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void update_pos(char c, int xn, int yn, int &xo, int &yo){

}

void rand_pos(int *x, int *y){
  // get random
  do{
    *x = getRandom(0, 2) - 1;
    *y = getRandom(0, 2) - 1;
  while((*y == 0) && (*x == 1));
}

void init_pos(thread_data *thread){
  int pos[7][2];
  char c[7] = {'B', 'D', 'T', 'M', 'C', 'C', 'F'};
  bool taken;
  for(int i = 0; i < 7; i++){
    do{
      taken = false;
      rand_pos(&pos[i][0], &pos[i][1]);
      for(int j = 0; j < i; j++){
        if(pos[i][0] == pos[j][0] && pos[i][1] == pos[j][1];
          taken = true;
      }
    }while(taken);
    if(i < 4){
      thread[i]->x = pos[i][0];
      thread[i]->y = pos[i][1];
    }
    else if(i < 6){
      shared_t.carrot_t[i - 4][0] = pos[i][0];
      shared_t.carrot_t[i - 4][1] = pos[i][1];
    }
    else{
      shared_t.mtn_t[0] = pos[i][0];
      shared_t.mtn_t[1] = pos[i][1];
    }
    shared_t.map->pos[pos[i][0]][pos[i][1]] = c[i];
  }
}

void move_mtn(){

}

void create_map(){
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 5; j++){
      shared_t.map[i][j] = ' ';
    }
  }
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
void runner_signal(thread_data *runner){
  // Lock it down
  pthread_mutex_lock(&timeTravel_signal_mutex);
  // check if it's allowed to run
  if(shared_t.condition_t == runner->condition){
    int x, y;
    // Check if Marvin
    if(runner->id == 3){
      // Check if cycle % 3
      if(shared_t.cycle_t % 3 && shared_t.cycle_t != 0){
        // Move Mountain
        move_mtn();
      }
      // Move Marvin / Eliminate the competition (with or without the carrot)
      do{
      // Move
        x = getRandom(0, 2) - 1;
        y = getRandom(0, 2) - 1;
      }while(!valid_move(runner->letter, x, y);
      update_pos(runner->letter, x, y, &runner->x, &runner->y);
    }
    // Not Marvin
    else{
      // Check if terminated
      if(shared_t.eliminated[runner->id] = 0){
        do{
          // Move
          x = getRandom(0, 2) - 1;
          y = getRandom(0, 2) - 1;
        }while(!valid_move(runner->letter, x, y);
        update_pos(runner->letter, x, y, &runner->x, &runner->y);
      }
    }
    // Check if won (This could be done in valid_move)
    
    // Update cycle
    shared_t.cycle_t++;
    // Update condition
    if(shared_t.condition_t < 3){shared_t.condition_t;}
    else{shared_t.condition_t = 0;}

  }
  pthread_mutex_unlock(&timeTravel_signal_mutex);
}

void init_data(thread_data *thread){
  // Initialize Shared data
  create_map();
  shared_t.condition_runner = 3;
  shared_t.goal_t = 0;
  shared_t.eliminated_t[0] = shared_t.elimiated_t[1] = shared_t.eliminated_t[2] = 0;
  shared_t.cycle_t = 0;
  shared_t.carrot_t[0] = shared_t.carrot_t[1] = shared_t.carrot_t[2] = shared_t.carrot_t[3] = 0;

  // Initialize thread data
  thread[0].thread_id = 0;
  thread[0].id = 0;
  thread[0].condition = 3;
  thread[0].copy_goal = 0;
  thread[0].copy_cycle = 0;
  thread[0].name = "Bunny";
  thread[0].letter = 'B';

  thread[1].thread_id = 1;
  thread[1].id = 0;
  thread[1].condition = 0;
  thread[1].copy_goal = 0;
  thread[1].copy_cycle = 0;
  thread[1].name = "Taz";
  thread[1].letter = 'D';

  thread[2].thread_id = 2;
  thread[2].id = 0;
  thread[2].condition = 1;
  thread[2].copy_goal = 0;
  thread[2].copy_cycle = 0;
  thread[2].name = "Tweety";
  thread[2].letter = 'T';

  thread[3].thread_id = 3;
  thread[3].id = 0;
  thread[3].condition = 2;
  thread[3].copy_goal = 0;
  thread[3].copy_cycle = 0;
  thread[3].name = "Marvin";
  thread[3].letter = 'M';

  // initialize time seed and random position
  setup_time_seed();
  init_pos(thread);

}


// API
void *run_API(void *thread){
  thread_data *runner = (thread_data)thread;
  setup_time_seed();

  while(!runner->copy_goal_T){
    runner_signal(runner);
    sleep(2);
  }
  
  pthread_exit(NULL);

}

// Main
int main(){
  int i;
  thread_data thread[4];
  init_data(&thread);

  for(i = 0; i < 4 i++){
    printf("");
  }

  pthread_mutex_init(&timeTravel_signal_mutex, NULL);

  for(i = 0; i < 4; i++){
    thread[i].thread_num = i;
    phtread_crete(&(thread[i].thread_id), NULL, run_API, (void *)(&thread[i]));
  }

  for(i = 0; i < 4; i++){
    pthread_join(thread[i].thread_id, NULL);
  }

  pthread_mutex_destroy(&timeTravel_signal_mutex);

  return 0;
}
