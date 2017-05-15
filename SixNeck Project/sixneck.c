#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

/*
[???] : 6??? ?????? ?¢¬? / ?????? 6??? ????
1. ?????? my_state ?? enemy_state ?? ?????.
-> void search_state(int ms[][MAP_LENGTH], int es[][MAP_LENGTH], int priority[], int mine);
???? ????????? ???.
*/
#define MAP_LENGTH 20
#define DIR_MAX 4
#define CANDIDATE_MAX 30
#define LOCATION_MAX 400

#define _STATE_MAX 9

#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define BLOCKING 3

/*
2. ?? ????? ???? ?????.
-> void act(int ms[][MAP_LENGTH], int es[][MAP_LENGTH], int priority[], int mine);
- ?????? ????? ????( mw, aw ) ?? 1?????? ?????? ??? ?¢¬???? ?¬Õ?.
-> int mw_location( int ms[][MAP_LENGTH], location where[], int priority[], int mine );
-> int aw_location( int ms[][MAP_LENGTH], location where[], int priority[], int mine );
-> int can_win(int ms[][MAP_LENGTH], int priority[], int mine);
- ???º§?? ????? ???? ( mw, aw ) ?? 1?????? ?????? ??? ???? ???.
-> int can_lose(int es[][MAP_LENGTH], int priority[], int mine);
- ???? ?? ?????? ??? ??, ??? ??????? ??????? ?? ??????? ????, ???? ??????? ?????????
???????? ?????.
-> void find_candidate_location(int ms[][MAP_LENGTH],int es[][MAP_LENGTH],location candidate[], int priority[], int mine)
- ??????? int priority[state_length] ?? ???? ??? ???.
3. ?¬Õ?.
->	location decide_location( int ms[][MAP_LENGTH], int es[][MAP_LENGTH], int priority[], int mine )
-> void add_stone(int who, location where, int map[][MAP_LENGTH], int mine)
4. ?¢¬? ???? ???, ???? ????? ??????? ??? ?? ????.
-> int who_win( int map[][MAP_LENGTH] );
5. ?¢¬? / ?¬Û? ?? ????. ???? ????? 1?? ???? ???? ????.
-> void initialize_map( int map[][MAP_LENGTH], int mine );
-> void array1_initailizer( int arr[], int length, int toInitialize );
-> void array2_initailizer( int arr[][MAP_LENGTH], int toInitialize );

??? ????? ???¢¬? ???????, ??????? ?¥ï???? ???.
#deifne NONE_STATE -1
#define EPSILONE_0 0
#define EPSILONE_1 1
NONE_STATE ?? -1,
?? EPSILONE ?? 0 * 9 + 0, ALPHA0?? 1 * 9 + 0 , ALPHA1 2 * 9 + 0 ... ????,
_0 ?? 0, _1 ?? 1 .. _n ?? n ?? ?????? ????? ????? ?????? ???????.
?? ALPHA3_6 = 4 * 9 + 6 ???.
... ???? ???¢¬? ????????? ???.
???? mw, aw ?? ???¢¥? ?? ????? ?? ???¥á????? ???????, ???? ?¬Ù? ?? ???.
- 4, 5 ???? ???? aw ?????? ?¬Ù??? ???? ?????.
???? ?????? */
#define NORTH_EAST 0
#define EAST 1
#define SOUTH_EAST 2
#define SOUTH 3
/*
?? ???? ???????.

???????
int priority[state_length];
?? ???? ???????.
- ??????? ???? ????? ??? ???.
??? ????? ?•U???? ???? ??? ?? ??, ???? ????????? ???? ????????? ???.
- ??? ???? ?????? ???? ???? ??? ?? ??..!
*/

// ?????? ????? ????? ??????? ¡À??.

/*	?? : EMPTY, ?? : MINE, ?? : ENEMY
?? / ??	_0	?? / ??	_1	?? / ??	_2
?? / ??	_3	?? / ??	_4	?? / ??	_5
?? / ??	_6	?? / ??	_7	?? / ??	_8
*/
#define _0 0
#define _1 1
#define _2 2
#define _3 3
#define _4 4
#define _5 5
#define _6 6
#define _7 7
#define _8 8
/*
State is defigned like DELTA5 * _5 or GAMMA7 * _2
*/
#define NONE_STATE - 1
/*
?? : 0???? ?? ?? ( ?? ) ????
???????
*/
#define EPSILONE 0
/*
??	1???? ?? ?? ( ?? ) ???? = { ??0, ??1, ??2 ... ??5 }
??0	???????	??1	???????	??2	???????	??3	???????	??4	???????
??5	???????
*/
#define ALPHA0 1
#define ALPHA1 2
#define ALPHA2 3
#define ALPHA3 4
#define ALPHA4 5
#define ALPHA5 6
#define ALPHA_MAX 6
/*
??	2???? ?? ?? ( ?? ) ???? = { ??0, ??1, ??2 ... ??14 }
??0	???????	??1	???????	??2	???????	??3	???????
??4	???????	??5	???????	??6	???????	??7	???????
??8	???????	??9	???????	??10	???????	??11	???????
??12	???????	??13	???????	??14	???????
*/
#define BETA0 7
#define BETA1 8
#define BETA2 9
#define BETA3 10
#define BETA4 11
#define BETA5 12
#define BETA6 13
#define BETA7 14
#define BETA8 15
#define BETA9 16
#define BETA10 17
#define BETA11 18
#define BETA12 19
#define BETA13 20
#define BETA14 21
#define BETA_MAX 15
/*
??	3???? ?? ?? ( ?? ) ???? = { ??0, ??1, ??2 ... ?? 19 }
??0	???????	??1	???????	??2	???????	??3	???????
??4	???????	??5	???????	??6	???????	??7	???????
??8	???????	??9	???????
??10	???????	??11	???????	??12	???????	??13	???????
??14	???????	??15	???????	??16	???????	??17	???????
??18	???????	??19	???????
*/
#define GAMMA0 22
#define GAMMA1 23
#define GAMMA2 24
#define GAMMA3 25
#define GAMMA4 26
#define GAMMA5 27
#define GAMMA6 28
#define GAMMA7 29
#define GAMMA8 30
#define GAMMA9 31
#define GAMMA10 32
#define GAMMA11 33
#define GAMMA12 34
#define GAMMA13 35
#define GAMMA14 36
#define GAMMA15 37
#define GAMMA16 38
#define GAMMA17 39
#define GAMMA18 40
#define GAMMA19 41
#define GAMMA_MAX 20
/*
??	4???? ?? ?? ( ?? ) ???? = { ??0, ??1, ??2 ... ??14 }
??0	???????	??1	???????	??2	???????	??3	???????
??4	???????	??5	???????	??6	???????	??7	???????
??8	???????	??9	???????
??10	???????	??11	???????	??12	???????	??13	???????
??14	???????
*/
#define SIGMA0 42
#define SIGMA1 43
#define SIGMA2 44
#define SIGMA3 45
#define SIGMA4 46
#define SIGMA5 47
#define SIGMA6 48
#define SIGMA7 49
#define SIGMA8 50
#define SIGMA9 51
#define SIGMA10 52
#define SIGMA11 53
#define SIGMA12 54
#define SIGMA13 55
#define SIGMA14 56
#define SIGMA_MAX 15
/*
??	5???? ?? ?? ( ?? ) ???? = { ??0, ??1, ??2 ... ??5 }
??0	???????	??1	???????	??2	???????	??3	???????	??4	???????
??5	???????
*/
#define THETA0 57
#define THETA1 58
#define THETA2 59
#define THETA3 60
#define THETA4 61
#define THETA5 62
#define THETA_MAX 6
/*
W	6???? ?? ?? ( ?? ) ???? = { W }
W	???????
*/
#define W 49

