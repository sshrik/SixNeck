#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/*
[목표] : 6칸을 만들어 승리 / 상대의 6칸을 방해
1. 탐색하여 my_state 와 enemy_state 를 채운다.
-> void search_state(int ms[][MAP_LENGTH], int es[][MAP_LENGTH], int priority[], int mine);
맵을 정의하여야 한다.
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
2. 행동 조건에 따라 행동한다.
-> void act(int ms[][MAP_LENGTH], int es[][MAP_LENGTH], int priority[], int mine);
- 내꺼가 이기는 상태( mw, aw ) 가 1개라도 있으면 바로 승리하게 둔다.
-> int mw_location( int ms[][MAP_LENGTH], location where[], int priority[], int mine );
-> int aw_location( int ms[][MAP_LENGTH], location where[], int priority[], int mine );
-> int can_win(int ms[][MAP_LENGTH], int priority[], int mine);
- 상대꺼가 이기는 상태 ( mw, aw ) 가 1개라도 있으면 바로 막게 된다.
-> int can_lose(int es[][MAP_LENGTH], int priority[], int mine);
- 위의 두 조건이 아닐 시, 후보 위치군을 추출하여 내 가중치를 최대화, 상대 가중치를 최소화시키는
방향으로 행동한다.
-> void find_candidate_location(int ms[][MAP_LENGTH],int es[][MAP_LENGTH],location candidate[], int priority[], int mine)
- 가중치는 int priority[state_length] 로 정의 되어 있다.
3. 둔다.
->	location decide_location( int ms[][MAP_LENGTH], int es[][MAP_LENGTH], int priority[], int mine )
-> void add_stone(int who, location where, int map[][MAP_LENGTH], int mine)
4. 승리 조건 판별, 다음 차례가 누구인가 판별 후 진행.
-> int who_win( int map[][MAP_LENGTH] );
5. 승리 / 패배 시 종료. 다음 차례는 1로 가서 계속 진행.
-> void initialize_map( int map[][MAP_LENGTH], int mine );
-> void array1_initailizer( int arr[], int length, int toInitialize );
-> void array2_initailizer( int arr[][MAP_LENGTH], int toInitialize );

이를 위해서 상태를 정의하고, 가중치를 부여해야 한다.
#deifne NONE_STATE -1
#define EPSILONE_0 0
#define EPSILONE_1 1
NONE_STATE 는 -1,
각 EPSILONE 은 0 * 9 + 0, ALPHA0은 1 * 9 + 0 , ALPHA1 2 * 9 + 0 ... 으로,
_0 은 0, _1 은 1 .. _n 은 n 을 각각에 대해서 더하는 것으로 진행한다.
즉 ALPHA3_6 = 4 * 9 + 6 이다.
... 으로 상태를 정의하도록 한다.
또한 mw, aw 의 상태는 각 숫자를 써서 프로그래머가 기억하고, 각각 분류 해 준다.
- 4, 5 개는 전부 aw 이상이니 분류가 쉬울 것이다.
또한 방향은 */
#define NORTH_EAST 0
#define EAST 1
#define SOUTH_EAST 2
#define SOUTH 3
/*
과 같이 정의한다.

가중치는
int priority[state_length];
과 같이 정의한다.
- 가중치에 대한 판단을 해야 한다.
이는 처음에 대략적인 값을 입력 한 뒤, 유전 알고리즘을 통해 진행하도록 한다.
- 이에 대한 사항은 알아서 진행 해야 할 듯..!
*/

// 상태의 정의는 아래에 주석으로 첨부.

