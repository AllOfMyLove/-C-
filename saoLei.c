
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// echo
#define LEI_ON (char)'x'
#define LEI_BIAOJI (char)'*'
#define UNKNOWN (char)'#'

// lei
#define LEI 1
#define NOLEI 0

//states
#define OPEN 1
#define CLOSE 0

//difficulty
#define EASY 1
#define MEDIUM 2
#define HARD 3

//gameOver()
#define SUCCESS 1
#define FAILED 0



typedef struct{
    int echo; //显示图像，*标记雷，0代表开出的空，x代表炸的雷，
    int lei; //是否是雷
    int states; //状态，开or没开
    int leishu; // 周围的雷数
}Node;

static int shengyu;

/* 函数声明区*/

Node **create(int difficulty);
void chuShiHua(Node **p, int difficulty);

void openNode(Node **p, int x, int y, int difficulty);
int openSurround(Node **p, int x, int y, int difficulty);
int edgeSearch(Node **p, int i, int j, int difficulty);
int checkOver(Node **p, int difficulty);
void checkAll(Node **p, int difficulty);

void printGra_Test(Node **p, int difficulty);
void printGra(Node **p, int difficulty);
int hangJudge(int difficulty);
int lieJudge(int difficulty);
int leishuJudge(int difficulty);
int leiGaiLv(int difficulty);

void gameOver(Node **p, int difficulty, int states);


/* 主函数*/

// 测试函数
void test01(){
    printf("%c\n", UNKNOWN);
}

// 测试主函数
void mainTest(){

    int difficulty;
    printf("Input the difficulty : ");
    scanf("%d", &difficulty);

    Node **p = create(difficulty);

    chuShiHua(p, difficulty);

    printGra(p, difficulty);
    printGra_Test(p, difficulty);

    printf("-------------------\n");

    int x, y;

    while(checkOver(p, difficulty)){
        printf("\nInput the x, y :");
        scanf("%d%d", &x, &y);

        openNode(p, x, y, difficulty);

        printGra(p, difficulty);
        printGra_Test(p, difficulty);
    }

    gameOver(p, difficulty, SUCCESS);
    

}

// 主函数
int main(){

    //test01();
    mainTest();


    system("pause");
    return 0;
}



// 执行函数

Node **create(int difficulty){
    Node **p;
    int hang, lie;

    if(difficulty <= 3 && difficulty >= 1){
        hang = hangJudge(difficulty);
        lie = lieJudge(difficulty);
    }
    else{
        printf("\nDifficulty input error, exit the program. \n");
        exit(1);
    }

    p = (Node **)malloc(sizeof(Node)*hang*lie);
    for(int hangMove = 0; hangMove < hang; hangMove ++){
        p[hangMove] = (Node *)malloc(sizeof(Node)*lie);
    }
    if(p == NULL){
        printf("\nMemory is not enough. \n");
        gameOver(p, difficulty, FAILED);
    }

    return p;
}


void chuShiHua(Node **p, int difficulty){

    int total_lei = 0;

    int hang = hangJudge(difficulty);
    int lie = lieJudge(difficulty);
    int leishu = leishuJudge(difficulty);

    shengyu = hang * lie - leishu;

    // 雷数初始化为0
    for(int i = 0; i < hang; i ++){
        for(int j = 0; j < lie; j ++){
            p[i][j].lei = NOLEI;
            p[i][j].leishu = 0;
        }
    }
    // 随机放雷
    putLei:
    for(int i = 0; i < hang; i ++){
        for(int j = 0; j < lie; j ++){

            if(p[i][j].lei == LEI){
                continue;
            }

            p[i][j].lei = leiGaiLv(difficulty);
            p[i][j].states = CLOSE;
            p[i][j].echo = UNKNOWN;

            if(p[i][j].lei == LEI){
                for(int a = (i-1); a <= (i+1); a ++){
                    for(int b = (j-1); b <= (j+1); b ++){
                        if(a >= 0 && a < hang && b >= 0 && b < lie){
                            p[a][b].leishu ++;
                        }
                    }
                }
                total_lei ++;
            }

            if(total_lei == leishu){
                break;
            }

        }

        if(total_lei == leishu){
            break;
        }
    }

    if(total_lei < leishu){
        goto putLei;
    }

}


void openNode(Node **p, int x, int y, int difficulty){

    int hang = hangJudge(difficulty);
    int lie = lieJudge(difficulty);
    int leishu = leishuJudge(difficulty);

    p[x][y].states = OPEN;
    shengyu --;
    if(p[x][y].lei == LEI){
        p[x][y].echo = LEI_ON;
        gameOver(p, difficulty, FAILED);
    }
    else if(p[x][y].lei == NOLEI){
        p[x][y].echo = p[x][y].leishu;
    }

    while(openSurround(p, x, y, difficulty)){}

}