//State is defigned like DELTA5 * _STATE_MAX + _5 or GAMMA7 * _2

#define STATE_LENGTH 558
// 50 * 9 + 1 ??
// -1 ~ 556

/*	mw = must win, aw = amado win ?? ??????.
mw set : aw set ?? ??????? 3?? ??? ???? ??.
aw set : { ??, ?? set } X { _0, _2, _6, _8 }
*/

#define GENERATION_MAX 30
#define PARENT_MAX 4
#define CROSS_POINT_MAX 30
#define SIVILING_MAX 5
#define PRIORITY_MAX 5000
#define FITNESS_MAX 5000
#define GAME_MAX 500

typedef struct {
	short x;
	short y;
}location;

typedef struct {
	short x;
	short y;
	short dir;	// will be value like SOUTH
}vector;

// AI Functions
void search_state(int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], int map[][MAP_LENGTH], int mine);
int get_state(int map[][MAP_LENGTH], int mine, vector start);
int aw_location(int state[][MAP_LENGTH][MAP_LENGTH], vector where[]);
int check_aw(int state[][MAP_LENGTH][MAP_LENGTH], vector start);
void set_prioirity(int priority[], int to[]);
unsigned long long int get_state_priority(int state[][MAP_LENGTH][MAP_LENGTH], int priority[]);
int who_win(int map[][MAP_LENGTH], location where_put);
int dir_row_win(int map[][MAP_LENGTH], location where_put, location dir, int mine);
int dir_win(int map[][MAP_LENGTH], location where_put, location dir, int mine);
int is_draw(int map[][MAP_LENGTH]);
location* find_candidate_location(int map[][MAP_LENGTH], int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], int priority[], int mine);
int add_stone(location where, int map[][MAP_LENGTH], int mine);
location* ai_turn(int map[][MAP_LENGTH], int priority[], int remain_turn, int mine);
int ai_game(int map[][MAP_LENGTH], int p0[], int p1[]);
location* aw_doing(int state, vector start_location, int remain_turn);

// Utilization Functions
void initialize_map(int map[][MAP_LENGTH]);
void array1_initializer(int arr[], int length, int toInitialize);
void array2_initializer(int arr[], int length1, int length2, int toInitialize);
void array3_initializer(int arr[], int length1, int length2, int length3, int toInitialize);
void location_copy(location *src, location *dst);
void vector_copy(vector *src, vector *dst);
void map_copy(int temp_map[][MAP_LENGTH], int map[][MAP_LENGTH]);
void print_map(int map[][MAP_LENGTH]);

// Gen Algorithm functions
void select_parent(int parent[][STATE_LENGTH], int selected[][STATE_LENGTH], int fitness[]);
void set_fitness(int generation[][STATE_LENGTH], int given_set[], int fitness[]);
void generate_generation(int geneartion[][STATE_LENGTH], int parent[][STATE_LENGTH]);
void cross_product(int p0[], int p1[], int gen[], int cross[]);
void generate_cross_point(int cross[]);
void generate_mutation(int generation[][STATE_LENGTH], int fitness[]);
int real_rand(int from, int to);

int main() {
	srand((unsigned)time(NULL));
	location* p_last = (location*)malloc(sizeof(location));
	int win, i;
	int remain_turn = 2;
	int turn = BLACK;
	int map[MAP_LENGTH][MAP_LENGTH];
	unsigned long long int p0[PRIORITY_MAX], p1[PRIORITY_MAX];

	initialize_map(map);

	printf("Initialize finishing...\n");

	p_last->x = -1;
	p_last->y = -1;

	for (i = 0; i < PRIORITY_MAX; i++) {
		p0[i] = 5;
		p1[i] = 5;
	}

	printf("Generating finishing...\n");

	p_last = ai_turn(map, p0, 1, turn);
	add_stone(*p_last, map, turn);
	print_map(map);
	printf("We putted to %d %d\n", p_last->x, p_last->y);
	// first turn.

	while (1) {
		win = is_draw(map);
		if (win == 1) {
			printf("draw\n");
			return -1;
		}
		else {
			win = who_win(map, *p_last);
			// check if win.

			if (turn == BLACK) {
				if (win == -1) {
					// if not win, do ai.
					p_last = ai_turn(map, p1, remain_turn, BLACK);
					add_stone(*p_last, map, turn);
					print_map(map);
					printf("We putted to %d %d\n", p_last->x, p_last->y);
				}
				else if (win == BLACK) {
					// if win, return.
					printf("BLACK win\n");
					return 0;
				}
				else if (win == WHITE) {
					// if win, return.
					printf("WHITE win\n");
					return 1;
				}

				remain_turn--;

				if (remain_turn == 0) {
					// if not remain_turn is 0...
					remain_turn = 2;
					turn = WHITE;
				}
			}
			else {
				if (win == -1) {
					// if not win, do ai.
					p_last = ai_turn(map, p0, remain_turn, WHITE);
					add_stone(*p_last, map, turn);
					print_map(map);
					printf("We putted to %d %d\n", p_last->x, p_last->y);
				}
				else if (win == WHITE) {
					// if win, return.
					printf("WHITE win\n");
					return 0;
				}
				else if (win == BLACK) {
					// if win, return.
					printf("BLACK win\n");
					return 1;
				}

				remain_turn--;

				if (remain_turn == 0) {
					// if not remain_turn is 0...
					remain_turn = 2;
					turn = BLACK;
				}
			}
		}
	}

	return 0;
}

void search_state(int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], int map[][MAP_LENGTH], int mine) {
	/*	Check my state(ms), enemy sate(es) from map and save to there.
	'mine' will be BLACK or WHITE, which is my color.
	State will save like ALPHA0 * _0 */
	int i, j, d;
	vector start;

	for (d = 0; d < DIR_MAX; d++) {
		start.dir = d;
		for (i = 0; i < MAP_LENGTH; i++) {
			start.x = i;
			for (j = 0; j < MAP_LENGTH; j++) {
				start.y = j;
				if (mine == BLACK) {
					ms[d][i][j] = get_state(map, BLACK, start);
					es[d][i][j] = get_state(map, WHITE, start);
				}
				else {
					ms[d][i][j] = get_state(map, WHITE, start);
					es[d][i][j] = get_state(map, BLACK, start);
				}
			}
		}
	}


}

