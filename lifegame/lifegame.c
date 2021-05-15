#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DEBUG 0

#define cls printf("\033[2J")
#define FIELD_X 30
#define FIELD_Y 15

void print_array(int array[FIELD_Y][FIELD_X]);
void set_array(int array[FIELD_Y][FIELD_X], FILE *input_field);
void chk_array(int arrayA[FIELD_Y][FIELD_X], int arrayB[FIELD_Y][FIELD_X]);
void init_array(int array[FIELD_Y][FIELD_X]);

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

    input_field = fopen("lifegame_field.csv", "r");
    if(input_field == NULL){
        printf("ファイルが読み込めません。\n");
        exit(1);
    }

    cls; // 一時画面消去

    set_array(arrayA, input_field);
    print_array(arrayA);

    while(1){
        cls;

        if(mode == 0){
            init_array(arrayB);
            chk_array(arrayA, arrayB);
            print_array(arrayB);
        }else if(mode == 1){
            init_array(arrayA);
            chk_array(arrayB, arrayA);
            print_array(arrayA);
        }
        usleep(250000);
        mode ^= 1;
        game_turn++;
    }

    fclose(input_field);
    return 0;
}



void print_array(int array[FIELD_Y][FIELD_X]){
    for(int y = 0; y < FIELD_Y; y++){
        for(int x = 0; x < FIELD_X; x++){
            if(array[y][x] == 0) printf("%s ", "□");
            else if(array[y][x] == 1) printf("%s ", "■");
            else printf("%s ", "?");
        }
        printf("\n");
    }
    printf("\n");
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
            int sum = 0; // 八近傍の合計値
            for(int i = 0; i < 8; i++){
                if(x == 0 && (i == 5 || i == 6 || i == 7)) continue;
                if(x == FIELD_X-1 && (i == 1 || i == 2 || i == 3)) continue;
                if(y == 0 && (i == 0 || i == 1 || i == 7)) continue;
                if(y == FIELD_Y-1 && (i == 3 || i == 4|| i == 5)) continue;

                sum += arrayA[y+dy[i]][x+dx[i]];
            }

            #if DEBUG == 1
            printf("%d ", sum);
            #endif

            if(arrayA[y][x] == 0 && sum == 3) arrayB[y][x] = 1;
            else if(arrayA[y][x] == 1 && (sum == 2 || sum == 3)) arrayB[y][x] = 1;
            else arrayB[y][x] == 0;
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