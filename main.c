#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<time.h>

#define MAX_CREATURES 10

typedef struct _Creature{
  int type;
  int position;
  int id;
}Creature;

typedef struct _Room{
  int state;
  int position;
  int c_count;
  int max_rooms;
  int north;
  int south;
  int west;
  int east;
  Creature *room_creatures[10];
}Room;

Room *all_rooms;
Creature *all_creatures;
Creature *pc;
int respect = 40;
/*
Method that returns void, which creates the rooms, creatures, and adds
creatures to their respective rooms.
*/
void start_game(){
  int num_rooms,num_creatures;
  int init_rooms = 0;
  int init_creatures = 0;

  printf("Enter number of Rooms: \n");
  scanf("%d",&num_rooms);

  all_rooms = malloc(num_rooms * sizeof(Room));
  all_rooms->max_rooms = num_rooms;
  while(init_rooms < num_rooms){
    int state, north, south, east, west;
    printf("Enter info for Room: %d\n",init_rooms);
    scanf("%d %d %d %d %d",
    &state,&north,&south,&east,&west);
    all_rooms[init_rooms].state = state;
    all_rooms[init_rooms].north = north;
    all_rooms[init_rooms].south = south;
    all_rooms[init_rooms].east = east;
    all_rooms[init_rooms].west = west;
    all_rooms[init_rooms].position = init_rooms;
    all_rooms[init_rooms].c_count = 0;
    for(int i = 0;i<10;i++){
      all_rooms[init_rooms].room_creatures[i] = NULL;
    }
    init_rooms++;
  }

  printf("Enter number of Creatures: \n");
  scanf("%d",&num_creatures);

  all_creatures = malloc(num_creatures * sizeof(Creature));
  while(init_creatures < num_creatures){
    int type,position;
    printf("Enter information for Creature: %d\n",init_creatures);
    scanf("%d %d",&type,&position);
    all_creatures[init_creatures].type = type;
    all_creatures[init_creatures].position = position;
    all_creatures[init_creatures].id = init_creatures;
    init_creatures++;
  }

  pc = malloc(sizeof(Creature));
  int i,j;
  for(i = 0;i<num_rooms;i++){
    for(j = 0;j<num_creatures;j++){
      if(all_rooms[i].position == all_creatures[j].position){
        int c_count;
        c_count = all_rooms[i].c_count;
        all_rooms[i].room_creatures[c_count] = &all_creatures[j];
        if(all_creatures[j].type == 0){
          pc->position = all_creatures[j].position;
          pc->type = all_creatures[j].type;
          pc->id = all_creatures[j].id;
        }
        all_rooms[i].c_count = all_rooms[i].c_count + 1;
      }
    }
  }
  printf("-----------\n");
}

int findCreatureIndex(int id){
  for(int i =0;i<all_rooms->max_rooms;i++){
    for(int j = 0;j<all_rooms[i].c_count;j++){
      if(all_rooms[i].room_creatures[j]->id == id){
        return j;
      }
    }
  }
  return 0;
}
void move(int new_room,int id){
  int old_room;
  old_room = pc->position;
  int old_count;
  old_count = all_rooms[old_room].c_count;
  int new_count = all_rooms[new_room].c_count;
  int pos = all_creatures[id].position;
  int index = findCreatureIndex(id);
  for(int i = index; i < all_rooms[old_room].c_count + 1;i++){
    all_rooms[old_room].room_creatures[i] = all_rooms[old_room].room_creatures[i+1];
  }
  all_rooms[new_room].room_creatures[new_count] = &all_creatures[id];
  if(all_rooms[new_room].room_creatures[new_count]->type == 0){
    pc->position = new_room;
  }else if(all_rooms[new_room].room_creatures[new_count]->type == 1){
    if(all_rooms[new_room].state == 2){
      all_rooms[new_room].state = 1;
      printf("Animal %d doesn't like room state...switching state to half-dirty\n",id);
    }
  }else if(all_rooms[new_room].room_creatures[new_count]->type == 2){
    if(all_rooms[new_room].state == 0){
      all_rooms[new_room].state = 1;
      printf("Human %d doesn't like room state...switching state to half-dirty\n",id);
    }
  }
  all_rooms[new_room].c_count++;
  all_rooms[old_room].c_count--;
}
int findNeighbor(){
  bool found = false;
  int pc_pos;
  pc_pos = pc->position;
  while(!found){
    srand(time(NULL));
    int r = rand() % 4;
    switch(r){
      case 0:
      if(all_rooms[pc_pos].north != -1){
        if(all_rooms[pc_pos].c_count < 10){
          found = true;
          return all_rooms[pc_pos].north;
        }
      }
      break;
      case 1:
      if(all_rooms[pc_pos].south != -1){
        if(all_rooms[pc_pos].c_count < 10){
          found = true;
          return all_rooms[pc_pos].south;
        }
      }
      break;
      case 2:
      if(all_rooms[pc_pos].east != -1){
        if(all_rooms[pc_pos].c_count < 10){
          found = true;
          return all_rooms[pc_pos].east;
        }
      }
      break;
      case 3:
      if(all_rooms[pc_pos].west != -1){
        if(all_rooms[pc_pos].c_count < 10){
          found = true;
          return all_rooms[pc_pos].west;
        }
      }
      break;
      default:
      printf("There are no valid neighbor creature can move too...how did you do that?\n");
      printf("Game is not playable ending session\n");
      exit(0);
      break;
    }
  }
}

