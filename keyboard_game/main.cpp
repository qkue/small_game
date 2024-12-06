#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <conio.h>

int main() {
    constexpr int letters_count {20};

    char letter[letters_count];
    int step {0};

    time_t current_time;
    time(&current_time);

    srand(current_time);

    step = 0;
    for (int i {0}; i < letters_count; ++i) {
        letter[i] = 'a' + rand() % (('z' - 'a') + 1);
    }

    system("cls");

    printf("\n\tReady?\n");

    _getch();

    printf("\t");
    for (int i = 0; i < 5; ++i) {
        printf(".");
        Sleep(175);
    }

    printf("\n\tGo!");
    Sleep(350);

    do {
        //Render
        system("cls");
        printf("\n\t");

        for (int i {0}; i < letters_count; ++i) {
            if (i < step) {
                printf("%c ", 176);
            } else {
                printf("%c ", letter[i] );
            }
        }
        //Input
        char input_char {_getch()};

        //Main Logic
        if (input_char == letter[step]) {
            ++step;
        }

        //if (step == letters_count && input_char == letter[step]) {
        //
        //}

    }
    while (step < letters_count);
    system("cls");
    printf("\n\t");

    for (int i {0}; i < letters_count; ++i) {
        printf("%c ", 176);
    }

    system("cls");
    printf("\n\tPress any key to continue...");

    _getch()

    return 0;

}
