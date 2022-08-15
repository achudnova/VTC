#define __STDC_LIB_EXT1__ 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>

#define SZ 256

#define MENU_TOP 0
#define MENU_MAIN 1
#define MENU_DCT 2
#define MENU_TRANSLATE_EN 3
#define MENU_TRANSLATE_DE 4
#define MENU_RESULTS 5

struct vocable
{
    char english[SZ];
    char german[SZ];
    struct vocable *next;
};

struct vocable *first = NULL;
const char *fname = NULL;
char dictionaries[SZ][50];
int dictSize = 0;
int correct = 0;
int wrong = 0;

int prev_menu = 0;

int is_read_input(int menu_type);
void translateEng();
void translateGer();
void showResults();
void showTranslateGer();
void showTranslateEng();
int listDirectory();
void showAddDct();
void showRemoveDct();
void showDctMenu();
int readInput();
void showMenu(int menu_type);
void showTopMenu(void);
void removeListItem(void);
void readDictionary();
void saveDictionary(void);
void showDictionary(void);
void addListItem(void);
struct vocable *allocateMemory(void);
void freeMemory();
int evaluateUserChoice(char, int);
void showMenuEdit(void);
void clearBuffer(void);

int main()
{
    int menu_type = MENU_TOP; // 0 = Top Menu, 1 = Main Menu, -1 = exit, 2 = chooseDict, 3 = TranslateEng Menu, 4 = TranslateGer, 5 = Results Menu

    readDictionary();

    do
    {
        showMenu(menu_type);

        int ch = 0;
        if (is_read_input(menu_type))
        {
            do
            {
                ch = readInput();
            } while (ch == '\n');
        }

        menu_type = evaluateUserChoice(ch, menu_type);
    } while (menu_type != -1);

    return 0;
}