/*	◎ : EMPTY, ○ : MINE, ● : ENEMY
◎ / ◎	_0	◎ / ○	_1	◎ / ●	_2
○ / ◎	_3	○ / ○	_4	○ / ●	_5
● / ◎	_6	● / ○	_7	● / ●	_8
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
ε : 0개의 내 돌 ( ○ ) 존재
◎◎◎◎◎◎
*/
#define EPSILONE 0
/*
α	1개의 내 돌 ( ○ ) 존재 = { α0, α1, α2 ... α5 }
α0	○◎◎◎◎◎	α1	◎○◎◎◎◎	α2	◎◎○◎◎◎	α3	◎◎◎○◎◎	α4	◎◎◎◎○◎
α5	◎◎◎◎◎○
*/
#define ALPHA0 1
#define ALPHA1 2
#define ALPHA2 3
#define ALPHA3 4
#define ALPHA4 5
#define ALPHA5 6
#define ALPHA_MAX 6
/*
β	2개의 내 돌 ( ○ ) 존재 = { β0, β1, β2 ... β14 }
β0	○○◎◎◎◎	β1	○◎○◎◎◎	β2	○◎◎○◎◎	β3	○◎◎◎○◎
β4	○◎◎◎◎○	β5	◎○○◎◎◎	β6	◎○◎○◎◎	β7	◎○◎◎○◎
β8	◎○◎◎◎○	β9	◎◎○○◎◎	β10	◎◎○◎○◎	β11	◎◎○◎◎○
β12	◎◎◎○○◎	β13	◎◎◎○◎○	β14	◎◎◎◎○○
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
γ	3개의 내 돌 ( ○ ) 존재 = { γ0, γ1, γ2 ... γ 19 }
γ0	○○○◎◎◎	γ1	○○◎○◎◎	γ2	○○◎◎○◎	γ3	○○◎◎◎○
γ4	○◎○○◎◎	γ5	○◎○◎○◎	γ6	○◎○◎◎○	γ7	○◎◎○○◎
γ8	○◎◎○◎○	γ9	○◎◎◎○○
γ10	◎○○○◎◎	γ11	◎○○◎○◎	γ12	◎○○◎◎○	γ13	◎○◎○○◎
γ14	◎○◎○◎○	γ15	◎○◎◎○○	γ16	◎◎○○○◎	γ17	◎◎○○◎○
γ18	◎◎○◎○○	γ19	◎◎◎○○○
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
δ	4개의 내 돌 ( ○ ) 존재 = { δ0, δ1, δ2 ... δ14 }
δ0	○○○○◎◎	δ1	○○○◎○◎	δ2	○○○◎◎○	δ3	○○◎○○◎
δ4	○○◎○◎○	δ5	○○◎◎○○	δ6	○◎○○○◎	δ7	○◎○○◎○
δ8	○◎○◎○○	δ9	○◎◎○○○
δ10	◎○○○○◎	δ11	◎○○○◎○	δ12	◎○○◎○○	δ13	◎○◎○○○
δ14	◎◎○○○○
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
θ	5개의 내 돌 ( ○ ) 존재 = { θ0, θ1, θ2 ... θ5 }
θ0	○○○○○◎	θ1	○○○○◎○	θ2	○○○◎○○	θ3	○○◎○○○	θ4	○◎○○○○
θ5	◎○○○○○
*/
#define THETA0 43
#define THETA1 44
#define THETA2 45
#define THETA3 46
#define THETA4 47
#define THETA5 48
#define THETA_MAX 6
/*
W	6개의 내 돌 ( ○ ) 존재 = { W }
W	○○○○○○
*/
#define W 49

//State is defigned like DELTA5 * _STATE_MAX + _5 or GAMMA7 * _2

#define STATE_LENGTH 451
// 50 * 9 + 1 개

