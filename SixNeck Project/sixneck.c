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
#define CANDIDATE_MAX 20
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

/*	mw = must win, aw = amado win 을 의미한다.
mw set : aw set 이 연속해서 3개 붙어 있을 때.
aw set : { δ, θ set } X { _0, _2, _6, _8 }
*/


typedef struct {
	short x;
	short y;
}location;

typedef struct {
	short x;
	short y;
	short dir;	// will be value like SOUTH
}vector;

void search_state(int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], int map[][MAP_LENGTH], int mine);
int get_state(int map[][MAP_LENGTH], int mine, location dir);
int mw_location(int state[][MAP_LENGTH][MAP_LENGTH], vector where[], location dri[]);
int check_mw(int state[][MAP_LENGTH][MAP_LENGTH], vector start, location dir);
int aw_location(int state[][MAP_LENGTH][MAP_LENGTH], vector where[]);
int check_aw(int state[][MAP_LENGTH][MAP_LENGTH], vector start);
unsigned long long int get_state_priority(int state[][MAP_LENGTH][MAP_LENGTH], int priority[]);
int can_win(int state[][MAP_LENGTH][MAP_LENGTH], vector mw[], location mw_dir[], vector aw[], int mine);
int who_win(int map[][MAP_LENGTH], location where_put);
int dir_row_win(int map[][MAP_LENGTH], location where_put, location dir, int mine);
int dir_win(int map[][MAP_LENGTH], location where_put, location dir, int mine);
void find_candidate_location(int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], location candidate[], int priority[], int mine);
location decide_location(int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], location candidate[], int priority[], int mine);
void add_stone(location where, int map[][MAP_LENGTH], int mine);
void initialize_map(int map[][MAP_LENGTH]);
void array1_initailizer(int arr[], int length, int toInitialize);
void array2_initailizer(int arr[], int length1, int length2, int toInitialize);
void array3_initailizer(int arr[], int length1, int length2, int length3, int toInitialize);
void location_copy(location *src, location *dst);
void vector_copy(vector *src, vector *dst);


int main() {
	int priority[STATE_LENGTH];
	int map[MAP_LENGTH][MAP_LENGTH];



	return 0;
}


void search_state(int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], int map[][MAP_LENGTH], int mine) {
	/*	Check my state(ms), enemy sate(es) from map and save to there.
		'mine' will be BLACK or WHITE, which is my color.
		State will save like ALPHA0 * _0 */



}

int get_state(int map[][MAP_LENGTH], int mine, location dir) {

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
					if (d == SOUTH) {
						dir.x = 1;
						dir.y = 0;
					}
					else if (d = SOUTH_EAST) {
						dir.x = 1;
						dir.y = 1;
					}
					else if (d = EAST) {
						dir.x = 0;
						dir.y = 1;
					}
					else if (d = NORTH_EAST) {
						dir.x = -1;
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

unsigned long long int get_state_priority(int state[][MAP_LENGTH][MAP_LENGTH], int priority[]) {
	// Calculate 'state' priority with given array priority.
	// state priority will be very large, so we return it data type of unsigned long long int.
	// Priority is always plus value, so we will use unsigned value.



}

int can_win(int state[][MAP_LENGTH][MAP_LENGTH], vector mw[], location mw_dir[], vector aw[],int mine) {
	// mine's turn, if ms has aw set or mw set, then return 1, else return 0;
	// if mine == real my value, then can win. if mine == real enemy value, then can lose.
	/* mw + mw ( 겹쳐도 상관 없다. ) 
	mw + aw ( 겹치는 부분이 빈 칸이 X ) 
	aw + aw + aw ( 겹치는 부분이 빈 칸이 X ) 로 판단.*/
	int i, j, k;
	int return_value;

	for (k = 0; k < DIR_MAX; k++) {
		for (i = 0; i < MAP_LENGTH; i++) {
			for (j = 0; j < MAP_LENGTH; j++) {
				
			}
		}
	}
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

	// At direction to SOUTH.
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

void find_candidate_location(int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], location candidate[], int priority[], int mine) {
	// Find candidate location and save it to candidate.
	// 후보 위치군은 다음에 어디에 두면 좋을까? 같은 것에 대한 후보 위치군이다.

}

location decide_location(int ms[][MAP_LENGTH][MAP_LENGTH], int es[][MAP_LENGTH][MAP_LENGTH], location candidate[], int priority[], int mine) {
	// Decied where to put stone from candidate location list 'candidate'.
	// ms, es, candidate, state_priority value with get_state_priority.
	// 어디에 놓는 것이 내 state priority 를 최대화 시키면서, 상대 state priority 를 최소화 시키는지 확인 해야 한다.


}

void add_stone(location where, int map[][MAP_LENGTH], int mine) {
	// Add stone to 'where' which stone is 'mine', BLACK, WHITE.
	map[where.x][where.y] = mine;
}

void initialize_map(int map[][MAP_LENGTH]) {
	// Initialize map to EMPTY which is defined.
	int i, j;

	array2_initailizer(map, MAP_LENGTH, MAP_LENGTH, EMPTY);
	// Using below function.
}

void array1_initailizer(int arr[], int length, int toInitialize) {
	// Initialize one dimension array with length 'length' to toInitialize.
	int i;

	for (i = 0; i < length; i++) {
		arr[i] = toInitialize;
	}// arr[i] = toInitialize for every i in range 0 ~ max.
}

void array2_initailizer(int arr[], int length1, int length2, int toInitialize) {
	// Initialize two dimension array with length 'length1' and 'length2' to toInitialize.
	int i, j;

	for (i = 0; i < length1; i++) {
		for (j = 0; j < length2; j++) {
			arr[i * length1 + j] = toInitialize;
		}// arr[i][j] = toInitialize for every i and j in range 0 ~ max.
	}

}

void array3_initailizer(int arr[], int length1, int length2, int length3, int toInitialize) {
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