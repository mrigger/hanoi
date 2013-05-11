#include <stdio.h>
#include <string.h>

#define MAX_PEC 10

int num_jogadas(int pecas);
int colunas[3][MAX_PEC];
int selectedSource, selectedDestination;
int selected;
int num_pecas;
int totalMoves;
void printField();
void downShift();

void moveSelectorLeft();
void moveSelectorRight();
void placeDestination();
void placeSource();
int makeMove();
int getTopmostPiece(int col);
void removeTopmostPiece(int col);
int checkForWin();

int main() {
    int i, j;
    char key;
    int gameRunning;
    while(1) {
        system("cls");
        printf("Novo jogo\n\n");
        do {
            printf("Entre com o numero de pecas: \n>");
            scanf("%d", &num_pecas);
            if(num_pecas <= 0 || num_pecas > MAX_PEC) {
                printf("Entre com um numero entre 1 e %i\n", MAX_PEC);
            }
            fflush(stdin);
        } while(num_pecas <= 0 || num_pecas > MAX_PEC);

        for(i = 0; i < 3; i++) {
            for(j = 1; j <= MAX_PEC; j++) {
                colunas[i][j - 1] = (j <= num_pecas && !i)?j:0;
            }
        }
        downShift();
        gameRunning = 1;
        selected = 0;
        selectedSource = -1;
        selectedDestination = -1;
        totalMoves = 0;
        while(gameRunning) {
            system("cls");
            printField();

            if(checkForWin()) {
                printf("Voce ganhou.");
                getch();
                break;
            }

            printf("\n\n>");
            //scanf("%c", &key);
            key = getch();
            if(key != 32) {
                switch(key) {
                    case 'a':
                        moveSelectorLeft();
                        break;
                    case 'd':
                        moveSelectorRight();
                        break;
                    case 's':
                        placeDestination();
                        break;
                    case 'w':
                        placeSource();
                        break;

                }
            } else {
                gameRunning = 0;
            }


            if(makeMove()) {
                totalMoves++;
                selectedDestination = -1;
                selectedSource = -1;
            }


        }
    }
    return 0;
}

int num_jogadas(int pecas) {
    if(pecas <= 0) {
        return 0;
    } else {
        return 2*num_jogadas(pecas - 1) + 1;
    }
}

void printField() {
    int maxSize = MAX_PEC*2 - 1;
    char lines[MAX_PEC][maxSize+3];
    int i, j, k, c;

    printf("Moves: %d\nMin. mov.: %d\n",
           totalMoves, num_jogadas(num_pecas));

    printf(" ");
    for(i = 0; i < 3; i++) {
        char place;
        if(i == selectedSource) {
            place = '-';
        } else if(i == selectedDestination) {
            place = '+';
        } else {
            place = ' ';
        }
        for(j = 0; j < maxSize; j++) {
            if(j == maxSize/2) {
                printf("%c", place);
            } else {
                printf(" ");
            }
        }
        printf(" ");
    }
    printf("\n");

    for(i = 0; i < MAX_PEC; i++) {
        c = 0;
        for(j = 0; j < 3; j++) {
            int m = 2*colunas[j][i] - 1;
            for(k = 0; k < maxSize; k++) {
                if(k >= (maxSize - m)/2 && k < (maxSize - m)/2 + m) {
                    lines[i][c] = '#';
                } else if(k == maxSize/2) {
                    lines[i][c] = '|';
                } else {
                    lines[i][c] = ' ';
                }
                c++;
            }
            lines[i][c++] = ' ';
        }
        lines[i][c++] = '\0';
        printf(" ");
        printf(lines[i]);
        printf("\n");
    }

    printf(" ");
    for(i = 0; i < 3; i++) {
        char place;
        if(i == selected) {
            place = 'O';
        } else {
            place = '-';
        }
        for(j = 0; j < maxSize; j++) {
            if(j == maxSize/2) {
                printf("%c", place);
            } else {
                printf("-");
            }
        }
        printf(" ");
    }
    printf("\n");
}

void downShift() {
    int change, i, j;

    for(i = 0; i < 3; i++) {
        do {
            change = 0;
            for(j = 0; j < MAX_PEC - 1; j++) {
                //printf("j = %d  value at (%d, %d): %d\n", j, i, j, colunas[i][j]);
                if(!colunas[i][j]) {
                    //printf("Skip j = %d\n", j);
                    continue;
                }
                if(!colunas[i][j+1]) {
                    change = 1;
                    //printf("j = %d, Switching %d for %d\n", j, colunas[i][j+1], colunas[i][j]);
                    colunas[i][j+1] = colunas[i][j];
                    colunas[i][j] = 0;
                }
            }
        } while(change);
    }
}

void moveSelectorLeft() {
    if(selected > 0) {
        selected--;
    }
}

void moveSelectorRight() {
    if(selected < 2) {
        selected++;
    }
}

void placeDestination() {
    if(selectedDestination == selected) {
        selectedDestination = -1;
    } else {
        selectedDestination = selected;
        if(selectedSource == selected) {
            selectedSource = -1;
        }
    }
}

void placeSource() {
    if(selectedSource == selected) {
        selectedSource = -1;
    } else {
        selectedSource = selected;
        if(selectedDestination == selected) {
            selectedDestination = -1;
        }
    }
}

int makeMove() {
    if(selectedDestination >= 0 && selectedSource >= 0) {
        int sourcePiece = getTopmostPiece(selectedSource);
        int destPiece = getTopmostPiece(selectedDestination);
        if((sourcePiece < destPiece || !destPiece) && sourcePiece) {
            colunas[selectedDestination][0] = sourcePiece;
            removeTopmostPiece(selectedSource);
            downShift();
            return 1;
        }
    }

    return 0;
}

int getTopmostPiece(int col) {
    int i;
    for(i = 0; i < MAX_PEC; i++) {
        if(colunas[col][i]) {
            return colunas[col][i];
        }
    }

    return 0;
}

void removeTopmostPiece(int col) {
    int i;
    for(i = 0; i < MAX_PEC; i++) {
        if(colunas[col][i]) {
            colunas[col][i] = 0;
            return;
        }
    }
}

int checkForWin() {
    int i, j, colcount[] = {0, 0}, prev;
    for(i = 1; i < 3; i++) {
        prev = 0;
        for(j = 0; j < MAX_PEC; j++) {
            if(colunas[i][j] == prev + 1) {
                prev = colunas[i][j];
                colcount[i-1]++;
            }
        }
    }
    return (colcount[0] == num_pecas) || (colcount[1] == num_pecas);
}
