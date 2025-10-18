#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>

#define BOARD_START_X 25
#define BOARD_START_Y 5
#define SPIN_X 2
#define SPIN_Y 2
#define BID_Y 22
#define BALANCE_X 55
#define BALANCE_Y 22

#define COLOR_RED_BLACK 1
#define COLOR_BLACK_RED 2
#define COLOR_ACID_GREEN 3
#define COLOR_GRAY 4
#define COLOR_BRIGHT_BLUE 5

#define MAX_BETS 50

typedef struct {
    int y;
    int x;
    int width;
    char type[10];
} BetPosition;

int roulette_numbers[37] = {0, 32, 15, 19, 4, 21, 2, 25, 17, 34, 6, 27, 13, 36, 11, 30, 8, 23, 10, 5, 24, 16, 33, 1, 20, 14, 31, 9, 22, 18, 29, 7, 28, 12, 35, 3, 26};
int is_red[37] = {0};

BetPosition bets[MAX_BETS];
int num_bets = 0;

void init_colors() {
    start_color();
    init_pair(COLOR_RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_BLACK_RED, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_ACID_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_GRAY, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_BRIGHT_BLUE, COLOR_CYAN, COLOR_BLACK);
}

void setup_roulette_colors() {
    int reds[] = {1,3,5,7,9,12,14,16,18,19,21,23,25,27,30,32,34,36};
    for (int i = 0; i < 18; i++) {
        is_red[reds[i]] = 1;
    }
}

void setup_bet_positions() {
    bets[num_bets].y = BOARD_START_Y;
    bets[num_bets].x = BOARD_START_X - 5;
    bets[num_bets].width = 3;
    strcpy(bets[num_bets].type, "0");
    num_bets++;

    for (int col = 0; col < 12; col++) {
        int num_top = 3 + col * 3;
        bets[num_bets].y = BOARD_START_Y;
        bets[num_bets].x = BOARD_START_X + col * 4;
        bets[num_bets].width = 3;
        sprintf(bets[num_bets].type, "%d", num_top);
        num_bets++;

        int num_mid = 2 + col * 3;
        bets[num_bets].y = BOARD_START_Y + 1;
        bets[num_bets].x = BOARD_START_X + col * 4;
        bets[num_bets].width = 3;
        sprintf(bets[num_bets].type, "%d", num_mid);
        num_bets++;

        int num_bot = 1 + col * 3;
        bets[num_bets].y = BOARD_START_Y + 2;
        bets[num_bets].x = BOARD_START_X + col * 4;
        bets[num_bets].width = 3;
        sprintf(bets[num_bets].type, "%d", num_bot);
        num_bets++;
    }

    bets[num_bets].y = BOARD_START_Y + 4;
    bets[num_bets].x = BOARD_START_X;
    bets[num_bets].width = 5;
    strcpy(bets[num_bets].type, "1st12");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y + 4;
    bets[num_bets].x = BOARD_START_X + 16;
    bets[num_bets].width = 5;
    strcpy(bets[num_bets].type, "2nd12");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y + 4;
    bets[num_bets].x = BOARD_START_X + 32;
    bets[num_bets].width = 5;
    strcpy(bets[num_bets].type, "3rd12");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y;
    bets[num_bets].x = BOARD_START_X + 48;
    bets[num_bets].width = 4;
    strcpy(bets[num_bets].type, "2to1_3");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y + 1;
    bets[num_bets].x = BOARD_START_X + 48;
    bets[num_bets].width = 4;
    strcpy(bets[num_bets].type, "2to1_2");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y + 2;
    bets[num_bets].x = BOARD_START_X + 48;
    bets[num_bets].width = 4;
    strcpy(bets[num_bets].type, "2to1_1");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y + 5;
    bets[num_bets].x = BOARD_START_X;
    bets[num_bets].width = 4;
    strcpy(bets[num_bets].type, "1to18");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y + 5;
    bets[num_bets].x = BOARD_START_X + 8;
    bets[num_bets].width = 4;
    strcpy(bets[num_bets].type, "even");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y + 5;
    bets[num_bets].x = BOARD_START_X + 16;
    bets[num_bets].width = 4;
    strcpy(bets[num_bets].type, "red");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y + 5;
    bets[num_bets].x = BOARD_START_X + 24;
    bets[num_bets].width = 5;
    strcpy(bets[num_bets].type, "black");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y + 5;
    bets[num_bets].x = BOARD_START_X + 32;
    bets[num_bets].width = 3;
    strcpy(bets[num_bets].type, "odd");
    num_bets++;

    bets[num_bets].y = BOARD_START_Y + 5;
    bets[num_bets].x = BOARD_START_X + 40;
    bets[num_bets].width = 5;
    strcpy(bets[num_bets].type, "19to36");
    num_bets++;
}