void movePC(int cardinal){
  int pc_pos = pc->position;
  int id = all_creatures[pc->id].id;
  int new_pos;
  int c_count = all_rooms[pc_pos].c_count;
  switch(cardinal){
    case 0:
    new_pos = all_rooms[pc_pos].north;
    if(new_pos != -1){
      if(c_count < 10){
        move(new_pos,id);
        printf("You moved north\n");
      }else {
        printf("No space left in room to the north\n");
      }
    }else {
      printf("No neighbor to the north\n");
    }
    break;
    case 1:
    new_pos = all_rooms[pc_pos].south;
    if(new_pos != -1){
      if(c_count < 10){
        move(new_pos,id);
        printf("You moved south\n");
      }else {
        printf("No space left in room to the south\n");
      }
    }else {
      printf("No neighbor to the south\n");
    }
    break;
    case 2:
    new_pos = all_rooms[pc_pos].east;
    if(new_pos != -1){
      if(c_count < 10){
        move(new_pos,id);
        printf("You moved east\n");
      }else {
        printf("No space left in room to the east\n");
      }
    }else {
      printf("No neighbor to the east\n");
    }
    break;
    case 3:
    new_pos = all_rooms[pc_pos].west;
    if(new_pos != -1){
      if(c_count < 10){
        move(new_pos,id);
        printf("You moved west\n");
      }else {
        printf("No space left in room to the west\n");
      }
    }else {
      printf("No neighbor to the west\n");
    }
    break;
    default:
    printf("There are no valid neighbor you can move too...how did you do that?\n");
    printf("Game is not playable ending session\n");
    exit(0);
    break;
  }
}
void moveCreature(int amount,int *id_list){
  int new_pos = findNeighbor();
  for(int i = 0;i<amount;i++){
    int id = all_creatures[id_list[i]].id;
    move(new_pos,id_list[i]);
    if(all_creatures[id_list[i]].type == 1){
      printf("Animal %d moved to room %d\n",id,new_pos);
    }else if(all_creatures[id_list[i]].type == 2){
      printf("Human %d moved to room %d\n",id,new_pos);
    }
  }
}