int get_state(int map[][MAP_LENGTH], int mine, vector start) {
	//Check map[start.x][start.y] at direction of start.dir.
	//Return -1 if NONE_STATE, else return state.
	location dir;
	int _state = -1;
	int basic_state = NONE_STATE;
	if (start.dir == EAST) {
		dir.x = 0;
		dir.y = 1;
	}
	else if (start.dir == NORTH_EAST) {
		dir.x = -1;
		dir.y = 1;
	}
	else if (start.dir == SOUTH) {
		dir.x = 1;
		dir.y = 0;
	}
	else if (start.dir == SOUTH_EAST) {
		dir.x = 1;
		dir.y = 1;
	}
	if (mine == WHITE) {
		// Check _State
		if ((start.x > MAP_LENGTH - 7) && (dir.x == 0 && dir.y == 1)) {  //EAST ????
			_state = NONE_STATE;
		}
		else if ((start.y > MAP_LENGTH - 7) && (dir.x == 1 && dir.y == 0)) { //SOUTH ????
			_state = NONE_STATE;
		}
		else if ((start.x < 7 || start.y > MAP_LENGTH - 7) && (dir.x == -1 && dir.y == 1)) { //NORTH_EAST ????
			_state = NONE_STATE;
		}
		else if ((start.x > MAP_LENGTH - 7 || start.y > MAP_LENGTH - 7) && (dir.x == 1 && dir.y == 1)) {   //SOUTH_EAST ????
			_state = NONE_STATE;
		}
		else if (map[start.x][start.y] == EMPTY && map[start.x + dir.x * 7][start.y + dir.y * 7] == EMPTY) {
			_state = _0;
		}
		else if (map[start.x][start.y] == EMPTY && map[start.x + dir.x * 7][start.y + dir.y * 7] == WHITE) {
			_state = _1;
		}
		else if (map[start.x][start.y] == EMPTY && map[start.x + dir.x * 7][start.y + dir.y * 7] == BLACK || map[start.x + dir.x * 7][start.y + dir.y * 7] == BLOCKING) {
			_state = _2;
		}
		else if (map[start.x][start.y] == WHITE && map[start.x + dir.x * 7][start.y + dir.y * 7] == EMPTY) {
			_state = _3;
		}
		else if (map[start.x][start.y] == WHITE && map[start.x + dir.x * 7][start.y + dir.y * 7] == WHITE) {
			_state = _4;
		}
		else if (map[start.x][start.y] == WHITE && map[start.x + dir.x * 7][start.y + dir.y * 7] == BLACK || map[start.x + dir.x * 7][start.y + dir.y * 7] == BLOCKING) {
			_state = _5;
		}
		else if (map[start.x][start.y] == BLACK || map[start.x][start.y] == BLOCKING && map[start.x + dir.x * 7][start.y + dir.y * 7] == EMPTY) {
			_state = _6;
		}
		else if (map[start.x][start.y] == BLACK || map[start.x][start.y] == BLOCKING && map[start.x + dir.x * 7][start.y + dir.y * 7] == WHITE) {
			_state = _7;
		}
		else if (map[start.x][start.y] == BLACK || map[start.x][start.y] == BLOCKING && map[start.x + dir.x * 7][start.y + dir.y * 7] == BLACK || map[start.x + dir.x * 7][start.y + dir.y * 7] == BLOCKING) {
			_state = _8;
		}
	}
	else {
		// Check _State
		if ((start.x > MAP_LENGTH - 7) && (dir.x == 0 && dir.y == 1)) {  //EAST ????
			_state = NONE_STATE;
		}
		else if ((start.y > MAP_LENGTH - 7) && (dir.x == 1 && dir.y == 0)) { //SOUTH ????
			_state = NONE_STATE;
		}
		else if ((start.x < 7 || start.y > MAP_LENGTH - 7) && (dir.x == -1 && dir.y == 1)) { //NORTH_EAST ????
			_state = NONE_STATE;
		}
		else if ((start.x > MAP_LENGTH - 7 || start.y > MAP_LENGTH - 7) && (dir.x == 1 && dir.y == 1)) {   //SOUTH_EAST ????
			_state = NONE_STATE;
		}
		else if (map[start.x][start.y] == EMPTY && map[start.x + dir.x * 7][start.y + dir.y * 7] == EMPTY) {
			_state = _0;
		}
		else if (map[start.x][start.y] == EMPTY && map[start.x + dir.x * 7][start.y + dir.y * 7] == BLACK) {
			_state = _1;
		}
		else if (map[start.x][start.y] == EMPTY && map[start.x + dir.x * 7][start.y + dir.y * 7] == WHITE || map[start.x + dir.x * 7][start.y + dir.y * 7] == BLOCKING) {
			_state = _2;
		}
		else if (map[start.x][start.y] == BLACK && map[start.x + dir.x * 7][start.y + dir.y * 7] == EMPTY) {
			_state = _3;
		}
		else if (map[start.x][start.y] == BLACK && map[start.x + dir.x * 7][start.y + dir.y * 7] == BLACK) {
			_state = _4;
		}
		else if (map[start.x][start.y] == BLACK && map[start.x + dir.x * 7][start.y + dir.y * 7] == WHITE || map[start.x + dir.x * 7][start.y + dir.y * 7] == BLOCKING) {
			_state = _5;
		}
		else if (map[start.x][start.y] == WHITE || map[start.x][start.y] == BLOCKING && map[start.x + dir.x * 7][start.y + dir.y * 7] == EMPTY) {
			_state = _6;
		}
		else if (map[start.x][start.y] == WHITE || map[start.x][start.y] == BLOCKING && map[start.x + dir.x * 7][start.y + dir.y * 7] == BLACK) {
			_state = _7;
		}
		else if (map[start.x][start.y] == WHITE || map[start.x][start.y] == BLOCKING && map[start.x + dir.x * 7][start.y + dir.y * 7] == WHITE || map[start.x + dir.x * 7][start.y + dir.y * 7] == BLOCKING) {
			_state = _8;
		}
	}

	// Check Basic state
	if (map[start.x + dir.x * 1][start.y + dir.y * 1] == mine) {
		if (map[start.x + dir.x * 2][start.y + dir.y * 2] == mine) {
			if (map[start.x + dir.x * 3][start.y + dir.y * 3] == mine) {
				if (map[start.x + dir.x * 4][start.y + dir.y * 4] == mine) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = W;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = THETA0;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = THETA1;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = SIGMA0;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else if (map[start.x + dir.x * 4][start.y + dir.y * 4] == EMPTY) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = THETA2;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = SIGMA1;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = SIGMA2;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = GAMMA0;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else {
					basic_state = NONE_STATE;
				}
			}
			else if (map[start.x + dir.x * 3][start.y + dir.y * 3] == EMPTY) {
				if (map[start.x + dir.x * 4][start.y + dir.y * 4] == mine) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = THETA3;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = SIGMA3;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = SIGMA4;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = GAMMA1;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else if (map[start.x + dir.x * 4][start.y + dir.y * 4] == EMPTY) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = SIGMA5;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = GAMMA2;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = GAMMA3;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = BETA0;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else {
					basic_state = NONE_STATE;
				}
			}
			else {
				basic_state = NONE_STATE;
			}
		}
		else if (map[start.x + dir.x * 2][start.y + dir.y * 2] == EMPTY) {
			if (map[start.x + dir.x * 3][start.y + dir.y * 3] == mine) {
				if (map[start.x + dir.x * 4][start.y + dir.y * 4] == mine) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = THETA4;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = SIGMA6;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = SIGMA7;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = GAMMA4;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else if (map[start.x + dir.x * 4][start.y + dir.y * 4] == EMPTY) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = SIGMA8;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = GAMMA5;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = GAMMA6;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = BETA1;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else {
					basic_state = NONE_STATE;
				}
			}
			else if (map[start.x + dir.x * 3][start.y + dir.y * 3] == EMPTY) {
				if (map[start.x + dir.x * 4][start.y + dir.y * 4] == mine) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = SIGMA9;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = GAMMA7;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = GAMMA8;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = BETA2;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else if (map[start.x + dir.x * 4][start.y + dir.y * 4] == EMPTY) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = GAMMA9;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = BETA3;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = BETA4;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = ALPHA0;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else {
					basic_state = NONE_STATE;
				}
			}
			else {
				basic_state = NONE_STATE;
			}
		}
		else {
			basic_state = NONE_STATE;
		}
	}
	else if (map[start.x + dir.x * 1][start.y + dir.y * 1] == EMPTY) {
		if (map[start.x + dir.x * 2][start.y + dir.y * 2] == mine) {
			if (map[start.x + dir.x * 3][start.y + dir.y * 3] == mine) {
				if (map[start.x + dir.x * 4][start.y + dir.y * 4] == mine) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = THETA5;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = SIGMA10;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = SIGMA11;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = GAMMA10;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else if (map[start.x + dir.x * 4][start.y + dir.y * 4] == EMPTY) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = SIGMA12;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = GAMMA11;;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = GAMMA12;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = BETA5;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else {
					basic_state = NONE_STATE;
				}
			}
			else if (map[start.x + dir.x * 3][start.y + dir.y * 3] == EMPTY) {
				if (map[start.x + dir.x * 4][start.y + dir.y * 4] == mine) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = SIGMA13;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = GAMMA13;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = GAMMA14;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = BETA6;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else if (map[start.x + dir.x * 4][start.y + dir.y * 4] == EMPTY) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = GAMMA15;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = BETA7;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = BETA8;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = ALPHA1;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else {
					basic_state = NONE_STATE;
				}
			}
			else {
				basic_state = NONE_STATE;
			}
		}
		else if (map[start.x + dir.x * 2][start.y + dir.y * 2] == EMPTY) {
			if (map[start.x + dir.x * 3][start.y + dir.y * 3] == mine) {
				if (map[start.x + dir.x * 4][start.y + dir.y * 4] == mine) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = SIGMA14;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = GAMMA16;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = GAMMA17;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = BETA9;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else if (map[start.x + dir.x * 4][start.y + dir.y * 4] == EMPTY) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = GAMMA18;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = BETA10;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = BETA11;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = ALPHA2;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else {
					basic_state = NONE_STATE;
				}
			}
			else if (map[start.x + dir.x * 3][start.y + dir.y * 3] == EMPTY) {
				if (map[start.x + dir.x * 4][start.y + dir.y * 4] == mine) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = GAMMA19;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = BETA12;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = BETA13;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = ALPHA3;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else if (map[start.x + dir.x * 4][start.y + dir.y * 4] == EMPTY) {
					if (map[start.x + dir.x * 5][start.y + dir.y * 5] == mine) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = BETA14;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = ALPHA4;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else if (map[start.x + dir.x * 5][start.y + dir.y * 5] == EMPTY) {
						if (map[start.x + dir.x * 6][start.y + dir.y * 6] == mine) {
							basic_state = ALPHA5;
						}
						else if (map[start.x + dir.x * 6][start.y + dir.y * 6] == EMPTY) {
							basic_state = EPSILONE;
						}
						else {
							basic_state = NONE_STATE;
						}
					}
					else {
						basic_state = NONE_STATE;
					}
				}
				else {
					basic_state = NONE_STATE;
				}
			}
			else {
				basic_state = NONE_STATE;
			}
		}
		else {
			basic_state = NONE_STATE;
		}
	}
	else {
		basic_state = NONE_STATE;
	}

	if (_state >= 0) {
		return (basic_state * _STATE_MAX) + _state;
	}
	else {
		return NONE_STATE;
	}
}

