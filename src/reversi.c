#include <stdio.h>
#include <stdlib.h>

#define EMPTY 0
#define BLACK 1
#define WHITE 2

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define UP_LEFT 4
#define UP_RIGHT 5
#define DOWN_LEFT 6
#define DOWN_RIGHT 7

char board[26][26]; //����
const int DIR[8][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

int n; //���̴�С������

int command_x;
int command_y;
int computer_x;
int computer_y;

int me_flag;
int computer_flag;
int computer_vaild_flag = 0;
int gameover_flag = 0; //1�����޺Ϸ����ӣ�2�����û�Υ�����
int win_flag = 0;
int legal_move_flag = 1; //0�������Ϸ�

//����������ʼ
void Init_board(void);                             //�������̴�С n
void printBoard(void);                             //�������
void computer_move(int color);                     //��������塣
void human_move(int color);                        //�û�����
int check_board(void);                             //�����Ϸ�Ƿ����,1Ϊ����
int check_legal_move(int row, int col, int color); //�����ɫΪ color �������������̸�(row, col)���Ƿ�Ϸ�
int flip(int row, int col, int color);             //��ת 8 �������ϵĶ�������
int position_score(int row, int col, int color);   //���㡰��ֵ��
void gameover(void);                               //��Ϸ������ͳ�Ƶ÷֣�������
//������������

int main(void)
{
    char tempchar;
    Init_board();
    getchar();
    printf("Computer plays (X/O) : ");
    scanf("%c", &tempchar);
    getchar();
    if (tempchar == 'X')
    {
        me_flag = WHITE;
        computer_flag = BLACK;
    }
    if (tempchar == 'O')
    {
        me_flag = BLACK;
        computer_flag = WHITE;
    }
    printBoard();
    while (check_board() == 1)
    {
        if (me_flag == BLACK)
        {
            human_move(me_flag);
            legal_move_flag = check_legal_move(command_x, command_y, me_flag);
            if (legal_move_flag == 0)
            {
                gameover_flag = 2;
                printf("Invalid move.\n");
                break;
            }
            board[command_x][command_y] = me_flag;
            flip(command_x, command_y, me_flag);
            printBoard();
            computer_move(computer_flag);
            board[command_x][command_y] = computer_flag;
            flip(command_x, command_y, computer_flag);
            printf("Computer places O at %c%c.\n", command_x + 97, command_y + 97);
            printBoard();
        }
        if (me_flag == WHITE)
        {
            computer_move(computer_flag);
            if (computer_vaild_flag == 1)
            {
                board[command_x][command_y] = computer_flag;
                flip(command_x, command_y, computer_flag);
                printf("Computer places X at %c%c.\n", command_x + 97, command_y + 97);
            }
            if (computer_vaild_flag == 0)
            {
                printf("X player has no valid move.\n");
            }
            printBoard();
            human_move(me_flag);
            legal_move_flag = check_legal_move(command_x, command_y, me_flag);
            if (legal_move_flag == 0)
            {
                gameover_flag = 2;
                printf("Invalid move.\n");
                break;
            }
            board[command_x][command_y] = me_flag;
            flip(command_x, command_y, me_flag);
            printBoard();
        }
    }
    gameover();
    printf("Press any key to exit...\n");
    getchar();
    return 0;
}
void Init_board(void)
{
    printf("Enter the board dimension: ");
    scanf("%d", &n);
    board[n / 2 - 1][n / 2 - 1] = WHITE;
    board[n / 2][n / 2] = WHITE;
    board[n / 2][n / 2 - 1] = BLACK;
    board[n / 2 - 1][n / 2] = BLACK;
}
void printBoard(void) //��ӡboard
{
    char visual_board[27][27] = {0};
    int i;
    int j;
    visual_board[0][0] = ' ';
    for (i = 0; i < n; i++)
    {
        visual_board[0][i + 1] = 97 + i;
        visual_board[i + 1][0] = 97 + i;
    }
    for (i = 1; i < n + 1; i++)
    {
        for (j = 1; j < n + 1; j++)
        {
            if (board[i - 1][j - 1] == EMPTY)
            {
                visual_board[i][j] = '.';
            }
            else if (board[i - 1][j - 1] == BLACK)
            {
                visual_board[i][j] = 'X';
            }
            else if (board[i - 1][j - 1] == WHITE)
            {
                visual_board[i][j] = 'O';
            }
        }
    }
    for (i = 0; i < n + 1; i++)
    {
        for (j = 0; j < n + 1; j++)
        {
            printf("%c ", visual_board[i][j]);
        }
        printf("\n");
    }
}
void gameover(void)
{
    printf("Game over.\n");
    int i;
    int j;
    int x = 0;
    int o = 0;
    if (gameover_flag == 1)
    {
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (board[i][j] == BLACK)
                    x++;
                if (board[i][j] == BLACK)
                    o++;
            }
        }
        printf("X : O = %d : %d\n", x, o);
        if (x > o)
            printf("X player wins.\n");
        if (x == o)
            printf("Draw!\n");
        if (x < o)
            printf("O player wins.\n");
        return;
    }
    if (gameover_flag == 2)
    {
        if (computer_flag == 1)
            printf("X player wins.\n");
        if (computer_flag == 2)
            printf("O player wins.\n");
        return;
    }
}
void computer_move(int color)
{
    int i;
    int j;
    int best_score = -99;
    int current_score = 0;
    computer_vaild_flag = 0;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (check_legal_move(i, j, computer_flag) == 1)
            {
                current_score = position_score(i, j, color);
                if (current_score > best_score)
                {
                    command_x = i;
                    command_y = j;
                    computer_vaild_flag = 1;
                }
            }
        }
    }
}
int check_legal_move(int row, int col, int color)
{
    if (board[row][col] != 0) //�жϴ˴��Ƿ���������
        return 0;
    int i, j, ii, jj;
    int book = 0;
    if (board[row][col + 1] == 3 - color && col <= 5) //������̽
    {
        for (i = col + 2; i < n; i++)
        {
            if (board[row][i] == 0)
                break;
            else if (board[row][i] == color)
            {
                book++;
                break;
            }
        }
    }
    if (board[row][col - 1] == 3 - color && col >= 2) //������̽
    {
        for (i = col - 2; i >= 0; i--)
        {
            if (board[row][i] == 0)
                break;
            else if (board[row][i] == color)
            {
                book++;
                break;
            }
        }
    }
    if (board[row - 1][col] == 3 - color && row >= 2) //������̽
    {
        for (i = row - 2; i >= 0; i--)
        {
            if (board[i][col] == 0)
                break;
            else if (board[i][col] == color)
            {
                book++;
                break;
            }
        }
    }
    if (board[row + 1][col] == 3 - color && row <= 5) //������̽
    {
        for (i = row + 2; i < n; i++)
        {
            if (board[i][col] == 0)
                break;
            else if (board[i][col] == color)
            {
                book++;
                break;
            }
        }
    }
    if (board[row + 1][col - 1] == 3 - color && (row <= 5 && col >= 2)) //��������̽
    {
        for (i = row + 2, j = col - 2; i < n, j >= 0; i++, j--)
        {
            if (board[i][j] == 0)
                break;
            else if (board[i][j] == color)
            {
                book++;
                break;
            }
        }
    }
    if (board[row - 1][col - 1] == 3 - color && (row >= 2 && col >= 2)) //��������̽
    {
        for (i = row - 2, j = col - 2; i >= 0, j >= 0; i--, j--)
        {
            if (board[i][j] == 0)
                break;
            else if (board[i][j] == color)
            {
                book++;
                break;
            }
        }
    }
    if (board[row + 1][col + 1] == 3 - color && (row <= 5 && col <= 5)) //��������̽
    {
        for (i = row + 2, j = col + 2; i < n, j < n; i++, j++)
        {
            if (board[i][j] == 0)
                break;
            else if (board[i][j] == color)
            {
                book++;
                break;
            }
        }
    }
    if (board[row - 1][col + 1] == 3 - color && (row >= 2 && col <= 5)) //��������̽
    {
        for (i = row - 2, j = col + 2; i >= 0, j < n; i--, j++)
        {
            if (board[i][j] == 0)
                break;
            else if (board[i][j] == color)
            {
                book++;
                break;
            }
        }
    }
    if (book > 0)
        return 1;
    else
        return 0;
}
void human_move(int color)
{
    char x;
    char y;
    char temp;
    if (color == 1)
        printf("Enter move for X (RowCol): ");
    if (color == 2)
        printf("Enter move for O (RowCol): ");
    x = getchar();
    y = getchar();
    temp = getchar();
    command_x = x - 97;
    command_y = y - 97;
    //printf("%d %d\n",command_x,command_y);
}
int check_board(void)
{
    int flag = 0;
    int i;
    int j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (check_legal_move(i, j, 1) == 1)
                flag = 1;
            if (check_legal_move(i, j, 2) == 1)
                flag = 1;
        }
    }
    if (flag == 0)
    {
        gameover_flag = 1;
        printf("Both players have no valid move.\n");
    }
    return flag;
}
int flip(int row, int col, int color)
{
    int i, j, ii, jj;
    int book = 0;
    if (board[row][col + 1] == 3 - color && col <= 5) //������̽
    {
        for (i = col + 2; i < n; i++)
        {
            if (board[row][i] == 0)
                break;
            else if (board[row][i] == color)
            {

                book++;
                for (j = i - 1; j > col; j--)
                {
                    if (color == WHITE)
                    {
                        board[row][j] = WHITE;
                    }

                    else
                    {
                        board[row][j] = BLACK;
                    }
                }

                break;
            }
        }
    }
    if (board[row][col - 1] == 3 - color && col >= 2) //������̽
    {
        for (i = col - 2; i >= 0; i--)
        {
            if (board[row][i] == 0)
                break;
            else if (board[row][i] == color)
            {

                book++;
                for (j = i + 1; j < col; j++)
                {
                    if (color == WHITE)
                    {
                        board[row][j] = WHITE;
                    }

                    else
                    {
                        board[row][j] = BLACK;
                    }
                }

                break;
            }
        }
    }
    if (board[row - 1][col] == 3 - color && row >= 2) //������̽
    {
        for (i = row - 2; i >= 0; i--)
        {
            if (board[i][col] == 0)
                break;
            else if (board[i][col] == color)
            {

                book++;
                for (j = i + 1; j < row; j++)
                {
                    if (color == WHITE)
                    {
                        board[j][col] = WHITE;
                    }

                    else
                    {
                        board[j][col] = BLACK;
                    }
                }

                break;
            }
        }
    }
    if (board[row + 1][col] == 3 - color && row <= 5) //������̽
    {
        for (i = row + 2; i < n; i++)
        {
            if (board[i][col] == 0)
                break;
            else if (board[i][col] == color)
            {

                book++;
                for (j = i - 1; j > row; j--)
                {
                    if (color == WHITE)
                    {
                        board[j][col] = WHITE;
                    }

                    else
                    {
                        board[j][col] = BLACK;
                    }
                }

                break;
            }
        }
    }
    if (board[row + 1][col - 1] == 3 - color && (row <= 5 && col >= 2)) //��������̽
    {
        for (i = row + 2, j = col - 2; i < n, j >= 0; i++, j--)
        {
            if (board[i][j] == 0)
                break;
            else if (board[i][j] == color)
            {

                book++;
                for (ii = i - 1, jj = j + 1; ii > row, jj < col; ii--, jj++)
                {
                    if (color == WHITE)
                    {
                        board[ii][jj] = WHITE;
                    }

                    else
                    {
                        board[ii][jj] = BLACK;
                    }
                }

                break;
            }
        }
    }
    if (board[row - 1][col - 1] == 3 - color && (row >= 2 && col >= 2)) //��������̽
    {
        for (i = row - 2, j = col - 2; i >= 0, j >= 0; i--, j--)
        {
            if (board[i][j] == 0)
                break;
            else if (board[i][j] == color)
            {

                book++;
                for (ii = i + 1, jj = j + 1; ii < row, jj < col; ii++, jj++)
                {
                    if (color == WHITE)
                    {
                        board[ii][jj] = WHITE;
                    }

                    else
                    {
                        board[ii][jj] = BLACK;
                    }
                }

                break;
            }
        }
    }
    if (board[row + 1][col + 1] == 3 - color && (row <= 5 && col <= 5)) //��������̽
    {
        for (i = row + 2, j = col + 2; i < n, j < n; i++, j++)
        {
            if (board[i][j] == 0)
                break;
            else if (board[i][j] == color)
            {

                book++;
                for (ii = i - 1, jj = j - 1; ii > row, jj > col; ii--, jj--)
                {
                    if (color == WHITE)
                    {
                        board[ii][jj] = WHITE;
                    }

                    else
                    {
                        board[ii][jj] = BLACK;
                    }
                }

                break;
            }
        }
    }
    if (board[row - 1][col + 1] == 3 - color && (row >= 2 && col <= 5)) //��������̽
    {
        for (i = row - 2, j = col + 2; i >= 0, j < n; i--, j++)
        {
            if (board[i][j] == 0)
                break;
            else if (board[i][j] == color)
            {

                book++;
                for (ii = i + 1, jj = j - 1; ii<row, jj> col; ii++, jj--)
                {
                    if (color == WHITE)
                    {
                        board[ii][jj] = WHITE;
                    }

                    else
                    {
                        board[ii][jj] = BLACK;
                    }
                }

                break;
            }
        }
    }
    if (book)
        return 1;
    else
        return 0;
}
int position_score(int row, int col, int color)
{
    char currentboard[26][26];
    int i;
    int j;
    int s_before;
    int s_after;
    for (i = 0; i < 26; i++)
    {
        for (j = 0; j < 26; j++)
        {
            if (board[i][j] == color)
                s_before++;
            currentboard[i][j] = board[i][j];
        }
    }
    flip(row, col, color);
    for (i = 0; i < 26; i++)
    {
        for (j = 0; j < 26; j++)
        {
            if (board[i][j] == color)
                s_after++;
            board[i][j] = currentboard[i][j];
        }
    }
    return s_after - s_before;
}