void clean(int id){
  int amount = 0;
  int new_pos = 0;
  int c_id = 0;
  int *id_list;
  id_list = malloc(10 * sizeof(int));
  int pc_pos = pc->position;
  if(all_rooms[pc_pos].state == 0){
    printf("Room is already clean\n");
    return;
  }
  if(id == pc->id){
    printf("You clean the room\n");
  }else {
    if(all_creatures[id].type == 1){
      printf("Animal %d cleans the room\n",id);
    }else if(all_creatures[id].type == 2){
      printf("Human %d cleans the room\n",id);
    }
  }
  if(all_rooms[pc_pos].state == 1){
    all_rooms[pc_pos].state = 0;
    printf("The room is now clean\n");
  }else if(all_rooms[pc_pos].state == 2){
    all_rooms[pc_pos].state = 1;
    printf("The room is now half-dirty\n");
  }
  for(int i =0;i<all_rooms[pc_pos].c_count;i++){
    if(all_rooms[pc_pos].room_creatures[i]->type == 1){
      if(all_rooms[pc_pos].state == 1){
        printf("Animal %d gives you a scoop of ice-cream\n",all_rooms[pc_pos].room_creatures[i]->id);
      }else if(all_rooms[pc_pos].state == 0){
        printf("Animal %d gives you a bunch of ice-cream\n",all_rooms[pc_pos].room_creatures[i]->id);
      }
    }else if(all_rooms[pc_pos].room_creatures[i]->type == 2){
      if(all_rooms[pc_pos].state == 1){
        printf("Human %d takes a scoop of ice-cream\n",all_rooms[pc_pos].room_creatures[i]->id);
        respect--;
      }else if(all_rooms[pc_pos].state == 0){
        printf("Human %d takes a bunch of ice-cream in a rage\n",all_rooms[pc_pos].room_creatures[i]->id);
        c_id = all_rooms[pc_pos].room_creatures[i]->id;
        id_list[amount] = c_id;
        respect--;
        amount++;
      }
    }
  }
  if(amount != 0){
    moveCreature(amount,id_list);
  }
  free(id_list);
  printf("Your respect is now %d\n",respect);
}
void dirty(int id){
  int amount = 0;
  int new_pos = 0;
  int c_id = 0;
  int *id_list;
  id_list = malloc(10 * sizeof(int));
  int pc_pos = pc->position;
  if(all_rooms[pc_pos].state == 2){
    printf("Room is already dirty\n");
    return;
  }
  if(id == pc->id){
    printf("You dirty the room\n");
  }else {
    if(all_creatures[id].type == 1){
      printf("Animal %d dirties the room\n",id);
    }else if(all_creatures[id].type == 2){
      printf("Human %d dirties the room\n",id);
    }
  }
  if(all_rooms[pc_pos].state == 1){
    all_rooms[pc_pos].state = 2;
    printf("The room is now dirty\n");
  }else if(all_rooms[pc_pos].state == 0){
    all_rooms[pc_pos].state = 1;
    printf("The room is now half-dirty\n");
  }

  for(int i =0;i<all_rooms[pc_pos].c_count;i++){
    if(all_rooms[pc_pos].room_creatures[i]->type == 2){
      if(all_rooms[pc_pos].state == 1){
        printf("Human %d gives you a scoop of ice-cream\n",all_rooms[pc_pos].room_creatures[i]->id);
        respect++;
      }else if(all_rooms[pc_pos].state == 2){
        printf("Human %d gives you a bunch of ice-cream\n",all_rooms[pc_pos].room_creatures[i]->id);
        respect++;
      }
    }else if(all_rooms[pc_pos].room_creatures[i]->type == 1){
      if(all_rooms[pc_pos].state == 1){
        printf("Animal %d takes a scoop of ice-cream\n",all_rooms[pc_pos].room_creatures[i]->id);
        respect--;
      }else if(all_rooms[pc_pos].state == 2){
        printf("Animal %d takes a bunch of ice-cream in a rage\n",all_rooms[pc_pos].room_creatures[i]->id);
        respect--;
        c_id = all_rooms[pc_pos].room_creatures[i]->id;
        id_list[amount] = c_id;
        amount++;
      }
    }
  }
  if(amount != 0){
    moveCreature(amount,id_list);
  }
  free(id_list);
  printf("Your respect is now %d\n",respect);
}
void look(){
  int pc_pos;
  int i;
  int state;
  int type;
  pc_pos = pc->position;
  state = all_rooms[pc_pos].state;
  printf("----------\n");
  printf("Room %d information\n",pc_pos);
  printf("State: ");
  switch(state){
    case 0:
    printf("clean\n");
    break;
    case 1:
    printf("half-dirty\n");
    break;
    case 2:
    printf("dirty\n");
    break;
    default:
    printf("Something happened in look()\n");
    break;
  }
  if(all_rooms[pc_pos].north != -1){
    printf("North Neighbor: %d\n",all_rooms[pc_pos].north);
  }
  if(all_rooms[pc_pos].south != -1){
    printf("South Neighbor: %d\n",all_rooms[pc_pos].south);
  }
  if(all_rooms[pc_pos].east != -1){
    printf("East Neighbor: %d\n",all_rooms[pc_pos].east);
  }
  if(all_rooms[pc_pos].west != -1){
    printf("West Neighbor: %d\n",all_rooms[pc_pos].west);
  }
  printf("Contains: \n");
  for(i = 0; i < all_rooms[pc_pos].c_count;i++){
    type = all_rooms[pc_pos].room_creatures[i]->type;
    switch(type){
      case 0:
      printf("#Bulgaria %d\n",pc->id);
      break;
      case 1:
      printf("Animal: %d\n",all_rooms[pc_pos].room_creatures[i]->id);
      break;
      case 2:
      printf("Human: %d\n",all_rooms[pc_pos].room_creatures[i]->id);
      break;
      default:
      printf("Something happened in look()\n");
      break;
    }
  }
}