int aw_location(int state[][MAP_LENGTH][MAP_LENGTH], vector where[]) {
	// Find all aw location in 'state' and put to 'where'.
	// And return number of aw number.
	int i, j, k;
	int vector_num = 0;
	vector temp;

	for (k = 0; k < DIR_MAX; k++) {
		temp.dir = k;	// Set dir to k.
						// We already defined NORTH_EAST to 0 ... etc.
		for (i = 0; i < MAP_LENGTH; i++) {
			temp.x = i;	// Set x to i.
			for (j = 0; j < MAP_LENGTH; j++) {
				temp.y = j;	// Set y to i.
				if (check_aw(state, temp) == 1) {
					// If mw from temp is right,
					vector_copy(&temp, &where[vector_num]);
					vector_num++;
				}
			}
		}
	}

	return vector_num;
}

int check_aw(int state[][MAP_LENGTH][MAP_LENGTH], vector start) {
	//Check from state[start.dir][start.x][start.y] does it is aw.
	//If right, return 1. Else return 0;
	/*	mw = must win, aw = amado win ?? ??????.
	aw set : { ??, ?? set } X { _0, _2, _6, _8 }
	*/
	int temp = state[start.dir][start.x][start.y];
	int i = 0;
	int return_value = 0;
	if (temp < 0) {
		return 0;
	}

	for (i = 0; i < SIGMA_MAX; i++) {
		if (temp == (SIGMA0 + i) * _STATE_MAX + _0 || temp == (SIGMA0 + i) * _STATE_MAX + _2) {
			return_value = 1;
		}
		else if (temp == (SIGMA0 + i) * _STATE_MAX + _6 || temp == (SIGMA0 + i) * _STATE_MAX + _8) {
			return_value = 1;
		}
	}
	for (i = 0; i < THETA_MAX; i++) {
		if (temp == (THETA0 + i) * _STATE_MAX + _0 || temp == (THETA0 + i) * _STATE_MAX + _2) {
			return_value = 1;
		}
		else if (temp == (THETA0 + i) * _STATE_MAX + _6 || temp == (THETA0 + i) * _STATE_MAX + _8) {
			return_value = 1;
		}
	}

	return return_value;
}

void set_prioirity(int priority[], int to[]) {
	// 0 = NONE_STATE.
	// priority[STATE + 1] = STATE's priority;
	// Max is STATE_LENGTH.
	int i;
	for (i = 0; i < STATE_LENGTH; i++) {
		priority[i] = to[i];
	}
}

unsigned long long int get_state_priority(int state[][MAP_LENGTH][MAP_LENGTH], int priority[]) {
	// Calculate 'state' priority with given array priority.
	// state priority will be very large, so we return it data type of unsigned long long int.
	// Priority is always plus value, so we will use unsigned value.
	unsigned long long int return_value = 0;
	int d, i, j;

	// Caculate with state value.
	for (d = 0; d < DIR_MAX; d++) {
		for (i = 0; i < MAP_LENGTH; i++) {
			for (j = 0; j < MAP_LENGTH; j++) {
				return_value += priority[state[d][i][j] + 1];
			}
		}
	}

	return return_value;
}

