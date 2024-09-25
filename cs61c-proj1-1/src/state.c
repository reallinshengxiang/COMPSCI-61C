#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  game_state_t* state = malloc(sizeof(game_state_t));
  state->num_rows = 18;
  state->board = malloc(sizeof(char*)*18);
  state->num_snakes = 1;
  state->snakes = malloc(sizeof(snake_t));

  // create board
  char** board = state->board;
  for (int i = 0; i < 18; i++) {
    board[i] = malloc(sizeof(char)*21);
    board[i][0] = board[i][19] = '#';
    for (int j = 1; j < 19; j++) {
      board[i][j] = (i==0||i==17)?'#':' ';
    }
    board[i][20] = '\0';
  }
  board[2][2] = 'd';
  board[2][3] = '>';
  board[2][4] = 'D';
  board[2][9] = '*';

  // create snakes
  snake_t* snake = state->snakes;
  snake->tail_row = 2;
  snake->tail_col = 2;
  snake->head_row = 2;
  snake->head_col = 4;
  snake->live = true;

  return state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  if(state==NULL) return;
  if(state->snakes!=NULL)free(state->snakes);
  if(state->board==NULL)
  {
    for (int i = 0; i < 18; i++) {
      if(state->board[i]!=NULL)free(state->board[i]);
    }
    free(state->board);
  }
  free(state);

  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  for(int i = 0; i < state->num_rows; i++) {
    fprintf(fp, "%s\n", state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  return c == 'w' || c == 'a' || c == 's' || c == 'd';
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  return c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x';
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  return is_tail(c) || is_head(c) || c == '^' || c == '<' || c == 'v' || c == '>';
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  if(c=='^') return 'w';
  if(c=='<') return 'a';
  if(c=='v') return 's';
  if(c=='>') return 'd';
  return '?'; // should never happen (if it does, something is wrong
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  if(c=='W') return '^';
  if(c=='A') return '<';
  if(c=='S') return 'v';
  if(c=='D') return '>';
  return '?'; // should never happen (if it does, something is wrong
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  if(c=='v'||c=='s'||c=='S') return cur_row+1;
  if(c=='^'||c=='w'||c=='W') return cur_row-1;
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  if(c=='>'||c=='d'||c=='D') return cur_col+1;
  if(c=='<'||c=='a'||c=='A') return cur_col-1;
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  char c=get_board_at(state,state->snakes[snum].head_row,state->snakes[snum].head_col);
  return get_board_at(state,get_next_row(state->snakes[snum].head_row,c),get_next_col(state->snakes[snum].head_col,c));
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  char c=get_board_at(state,state->snakes[snum].head_row,state->snakes[snum].head_col);
  set_board_at(state,state->snakes[snum].head_row,state->snakes[snum].head_col,head_to_body(c));
  if(c=='W'){
    set_board_at(state,state->snakes[snum].head_row-1,state->snakes[snum].head_col,'W');
    state->snakes[snum].head_row--;
  }
  if(c=='A'){
    set_board_at(state,state->snakes[snum].head_row,state->snakes[snum].head_col-1,'A');
    state->snakes[snum].head_col--;
  }
  if(c=='S'){
    set_board_at(state,state->snakes[snum].head_row+1,state->snakes[snum].head_col,'S');
    state->snakes[snum].head_row++;
  }
  if(c=='D'){
    set_board_at(state,state->snakes[snum].head_row,state->snakes[snum].head_col+1,'D');
    state->snakes[snum].head_col++;
  }
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  char c=get_board_at(state,state->snakes[snum].tail_row,state->snakes[snum].tail_col);
  set_board_at(state,state->snakes[snum].tail_row,state->snakes[snum].tail_col,' ');
  if(c=='w'){
    char body=get_board_at(state,state->snakes[snum].tail_row-1,state->snakes[snum].tail_col);
    set_board_at(state,state->snakes[snum].tail_row-1,state->snakes[snum].tail_col,body_to_tail(body));
    state->snakes[snum].tail_row--;
  }
  if(c=='a'){
    char body=get_board_at(state,state->snakes[snum].tail_row,state->snakes[snum].tail_col-1);
    set_board_at(state,state->snakes[snum].tail_row,state->snakes[snum].tail_col-1,body_to_tail(body));
    state->snakes[snum].tail_col--;
  }
  if(c=='s'){
    char body=get_board_at(state,state->snakes[snum].tail_row+1,state->snakes[snum].tail_col);
    set_board_at(state,state->snakes[snum].tail_row+1,state->snakes[snum].tail_col,body_to_tail(body));
    state->snakes[snum].tail_row++;
  }
  if(c=='d'){
    char body=get_board_at(state,state->snakes[snum].tail_row,state->snakes[snum].tail_col+1);
    set_board_at(state,state->snakes[snum].tail_row,state->snakes[snum].tail_col+1,body_to_tail(body));
    state->snakes[snum].tail_col++;
  }
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  for(int i=0;i<state->num_snakes;i++){
    if(!state->snakes[i].live) continue;
    char c=next_square(state,i);
    if(c==' '){
      update_head(state,i);
      update_tail(state,i);
    }
    else if(c=='*'){
      update_head(state,i);
      add_food(state);
    }
    else{
      state->snakes[i].live=false;
      set_board_at(state,state->snakes[i].head_row,state->snakes[i].head_col,'x');
    }
  }
  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  FILE* f=fopen(filename,"r");
  if(f==NULL) return NULL;
  game_state_t* state=malloc(sizeof(game_state_t));
  state->num_rows=0;
  state->board=NULL;
  state->num_snakes=0;
  state->snakes=NULL;
  char* line=NULL;
  size_t len=0;
  size_t max_line_len=0;
  char c;
  while(fscanf(f,"%c",&c)!=EOF){
    if(c=='\n'){
      if(len>max_line_len) max_line_len=len;
      state->num_rows++;
      len=0;
      continue;
    }
    len++;
  }
  if(len){
    if(len>max_line_len) max_line_len=len;
    state->num_rows++;
  }

  line=malloc(sizeof(char)*(max_line_len+1));
  state->board=malloc(sizeof(char*)*state->num_rows);
  fseek(f,0,SEEK_SET);
  for(int i=0;i<state->num_rows;i++){
    c='\0';
    len=0;
    while(c!='\n'){
      fscanf(f,"%c",&c);
      line[len++]=c;
    }
    line[len-1]='\0';
    
    state->board[i]=malloc(sizeof(char)*(strlen(line)+1));
    strcpy(state->board[i],line);
  }
  free(line);
  return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  int row=state->snakes[snum].tail_row;
  int col=state->snakes[snum].tail_col;
  char c=get_board_at(state,row,col);
  while(true){
    if(c=='w') row--;
    if(c=='a') col--;
    if(c=='s') row++;
    if(c=='d') col++;
    c=get_board_at(state,row,col);
    if(is_head(c)) break;
    c=body_to_tail(c);
  }
  state->snakes[snum].head_row=row;
  state->snakes[snum].head_col=col;
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  state->num_snakes=0;
  for(int i=0;i<state->num_rows;i++){
    for(int j=strlen(state->board[i])-1;j>=0;j--){
      if(is_tail(state->board[i][j])) state->num_snakes++;
    }
  }
  state->snakes=malloc(sizeof(snake_t)*state->num_snakes);
  int snum=0;
  for(int i=0;i<state->num_rows;i++){
    int len=strlen(state->board[i]);
    for(int j=0;j<len;j++){
      if(is_tail(state->board[i][j])){
        state->snakes[snum].tail_row=i;
        state->snakes[snum].tail_col=j;
        find_head(state,snum);
        state->snakes[snum].live=true;
        snum++;
      }
    }
  }
  return state;
}
