#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// 1 : デバッグモード
#define DEBUG 0

#define cls printf("\033[H\033[2J")
#define set printf("\033[s")
#define put printf("\033[u")

#define FIELD_X 30
#define FIELD_Y 15
#define MAGNIFICATION 0.4

void print_array(int array[FIELD_Y][FIELD_X]);
void set_array(int array[FIELD_Y][FIELD_X], FILE *input_field);
void chk_array(int arrayA[FIELD_Y][FIELD_X], int arrayB[FIELD_Y][FIELD_X]);
void init_array(int array[FIELD_Y][FIELD_X]);
int win_lose(int array[FIELD_Y][FIELD_X]);
int sell_extension(int sum);
void print_rate(int array[FIELD_Y][FIELD_X]);

/*//===================================================================

■■■     ■■■  ■■■■■■■ ■■■■■■■    ■■■■■■     ■     ■■      ■■  ■■■■■■■
 ■       ■    ■    ■  ■    ■   ■     ■     ■■     ■■     ■■   ■    ■
 ■       ■    ■       ■       ■             ■     ■■    ■ ■   ■     
 ■       ■    ■  ■    ■  ■    ■           ■ ■     ■ ■     ■   ■  ■  
 ■       ■    ■■■■    ■■■■    ■              ■    ■ ■  ■  ■   ■■■■  
 ■       ■    ■  ■    ■  ■    ■     ■■   ■■■■■    ■ ■  ■  ■   ■  ■  
 ■    ■  ■    ■       ■       ■■     ■   ■    ■   ■  ■    ■   ■     
 ■    ■  ■    ■       ■    ■   ■■   ■■  ■     ■   ■  ■■   ■   ■    ■
■■■■■■■ ■■■  ■■■     ■■■■■■■    ■■■■ ■ ■■     ■■ ■■   ■  ■■  ■■■■■■■

*///===================================================================


int main(){
    FILE *input_field;
    int arrayA[FIELD_Y][FIELD_X] = {};
    int arrayB[FIELD_Y][FIELD_X] = {};
    int game_turn = 0;
    int mode = 0;

    input_field = fopen("lifeterritory_field.csv", "r");
    if(input_field == NULL){
        printf("ファイルが読み込めません。\n");
        exit(1);
    }

    cls; // 一時画面消去
    set;

    set_array(arrayA, input_field);
    print_array(arrayA);

    srand((unsigned)time(NULL));
    while(1){
        int end = 0;
        
        #if DEBUG == 0
        put;
        #endif

        if(mode == 0){
            init_array(arrayB);
            chk_array(arrayA, arrayB);
            print_array(arrayB);
            end = win_lose(arrayB);
            print_rate(arrayB);
        }else if(mode == 1){
            init_array(arrayA);
            chk_array(arrayB, arrayA);
            print_array(arrayA);
            end = win_lose(arrayA);
            print_rate(arrayA);
        }
        
        if(end == 1){
            printf("Blue win!\n");
            break;
        }else if(end == 2){
            printf("Red win!\n");
            break;
        }

        usleep(100000);
        mode ^= 1;
        game_turn++;
    }

    fclose(input_field);
    return 0;
}

//----------------------------------------------------------------------------------

void print_array(int array[FIELD_Y][FIELD_X]){
    for(int y = 0; y < FIELD_Y; y++){
        for(int x = 0; x < FIELD_X; x++){
            if(array[y][x] == 0){
                printf("%s ", "□");
            }else if(array[y][x] == 1){
                printf("\033[34m%s\033[37m ", "■");
            }else if(array[y][x] == 2){
                printf("\033[31m%s\033[37m ", "■");
            }else if(array[y][x] == 3){
                printf("\033[33m%s\033[37m ", "■");
            }
            else printf("%s ", "?");
        }
        printf("\n");
    }
    printf("\n\n");
}

void set_array(int array[FIELD_Y][FIELD_X], FILE *input_field){
    for(int y = 0; y < FIELD_Y; y++){
        for(int x = 0; x < FIELD_X; x++){
            fscanf(input_field, "%d,", &array[y][x]);
        }
    }
}

void chk_array(int arrayA[FIELD_Y][FIELD_X], int arrayB[FIELD_Y][FIELD_X]){
    // 八近傍   上 右上 右 右下 下 左下 左 左上
    int dx[8] = {0,   1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {-1, -1, 0, 1, 1,  1,  0, -1};

    for(int y = 0; y < FIELD_Y; y++){
        for(int x = 0; x < FIELD_X; x++){
            int sum[2] = {}; // 八近傍の合計値(2チーム)

            arrayB[y][x] = arrayA[y][x];

            for(int i = 0; i < 8; i++){
                if(x == 0 && (i == 5 || i == 6 || i == 7)) continue;
                if(x == FIELD_X-1 && (i == 1 || i == 2 || i == 3)) continue;
                if(y == 0 && (i == 0 || i == 1 || i == 7)) continue;
                if(y == FIELD_Y-1 && (i == 3 || i == 4|| i == 5)) continue;
                
                if(arrayA[y+dy[i]][x+dx[i]] == 1){
                    sum[0]++;
                }else if(arrayA[y+dy[i]][x+dx[i]] == 2){
                    sum[1]++;
                }
            }

            if(arrayA[y][x] == 3) continue;

            if(sell_extension(sum[1])){
                arrayB[y][x] = 2;
            }
            if(sell_extension(sum[0])){
                arrayB[y][x] = 1;
            }
            

            #if DEBUG == 1
            printf("%d ", arrayB[y][x]);
            #endif
        }
        #if DEBUG == 1
        printf("\n");
        #endif
    }
}

void init_array(int array[FIELD_Y][FIELD_X]){
    for(int i = 0; i < FIELD_Y; i++){
        for(int j = 0; j < FIELD_X; j++){
            array[i][j] = 0;
        }
    }
}

int win_lose(int array[FIELD_Y][FIELD_X]){
    int sumA = 0, sumB = 0;
    int sum_other = 0;

    for(int i = 0; i < FIELD_Y; i++){
        for(int j = 0; j <FIELD_X; j++){
            if(array[i][j] == 1) sumA++;
            else if(array[i][j] == 2) sumB++;
            else sum_other++;
        }
    }

    if(sumA == FIELD_X * FIELD_Y - sum_other){
        return 1;
    }else if(sumB == FIELD_X * FIELD_Y - sum_other){
        return 2;
    }else{
        return 0;
    }
}

int sell_extension(int sum){
    int per = (int)((double)(sum / 8.0 * MAGNIFICATION) * 100); // 8近傍の生きてるマスの比率
    int rand_num = rand() % 101; // 0 ~ 100までランダム
    if(per > rand_num){
        return 1;
    }
    return 0;
}

void print_rate(int array[FIELD_Y][FIELD_X]){
    int sumA = 0, sumB = 0, sum_other = 0;
    int perA = 0, perB = 0, perN = 0;

    for(int i = 0; i < FIELD_Y; i++){
        for(int j = 0; j < FIELD_X; j++){
            if(array[i][j] == 1) sumA++;
            else if(array[i][j] == 2) sumB++;
            else sum_other++;
        }
    }

    perA = (int)((double)sumA/(FIELD_X*FIELD_Y) * 100);
    perB = (int)((double)sumB/(FIELD_X*FIELD_Y) * 100);

    printf("Blue : %d %%\n", perA);
    printf("Red  : %d %%\n", perB);

}