int who_win(int map[][MAP_LENGTH], location where_put) {
	/*	Check map from location 'where_put'
	if black win	return BLACK;
	if white win,	return WHITE;
	else,			return -1;	*/
	int i = 0;
	int result = -1;
	int mine = map[where_put.x][where_put.y];
	location dir;

	// At direction to SOUTH.
	if (result == -1) {
		dir.x = 1;
		dir.y = 0;
		result = dir_row_win(map, where_put, dir, mine);
	}

	// At direction to SOUTH_EAST.
	if (result == -1) {
		dir.x = 1;
		dir.y = 1;
		result = dir_row_win(map, where_put, dir, mine);
	}

	// At direction to EAST.
	if (result == -1) {
		dir.x = 0;
		dir.y = 1;
		result = dir_row_win(map, where_put, dir, mine);
	}

	// At direction to NORTH_EAST.
	if (result == -1) {
		dir.x = -1;
		dir.y = 1;
		result = dir_row_win(map, where_put, dir, mine);
	}

	return result;
}

int dir_row_win(int map[][MAP_LENGTH], location where_put, location dir, int mine) {
	int i;
	int result = -1;
	location to_func;

	for (i = -5; i < 6; i++) {	// Check if before stone is not mine because 7 stone is not winning condition.
		if (map[where_put.x + (i - 1) * dir.x][where_put.y + (i - 1) * dir.y] != mine) {
			to_func.x = where_put.x + i * dir.x;
			to_func.x = where_put.y + i * dir.y;
			result = dir_win(map, to_func, dir, mine);
		}
		if (result != -1) {
			break;
		}
	}

	return result;
}

int dir_win(int map[][MAP_LENGTH], location start, location dir, int mine) {
	int add = 0;

	while (add != 7) {
		if (map[start.x + (add * dir.x)][start.y + (add * dir.x)] == mine) {
			// (x_up, y_up) will be given like ( 1, 0 ) at direction SOUTH.
			// So, at direction SOUTH, from location start, check 6 is same.
			// If 7 stones are same, then add will be 7, and we can check this easily like below.
			add++;
		}
		else {
			break;
		}
	}
	if (add == 6) {
		// Not 7, only 6.
		return mine;
	}
	else {
		return -1;
	}
}

int is_draw(int map[][MAP_LENGTH]) {
	//return 1 if draw, else return 0
	int i, j;
	int flag = 1;

	for (i = 0; i < MAP_LENGTH; i++) {
		for (j = 0; j < MAP_LENGTH; j++) {
			if (map[i][j] == EMPTY) {
				flag = 0;
			}
		}
	}

	return flag;
}

location* find_candidate_location(int map[][MAP_LENGTH], int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], int priority[], int mine) {
	// Find candidate location and save it to candidate.
	// if no more candidate is recommended, save (-1, -1).
	// Check every area of map, do it, and check priority is increase or decrease.

	// Decied where to put stone from candidate location list 'candidate'.
	// ms, es, candidate, state_priority value with get_state_priority.
	// ?????? ???? ???? ?? state priority ?? ???? ?????, ???? state priority ?? ???? ??????? ??? ??? ???.
	// Calculate where to put.

	location to;
	location candidate[MAP_LENGTH * MAP_LENGTH * 2];
	location candidate_real[MAP_LENGTH * MAP_LENGTH];

	unsigned long long int enemy_priority[MAP_LENGTH * MAP_LENGTH], mine_priority[MAP_LENGTH * MAP_LENGTH];
	// case of all area is candidate.
	// Find all case can do.
	unsigned long long int now_mine = get_state_priority(ms, priority);
	unsigned long long int now_enemy = get_state_priority(es, priority);
	unsigned long long int temp_mine, temp_enemy;
	unsigned long long int highest_mine, highest_enemy;

	int candidate_number = 0;
	int candidate_number_real = 0;
	int i, j;

	int temp_map[MAP_LENGTH][MAP_LENGTH];
	int temp_ms[DIR_MAX][MAP_LENGTH][MAP_LENGTH], temp_es[DIR_MAX][MAP_LENGTH][MAP_LENGTH];

	map_copy(temp_map, map);
	highest_mine = -1;
	highest_enemy = -1;

	for (i = 0; i < MAP_LENGTH; i++) {
		to.x = i;
		for (j = 0; j < MAP_LENGTH; j++) {
			to.y = j;
			// Check Every area.
			if (add_stone(to, temp_map, mine) == 1) {
				// If can do add at some place..
				search_state(temp_ms, temp_es, temp_map, mine);
				temp_mine = get_state_priority(temp_ms, priority);
				temp_enemy = get_state_priority(temp_es, priority);
				// Caculate changed priority or state.
				if (now_mine < temp_mine) {
					if (highest_mine < temp_mine) {
						highest_mine = temp_mine;
					}// find highest value of my priority

					location_copy(&to, &candidate[candidate_number]);
					mine_priority[candidate_number] = temp_mine;
					enemy_priority[candidate_number] = temp_enemy;
					// Save it to existing array.
					candidate_number++;
				}

				if (now_enemy < temp_enemy) {
					if (highest_enemy < temp_enemy) {
						highest_enemy = temp_enemy;
					}// find highest value of enemy priority

					location_copy(&to, &candidate[candidate_number]);
					mine_priority[candidate_number] = temp_mine;
					enemy_priority[candidate_number] = temp_enemy;
					// Save it to existing array.
					candidate_number++;
				}
				map_copy(temp_map, map);
				array3_initializer(temp_ms, DIR_MAX, MAP_LENGTH, MAP_LENGTH, NONE_STATE);
				array3_initializer(temp_es, DIR_MAX, MAP_LENGTH, MAP_LENGTH, NONE_STATE);
				// Initialize to reuse.
			}
		}
	}
	if (highest_mine - now_mine >= highest_enemy - now_enemy) {
		// if attack...
		for (i = 0; i < candidate_number; i++) {
			if (mine_priority[i] == highest_mine) {
				// find highest my candidate or candidates
				location_copy(&candidate[i], &candidate_real[candidate_number_real]);
				candidate_number_real++;
			}
		}
	}
	else {
		// if defense...
		for (i = 0; i < candidate_number; i++) {
			if (enemy_priority[i] == highest_enemy) {
				// find highest enemy's candidate or candidates
				location_copy(&candidate[i], &candidate_real[candidate_number_real]);
				candidate_number_real++;
			}
		}
	}
	return &candidate_real[real_rand(0, candidate_number_real)];
	// return random value of 0 ~ candidate's number.
	// this can make sure if highest candidates are many, we can choose randomly location.
}

int add_stone(location where, int map[][MAP_LENGTH], int mine) {
	// Add stone to 'where' which stone is 'mine', BLACK, WHITE.
	// If can't do, return 0, else 1
	if (map[where.x][where.y] == EMPTY) {
		map[where.x][where.y] = mine;
		return 1;
	}
	else {
		return 0;
	}
}