int cont = 0;
int openSurround(Node **p, int x, int y, int difficulty){

    cont = 0;

    int hang = hangJudge(difficulty);
    int lie = lieJudge(difficulty);

    // 左上
    for(int i = x; i >= 0; i --){
        for(int j = y; j >= 0; j --){
            if(edgeSearch(p, i, j, difficulty) && p[i][j].lei == NOLEI && p[i][j].states == CLOSE){
                p[i][j].states = OPEN;
                p[i][j].echo = p[i][j].leishu;
                cont = 1;
            }
        }
    }
    // 左下
    for(int i = x; i < hang; i ++){
        for(int j = y; j >= 0; j --){
            if(edgeSearch(p, i, j, difficulty) && p[i][j].lei == NOLEI && p[i][j].states == CLOSE){
                p[i][j].states = OPEN;
                p[i][j].echo = p[i][j].leishu;
                cont = 1;
            }
        }
    }
    // 右上
    for(int i = x; i >= 0; i --){
        for(int j = y; j < lie; j ++){
            if(edgeSearch(p, i, j, difficulty) && p[i][j].lei == NOLEI && p[i][j].states == CLOSE){
                p[i][j].states = OPEN;
                p[i][j].echo = p[i][j].leishu;
                cont = 1;
            }
        }
    }
    // 右下
    for(int i = x; i < hang; i ++){
        for(int j = y; j < lie; j ++){
            if(edgeSearch(p, i, j, difficulty) && p[i][j].lei == NOLEI && p[i][j].states == CLOSE){
                p[i][j].states = OPEN;
                p[i][j].echo = p[i][j].leishu;
                cont = 1;
            }
        }
    }

    return cont;

}


int edgeSearch(Node **p, int i, int j, int difficulty){
    int hang = hangJudge(difficulty);
    int lie = lieJudge(difficulty);

    for(int a = i-1; a <= i+1; a ++){
        for(int b = j-1; b <= j+1; b ++){
            if(a >= 0 && a < hang && b >= 0 && b < lie && p[a][b].states == OPEN && p[a][b].leishu == 0){
                return 1;
            }
        }
    }

    return 0;
}


#define CONTINUE 1
#define END 0
int checkOver(Node **p, int difficulty){

    int hang = hangJudge(difficulty);
    int lie = lieJudge(difficulty);

    for(int i = 0; i < hang; i ++){
        for(int j = 0; j < lie; j ++){
            if(p[i][j].states == CLOSE && p[i][j].lei == NOLEI){
                return CONTINUE;
            }
        }
    }

    return END;
}



// 工具函数

int hangJudge(int difficulty){
    if(difficulty == EASY){
        return 9;
    }
    else if(difficulty == MEDIUM){
        return 16;
    }
    else if(difficulty == HARD){
        return 30;
    }
}
int lieJudge(int difficulty){
    if(difficulty == EASY){
        return 9;
    }
    else if(difficulty == MEDIUM){
        return 16;
    }
    else if(difficulty == HARD){
        return 16;
    }
}
int leishuJudge(int difficulty){
    if(difficulty == EASY){
        return 10;
    }
    else if(difficulty == MEDIUM){
        return 40;
    }
    else if(difficulty == HARD){
        return 99;
    }
}
int leiGaiLv(int difficulty){
    static int seed = 0;
    srand((unsigned)time(NULL) + seed*seed);
    seed ++;
    int ran = -1;
    if(difficulty == EASY){
        ran = rand()%9;
    }
    else if(difficulty == MEDIUM){
        ran = rand()%7;
    }
    else if(difficulty == HARD){
        ran = rand()%5;
    }

    if(ran == 0){
        return LEI;
    }
    else{
        return NOLEI;
    }
}


void printGra_Test(Node **p, int difficulty){
    printf("\n");
    int hang = hangJudge(difficulty);
    int lie = lieJudge(difficulty);
    for(int i = 0; i < hang; i ++){
        for(int j = 0; j < lie; j ++){
            printf("%3d", p[i][j].leishu);
            if(p[i][j].lei == LEI){
                printf("*");
            }
            else if(p[i][j].lei == NOLEI){
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
}


void printGra(Node **p, int difficulty){
    printf("\n");
    int hang = hangJudge(difficulty);
    int lie = lieJudge(difficulty);
    for(int i = 0; i < hang; i ++){
        for(int j = 0; j < lie; j ++){
            if(p[i][j].states == OPEN && p[i][j].lei == NOLEI){
                printf("%2d", p[i][j].echo);
            }
            else{
                printf("%2c", p[i][j].echo);
            }
        }
        printf("\n");
    }
    printf("\n");
}


void gameOver(Node **p, int difficulty, int states){

    int hang = hangJudge(difficulty);
    int lie = lieJudge(difficulty);

    if(states == SUCCESS){
        printf("\nCongratulations ! \n");
    }
    else{
        printf("\nYou are failed . \n");
    }

    for(int i = 0; i < hang; i ++){
        for(int j = 0; j < lie; j ++){
            if(p[i][j].lei == LEI){
                p[i][j].echo = LEI_ON;
            }
        }
    }

    printGra(p, difficulty);

    system("pause");

    exit(0);

}
