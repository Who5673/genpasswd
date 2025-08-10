#define __POSIX_C_SOURCE 200809L
#define NUM_OPTIONS 4
#define NUM_OPTIONS_TWO 2
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/random.h>
#include <ncurses.h>

// Generate password:
void generate_password(char *output, unsigned short length) {
  const char charset[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#$%^&*()-_=+[]{}|;:,.<>?/";
  int charset_size = sizeof(charset) - 1; 
  unsigned char buffer[length];
  // Random strings from kernel (safe)
  if (getrandom(buffer, length, 0) == -1) {
    perror("getrandom failed");
    exit(1);
  }
  for (int i = 0; i < length; i++) {
    int index = buffer[i] % charset_size;
    output[i] = charset[index];
  }
  output[length] = '\0'; // End strings, the password is ready!
}

// Lists:
const char *options[NUM_OPTIONS] = {
  "Create a recommended password (length = 24 strings)",
  "Create a custom password with a custom amount of length (work in progress)",
  "Delete logs (work in progress)",
  "Get help",
};

const char *options2[NUM_OPTIONS_TWO] = {
  "Get help with genpasswd in MANPAGE (work in progress, but you can still open it)",
  "Get help with genpasswd in app via CLI (work in progress)",
};

void draw_ascii() {
  init_pair(10, COLOR_YELLOW, COLOR_BLACK);
  FILE *fp = popen("figlet genpasswd | boxes -d parchment", "r");
  if (fp == NULL) {
    endwin();
    perror("figlet failed");
    exit(1);
  }
  char line[256];
  attron(A_BOLD | COLOR_PAIR(10));
  short dow = 1;
  while (fgets(line, sizeof(line), fp) != NULL) {
    mvprintw(dow++, 4, "%s", line);
  }
  attroff(A_BOLD | COLOR_PAIR(10));
  pclose(fp);
}

// Draw menu func:
void draw_manu(short highlight) {
  long int rows, cols;
  getmaxyx(stdscr, rows, cols);

  init_pair(5, COLOR_BLUE, COLOR_WHITE);
  init_pair(10, COLOR_YELLOW, COLOR_BLACK);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  clear();

  /* "Press q to quit" header  */
  attron(A_BOLD | COLOR_PAIR(1));
  char *quiting = "Press q to quit this program";
  unsigned short lenquit = strlen(quiting);
  short quitrow = rows*0;
  unsigned int quitcol = cols / 2 - (lenquit / 2);
  mvprintw(quitrow, quitcol, "%s", quiting);
  attroff(A_BOLD | COLOR_PAIR(1));

  draw_ascii(); 

  for (short i = 0; i < NUM_OPTIONS; i++) {
    if (i == highlight) 
      attron(A_REVERSE | COLOR_PAIR(5));
    mvprintw(12 + i, 4, "%s", options[i]);
    if (i == highlight)
      attroff(A_REVERSE | COLOR_PAIR(5));
  }
  refresh();
}
// Another charset of the menu:
void draw_manu2(short highlight) {
  init_pair(5, COLOR_BLUE, COLOR_WHITE);
  init_pair(10, COLOR_YELLOW, COLOR_BLACK);
  clear();
  FILE *fp = popen("figlet genpasswd | boxes -d parchment", "r");
  if (fp == NULL) {
    endwin();
    perror("figlet failed");
    exit(1);
  }
  char line[256];
  attron(A_BOLD | COLOR_PAIR(10));
  short dow = 1;
  while (fgets(line, sizeof(line), fp) != NULL) {
    mvprintw(dow++, 4, "%s", line);
  }
  attroff(A_BOLD | COLOR_PAIR(10));
  pclose(fp);


  for (short j = 0; j < NUM_OPTIONS_TWO; j++) {
    if (j == highlight) 
      attron(A_REVERSE | COLOR_PAIR(5));
    mvprintw(12 + j, 5, "%s", options2[j]);
    if (j == highlight) 
      attroff(A_REVERSE | COLOR_PAIR(5));
  } refresh();
}
void password_genpasswd(short length) {
  init_pair(8, COLOR_GREEN, COLOR_BLACK);
  init_pair(7, COLOR_YELLOW, COLOR_BLACK);
  init_pair(6, COLOR_WHITE, COLOR_BLACK);
  init_pair(4, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  char password[150];
  generate_password(password, length);
  attron(A_BOLD | COLOR_PAIR(2));
  mvprintw(12, 5, "Generated strong password: ");
  attroff(A_BOLD | COLOR_PAIR(2));
  attron(A_BOLD | COLOR_PAIR(6));
  mvprintw(14, 5, "----->     ");
  attroff(A_BOLD | COLOR_PAIR(6));
  attron(A_BOLD | COLOR_PAIR(7));
  mvprintw(14, 16, "%s", password);
  attroff(A_BOLD | COLOR_PAIR(7));
  attron(A_BOLD | COLOR_PAIR(4));
  mvprintw(16, 5, "DO NOT SHARE THAT PASSWORD TO ANYONE!");
  attroff(A_BOLD | COLOR_PAIR(4));
  attron(A_BOLD | COLOR_PAIR(8));
  mvprintw(18, 5, "Press any key to go back...");
  attroff(A_BOLD | COLOR_PAIR(8));
}

// Main scripts
int main(int args, char *argv[]) {
  if (args > 2) {
    printf("Error: Invalid syntax!\n");
    printf("\033[1;34mUsage: genpasswdtui [OPTIONS]\033[0m\n");
    printf("\033[1;37mSee our manpage or use 'genpasswdtui -h' for help. Note: No mouse mode is being worked in progess so you cannot enable it.\033[0m\n");
    return 1;
  } 
  else if (args == 2){
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "get-help") == 0) {
      printf("\033[1;34mUsage: genpasswdtui [OPTIONS]\033[0m\n");
      printf("Options in genpasswdtui are: \n");
      printf("\033[1;32m\t--help -h get-help: \033[0m");
      printf("\033[1;36mGet the instructions fast so that you know some OPTIONS to deal with genpasswdtui\033[0m\n");
      printf("\033[1;32m\t--no-mouse -nm no-mouse-mode=enabled no-mouse-mode=yes: \033[0m");
      printf("\033[1;36m(Work in progress) Enable no-mouse mode so that you only can control the app via keyboard\033[0m\n");
      printf("Should you want more information, please see our manpage using this command: 'man genpasswdtui'\n");
      return 0;
    }
    else {
      printf("Error: Invalid syntax!\n");
      printf("\033[1;34mUsage: genpasswdtui [OPTIONS]\033[0m\n");
      printf("\033[1;37mSee our manpage or use 'genpasswdtui -h' for help. Note: No mouse mode is being worked in progess so you cannot enable it.\033[0m\n");
      return 1;
    }
  }
  
  // Basic info about the window:
  MEVENT event;
  initscr();
  cbreak();
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  short choice = 0;
  short choicehai = 0;
  mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

  // Get window winfo:
  //long int rows, cols;
  //getmaxyx(stdscr, rows, cols);
  
  // Define some color pairs:
  start_color();
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_RED, COLOR_BLACK);
  init_pair(6, COLOR_WHITE, COLOR_BLACK);
  init_pair(7, COLOR_YELLOW, COLOR_BLACK);
  init_pair(8, COLOR_GREEN, COLOR_BLACK);
  init_pair(10, COLOR_YELLOW, COLOR_BLACK);

  
  // Process
  int ch;
  draw_manu(choice);
  while (1) {
    ch = getch();
    if (ch == 'q') {
      break;
    }
    switch (ch) {
      case KEY_MOUSE:
        if (getmouse(&event) == OK) {
          short x = event.x;
          short y = event.y;
        }
      case KEY_UP: 
        if (choice > 0) {
          choice--;
        }
        break;
      case KEY_DOWN:
        if (choice < NUM_OPTIONS - 1) { choice++; }
        break;
      case 10: // Enter
      case KEY_RIGHT:
        clear();
        if (choice == 0) {
          FILE *fp = popen("figlet genpasswd | boxes -d parchment", "r");
          if (fp == NULL) {
            endwin();
            perror("figlet failed");
            exit(1);
          }
          char line[256];
          attron(A_BOLD | COLOR_PAIR(10));
          short dow = 1;
          while (fgets(line, sizeof(line), fp) != NULL) {
            mvprintw(dow++, 4, "%s", line);
          }
          attroff(A_BOLD | COLOR_PAIR(10));
          pclose(fp);

          char password[24 + 1];
          generate_password(password, 24);
          attron(A_BOLD | COLOR_PAIR(2));
          mvprintw(12, 5, "Generated strong password: ");
          attroff(A_BOLD | COLOR_PAIR(2));
          attron(A_BOLD | COLOR_PAIR(6));
          mvprintw(14, 5, "----->     ");
          attroff(A_BOLD | COLOR_PAIR(6));
          attron(A_BOLD | COLOR_PAIR(7));
          mvprintw(14, 16, "%s", password);
          attroff(A_BOLD | COLOR_PAIR(7));
          attron(A_BOLD | COLOR_PAIR(4));
          mvprintw(16, 5, "DO NOT SHARE THAT PASSWORD TO ANYONE!");
          attroff(A_BOLD | COLOR_PAIR(4));
          attron(A_BOLD | COLOR_PAIR(8));
          mvprintw(18, 5, "Press any key to go back...");
          attroff(A_BOLD | COLOR_PAIR(8));
          refresh();
          getch();
        } else if (choice == 3) {
            clear();
            draw_manu2(choicehai);
            while (1) {
              ch = getch();
              if (ch == KEY_LEFT) break;
              switch (ch) {
                case KEY_UP:
                  if (choicehai > 0) choicehai--;
                  break;
                case KEY_DOWN:
                  if (choicehai < NUM_OPTIONS_TWO - 1) choicehai++;
                  break;
                case 10:
                case KEY_RIGHT:
                  if (choicehai == 0) {
                    clear(); // Enter
                    endwin();
                    system("man genpasswdtui");
                    refresh();
                    break;
                  }
                  else if (choicehai == 1) {
                    clear();
                    draw_ascii();
                    mvprintw(12, 5, "genpasstui app.");
                    mvprintw(13, 5, "genpasstui is a Text-based User Interface app for generating secure passwords to protect you from being attacked in the security.");
                    mvprintw(14, 5, "Here are some options:");
                    mvprintw(15, 5, "\t(Run with no arguments): Normally run the app with no mouse mode is enabled (as no mouse mode is being worked in place)");
                    mvprintw(16, 5, "\t--help, -h, get-help: Get help with genpasswdtui in Command Line interface to see how to use OPTIONS fast");
                    mvprintw(17, 5, "\t--no-mouse no-mouse-mode=enabled no-mouse-mode=yes -nm: (Work in progress) Run the app with no-mouse mode which options can only be interacted via keyboard");
                    mvprintw(18, 5, "Should you have more info, please see our manpage using this command: 'man genpasswdtui'.");
                    refresh();
                    getch();
                    break;
                  }
              
              }
              draw_manu2(choicehai);
            }
        }
        else {
          mvprintw(10, 5, "Test: %s", options[choice]);
          refresh();
          getch();
        }
        break;
    }
    draw_manu(choice);
  }
  endwin();
  FILE *gf = popen("echo $DISPLAY", "r");
  char buffer[255];
  char *abc = fgets(buffer, 255, gf);
  buffer[strcspn(buffer, "\n")] = 0;
  if (strcmp(abc, "") == 0) {
    system("clear");
  } pclose(gf);
  return 0;
}
