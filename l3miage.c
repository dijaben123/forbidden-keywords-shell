#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include<stdbool.h>
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\n"
volatile int ctrl_c_pressed = 0;

void ctrl_c_handler(int sig) {
    ctrl_c_pressed = 1;
    printf("\nTo exit, please type 'exit'\n");
}

char *sh_read_line(FILE *f) {
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, f);
    return line;
}



char **sh_split_line(char *line) {
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int is_command_forbidden(char **args) {
    char *forbidden = getenv("FORBIDDEN");
    if (forbidden == NULL) {
        return 0;
    }

    for (int i = 0; args[i] != NULL; i++) {
        char *token = strtok(forbidden, ":");
        while (token != NULL) {
            if (strstr(args[i], token) != NULL) {
                return 1;
            }
            token = strtok(NULL, ":");
        }
    }

    return 0;
}



void newf_command(char *keyword) {
    int add = 0; // Utilisez un int pour représenter un booléen (0 pour faux, 1 pour vrai)
    char *forbidden = getenv("FORBIDDEN");
    if (forbidden == NULL) {
        setenv("FORBIDDEN", keyword, 1);
        add = 1; // Mettez à jour add lorsque le mot-clé est ajouté avec succès
    } else {
        size_t new_size = strlen(forbidden) + strlen(keyword) + 2;
        char *new_forbidden = (char *)malloc(new_size);
        if (new_forbidden == NULL) {
            perror("lsh");
            exit(EXIT_FAILURE);
        }
        snprintf(new_forbidden, new_size, "%s:%s", forbidden, keyword);
        setenv("FORBIDDEN", new_forbidden, 1);
        add = 1; // Mettez à jour add lorsque le mot-clé est ajouté avec succès
        free(new_forbidden);
    }

    if (add) {
        printf("The keyword '%s' has been successfully added to the variable.\n", keyword);
    }
}




void rmf_command(char *keyword) {
    char *forbidden = getenv("FORBIDDEN");
    if (forbidden == NULL) {
        printf("La variable FORBIDDEN n'existe pas.\n");
        return;
    }

    // Allouer de la mémoire pour la nouvelle chaîne
    size_t new_size = strlen(forbidden) + 1; // La taille est la même que la chaîne originale
    char *new_forbidden = (char *)malloc(new_size);

    if (new_forbidden == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(EXIT_FAILURE);
    }

    // Initialiser la nouvelle chaîne avec une chaîne vide
    new_forbidden[0] = '\0';

    // Variable pour suivre si le mot-clé a été supprimé
    int removed = 0;

    // Copiez tous les mots-clés existants, sauf celui que vous souhaitez supprimer
    char *token = strtok(forbidden, ":");
    while (token != NULL) {
        if (strcmp(token, keyword) != 0) {
            if (new_forbidden[0] != '\0') {
                strcat(new_forbidden, ":");
            }
            strcat(new_forbidden, token);
        } else {
            removed = 1; // Le mot-clé a été supprimé
        }
        token = strtok(NULL, ":");
    }

    setenv("FORBIDDEN", new_forbidden, 1);
    free(new_forbidden);

    if (removed) {
        printf("The keyword '%s' has been successfully removed.\n", keyword);
    } else {
        printf("The keyword '%s' was not found in the FORBIDDEN variable.\n", keyword);
    }
}





int sh_execute(char **args) {
    if (args[0] == NULL) {
        // An empty command was entered.
        return 1;
    }

    if (ctrl_c_pressed) {
       
        return 1;
    }

    if (is_command_forbidden(args)) {
        if (strcmp(args[0], "rmf") != 0){ 
        printf("Work instead of playing!\n");
        }
        else{
          if (args[1] != NULL) {
            rmf_command(args[1]);
        } else {
            printf("Usage: rmf <keyword>\n");
        }
        return 1;
        }
        return 1;
    }

    if (strcmp(args[0], "exit") == 0) {
        // Handle the 'exit' command.
        return 0;
    } else if (strcmp(args[0], "newf") == 0) {
         ///printf(args[1]);
        // Handle the 'newf' command.
        if (args[1] != NULL) {
            newf_command(args[1]);
        } else {
            printf("Usage: newf <keyword>\n");
        }
        return 1;
    } else if (strcmp(args[0], "rmf") == 0) {
        // Handle the 'rmf' command.
        if (args[1] != NULL) {
            rmf_command(args[1]);
        } else {
            printf("Usage: rmf <keyword>\n");
        }
        return 1;
    }

    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("lsh");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

void sh_loop(void) {
    char *prompt = "l3miage shell > ";
    char *line;
    char **args;
    int status;

    signal(SIGINT, ctrl_c_handler); // Register Ctrl+C handler

    do {
        ctrl_c_pressed = 0; // Reset Ctrl+C flag
        printf("%s", prompt);
        fflush(stdout);

        line = sh_read_line(stdin);
        args = sh_split_line(line);
        status = sh_execute(args);

        free(line);
        free(args);
    } while (status);
}

int main(int argc, char *argv[]) {
    char *forbidden = getenv("FORBIDDEN");
    sh_loop();
    return EXIT_SUCCESS;
}