void draw_board() {
    attron(COLOR_PAIR(COLOR_ACID_GREEN));
    attron(A_BOLD);
    mvprintw(BOARD_START_Y, BOARD_START_X - 5, " 0 ");
    attroff(A_BOLD);
    attroff(COLOR_PAIR(COLOR_ACID_GREEN));

    for (int col = 0; col < 12; col++) {
        int num_top = 3 + col * 3;
        attron(is_red[num_top] ? COLOR_PAIR(COLOR_RED_BLACK) : COLOR_PAIR(COLOR_BLACK_RED));
        attron(A_BOLD);
        mvprintw(BOARD_START_Y, BOARD_START_X + col * 4, "%2d ", num_top);
        attroff(A_BOLD);
        attroff(is_red[num_top] ? COLOR_PAIR(COLOR_RED_BLACK) : COLOR_PAIR(COLOR_BLACK_RED));

        int num_mid = 2 + col * 3;
        attron(is_red[num_mid] ? COLOR_PAIR(COLOR_RED_BLACK) : COLOR_PAIR(COLOR_BLACK_RED));
        attron(A_BOLD);
        mvprintw(BOARD_START_Y + 1, BOARD_START_X + col * 4, "%2d ", num_mid);
        attroff(A_BOLD);
        attroff(is_red[num_mid] ? COLOR_PAIR(COLOR_RED_BLACK) : COLOR_PAIR(COLOR_BLACK_RED));

        int num_bot = 1 + col * 3;
        attron(is_red[num_bot] ? COLOR_PAIR(COLOR_RED_BLACK) : COLOR_PAIR(COLOR_BLACK_RED));
        attron(A_BOLD);
        mvprintw(BOARD_START_Y + 2, BOARD_START_X + col * 4, "%2d ", num_bot);
        attroff(A_BOLD);
        attroff(is_red[num_bot] ? COLOR_PAIR(COLOR_RED_BLACK) : COLOR_PAIR(COLOR_BLACK_RED));
    }

    attron(COLOR_PAIR(COLOR_GRAY));
    mvprintw(BOARD_START_Y + 4, BOARD_START_X, "1st12");
    mvprintw(BOARD_START_Y + 4, BOARD_START_X + 16, "2nd12");
    mvprintw(BOARD_START_Y + 4, BOARD_START_X + 32, "3rd12");

    mvprintw(BOARD_START_Y, BOARD_START_X + 48, "2to1");
    mvprintw(BOARD_START_Y + 1, BOARD_START_X + 48, "2to1");
    mvprintw(BOARD_START_Y + 2, BOARD_START_X + 48, "2to1");

    mvprintw(BOARD_START_Y + 5, BOARD_START_X, "1-18");
    mvprintw(BOARD_START_Y + 5, BOARD_START_X + 8, "EVEN");
    attroff(COLOR_PAIR(COLOR_GRAY));

    attron(COLOR_PAIR(COLOR_RED_BLACK));
    mvprintw(BOARD_START_Y + 5, BOARD_START_X + 16, "RED ");
    attroff(COLOR_PAIR(COLOR_RED_BLACK));
    attron(COLOR_PAIR(COLOR_GRAY));
    mvprintw(BOARD_START_Y + 5, BOARD_START_X + 24, "BLACK");
    attroff(COLOR_PAIR(COLOR_GRAY));

    attron(COLOR_PAIR(COLOR_GRAY));
    mvprintw(BOARD_START_Y + 5, BOARD_START_X + 32, "ODD ");
    mvprintw(BOARD_START_Y + 5, BOARD_START_X + 40, "19-36");
    attroff(COLOR_PAIR(COLOR_GRAY));
}

void draw_balance(int balance) {
    attron(COLOR_PAIR(COLOR_ACID_GREEN));
    attron(A_BOLD);
    mvprintw(BALANCE_Y, BALANCE_X, "TUX$:%d   ", balance);
    attroff(A_BOLD);
    attroff(COLOR_PAIR(COLOR_ACID_GREEN));
}

void draw_spin(int num) {
    if (num == -1) {
        mvprintw(SPIN_Y, SPIN_X, "00");
    } else {
        if (num == 0) {
            attron(COLOR_PAIR(COLOR_ACID_GREEN));
            attron(A_BOLD);
            mvprintw(SPIN_Y, SPIN_X, "%2d", num);
            attroff(A_BOLD);
            attroff(COLOR_PAIR(COLOR_ACID_GREEN));
        } else {
            attron(is_red[num] ? COLOR_PAIR(COLOR_RED_BLACK) : COLOR_PAIR(COLOR_BLACK_RED));
            attron(A_BOLD);
            mvprintw(SPIN_Y, SPIN_X, "%2d", num);
            attroff(A_BOLD);
            attroff(is_red[num] ? COLOR_PAIR(COLOR_RED_BLACK) : COLOR_PAIR(COLOR_BLACK_RED));
        }
    }
}

