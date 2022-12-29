#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define rock     1
#define paper    2
#define scissors 3
#define win      6
#define draw     3

int charToScore(char c)
{
    switch (c) {
        case 'X':
        case 'A':
            return rock;
        case 'B':
        case 'Y':
            return paper;
        case 'C':
        case 'Z':
            return scissors;
        default:
            return 0;
    }
}

char getDraw(char opponent)
{
    return opponent;
}

char getWin(char opponent)
{
    switch(opponent) {
        case 'A':
            return 'Y';
        case 'B':
            return 'Z';
    }
    return 'X';
}

char getLose(char opponent)
{
    switch(opponent) {
        case 'A':
            return 'Z';
        case 'B':
            return 'X';
        default:
            return 'Y';
    }
}

char getPlayerOnStrategy(char opponent, char strategy)
{
    switch(strategy) {
        case 'X':
            return getLose(opponent);
        case 'Y':
            return getDraw(opponent);
    }
    return getWin(opponent);
}


int calcScore(char opponent, char player)
{

    if ((opponent < 'A') || opponent > 'C') {
        return 0;
    }
    player = getPlayerOnStrategy(opponent, player);

    if (charToScore(opponent) == charToScore(player)) {
        return charToScore(player) + draw;
    }
    if (opponent == 'A') {
        return (player == 'Y') ? charToScore(player) + win : charToScore(player);
    } else if (opponent == 'B') {
        return (player == 'Z') ? charToScore(player) + win : charToScore(player);
    }
    return (player == 'X') ? charToScore(player) + win : charToScore(player);
}

int main()
{
    FILE* fp = fopen("input.txt", "r");
    char row[10];
    int score = 0;
    while (fgets(row, 5, fp) != NULL) {
        int calc = calcScore(row[0], row[2]);
        score += calc;
        //printf("Opponent: %c Player: %c, calc: %d\n", row[0], row[2], calc);

    }
    printf("Score: %d", score);
    fclose(fp);
    return 0;
}