void help(){
  puts("List of commands available");
  puts("'look' shows current room the PC is in");
  puts("'clean' cleans the current room the PC is in");
  puts("'dirty' dirties the current room the PC is in");
  puts("'[direction]' will move player to [direction] if room is visible");
  puts("'[id]:clean' has the creature correlated to [id] clean the current room\nIf the creature is not in current room, the commmand will not do anything");
  puts("'[id]:dirty' has the creature correlated to [id] dirty the current room\nIf the creature is not in current room, the commmand will not do anything");
  puts("Types of [direction]:");
  puts("\t 'north'");
  puts("\t 'south'");
  puts("\t 'east'");
  puts("\t 'west'");
  puts("'respect' shows your current respect");

}
int showRespect(){
  return respect;
}
int parseString(char *input,char delim){
  int size = strlen(input);
  for(int i = 0;i<size;i++){
    if(input[i] == delim){
      return 1;
    }
  }
  return 0;
}

int main(int argc, char **argv){
  int pc_pos;
  char user_input[256];
  int i = 0;
  bool found = false;
  printf("Initializing Game\n");
  start_game();
  pc_pos = pc->position;
  puts("Please enter command to start playing. For list of commands enter 'help'");
  scanf("%s",user_input);
  while(strcmp(user_input,"quit") !=0){
    i = 0;
    found = false;
    pc_pos = pc->position;
    if(strcmp(user_input,"look") == 0){
      look();
    }
    else if(strcmp(user_input,"help") == 0){
      help();
    }else if(strcmp(user_input,"respect") == 0){
      printf("%d\n",showRespect());
    }else if(strcmp(user_input,"clean") == 0){
      clean(pc->id);
    }else if(strcmp(user_input,"dirty") == 0){
      dirty(pc->id);
    }else if(strcmp(user_input,"north") == 0){
      movePC(0);
    }else if(strcmp(user_input,"south") == 0){
      movePC(1);
    }else if(strcmp(user_input,"east") == 0){
      movePC(2);
    }else if(strcmp(user_input,"west") == 0){
      movePC(3);
    }else {
      if(parseString(user_input,':') == 1){
        int creature_id = atoi(strtok(user_input,":"));
        for(int i = 0;i <all_rooms[pc_pos].c_count && !found;i++){
          if(all_rooms[pc_pos].room_creatures[i]->id == creature_id){
            found = true;
          }
        }
        if(found){
          char *action = strtok(NULL,": \n");
          if(strcmp(action,"clean") == 0){
            clean(creature_id);
          }else if(strcmp(action,"dirty") == 0){
            dirty(creature_id);
          }else {
            printf("Invalid format, please use: [creature id]:{'clean','dirty'}\n");
            printf("i.e. \n \t2:clean\n \t3:dirty\n");
          }
        }else {
          printf("Creature %d is not in current room\n",creature_id);
        }
      }else {
        printf("Invalid Command\n");
      }
    }
    scanf("%s",user_input);
  }
  for(int i = 0;i<all_rooms->max_rooms;i++){
    free(all_rooms[i].room_creatures);
  }
  free(all_rooms);
  free(all_creatures);
  free(pc);
  puts("End of game");
}