location* ai_turn(int map[][MAP_LENGTH], int priority[], int remain_turn, int mine) {
	// real ai part.
	// AI do his job with given map.
	// win when can win, defense when can lose.
	// else, attack or defense with given candidate.

	int ms[DIR_MAX][MAP_LENGTH][MAP_LENGTH], es[DIR_MAX][MAP_LENGTH][MAP_LENGTH];
	int aw_number, mw_number, i, j;
	vector m_aw_where[MAP_LENGTH * MAP_LENGTH], m_mw_where[MAP_LENGTH * MAP_LENGTH];
	vector e_aw_where[MAP_LENGTH * MAP_LENGTH], e_mw_where[MAP_LENGTH * MAP_LENGTH];
	location m_mw_dir[MAP_LENGTH * MAP_LENGTH], e_mw_dir[MAP_LENGTH * MAP_LENGTH];
	location* p_last = (location*)malloc(sizeof(location));

	search_state(ms, es, map, mine);
	p_last->x = -1;
	p_last->y = -1;
	printf("%d %d is object.\n", p_last->x, p_last->y);

	aw_number = aw_location(ms, m_aw_where);

	if (aw_number > 0) {
		for (i = 0; i < aw_number; i++) {
			// if aw is exist, then we can win.
			// doing job with remain_turn.
			p_last = aw_doing(ms[m_aw_where[i].dir][m_aw_where[i].x][m_aw_where[i].y], m_aw_where[i], remain_turn);

			printf("%d %d is object.\n", p_last->x, p_last->y);
			if (p_last->x != -1) {
				break;
			}
		}
	}
	if (p_last->x != -1) {
		// If aw is exist and doing finishing...
		return &p_last;
	}
	else {
		// If aw is exist but can't finish...
		aw_number = aw_location(es, e_aw_where);
		if (aw_number > 0) {
			for (i = 0; i < aw_number; i++) {
				// if aw is exist, then we can lose, so defense it.
				// doing job with remain_turn.
				p_last = aw_doing(ms[m_aw_where[i].dir][m_aw_where[i].x][m_aw_where[i].y], m_aw_where[i], remain_turn);

				printf("%d %d is object.\n", p_last->x, p_last->y);
				if (p_last->x != -1) {
					break;
				}
			}
		}
	}
	if (p_last->x != -1) {
		// If aw is exist and can be finishing...
		return p_last;
	}
	else {
		// if i can't finish and enemy can't finish.
		p_last = find_candidate_location(map, ms, es, priority, mine);
		printf("%d %d is object.\n", p_last->x, p_last->y);
	}

	printf("%d %d is object.\n", p_last->x, p_last->y);
	return p_last;
}

int ai_game(int map[][MAP_LENGTH], int p0[], int p1[]) {
	// ai's game with p0, p1 proiority.
	// return 0 if p0 win, return 1 if p1 win, return -1 if draw.
	location* p_last = (location*)malloc(sizeof(location));
	int win;
	int remain_turn = 2;
	int turn = BLACK;

	p_last->x = -1;
	p_last->y = -1;

	p_last = ai_turn(map, p0, 1, turn);
	add_stone(*p_last, map, turn);
	// first turn.

	while (1) {
		win = is_draw(map);
		if (win == 1) {
			return -1;
		}
		else {
			win = who_win(map, *p_last);
			// check if win.

			if (turn == BLACK) {
				if (win == -1) {
					// if not win, do ai.
					p_last = ai_turn(map, p1, remain_turn, BLACK);
					add_stone(*p_last, map, turn);
				}
				else if (win == BLACK) {
					// if win, return.
					return 0;
				}
				else if (win == WHITE) {
					// if win, return.
					return 1;
				}

				remain_turn--;

				if (remain_turn == 0) {
					// if not remain_turn is 0...
					remain_turn = 2;
					turn = WHITE;
				}
			}
			else {
				if (win == -1) {
					// if not win, do ai.
					p_last = ai_turn(map, p0, remain_turn, WHITE);
					add_stone(*p_last, map, turn);
				}
				else if (win == WHITE) {
					// if win, return.
					return 0;
				}
				else if (win == BLACK) {
					// if win, return.
					return 1;
				}

				remain_turn--;

				if (remain_turn == 0) {
					// if not remain_turn is 0...
					remain_turn = 2;
					turn = BLACK;
				}
			}
		}
	}
}

