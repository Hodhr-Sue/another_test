#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE    8
#define GRID_BLACK  1
#define GRID_WHITE -1
#define GRID_ERROR  0
#define GRID_ARROW  2
#define GRID_EMPTY  0
#define MODE_PVE    11
#define MODE_PVP    12

/*
* @brief 存储对局基本信息的结构体
*/

typedef struct {
    int game_Mode;     // 游戏模式
    int User_Grid;     // 玩家选择的颜色
    int current_Grid;  // 当前该谁走
    int Grid_Board[MAX_SIZE][MAX_SIZE];
    int turn_Counter;
} game_state_t;

/*
* @brief 存储每一步走法的结构体
*/

typedef struct {
    int stratX, startY;
    int endX, endY;
    int arrowX, arrowY;     //箭的位置
} movements_t;


/**********************************函数声明************************************/

void case4_help_handler(void);
void Is_choose_offensive(void);
int Is_Mode_input_effective(int input);
void Grid_Board_Init(void);
void case1_PVE_handler(void);
void case2_PVP_handler(void);

/*****************************************************************************/


/**********************************全局变量************************************/

int g_choose_Mode = 0;
game_state_t g_game_State;
movements_t g_movements1, g_movements2;

/****************************************************************************/


/*
* @brief 看看有没有走出棋盘
* @param x,y 坐标
*/

_Bool is_In_Board(int x, int y){
    return (x >= 0 && x < MAX_SIZE && y >= 0 && y < MAX_SIZE);
}

/*
* @brief 单人模式初始化函数,包括棋盘初始化,玩家选择颜色和先手的函数调用
*/

void PVE_Init(void){
    memset(g_game_State.Grid_Board, 0, sizeof (g_game_State.Grid_Board));
    g_game_State.User_Grid = choose_User_color();
    Is_choose_offensive();  // 选择先手,也就是current_Grid的初始值
    
    Grid_Board_Init();  // 棋子初始化

    g_game_State.turn_Counter = 0;  // 置0回合数
}

/*
* @brief 封装初始化棋盘上棋子位置的函数
*/

void Grid_Board_Init(void){
    g_game_State.Grid_Board[0][2] = GRID_BLACK;
    g_game_State.Grid_Board[2][0] = GRID_BLACK;
    g_game_State.Grid_Board[5][0] = GRID_BLACK;
    g_game_State.Grid_Board[7][2] = GRID_BLACK;
    g_game_State.Grid_Board[0][5] = GRID_WHITE;
    g_game_State.Grid_Board[2][7] = GRID_WHITE;
    g_game_State.Grid_Board[5][7] = GRID_WHITE;
    g_game_State.Grid_Board[7][5] = GRID_WHITE;
}

/*
* @brief 双人模式初始化函数
*/

void PVP_Init(){
    memset(g_game_State.Grid_Board, 0, sizeof (g_game_State.Grid_Board));
    g_game_State.current_Grid = GRID_BLACK;

    Grid_Board_Init();  // 初始化棋子位置

    g_game_State.turn_Counter = 0;  //置0回合数
}

/*
* @brief 玩家选颜色的函数
* @param 返回对应宏定义颜色
*/

int choose_User_color(void){
    int choose_state = 0;
    char User_choice = '0';
    while(choose_state == 0) {
        char hit_info[] = "输入B/W来选择颜色,B是黑色,W是白色";
        puts(hit_info);
        User_choice = getchar();
        if (User_choice == 'b' || User_choice == 'B') {
            choose_state = GRID_BLACK;
            char succ_info[] = "选择了黑色";
            puts(succ_info);
            return GRID_BLACK;
        } else if (User_choice == 'W' || User_choice == 'w') {
            choose_state = GRID_WHITE;
            char succ_info[] = "选择了白色";
            puts(succ_info);
            return GRID_WHITE;
        } else {
            char err_info[] = "输入无效,请输入B或者W";
            puts(err_info);
            choose_state = GRID_ERROR;
        }
    }
}

/*
* @brief 玩家选择是否先手的函数
*/

void Is_choose_offensive(void){
    char hint_info1[] = "你想要先手吗\n";
    char hint_info2[] = "Y表示要,N表示不要\n";
    puts(hint_info1);
    puts(hint_info2);
    char buffer[10] = "0";
    char err_info[] = "输入无效\n";
    int pass_flag = 0;  //输入有效标志
    while(pass_flag == 0){
        fgets(buffer, 9, stdin);
        if (strlen (buffer) == 2) {
            if (buffer[0] == 'y' || buffer[0] == 'Y') {
                g_game_State.current_Grid = g_game_State.User_Grid;
                pass_flag = 1;
            } else if (buffer[0] == 'n' || buffer[0] == 'N') {
                g_game_State.current_Grid = -1 * g_game_State.User_Grid;
                pass_flag = 1;
            } else {
                pass_flag = 0;
                puts(err_info);
            }
        } else {
            pass_flag = 0;
            puts(err_info);
        }
    }
}

