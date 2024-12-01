#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state()
{
  // TODO: Implement this function.
  game_state_t *st = malloc(sizeof(game_state_t));
  if (st == NULL)
    return NULL;
  unsigned int width = 20, height = 18;
  st->num_rows = height;
  st->board = malloc(sizeof(char *) * height);
  st->num_snakes = 1;
  st->snakes = malloc(sizeof(snake_t));
  char **b = st->board;
  for (int i = 0; i < height; i++)
  {
    b[i] = malloc(sizeof(char) * (width + 1));
    for (int j = 0; j < width; j++)
    {
      b[i][j] = ' ';
      if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
        b[i][j] = '#';
    }
    b[i][width] = '\0';
  }
  snake_t *sn = st->snakes;
  sn->head_row = 2;
  sn->head_col = 4;
  sn->tail_row = 2;
  sn->tail_col = 2;
  sn->live = 1;
  b[sn->head_row][sn->head_col] = 'D';
  b[sn->tail_row][sn->tail_col] = 'd';
  b[2][3] = '>';
  b[2][9] = '*';
  return st;
}

/* Task 2 */
void free_state(game_state_t *state)
{
  // TODO: Implement this function.
  unsigned int height = 18;
  if (state == NULL)
    return;
  if (state->board != NULL)
  {
    for (int i = 0; i < state->num_rows; i++)
    {
      if (state->board[i] != NULL)
        free(state->board[i]);
    }
    free(state->board);
  }
  if (state->snakes != NULL)
  {
    free(state->snakes);
  }
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
  // TODO: Implement this function.
  for (int i = 0; i < state->num_rows; i++)
    fprintf(fp, "%s\n", state->board[i]);
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename)
{
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch)
{
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c)
{
  // TODO: Implement this function.
  if (!(c == 'a' || c == 'w' || c == 's' || c == 'd'))
    return false;
  return true;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c)
{
  // TODO: Implement this function.
  if (!(c == 'A' || c == 'W' || c == 'S' || c == 'D' || c == 'x'))
    return false;
  return true;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c)
{
  // TODO: Implement this function.
  if (!is_tail(c) && !is_head(c) && c != '^' && c != '<' && c != '>' && c != 'v')
    return false;
  return true;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c)
{
  // TODO: Implement this function.
  if (c == '^')
    return 'w';
  if (c == '<')
    return 'a';
  if (c == 'v')
    return 's';
  if (c == '>')
    return 'd';
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c)
{
  // TODO: Implement this function.
  if (c == 'W')
    return '^';
  if (c == 'A')
    return '<';
  if (c == 'S')
    return 'v';
  if (c == 'D')
    return '>';
  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c)
{
  // TODO: Implement this function.
  if (c == 'v' || c == 's' || c == 'S')
    cur_row++;
  if (c == '^' || c == 'w' || c == 'W')
    cur_row--;
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c)
{
  // TODO: Implement this function.
  if (c == '>' || c == 'd' || c == 'D')
    cur_col++;
  if (c == '<' || c == 'a' || c == 'A')
    cur_col--;
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.
  char c = get_board_at(state, state->snakes[snum].head_row, state->snakes[snum].head_col);
  char ans = get_board_at(state, get_next_row(state->snakes[snum].head_row, c), get_next_col(state->snakes[snum].head_col, c));
  return ans;
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.
  char c = get_board_at(state, state->snakes[snum].head_row, state->snakes[snum].head_col);
  set_board_at(state, state->snakes[snum].head_row, state->snakes[snum].head_col, head_to_body(c));
  if (c == 'W')
    state->snakes[snum].head_row--;
  else if (c == 'A')
    state->snakes[snum].head_col--;
  else if (c == 'D')
    state->snakes[snum].head_col++;
  else if (c == 'S')
    state->snakes[snum].head_row++;
  set_board_at(state, state->snakes[snum].head_row, state->snakes[snum].head_col, c);
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum)
{
  // TODO: Implement this function.
  char c = get_board_at(state, state->snakes[snum].tail_row, state->snakes[snum].tail_col);
  set_board_at(state, state->snakes[snum].tail_row, state->snakes[snum].tail_col, ' ');
  if (c == 'w')
    state->snakes[snum].tail_row--;
  else if (c == 'a')
    state->snakes[snum].tail_col--;
  else if (c == 's')
    state->snakes[snum].tail_row++;
  else if (c == 'd')
    state->snakes[snum].tail_col++;
  else
    return;
  c = get_board_at(state, state->snakes[snum].tail_row, state->snakes[snum].tail_col);
  set_board_at(state, state->snakes[snum].tail_row, state->snakes[snum].tail_col, body_to_tail(c));
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
  // TODO: Implement this function.
  for (unsigned int i = 0; i < state->num_snakes; i++)
  {
    snake_t *sn = &state->snakes[i];
    if (sn->live == 0)
      continue;
    char c = next_square(state, i);
    if (c == ' ')
    {
      update_head(state, i);
      update_tail(state, i);
    }
    else if (c == '*')
    {
      update_head(state, i);
      add_food(state);
    }
    else
    {
      sn->live = 0;
      set_board_at(state, state->snakes[i].head_row, state->snakes[i].head_col, 'x');
    }
  }
  return;
}