/*	mw = must win, aw = amado win 을 의미한다.
mw set : aw set 이 연속해서 3개 붙어 있을 때.
aw set : { δ, θ set } X { _0, _2, _6, _8 }
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
int mw_location(int state[][MAP_LENGTH][MAP_LENGTH], vector where[], location dri[]);
int check_mw(int state[][MAP_LENGTH][MAP_LENGTH], vector start, location dir);
int aw_location(int state[][MAP_LENGTH][MAP_LENGTH], vector where[]);
int check_aw(int state[][MAP_LENGTH][MAP_LENGTH], vector start);
void set_prioirity(int priority[], int to[]);
unsigned long long int get_state_priority(int state[][MAP_LENGTH][MAP_LENGTH], int priority[]);
int who_win(int map[][MAP_LENGTH], location where_put);
int dir_row_win(int map[][MAP_LENGTH], location where_put, location dir, int mine);
int dir_win(int map[][MAP_LENGTH], location where_put, location dir, int mine);
int is_draw(int map[][MAP_LENGTH]);
location find_candidate_location(int map[][MAP_LENGTH], int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], int priority[], int mine);
int add_stone(location where, int map[][MAP_LENGTH], int mine);
location ai_turn(int map[][MAP_LENGTH], int priority[], location last_put);
int ai_game(int map[][MAP_LENGTH], int p0[], int p1[]);
location aw_doing(int state, vector start_location, int remain_turn);

// Utilization Functions
void initialize_map(int map[][MAP_LENGTH]);
void array1_initializer(int arr[], int length, int toInitialize);
void array2_initializer(int arr[], int length1, int length2, int toInitialize);
void array3_initializer(int arr[], int length1, int length2, int length3, int toInitialize);
void location_copy(location *src, location *dst);
void vector_copy(vector *src, vector *dst);
void map_copy(int temp_map[][MAP_LENGTH], int map[][MAP_LENGTH]);

// Gen Algorithm functions
void select_parent(int parent[][STATE_LENGTH], int selected[][STATE_LENGTH], int fitness[]);
void set_fitness(int parent[][STATE_LENGTH], int fitness[]);
void generate_generation(int geneartion[][STATE_LENGTH], int parent[][STATE_LENGTH]);
void cross_product(int p0[], int p1[], int gen[], int cross[]);
void generate_cross_point(int cross[]);
void generate_mutation(int generation[][STATE_LENGTH], int fitness[]);
void suffle_generation(int generation[][STATE_LENGTH]);
int real_rand(int from, int to);

int main() {
	srand((unsigned)time(NULL));


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
		dir.x = 1;
		dir.y = 0;
	}
	else if (start.dir = NORTH_EAST) {
		dir.x = 1;
		dir.y = -1;
	}
	else if (start.dir = SOUTH) {
		dir.x = 0;
		dir.y = 1;
	}
	else if (start.dir = SOUTH_EAST) {
		dir.x = 1;
		dir.y = 1;
	}
	if (mine == WHITE) {
		// Check _State
		if (start.x > MAP_LENGTH - 7 && dir.x == 1) {  //동쪽 견제
			_state = NONE_STATE;   //그러므로 check_~ function에서 temp < 0 이 되면 나오면 계산 결과 무시
		}
		else if (start.y > MAP_LENGTH - 7 && dir.y == 1) { //남쪽 견제      //따라서 동남쪽 견제
			_state = NONE_STATE;
		}
		else if (start.y < 6 && dir.y == -1) {   //북동쪽 견제
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
		if (start.x > MAP_LENGTH - 7 && dir.x == 1) {  //동쪽 견제
			_state = NONE_STATE;   //그러므로 check_~ function에서 temp < 0 이 되면 나오면 계산 결과 무시
		}
		else if (start.y > MAP_LENGTH - 7 && dir.y == 1) { //남쪽 견제      //따라서 동남쪽 견제
			_state = NONE_STATE;
		}
		else if (start.y < 6 && dir.y == -1) {   //북동쪽 견제
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

	if (_state > 0 || basic_state > 0) {
		return (basic_state * _STATE_MAX) + _state;
	}
	else {
		return NONE_STATE;
	}
}

int mw_location(int state[][MAP_LENGTH][MAP_LENGTH], vector where[], location dir_list[]) {
	// Find all mw location in 'state' and put to 'where'. 'dir' is the direction of 'where'.
	// And return number of mw number.
	int i, j, k, d;
	int vector_num = 0;
	vector temp;
	location dir;

	for (k = 0; k < DIR_MAX; k++) {
		temp.dir = k;	// Set dir to k.
		// We already defined NORTH_EAST to 0 ... etc.
		for (i = 0; i < MAP_LENGTH; i++) {
			temp.x = i;	// Set x to i.
			for (j = 0; j < MAP_LENGTH; j++) {
				temp.y = j;	// Set y to i.
				for (d = 0; d < DIR_MAX; d++) {
					// Set dir where to check.
					if (d == EAST) {
						dir.x = 1;
						dir.y = 0;
					}
					else if (d == NORTH_EAST) {
						dir.x = 1;
						dir.y = -1;
					}
					else if (d == SOUTH) {
						dir.x = 0;
						dir.y = 1;
					}
					else if (d == SOUTH_EAST) {
						dir.x = 1;
						dir.y = 1;
					}

					if (check_mw(state, temp, dir) == 1) {
						// If mw from temp is right, add to list.
						vector_copy(&where[vector_num], &temp);
						location_copy(&dir_list[vector_num], &dir);
						vector_num++;
					}
				}
			}
		}
	}

	return vector_num;
}

int check_mw(int state[][MAP_LENGTH][MAP_LENGTH], vector start, location dir) {
	//Check from state[start.dir][start.x][start.y] does it is mw.
	// Because mw need 3 aw, so location dir are needed.
	//If right, return 1. Else return 0;
	/*	mw = must win, aw = amado win 을 의미한다.
	mw set : aw set 이 연속해서 3개 붙어 있을 때.
	*/
	int i;
	vector temp;
	vector_copy(&temp, &start);
	for (i = 0; i < 3; i++) {
		if (check_aw(state, temp) == 1) {	// To check 3 aw in case.
			temp.x += dir.x;
			temp.y += dir.y;
		}
		else {
			break;
		}
	}
	if (i != 3) {
		return 0;
	}
	else {
		return 1;
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
					vector_copy(&where[vector_num], &temp);
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
	/*	mw = must win, aw = amado win 을 의미한다.
	aw set : { δ, θ set } X { _0, _2, _6, _8 }
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

int is_draw(int map[][MAP_LENGTH])	{
	//return 1 if draw, else return 0
	int i, j;

	for (i = 0; i < MAP_LENGTH; i++)	{
		for (j = 0; j < MAP_LENGTH; j++)	{
			if (map[i][j] != EMPTY || map[i][j] != BLOCKING)	{
				break;
			}
		}
		if (j != MAP_LENGTH)	{
			break;
		}
	}
	if (i != MAP_LENGTH)	{
		return 1;
	}
	else {
		return 0;
	}
}

location find_candidate_location(int map[][MAP_LENGTH], int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], int priority[], int mine) {
	// Find candidate location and save it to candidate.
	// if no more candidate is recommended, save (-1, -1).
	// Check every area of map, do it, and check priority is increase or decrease.

	// Decied where to put stone from candidate location list 'candidate'.
	// ms, es, candidate, state_priority value with get_state_priority.
	// 어디에 놓는 것이 내 state priority 를 최대화 시키면서, 상대 state priority 를 최소화 시키는지 확인 해야 한다.
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
					if (highest_mine < temp_mine)	{
						highest_mine = temp_mine;
					}// find highest value of my priority

					location_copy(&to, &candidate[candidate_number]);
					mine_priority[candidate_number] = temp_mine;
					enemy_priority[candidate_number] = temp_enemy;
					// Save it to existing array.
					candidate_number++;
				}

				if (now_enemy < temp_enemy) {
					if (highest_enemy < temp_enemy)	{
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
	if (highest_mine - now_mine >= highest_enemy - now_enemy) 	{
		// if attack...
		for (i = 0; i < candidate_number; i++)	{
			if (mine_priority[i] == highest_mine)	{
				// find highest my candidate or candidates
				location_copy(&candidate[i], &candidate_real[candidate_number_real]);
				candidate_number_real++;
			}
		}
	}
	else {
		// if defense...
		for (i = 0; i < candidate_number; i++)	{
			if (enemy_priority[i] == highest_enemy)	{
				// find highest enemy's candidate or candidates
				location_copy(&candidate[i], &candidate_real[candidate_number_real]);
				candidate_number_real++;
			}
		}
	}

	return candidate_real[real_rand(0, candidate_number_real)];
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

location ai_turn(int map[][MAP_LENGTH], int priority[], location last_put, int remain_turn, int mine)	{
	// real ai part.
	// AI do his job with given map.
	// win when can win, defense when can lose.
	// else, attack or defense with given candidate.
	int ms[DIR_MAX][MAP_LENGTH][MAP_LENGTH], es[DIR_MAX][MAP_LENGTH][MAP_LENGTH];
	int aw_number, mw_number, i, j;
	vector m_aw_where[MAP_LENGTH * MAP_LENGTH], m_mw_where[MAP_LENGTH * MAP_LENGTH];
	vector e_aw_where[MAP_LENGTH * MAP_LENGTH], e_mw_where[MAP_LENGTH * MAP_LENGTH];
	location m_mw_dir[MAP_LENGTH * MAP_LENGTH], e_mw_dir[MAP_LENGTH * MAP_LENGTH];
	location return_value;

	search_state(ms, es, map, mine);

	aw_number = aw_location(ms, m_aw_where);
	mw_number = mw_location(ms, m_mw_where, m_mw_dir);

	if (mw_number > 0)	{

	}
	else if (aw_number > 0)	{
		if (remain_turn == 1)	{
			for (i = 0; i < aw_number; i++)	{
				if (ms[m_aw_where[i].dir][m_aw_where[i].x][m_aw_where[i].y] >= (SIGMA0 * _STATE_MAX + _0) && ms[m_aw_where[i].dir][m_aw_where[i].x][m_aw_where[i].y] < (THETA0 * _STATE_MAX + _0))	{
					// if state is at SIGMA and remain turn is 1...
					return_value.x = -1;
					return_value.y = -1;
				}
			}
		}
	}

	aw_number = aw_location(es, e_aw_where);
	mw_number = mw_location(es, e_mw_where, e_mw_dir);


	return return_value;
}

int ai_game(int map[][MAP_LENGTH], int p0[], int p1[])	{
	// ai's game with p0, p1 proiority.
	// return 0 if p0 win, return 1 if p1 win, return -1 if draw.
	location last_put;
	int win;
	int remain_turn = 2;
	int turn = BLACK;

	last_put.x = -1;
	last_put.y = -1;

	last_put = ai_turn(map, p0, last_put, 1, turn);
	// first turn.

	while (1)	{
		win = is_draw(map);
		if (win == 1)	{
			return -1;
		}
		else {
			win = who_win(map, last_put);
			// check if win.

			if (turn == BLACK)	{
				if (win == -1)	{
					// if not win, do ai.
					last_put = ai_turn(map, p1, last_put, remain_turn, BLACK);
				}
				else if (win == BLACK)	{
					// if win, return.
					return 0;
				}
				else if (win == WHITE)	{
					// if win, return.
					return 1;
				}

				remain_turn--;

				if (remain_turn == 0)	{
					// if not remain_turn is 0...
					remain_turn = 2;
					turn = WHITE;
				}
			}
			else {
				if (win == -1)	{
					// if not win, do ai.
					last_put = ai_turn(map, p0, last_put, remain_turn, WHITE);
				}
				else if (win == WHITE)	{
					// if win, return.
					return 0;
				}
				else if (win == BLACK)	{
					// if win, return.
					return 1;
				}

				remain_turn--;

				if (remain_turn == 0)	{
					// if not remain_turn is 0...
					remain_turn = 2;
					turn = BLACK;
				}
			}
		}
	}
}

int can_win_immidiate()	{

}

location mw_doing()	{

}

location aw_doing(int state, vector start_location, int remain_turn)	{
	// at remain_turn is 2, SIGMA can win and at remain_turn is 1, THETA can win.
	// return where to put to win at that time.
	location return_value;

	if (remain_turn == 2)	{
		if (state >= (SIGMA0 * _STATE_MAX + _0) && state < (THETA0 * _STATE_MAX + _0))	{
			// if state is at SIGMA and remain turn is 2...
			/*
			θ	5개의 내 돌 ( ○ ) 존재 = { θ0, θ1, θ2 ... θ5 }
			θ0	○○○○○◎	θ1	○○○○◎○	θ2	○○○◎○○	θ3	○○◎○○○	θ4	○◎○○○○
			θ5	◎○○○○○
			*/
		}
	}
	else {
		if (state < (SIGMA0 * _STATE_MAX + _0) || state >= (THETA0 * _STATE_MAX + _0)) {
			// if state is at THETA and remain turn is 1...
			if (state == THETA0 * _STATE_MAX + _0 || state == THETA0 * _STATE_MAX + _2 || state == THETA0 * _STATE_MAX + _6 || state == THETA0 * _STATE_MAX + _8)	{
				if (start_location.dir == SOUTH)	{ // increase ( 0, 1 )

				}
				else if (start_location.dir == SOUTH_EAST)	{ // increase ( 1, 1 )

				}
				else if (start_location.dir == EAST)	{ // increase ( 1 , 0 )

				}
				else {	// increase ( 1, -1 )

				}
			}
			else if (state == THETA1 * _STATE_MAX + _0 || state == THETA1 * _STATE_MAX + _2 || state == THETA1 * _STATE_MAX + _6 || state == THETA1 * _STATE_MAX + _8)	{

			}
			else if (state == THETA2 * _STATE_MAX + _0 || state == THETA2 * _STATE_MAX + _2 || state == THETA2 * _STATE_MAX + _6 || state == THETA2 * _STATE_MAX + _8)	{

			}
			else if (state == THETA3 * _STATE_MAX + _0 || state == THETA3 * _STATE_MAX + _2 || state == THETA3 * _STATE_MAX + _6 || state == THETA3 * _STATE_MAX + _8)	{

			}
			else if (state == THETA4 * _STATE_MAX + _0 || state == THETA4 * _STATE_MAX + _2 || state == THETA4 * _STATE_MAX + _6 || state == THETA4 * _STATE_MAX + _8)	{

			}
			else if (state == THETA5 * _STATE_MAX + _0 || state == THETA5 * _STATE_MAX + _2 || state == THETA5 * _STATE_MAX + _6 || state == THETA5 * _STATE_MAX + _8)	{

			}
		}
	}

	return return_value;
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