int is_read_input(int menu_type)
{
    if (menu_type != 3 && menu_type != 4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void showResults()
{
    printf("=================\n");
    printf("RESULTS:\n");
    printf("=================\n");
    printf("Total: %d correct\n", correct);
    printf("       %d wrong\n", wrong);
    printf("%.2f %%\n", ((float)correct / (float)(correct + wrong)) * 100.0f);
    printf("-------------------\n");
    printf("[1] Try again\n");
    printf("[2] Change mode\n");
    printf("[3] Exit\n");
}

void showTranslateGer()
{
}

void translateGer()
{
    struct vocable *current = first;
    correct = 0;
    wrong = 0;

    while (current)
    {
        printf("english: %s\n", current->english);
        printf("german: ");

        char buf[SZ];
        scanf("%s", buf);

        int n1 = strlen(current->german);
        int n2 = strlen(buf);

        int sz = n1 > n2 ? n1 : n2;

        if (strncmp(current->german, buf, sz) == 0)
        {
            correct++;
            printf("CORRECT\n");
        }
        else
        {
            wrong++;
            printf("WRONG\n");
        }

        current = current->next;
    }
}

void showTranslateEng()
{
}

void translateEng()
{
    struct vocable *current = first;
    correct = 0;
    wrong = 0;

    while (current)
    {
        printf("german: %s\n", current->german);
        printf("english: ");

        char buf[SZ];
        scanf("%s", buf);

        int n1 = strlen(current->english);
        int n2 = strlen(buf);

        int sz = n1 > n2 ? n1 : n2;

        if (strncmp(current->english, buf, sz) == 0)
        {
            correct++;
            printf("CORRECT\n");
        }
        else
        {
            wrong++;
            printf("WRONG\n");
        }

        current = current->next;
    }
}

void showAddDct()
{
    int is_txt = 0;
    int is_exists = 1;
    do
    {
        puts("Enter the name: ");

        char *res;
        do
        {
            res = fgets(dictionaries[0], SZ, stdin);
            if (res == NULL)
            {
                puts("The name cannot be read");
                exit(1);
            }

        } while (dictionaries[0][0] == '\n' || strlen(dictionaries[0]) == 0);
        dictionaries[0][strlen(dictionaries[0]) - 1] = '\0';

        int name_len = strlen(dictionaries[0]);

        char *pos = strstr(dictionaries[0], ".txt");
        if (pos && name_len > 4)
        {
            char *pos_end = dictionaries[0] + name_len;
            char *pos_start = pos_end - 4;
            if (pos_start == pos)
            {
                is_txt = 1;
            }
        }

        if(is_txt != 1){
            puts("*************************************************************");
            puts("The name is incorrect! Please enter new name. (E.g.: test.txt)");
        }

        FILE *fp = fopen(dictionaries[0], "r");
        if(!fp){
            is_exists = 0;
        } else{
            fclose(fp);
            puts("This name already exists");
        }

    } while (is_txt == 0 || is_exists == 1);

    dictSize = 1;
    fname = dictionaries[0];

    FILE *fp = fopen(fname, "w");

    if (!fp)
    {
        puts("Cannot open the file");
        exit(1);
    }
    fclose(fp);
}

void showRemoveDct()
{
    puts("Which dictionary do you want to remove?");

    int n = listDirectory();
    int ch;
    do
    {
        ch = readInput();
    } while (ch == '\n');

    int i = ch - 'A';
    if (i < n)
    {
        printf("Removing '%s'\n", dictionaries[i]);
        remove(dictionaries[i]);
    }
}

int readInput()
{
    int choice;
    choice = getchar();
    choice = toupper(choice);
    return choice;
}

void freeMemory()
{
    struct vocable *current = first;
    while (current != NULL)
    {
        struct vocable *t = current;
        current = current->next;
        free(t);
    }

    first = NULL;
}

void showMenu(int menu_type)
{
    switch (menu_type)
    {
    case 0:
        showTopMenu();
        break;
    case 1:
        showMenuEdit();
        break;
    case 2:
        showDctMenu();
        break;
    case 3:
        showTranslateEng();
        break;
    case 4:
        showTranslateGer();
        break;
    case 5:
        showResults();
        break;
    }
}

void removeListItem(void)
{
    struct vocable *current = NULL;
    if (!first)
    {
        puts("You can not remove anything!");
        return;
    }
    struct vocable *prev = NULL;
    int removeItem;
    int count = 1;

input:
    puts("\n----------------------------------");
    puts("Which entry do you want to remove?");
    showDictionary();
    scanf("%d", &removeItem);
    clearBuffer();

    if (removeItem < count)
    {
        goto input;
    }
    current = first;

    while (count < removeItem)
    {
        prev = current;          // Die Addresse des ersten Listenintrages
        current = current->next; // current zeigt auf den Eintrag, der entfernt werden soll
        count++;                 // wird inkrementiert; count wird um einen bestimmten Wert erhöht

        if (current == NULL)
        {
            puts("This entry does not exist");
            return;
        }
    }

    if (prev == NULL)
    {
        first = current->next; // Das 2-te ListItem = der 1-ste Listeintrag
    }
    else
    {
        prev->next = current->next;
    }
    free(current);

    puts("\n***********************************");
    puts(">> The vocabulary has been removed!\n");
}

void readDictionary()
{
    first = NULL;
    struct vocable *current = NULL;

    freeMemory();

    FILE *fp = fopen(fname, "r");
    if (!fp)
    {
        perror("File opening failed");
        return;
    }

    char buf[SZ * 2 + 1];
    while (fgets(buf, sizeof buf, fp) != NULL)
    {
        char *pos = strchr(buf, '|');
        if (pos == NULL)
        {
            puts("The file cannot be read");
            exit(1);
        }

        char *pos2 = strchr(buf, '\n');
        if (pos2 == NULL)
        {
            puts("The file cannot be read");
            exit(1);
        }

        struct vocable *ptr = allocateMemory();
        strncpy(ptr->english, buf, pos - buf);
        strncpy(ptr->german, pos + 1, pos2 - pos - 1);

        if (first == NULL)
        {
            first = ptr;
            current = ptr;
        }
        else
        {
            current->next = ptr;
            current = current->next;
        }
    }

    fclose(fp);
}

void saveDictionary(void)
{
    struct vocable *current = NULL;

    current = first;

    FILE *fp = fopen(fname, "w");
    if (!fp)
    {
        perror("File opening failed");
        exit(1);
    }

    while (current)
    {
        fprintf(fp, "%s|%s\n", current->english, current->german);
        current = current->next;
    }
    puts("\n*****************************************");
    puts(">> Your file has been successfully saved!\n");
    fclose(fp);
}

void showDictionary(void)
{
    struct vocable *current = NULL;

    if (!first)
    {
        puts("\n***************************");
        puts(">> The dictionary is empty!");
        return;
    }

    int index = 1;
    current = first;

    puts("\n***************************");
    puts(">> Here is your dictionary:\n");

    while (current)
    {
        printf("No.%d: [%s - %s]\n", index++, current->english, current->german);
        current = current->next;
    }
}

void addListItem(void)
{
    struct vocable *current = NULL;

    if (!first)
    {
        first = allocateMemory();
        current = first;
    }
    else
    {
        current = first;

        while (current->next)
        {
            current = current->next;
        }

        current->next = allocateMemory();
        current = current->next;
    }

    printf("english: ");
    scanf("%s", current->english);
    clearBuffer();

    printf("german: ");
    scanf("%s", current->german);
    clearBuffer();

    current->next = NULL;

    puts("\n*********************************************");
    puts(">> The word has been added to the dictionary!\n");
}

struct vocable *allocateMemory(void)
{
    struct vocable *pVocable = (struct vocable *)malloc(sizeof(struct vocable));

    if (pVocable == NULL)
    {
        puts("Error");
        exit(1);
    }
    return pVocable;
}

int evaluateUserChoice(char choice, int menu_type)
{
    if (menu_type == MENU_MAIN)
    {
        switch (choice)
        {
        case 'A':
            puts("Show dictionary");
            showDictionary();
            break;
        case 'B':
            puts("Add word to dictionary");
            addListItem();
            break;
        case 'C':
            puts("Remove word from dictionary");
            removeListItem();
            break;
        case 'D':
            puts("Save");
            saveDictionary();
            break;
        case 'E':
            puts("Exit");
            menu_type = MENU_TOP;
            fname = NULL;
            break;
        case '1':
            puts("Translate to english");
            menu_type = MENU_TRANSLATE_EN;
            break;
        case '2':
            puts("Translate to german");
            menu_type = MENU_TRANSLATE_DE;
            break;
        default:
            puts("Your entry was invalid!");
            break;
        }
    }
    else
    {
        if (menu_type == MENU_TOP)
        {
            switch (choice)
            {
            case '1':
                puts("Learn vocabulary");
                menu_type = MENU_DCT;
                break;
            case '2':
                puts("Exit");
                menu_type = -1;
                break;
            }
        }
        else
        {
            if (menu_type == MENU_DCT)
            {
                switch (choice)
                {
                case '+':
                    puts("Add new dictionary");
                    showAddDct();
                    menu_type = MENU_DCT;
                    break;
                case '-':
                    puts("Remove dictionary");
                    showRemoveDct();
                    menu_type = MENU_DCT;
                    break;
                case '3':
                    puts("Exit");
                    menu_type = MENU_TOP;
                    break;
                default:
                    if (choice >= 'A' && choice <= 'Z')
                    {
                        int n = choice - 'A';
                        if (n < dictSize)
                        {
                            fname = dictionaries[n];
                            printf("Dictionary: %s was selected\n", fname);
                            menu_type = MENU_MAIN;
                            readDictionary();
                        }
                    }
                }
            }
            else
            {
                if (menu_type == MENU_RESULTS)
                {
                    switch (choice)
                    {
                    case '1':
                        menu_type = prev_menu;
                        break;
                    case '2':
                        menu_type = MENU_MAIN;
                        break;
                    case '3':
                        menu_type = MENU_TOP;
                        break;
                    default:
                        break;
                    }
                }
                else if (menu_type == MENU_TRANSLATE_EN)
                {
                    prev_menu = menu_type;
                    translateEng();
                    menu_type = MENU_RESULTS;
                }
                else if (menu_type == MENU_TRANSLATE_DE)
                {
                    prev_menu = menu_type;
                    translateGer();
                    menu_type = MENU_RESULTS;
                }
            }
        }
    }

    return menu_type;
}
void showTopMenu(void)
{
    puts("=================================");
    puts("TOP MENU: ");
    puts("=================================");
    puts("[1] Learn vocabulary ");
    puts("[2] Exit ");
    puts("=================================");
}

void showMenuEdit(void)
{
    puts("=================================");
    puts("MAIN MENU: ");
    puts("=================================");
    puts("[A] Show dictionary ");
    puts("[B] Add new word to dictionary ");
    puts("[C] Remove word from dictionary ");
    puts("[D] Save ");
    puts("[E] Exit ");
    puts("---------------------------------");
    puts("START LEARNING MODE");
    puts("[1] german -> english");
    puts("[2] english -> german");
    puts("=================================");
}

void showDctMenu(void)
{
    puts("=================================");
    puts("CHOOSE DICTIONARY: ");
    puts("=================================");

    dictSize = listDirectory();

    puts("[+] Add new dictionary");
    puts("[-] Remove dictionary");
    puts("[3] Exit");
    puts("=================================");

}

int listDirectory()
{
    int i = 0;
    int ch;

    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char *pos = strstr(dir->d_name, ".txt");
            if (pos)
            {
                printf("[%c] %s\n", i + 'A', dir->d_name);
                strncpy(dictionaries[i], dir->d_name, strlen(dir->d_name));
                dictionaries[i][strlen(dir->d_name)] = '\0';

                i++;
            }
        }
        closedir(d);
    }

    return i;
}

void clearBuffer(void)
{
    while (getchar() != '\n')
    {
        ;
    }
}