/*
* @brief 用来封装第一步显示菜单的函数
*/

void display_Menu(void){
    printf ("=====================================\n");
    printf ("          亚马逊棋 (Amazons)          \n");
    printf ("=====================================\n");
    printf ("1. 新游戏 - 人机对战\n");
    printf ("2. 新游戏 - 双人对战\n");
    printf ("3. 加载游戏\n");
    printf ("4. 游戏说明\n");
    printf ("5. 退出游戏\n");
    printf ("=====================================\n");
    printf ("请选择 (1-5): ");
}

/*
* @brief 用来封装显示棋盘的函数,根据棋盘数组的值显示不同的符号
*/

void display_Board(void){
    // 打印顶部列号（两个空格前缀）
    printf("  ");
    for (int i = 0; i < MAX_SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");

    // 打印顶部边框
    printf("  +");
    for (int i = 0; i < MAX_SIZE; i++) {
        printf("--");
    }
    printf("+\n");

    // 打印棋盘内容
    for (int i = 0; i < MAX_SIZE; i++) {
        // 左侧行号 + 竖线
        printf("%d |", i);
        for (int j = 0; j < MAX_SIZE; j++) {
            int cell = g_game_State.Grid_Board[i][j];
            if (cell == GRID_BLACK) {
                printf("● ");
            } else if (cell == GRID_WHITE) {
                printf("○ ");
            } else if (cell == GRID_EMPTY) {
                printf(". ");
            } else if (cell == GRID_ARROW) {
                printf("X ");
            }
        }
        // 右侧竖线 + 行号
        printf("| %d\n", i);
    }

    // 打印底部边框
    printf("  +");
    for (int i = 0; i < MAX_SIZE; i++) {
        printf("--");
    }
    printf("+\n");

    // 打印底部列号
    printf("  ");
    for (int i = 0; i < MAX_SIZE; i++) {
        printf("%d ", i);
    }
    printf("\n");
}

void display_paly_info(void){
    printf("当前回合: %d\n", g_game_State.turn_Counter);
    if (g_game_State.current_Grid == GRID_BLACK) {
        printf("当前玩家: 黑方 (●)\n");
    } else if (g_game_State.current_Grid == GRID_WHITE) {
        printf("当前玩家: 白方 (○)\n");
    }
}

/*
* @brief 判断模式输入是否合法的函数
* @param input 等待被输入的变量
* @param 返回合法的输入值
*/

int Is_Mode_input_effective(int input){
    while (input >= 6 || input <= 0){
        char Mode_buffer[10] = "0";
        fgets(Mode_buffer, 9, stdin); // gets即使在C99也被警告
        if (strlen (Mode_buffer) == 2) { // fgets会把\n也放进来
            if (Mode_buffer[0] >= '1' && Mode_buffer[0] <= '5') {
                input = Mode_buffer[0] - '0';
            } else {
                printf("输入无效\n");
                printf ("请选择 (1-5): ");
            }
        } else {
            printf("输入无效\n");
            printf ("请选择 (1-5): ");
        }
    }
    return input;    
}

/*
* @brief 终于到主函数了说是
*/

int main(void){
    
    display_Menu();
    while (g_choose_Mode == 0) {
        g_choose_Mode = Is_Mode_input_effective(g_choose_Mode);    
        switch (g_choose_Mode) {
            case 1: {    // 情况1：人机对战
                g_game_State.game_Mode = MODE_PVE;
                case1_PVE_handler();
                //waiting...
                break;
            } case 2: {  // 情况2：双人对战
                g_game_State.game_Mode = MODE_PVP;
                case2_PVP_handler();
                //waiting...
                break;
            } case 3: {  // 情况3：加载游戏
                //waiting...
                break;
            } case 4: {  // 情况4：游戏说明
                case4_help_handler();
                break;
            } case 5: {  // 情况5：退出
                printf("感谢游玩\n");
                break;
            } default: {
                printf("无效输入\n");
            }
        }
    }
    
    return 0;
}

/*
* @brief 完成并封装case1:PVE功能的函数
*/

void case1_PVE_handler(void){
    PVE_Init();
}

void case2_PVP_handler(void){
    PVP_Init();
}

/*
* @brief 完成并封装case4:游戏说明功能的函数,输出完规则之后又重新开始接收模式
*/

void case4_help_handler(void){
    printf ("\n============= 游戏规则 =============\n");
    printf ("1. 棋盘: 8x8方格,坐标从0开始\n");
    printf ("2. 每方4个棋子,初始位置固定\n");
    printf ("3. 棋子走法同国际象棋皇后(8方向)\n");
    printf ("4. 每步: 移动棋子 + 放置障碍\n");
    printf ("5. 对方无路可走时获胜\n");
    printf ("6. 黑方先手\n");
    printf ("====================================\n");
    g_choose_Mode = 0;
    printf("请重新选择\n");
}