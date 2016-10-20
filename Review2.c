
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
  int carrot;
  char name[6], letter;
  
} thread_data;

struct Workspace{
  
  char pos[5][5];
};

struct Shared{
  int condition_t;
  int goal_t;
  int eliminated_t[3];
  int cycle_t;
  int carrot_t[2][2];
  int carrot_holder_t[2];
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
void check_pos(thread_data *runner, int x, int y);
int check_person(int x, int y);
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

void print_map(){
  int i, j, k, person, carrots;
  for(i = 0; i < 5; i++){
    printf("\n-------------------------------------\n|");
    for(j = 0; j < 5; j++){
      printf("%s", shared_t.map.pos[i][j]);
      person = check_person(i, j);
      if(person != -1){
        carrots = 0;
        for(k = 0; k < 2; k++){
          if(shared_t.carrot_holder_t[k] == person)
            carrots++;
        }
      }
      if(carrots == 2){printf("%s", "(K)");}
      else if(carrots == 1){printf("%s", "(C)");}
      else{printf("   ");
      printf("|");
    }
  }
}

bool valid_move(char c, int x, int y){
  bool valid = true;
  int person, i;
  char letter = {'B', 'D', 'T', 'M'};
  for(i = 0; i < 4; i++){
    if(c == letter[i]){
      person = i;
    }
  }
  // not jumping off the map
  if(x < 0 || x > 4 || y < 0 || y > 4){valid = false;}
  // check if mtn is in the way / have the one ring
  else if(shared_t.map.pos[x][y] == 'F' && !(shared_t.carrot_holder_t[0] == person || shared_t.carrot_holder_t[1] == person)
  {valid = false;}
  // check if bumping into other competition / if marvin
  else if(shared_t.map.pos[x][y] != person && shared_t.map.pos[x][y] != ' ')
  {valid = fasle;}

  return valid;
}

void check_pos(thread_data *runner, int x, int y){
  int i;
  // check if marvin
  if(runner->id == 3){
    // check if moving on a person
    int person = check_person(x, y);
    if(person != -1){
      shared_t.eliminate_t[peson]++;
      if(shared_t.carrot_holder[0] == person){runner->carrot++;shared_t.carrot_holder_t[0] = runner->id;}
      else if(shared-t.carrot_holder[1] == person){runner->carrot++;shared_t.carrot_holder_t[1] = runner->id;}
    }
  }
  // check for carrot
  for(i = 0; i < 2; i++){
    if(shared_t.carrot_t[i][0] == x && shared_t.carrot_t[i][1] == y){
        // pickup carrot
	shared_t.carrot_holder_t[i] = runner->id;
        runner->carrot++;
    }
  }
}

int check_person(int x, int y){
  int id;
  char person = shared_t.map.pos[x][y];
  if(person == 'B'){id = 0;}
  else if(person == 'D'){id = 1;}
  else if(person == 'T'){id = 2;}
  else{id = -1}
  return id;
}

void update_pos(char c, int xn, int yn, int &xo, int &yo){
  // replace old spot with ' '
  shared_t.map.pos[xo][yo] = ' ';
  // replace new spot with c
  shared_t.map.pos[xn][yn] = c;
  // old pos = new pos
  xo = xn;
  yo = yn;
}

void rand_pos(int *x, int *y){
  // get random
  do{
    *x = getRandom(0, 2) - 1;
    *y = getRandom(0, 2) - 1;
  }while((*y == 0) && (*x == 1));
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
  int x, y, moved = 0;
  do{
    x = getRandom(0, 4);
    y = getRandom(0, 4);
    if(shared_t.map.pos[x][y] == ' '){
      shared_t.map.pos[shared_t.mtn_t[0]][shared_t.mtn_t[1]] = ' ';
      shared_t.map.pos[x][y] == 'F';
      shared_t.mtn_t[0] = x;
      shared_t.mtn_t[1] = y;
      moved = 1;
    }
  }while(moved == 0);
}

void create_map(){
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 5; j++){
      shared_t.map[i][j] = ' ';
    }
  }
}

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
        x = getRandom(0, 2) - 1 + runner->x;
        y = getRandom(0, 2) - 1 + runner->y;
      }while(!valid_move(runner->letter, x, y);
      // check if carrot or competition to take/takedown
      check_pos(runner, x, y);
      update_pos(runner->letter, x, y, &runner->x, &runner->y);
    }
    // Not Marvin
    else{
      // Check if terminated
      if(shared_t.eliminated[runner->id] == 0){
        do{
          // Move
          x = getRandom(0, 2) - 1;
          y = getRandom(0, 2) - 1;
        }while(!valid_move(runner->letter, x, y);
        update_pos(runner->letter, x, y, &runner->x, &runner->y);
        // check for carrot to take
      }
    }
    // Check if won
    if(runner->x == shared_t.mtn_t[0] && runner->y == shared_t.mtn[1] && runner->carrot > 0){
      shared_t.goal_t = 1;
      shared_t.winner_t = runner->name;
    }

    // Update goal
    runner->copy_goal = shared_t.goal_t;
    // Update cycle
    shared_t.cycle_t++;
    // Update condition
    if(shared_t.condition_t < 3){shared_t.condition_t;}
    else{shared_t.condition_t = 0;}
    print_map();
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
  shared_t.carrot_holder_t[0] = shared_t.carrot_holder_t[1] = -1;

  // Initialize thread data
  thread[0].thread_id = 0;
  thread[0].id = 0;
  thread[0].condition = 3;
  thread[0].copy_goal = 0;
  thread[0].copy_cycle = 0;
  thread[0].name = "Bunny";
  thread[0].letter = 'B';
  thread[0].carrot = 0;

  thread[1].thread_id = 1;
  thread[1].id = 0;
  thread[1].condition = 0;
  thread[1].copy_goal = 0;
  thread[1].copy_cycle = 0;
  thread[1].name = "Taz";
  thread[1].letter = 'D';
  thread[1].carrot = 0;

  thread[2].thread_id = 2;
  thread[2].id = 0;
  thread[2].condition = 1;
  thread[2].copy_goal = 0;
  thread[2].copy_cycle = 0;
  thread[2].name = "Tweety";
  thread[2].letter = 'T';
  thread[2].carrot = 0;

  thread[3].thread_id = 3;
  thread[3].id = 0;
  thread[3].condition = 2;
  thread[3].copy_goal = 0;
  thread[3].copy_cycle = 0;
  thread[3].name = "Marvin";
  thread[3].letter = 'M';
  thread[3].carrot = 0;

  // initialize time seed and random position
  setup_time_seed();
  init_pos(thread);

}


// API
void *run_API(void *thread){
  thread_data *runner = (thread_data)thread;
  setup_time_seed();

  while(!runner->copy_goal){
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