/* Task 5.1 */
/*char *read_line(FILE *fp) {
  // TODO: Implement this function.
  unsigned int length=365535;
  char* str;
  str=(char*)malloc(length);
  while(fgets(str,365535,fp))
  {
    char* ptr;
    ptr=strchr(str,'\n');
    length=ptr-str;
    str=(char*)realloc(str,length);
    return str;
   }
}*/
char *read_line(FILE *fp)
{
  // TODO: Implement this function.
  unsigned int length = 10;
  unsigned int mlength=10;
  char *str;
  char *ptr;
  str = (char *)malloc(length);
  char *sstr = str;
  while (fgets(sstr, length, fp))
  {
    ptr = strchr(str, '\0');
    if (ptr - sstr == length - 1)
    {
      ptr = NULL;
    }
    if (ptr == NULL)
    {
      int len=sstr-str;
      int len2=length;
      length = length * 1.5;
      mlength+=length;
      // sstr = NULL;
      str = realloc(str, mlength);
      sstr=str+len+len2-1;
    }
    else
    {
      //length = ptr - str;
      //str = (char *)realloc(str, length);
      return str;
    }
  }
  //length = ptr - str;
  //str = (char *)realloc(str, length);
  free(str);
  return NULL;
}
/* Task 5.2 */
game_state_t* load_board(FILE *fp) {
    game_state_t* state = malloc(sizeof(game_state_t));
    if (state == NULL) {
        return NULL;
    }

    state->num_rows = 0;
    state->board = NULL;
    state->num_snakes = 0;
    state->snakes = NULL;

    // 使用 read_line 函数读取文件中的每一行
    char* line = NULL;
    while ((line = read_line(fp)) != NULL) {
        state->num_rows++;
        state->board = realloc(state->board, sizeof(char*) * state->num_rows);
        if (state->board == NULL) {
            free(state);
            free(line);
            return NULL;
        }
        state->board[state->num_rows - 1] = line;
        state->board[state->num_rows - 1][strlen(line)-1]='\0';
    } 
    return state;
  /*char **board=NULL;
  board = malloc(sizeof(char*)*100);
  int width=0;
  size_t size;
  while((line =read_line(fp))!=NULL)
  {
    num_row++;
    size=strlen(line)+1;
    board[num_row]=malloc(size-1);
    strncpy(board[num_row],line,size-2);
    board[num_row][size-2]='\0';
    if(width<size-2) width=size-2;
  }
 state->num_rows=num_row+1;
 state->board = malloc(sizeof(char*)*(100));
 for(int i=0;i<state->num_rows;i++)
 {
   state->board[i]=malloc(width+1);
   for(int j=0;j<=width;j++) state->board[i][j]=board[i][j];
 }
 for(int i=0;i<99;i++)
 {
   if(board[i]!=NULL) free(board[i]);
 }
 free(board);
 return state;*/
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum)
{
  int row = state->snakes[snum].tail_row;
  int col = state->snakes[snum].tail_col;
  char c = get_board_at(state, row, col);
  while (true)
  {
    if (c == 'w')
      row--;
    if (c == 'a')
      col--;
    if (c == 's')
      row++;
    if (c == 'd')
      col++;
    c = get_board_at(state, row, col);
    if (is_head(c))
      break;
    c = body_to_tail(c);
  }
  state->snakes[snum].head_row = row;
  state->snakes[snum].head_col = col;
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
  // TODO: Implement this function.
  state->num_snakes = 0;
  for (unsigned int i = 0; i < state->num_rows; i++)
  {
    for (unsigned int j = 0; state->board[i][j] != '\0'; j++)
    {
      if (is_head(state->board[i][j]))
      {
        state->num_snakes++;
      }
    }
  }
  state->snakes = malloc(sizeof(snake_t) * state->num_snakes);
  unsigned int snum = 0;
  for (unsigned int i = 0; i < state->num_rows - 1; i++)
  {
    for (unsigned int j = 0; state->board[i][j] != '\0'; j++)
    {
      if (is_tail(state->board[i][j]))
      {
        state->snakes[snum].tail_row = i;
        state->snakes[snum].tail_col = j;
        find_head(state, snum);
        state->snakes[snum].live = true;
        snum++;
      }
    }
  }
  return state;
}