location* aw_doing(int state, vector start_location, int remain_turn) {
	// at remain_turn is 2, SIGMA can win and at remain_turn is 1, THETA can win.
	// return where to put to win at that time.
	location* p_last = (location*)malloc(sizeof(location));

	p_last->x = -1;
	p_last->y = -1;

	if (remain_turn == 2) {
		if (state >= (SIGMA0 * _STATE_MAX + _0) && state < (THETA0 * _STATE_MAX + _0)) {
			// if state is at SIGMA and remain turn is 2...
			if (state == SIGMA0  * _STATE_MAX + _0 || state == SIGMA0  * _STATE_MAX + _2 || state == SIGMA0  * _STATE_MAX + _6 || state == SIGMA0  * _STATE_MAX + _8) {
				//??0	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 5;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y + 5;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y - 5;
				}
			}
			else if (state == SIGMA1  * _STATE_MAX + _0 || state == SIGMA1  * _STATE_MAX + _2 || state == SIGMA1  * _STATE_MAX + _6 || state == SIGMA1  * _STATE_MAX + _8) {
				//??1	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y - 4;
				}
			}
			else if (state == SIGMA2  * _STATE_MAX + _0 || state == SIGMA2  * _STATE_MAX + _2 || state == SIGMA2  * _STATE_MAX + _6 || state == SIGMA2  * _STATE_MAX + _8) {
				//??2	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y - 4;
				}
			}
			else if (state == SIGMA3  * _STATE_MAX + _0 || state == SIGMA3  * _STATE_MAX + _2 || state == SIGMA3  * _STATE_MAX + _6 || state == SIGMA3  * _STATE_MAX + _8) {
				//??3	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 3;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y + 3;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y - 3;
				}
			}
			else if (state == SIGMA4  * _STATE_MAX + _0 || state == SIGMA4  * _STATE_MAX + _2 || state == SIGMA4  * _STATE_MAX + _6 || state == SIGMA4  * _STATE_MAX + _8) {
				//??4	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 3;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y + 3;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y - 3;
				}
			}
			else if (state == SIGMA5  * _STATE_MAX + _0 || state == SIGMA5  * _STATE_MAX + _2 || state == SIGMA5  * _STATE_MAX + _6 || state == SIGMA5  * _STATE_MAX + _8) {
				//??5	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y - 4;
				}
			}
			else if (state == SIGMA6  * _STATE_MAX + _0 || state == SIGMA6  * _STATE_MAX + _2 || state == SIGMA6  * _STATE_MAX + _6 || state == SIGMA6  * _STATE_MAX + _8) {
				//??6	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 2;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 6;
					p_last->y = start_location.y + 2;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 2;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 2;
					p_last->y = start_location.y - 2;
				}
			}
			else if (state == SIGMA7  * _STATE_MAX + _0 || state == SIGMA7  * _STATE_MAX + _2 || state == SIGMA7  * _STATE_MAX + _6 || state == SIGMA7  * _STATE_MAX + _8) {
				//??7	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 5;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y + 5;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y - 5;
				}
			}
			else if (state == SIGMA8  * _STATE_MAX + _0 || state == SIGMA8  * _STATE_MAX + _2 || state == SIGMA8  * _STATE_MAX + _6 || state == SIGMA8  * _STATE_MAX + _8) {
				//??8	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y - 4;
				}
			}
			else if (state == SIGMA9  * _STATE_MAX + _0 || state == SIGMA9  * _STATE_MAX + _2 || state == SIGMA9  * _STATE_MAX + _6 || state == SIGMA9  * _STATE_MAX + _8) {
				//??9	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 3;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y + 3;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y - 3;
				}
			}
			else if (state == SIGMA10  * _STATE_MAX + _0 || state == SIGMA10  * _STATE_MAX + _2 || state == SIGMA10  * _STATE_MAX + _6 || state == SIGMA10  * _STATE_MAX + _8) {
				//??10	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 6;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 6;
					p_last->y = start_location.y + 6;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 6;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 6;
					p_last->y = start_location.y - 6;
				}
			}
			else if (state == SIGMA11  * _STATE_MAX + _0 || state == SIGMA11  * _STATE_MAX + _2 || state == SIGMA11  * _STATE_MAX + _6 || state == SIGMA11  * _STATE_MAX + _8) {
				//??11	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 5;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y + 5;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y - 5;
				}
			}
			else if (state == SIGMA12  * _STATE_MAX + _0 || state == SIGMA12  * _STATE_MAX + _2 || state == SIGMA12  * _STATE_MAX + _6 || state == SIGMA12  * _STATE_MAX + _8) {
				//??12	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y - 4;
				}
			}
			else if (state == SIGMA13  * _STATE_MAX + _0 || state == SIGMA13  * _STATE_MAX + _2 || state == SIGMA13  * _STATE_MAX + _6 || state == SIGMA13  * _STATE_MAX + _8) {
				//??13	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 3;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y + 3;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 1;
					p_last->y = start_location.y - 1;
				}
			}
			else if (state == SIGMA14  * _STATE_MAX + _0 || state == SIGMA14  * _STATE_MAX + _2 || state == SIGMA14  * _STATE_MAX + _6 || state == SIGMA14  * _STATE_MAX + _8) {
				//??14	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 2;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 2;
					p_last->y = start_location.y + 2;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 2;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 2;
					p_last->y = start_location.y - 2;
				}
			}
		}
	}
	else {
		if (state < (SIGMA0 * _STATE_MAX + _0) || state >= (THETA0 * _STATE_MAX + _0)) {
			// if state is at THETA and remain turn is 1...
			if (state == THETA0 * _STATE_MAX + _0 || state == THETA0 * _STATE_MAX + _2 || state == THETA0 * _STATE_MAX + _6 || state == THETA0 * _STATE_MAX + _8) {
				//??0	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 6;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 6;
					p_last->y = start_location.y + 6;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 6;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 6;
					p_last->y = start_location.y - 6;
				}
			}
			else if (state == THETA1 * _STATE_MAX + _0 || state == THETA1 * _STATE_MAX + _2 || state == THETA1 * _STATE_MAX + _6 || state == THETA1 * _STATE_MAX + _8) {
				//??1	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 5;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y + 5;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 5;
					p_last->y = start_location.y - 5;
				}
			}
			else if (state == THETA2 * _STATE_MAX + _0 || state == THETA2 * _STATE_MAX + _2 || state == THETA2 * _STATE_MAX + _6 || state == THETA2 * _STATE_MAX + _8) {
				//??2	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 4;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 4;
					p_last->y = start_location.y - 4;
				}
			}
			else if (state == THETA3 * _STATE_MAX + _0 || state == THETA3 * _STATE_MAX + _2 || state == THETA3 * _STATE_MAX + _6 || state == THETA3 * _STATE_MAX + _8) {
				//??3	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 3;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y + 3;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 3;
					p_last->y = start_location.y - 3;
				}
			}
			else if (state == THETA4 * _STATE_MAX + _0 || state == THETA4 * _STATE_MAX + _2 || state == THETA4 * _STATE_MAX + _6 || state == THETA4 * _STATE_MAX + _8) {
				//??4	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 2;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 2;
					p_last->y = start_location.y + 2;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 2;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 2;
					p_last->y = start_location.y - 2;
				}
			}
			else if (state == THETA5 * _STATE_MAX + _0 || state == THETA5 * _STATE_MAX + _2 || state == THETA5 * _STATE_MAX + _6 || state == THETA5 * _STATE_MAX + _8) {
				//??5	???????
				if (start_location.dir == SOUTH) { // increase ( 0, 1 )
					p_last->x = start_location.x + 0;
					p_last->y = start_location.y + 1;
				}
				else if (start_location.dir == SOUTH_EAST) { // increase ( 1, 1 )
					p_last->x = start_location.x + 1;
					p_last->y = start_location.y + 1;
				}
				else if (start_location.dir == EAST) { // increase ( 1 , 0 )
					p_last->x = start_location.x + 1;
					p_last->y = start_location.y + 0;
				}
				else {	// increase ( 1, -1 )
					p_last->x = start_location.x + 1;
					p_last->y = start_location.y - 1;
				}
			}
		}
	}

	return p_last;
}



void print_state(int state)	{
	int _state = state % _STATE_MAX;
	int basic_state = (int)((state - _state) / _STATE_MAX);

	if(basic_state >= ALPHA0 && basic_state < BETA0)	{
		printf("ALPHA%d_%d", basic_state-ALPHA0, _state);
	}
	else if(basic_state >= BETA0 && basic_state < GAMMA0)
		printf("BETA%d_%d", basic_state-BETA0, _state);
	}
	else if(basic_state >= GAMMA0 && basic_state < SIGMA0)	{
		printf("GAMMA%d_%d", basic_state-GAMMA0, _state);
	}
	else if(basic_state >= SIGMA0 && basic_state < THETA0)	{
		printf("SIGMA%d_%d", basic_state-SIGMA0, _state);
	}
	else if(basic_state >= THETA0 && basic_state < W)	{
		printf("W");
	}
}

void initialize_map(int map[][MAP_LENGTH]) {
	// Initialize map to EMPTY which is defined.
	array2_initializer(map, MAP_LENGTH, MAP_LENGTH, EMPTY);
	// Using below function.
}

void array1_initializer(int arr[], int length, int toInitialize) {
	// Initialize one dimension array with length 'length' to toInitialize.
	int i;

	for (i = 0; i < length; i++) {
		arr[i] = toInitialize;
	}// arr[i] = toInitialize for every i in range 0 ~ max.
}

void array2_initializer(int arr[], int length1, int length2, int toInitialize) {
	// Initialize two dimension array with length 'length1' and 'length2' to toInitialize.
	int i, j;

	for (i = 0; i < length1; i++) {
		for (j = 0; j < length2; j++) {
			arr[i * length1 + j] = toInitialize;
		}// arr[i][j] = toInitialize for every i and j in range 0 ~ max.
	}

}

void array3_initializer(int arr[], int length1, int length2, int length3, int toInitialize) {
	// Initialize two dimension array with length 'length1', 'length2' and 'length3' to toInitialize.
	int i, j, k;

	for (i = 0; i < length1; i++) {
		for (j = 0; j < length2; j++) {
			for (k = 0; k < length3; k++) {
				arr[i * length1 + j*length2 + k] = toInitialize;
			}// arr[i][j][k] = toInitialize for every i, j and k in range 0 ~ max.
		}
	}

}

void location_copy(location *src, location *dst) {
	//Copy src to dst.
	dst->x = src->x;
	dst->y = src->y;
}

void vector_copy(vector *src, vector *dst) {
	//Copy src to dst.
	dst->x = src->x;
	dst->y = src->y;
	dst->dir = src->dir;
}

void map_copy(int temp_map[][MAP_LENGTH], int map[][MAP_LENGTH]) {
	int i, j;

	for (i = 0; i < MAP_LENGTH; i++) {
		for (j = 0; j < MAP_LENGTH; j++) {
			temp_map[i][j] = map[i][j];
		}
	}
}