void select_parent(int parent[][STATE_LENGTH], int selected[][STATE_LENGTH], int fitness[]) {
	// Select PARAENT_MAX parents from parent[GENERATION_MAX][STATE_LENGTH] with fitness[GENERATION_MAX] values.
	// We will use rullet-wheel method to select parents.
	int i, j, temp;
	int index[GENERATION_MAX];
	int temp_fit[GENERATION_MAX];
	for (i = 0; i < GENERATION_MAX; i++) {
		index[i] = i;
	}

	for (i = 0; i < GENERATION_MAX; i++)	{
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

	for (i = 0; i < GENERATION_MAX; i++)	{
		while (play < GAME_MAX)	{
			win = ai_game(map, &generation[i], given_set);
			// game processed generation[i] first.

			if (win == 0)	{
				//generation[i]'s win.
				win_number++;
				play++;
			}

			initialize_map(map);

			win = ai_game(map, given_set, &generation[i]);
			// game processed given_set first.

			if (win == 0)	{
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

	for (i = 0; i < GENERATION_MAX; i++)	{
		for (j = 0; j < STATE_LENGTH; j++)	{
			if (real_rand(0, FITNESS_MAX) < fitness[i])	{
				generation[i][j] = real_rand(0, PRIORITY_MAX);
			}
		}
	}
}

int real_rand(int from, int to)	{
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