void highlight_position(int index, int on) {
    BetPosition pos = bets[index];
    if (on) {
        attron(A_REVERSE);
    }
    if (isdigit(pos.type[0])) {
        int num = atoi(pos.type);
        if (num == 0) {
            attron(COLOR_PAIR(COLOR_ACID_GREEN));
        } else {
            attron(is_red[num] ? COLOR_PAIR(COLOR_RED_BLACK) : COLOR_PAIR(COLOR_BLACK_RED));
        }
        attron(A_BOLD);
        mvprintw(pos.y, pos.x, "%*s", pos.width, pos.type);
        attroff(A_BOLD);
        if (num == 0) {
            attroff(COLOR_PAIR(COLOR_ACID_GREEN));
        } else {
            attroff(is_red[num] ? COLOR_PAIR(COLOR_RED_BLACK) : COLOR_PAIR(COLOR_BLACK_RED));
        }
    } else if (strcmp(pos.type, "red") == 0) {
        attron(COLOR_PAIR(COLOR_RED_BLACK));
        mvprintw(pos.y, pos.x, "RED ");
        attroff(COLOR_PAIR(COLOR_RED_BLACK));
    } else if (strcmp(pos.type, "black") == 0) {
        attron(COLOR_PAIR(COLOR_GRAY));
        mvprintw(pos.y, pos.x, "BLACK");
        attroff(COLOR_PAIR(COLOR_GRAY));
    } else {
        attron(COLOR_PAIR(COLOR_GRAY));
        mvprintw(pos.y, pos.x, "%s", pos.type);
        attroff(COLOR_PAIR(COLOR_GRAY));
    }
    if (on) {
        attroff(A_REVERSE);
    }
}

int get_random_animation() {
    return rand() % 37;
}

int get_true_random() {
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) return rand() % 37;
    unsigned char buf[1];
    read(fd, buf, 1);
    close(fd);
    return buf[0] % 37;
}

int animate_spin() {
    int delay = 20000;
    for (int i = 0; i < 30; i++) {
        int num = get_random_animation();
        attron(COLOR_PAIR(COLOR_WHITE)); 
        attron(A_BOLD);
        mvprintw(SPIN_Y, SPIN_X, "%2d", num);
        attroff(A_BOLD);
        attroff(COLOR_PAIR(COLOR_WHITE));
        refresh();
        usleep(delay);
        if (i > 15) delay += 1500;
    }
    int final = get_true_random();
    draw_spin(final);
    refresh();
    usleep(1500000);
    return final;
}

int is_win(int num, char* bet_type, int* payout_ratio) {
    if (num == 0) {
        *payout_ratio = 0;
        return 0;
    }
    int is_even = num % 2 == 0;
    int is_odd = !is_even;
    int is_red_num = is_red[num];

    char lower_type[20];
    strcpy(lower_type, bet_type);
    for(int i = 0; lower_type[i]; i++) lower_type[i] = tolower(lower_type[i]);

    if (strcmp(lower_type, "red") == 0) {
        *payout_ratio = 1;
        return is_red_num;
    } else if (strcmp(lower_type, "black") == 0) {
        *payout_ratio = 1;
        return !is_red_num;
    } else if (strcmp(lower_type, "even") == 0) {
        *payout_ratio = 1;
        return is_even;
    } else if (strcmp(lower_type, "odd") == 0) {
        *payout_ratio = 1;
        return is_odd;
    } else if (strcmp(lower_type, "1to18") == 0) {
        *payout_ratio = 1;
        return num <= 18;
    } else if (strcmp(lower_type, "19to36") == 0) {
        *payout_ratio = 1;
        return num > 18;
    } else if (strcmp(lower_type, "1st12") == 0) {
        *payout_ratio = 2;
        return num <= 12;
    } else if (strcmp(lower_type, "2nd12") == 0) {
        *payout_ratio = 2;
        return num > 12 && num <= 24;
    } else if (strcmp(lower_type, "3rd12") == 0) {
        *payout_ratio = 2;
        return num > 24;
    } else if (strcmp(lower_type, "2to1_1") == 0) {
        *payout_ratio = 2;
        return num % 3 == 1;
    } else if (strcmp(lower_type, "2to1_2") == 0) {
        *payout_ratio = 2;
        return num % 3 == 2;
    } else if (strcmp(lower_type, "2to1_3") == 0) {
        *payout_ratio = 2;
        return num % 3 == 0;
    } else {
        int bet_num = atoi(bet_type);
        if (bet_num >= 0 && bet_num <= 36) {
            *payout_ratio = 35;
            return num == bet_num;
        }
    }
    return 0;
}