void print_map(int map[][MAP_LENGTH]) {
	int i, j;
	printf("[ MAP ]\n");
	for (i = 0; i < MAP_LENGTH; i++) {
		for (j = 0; j < MAP_LENGTH; j++) {
			if (map[i][j] == WHITE) {
				printf("??");
			}
			else if (map[i][j] == BLACK) {
				printf("??");
			}
			else if (map[i][j] == BLOCKING) {
				printf("??");
			}
			else {
				printf("??");
			}
		}
		printf("\n");
	}
	Sleep(1000);
}



void select_parent(int parent[][STATE_LENGTH], int selected[][STATE_LENGTH], int fitness[]) {
	// Select PARAENT_MAX parents from parent[GENERATION_MAX][STATE_LENGTH] with fitness[GENERATION_MAX] values.
	// We will use rullet-wheel method to select parents.
	int i, j, temp;
	int index[GENERATION_MAX];
	int temp_fit[GENERATION_MAX];
	for (i = 0; i < GENERATION_MAX; i++) {
		index[i] = i;
	}

	for (i = 0; i < GENERATION_MAX; i++) {
		temp_fit[i] = fitness[i];
	}

	// Sort with fitness value and take top PARAENT_MAX.
	for (i = 0; i < GENERATION_MAX - 1; i++) {
		for (j = i + 1; j < GENERATION_MAX; j++) {
			if (temp_fit[i] < temp_fit[j]) {
				temp = temp_fit[i];
				temp_fit[i] = temp_fit[j];
				temp_fit[j] = temp;

				temp = index[i];
				index[i] = index[j];
				index[j] = temp;
				// index is chagned too, so can calculate value of original index.
			}
		}
	}

	for (i = 0; i < PARENT_MAX; i++) {
		// Select top PARAENT_MAX values.
		for (j = 0; j < STATE_LENGTH; j++) {
			selected[i][j] = parent[index[i]][j];
		}
	}
}

void set_fitness(int generation[][STATE_LENGTH], int given_set[], int fitness[]) {
	// Set fitness score to fitenss[GENERATION_MAX].
	// generation[i] VS given_set 's win percent will be fitness[i]
	// Probability of victory is average of result of every other generations.

	int map[MAP_LENGTH][MAP_LENGTH];
	int win, i;
	int play = 0;
	int win_number = 0;

	initialize_map(map);

	for (i = 0; i < GENERATION_MAX; i++) {
		while (play < GAME_MAX) {
			win = ai_game(map, &generation[i], given_set);
			// game processed generation[i] first.

			if (win == 0) {
				//generation[i]'s win.
				win_number++;
				play++;
			}

			initialize_map(map);

			win = ai_game(map, given_set, &generation[i]);
			// game processed given_set first.

			if (win == 0) {
				//generation[i]'s win.
				win_number++;
				play++;
			}

			initialize_map(map);
			// initialize map for reusing.
		}
		fitness[i] = (int)(FITNESS_MAX / GAME_MAX) * win_number;
		//calculate fitness with win number.
	}

}

void generate_generation(int geneartion[][STATE_LENGTH], int parent[][STATE_LENGTH]) {
	// Doing CROSS_POINT_MAX - point Cross product from each parent[GENERATION_MAX][STATE_LENGTH] pair.
	// Do Cross product parent pair (0, 1), (0, 2), (0, 3), (1, 2), (1, 3), (2, 3)
	// Each pair generate 5 next generation.
	int i, gen_num = 0;
	int cross[CROSS_POINT_MAX];
	for (i = 0; i < SIVILING_MAX; i++) {
		// Generate with (0, 1) parents pair.
		generate_cross_point(cross);
		cross_product(&parent[0], &parent[1], &geneartion[gen_num], cross);
		gen_num++;
	}
	for (i = 0; i < SIVILING_MAX; i++) {
		// Generate with (0, 2) parents pair.
		generate_cross_point(cross);
		cross_product(&parent[0], &parent[2], &geneartion[gen_num], cross);
		gen_num++;
	}
	for (i = 0; i < SIVILING_MAX; i++) {
		// Generate with (0, 3) parents pair.
		generate_cross_point(cross);
		cross_product(&parent[0], &parent[3], &geneartion[gen_num], cross);
		gen_num++;
	}
	for (i = 0; i < SIVILING_MAX; i++) {
		// Generate with (1, 2) parents pair.
		generate_cross_point(cross);
		cross_product(&parent[1], &parent[2], &geneartion[gen_num], cross);
		gen_num++;
	}
	for (i = 0; i < SIVILING_MAX; i++) {
		// Generate with (1, 3) parents pair.
		generate_cross_point(cross);
		cross_product(&parent[1], &parent[3], &geneartion[gen_num], cross);
		gen_num++;
	}
	for (i = 0; i < SIVILING_MAX; i++) {
		// Generate with (2, 3) parents pair.
		generate_cross_point(cross);
		cross_product(&parent[2], &parent[3], &geneartion[gen_num], cross);
		gen_num++;
	}
}

void generate_cross_point(int cross[]) {
	int i, j, temp;
	// Generate random cross-point.
	for (i = 0; i < CROSS_POINT_MAX; i++) {
		cross[i] = real_rand(0, STATE_LENGTH);
	}

	// Sorting with large number.
	for (i = 0; i < CROSS_POINT_MAX - 1; i++) {
		for (j = i + 1; j < CROSS_POINT_MAX; j++) {
			if (cross[i] < cross[j]) {
				temp = cross[i];
				cross[i] = cross[j];
				cross[j] = temp;
			}
		}
	}
}

void cross_product(int p0[], int p1[], int gen[], int cross[]) {
	// Doing cross product.
	// gen = { p1|p2|p1|p2 ... } for cross[i] in range(0, CORSS_POINT_MAX)
	int par = 0;
	int cross_number = 0;
	int i;

	for (i = 0; i < STATE_LENGTH; i++) {
		if (i < cross[cross_number]) {
			if (par == 0) {
				gen[i] = p0[i];
			}
			else {
				gen[i] = p1[i];
			}
		}
		else {
			cross_number++;
			while (cross[cross_number - 1] == cross[cross_number]) {
				cross_number++;
			}
			if (par == 0) {
				par = 1;
				gen[i] = p1[i];
			}
			else {
				par = 0;
				gen[i] = p0[i];
			}
		}
	}
}

void generate_mutation(int generation[][STATE_LENGTH], int fitness[]) {
	// Mutation is appear at generation[GENERATION_MAX][STATE_LENGTH] with percent of fitness[].
	// Mutation appear at [i] change into random number.
	// The probability of mutation will be low if parent's fitness average is low.
	// fitess[i] is over 0, below FITNESS_MAX, so if real_rand(0, FITNESS_MAX) 's output is over fitness[i], then mutate.
	// if else, don't mutate, for all part of generic.

	int i, j, random_value;

	for (i = 0; i < GENERATION_MAX; i++) {
		for (j = 0; j < STATE_LENGTH; j++) {
			if (real_rand(0, FITNESS_MAX) < fitness[i]) {
				generation[i][j] = real_rand(0, PRIORITY_MAX);
			}
		}
	}
}

int real_rand(int from, int to) {
	// return real random value from 'from' to 'to'.
	int return_value = -1;
	srand((unsigned)time(NULL));
	// Seed Initialize.

	while (return_value < from || return_value >= to) {
		// If out of range, iterate.
		return_value = rand();
	}

	return return_value;
}