int find_closest(int current, int dy, int dx) {
    BetPosition curr_pos = bets[current];
    int best = -1;
    int min_dist = INT_MAX;
    for (int i = 0; i < num_bets; i++) {
        if (i == current) continue;
        BetPosition p = bets[i];
        int dist = abs(p.y - (curr_pos.y + dy)) + abs(p.x - (curr_pos.x + dx));
        if (dist < min_dist) {
            min_dist = dist;
            best = i;
        }
    }
    return best;
}

int is_lower_sector(int index) {
    return (bets[index].y == BOARD_START_Y + 4 || bets[index].y == BOARD_START_Y + 5);
}

int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    init_colors();
    setup_roulette_colors();
    setup_bet_positions();
    clear();

    srand(time(NULL));

    int balance = 1000;
    char bet_str[10];
    char type_str[20];
    int bet_amount;

    while (1) {
        clear();
        draw_board();
        draw_balance(balance);
        draw_spin(-1);
        
        attron(COLOR_PAIR(COLOR_BRIGHT_BLUE));
        attron(A_BOLD);
        mvprintw(BID_Y, 2, "BID: ");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(COLOR_BRIGHT_BLUE));
        refresh();

        echo();
        move(BID_Y, 7);
        getstr(bet_str);
        noecho();
        bet_amount = atoi(bet_str);
        if (bet_amount <= 0 || bet_amount > balance) {
            attron(COLOR_PAIR(COLOR_ACID_GREEN));
            mvprintw(BID_Y + 1, 2, "Invalid bet! Press any key...");
            attroff(COLOR_PAIR(COLOR_ACID_GREEN));
            getch();
            continue;
        }

        draw_board();
        draw_balance(balance);
        draw_spin(-1);
        
        attron(COLOR_PAIR(COLOR_BRIGHT_BLUE));
        attron(A_BOLD);
        mvprintw(BID_Y, 2, "BID: %d", bet_amount);
        attroff(A_BOLD);
        attroff(COLOR_PAIR(COLOR_BRIGHT_BLUE));
        
        mvprintw(BID_Y + 1, 2, "Use arrows to select, Enter to confirm");
        refresh();

        int current = 0;
        highlight_position(current, 1);
        refresh();

        int ch;
        while ((ch = getch()) != 10) {
            highlight_position(current, 0);
            
            if (is_lower_sector(current)) {
                if (ch == KEY_UP) {
                    current = find_closest(current, -1, 0);
                } else if (ch == KEY_DOWN) {
                    current = find_closest(current, 1, 0);
                } else if (ch == KEY_LEFT) {
                    current = find_closest(current, 0, -8);
                } else if (ch == KEY_RIGHT) {
                    current = find_closest(current, 0, 8);
                }
            } else {
                if (ch == KEY_UP) {
                    current = find_closest(current, -2, 0);
                } else if (ch == KEY_DOWN) {
                    current = find_closest(current, 2, 0);
                } else if (ch == KEY_LEFT) {
                    current = find_closest(current, 0, -4);
                } else if (ch == KEY_RIGHT) {
                    current = find_closest(current, 0, 4);
                }
            }
            
            if (current == -1) current = 0;
            highlight_position(current, 1);
            refresh();
        }

        strcpy(type_str, bets[current].type);

        int num = animate_spin();

        int payout_ratio;
        int win = is_win(num, type_str, &payout_ratio);
        if (win) {
            int profit = bet_amount * payout_ratio;
            balance += profit;
            attron(COLOR_PAIR(COLOR_ACID_GREEN));
            mvprintw(BID_Y + 2, 2, "You win %d! Total: %d", profit, balance);
            attroff(COLOR_PAIR(COLOR_ACID_GREEN));
        } else {
            balance -= bet_amount;
            attron(COLOR_PAIR(COLOR_ACID_GREEN));
            mvprintw(BID_Y + 2, 2, "You lose! Total: %d", balance);
            attroff(COLOR_PAIR(COLOR_ACID_GREEN));
        }

        draw_balance(balance);
        refresh();
        getch();

        if (balance <= 0) {
            attron(COLOR_PAIR(COLOR_ACID_GREEN));
            mvprintw(BID_Y + 3, 2, "Game over! ");
            attroff(COLOR_PAIR(COLOR_ACID_GREEN));
            refresh();
            getch();
            break;
        }
    }

    endwin();
    return 0;
}
