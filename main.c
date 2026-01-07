#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256
#define MAX_TASK_NUMBER 9

#define COLOR_RESET  "\033[0m"
#define COLOR_BOLD   "\033[1m"
#define COLOR_RED    "\033[31m"
#define COLOR_GREEN  "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE   "\033[34m"
#define COLOR_CYAN   "\033[36m"

void getPath(char *buffer, const char *filename) {
    char *home = getenv("HOME");
    if (home) {
        sprintf(buffer, "%s/%s", home, filename);
    } else {
        strcpy(buffer, filename);
    }
}

int getTasksNumber(void) {
	char path[512];
	getPath(path, ".todo_data.txt");

	FILE *fprt = fopen(path, "r");
	if (!fprt) {
		fprintf(stderr, COLOR_RED COLOR_BOLD "error" COLOR_RESET ": missing data file\n");
		return 0;
	}

	char data[1024];
	int i = 0;

	while (fgets(data, sizeof(data), fprt)) {
		i++;
	}

	if (i > 0) {
		fclose(fprt);
		return i;
	}

	fclose(fprt);
	return 0;
}

int showTasks(void) {
	char path[512];
	getPath(path, ".todo_data.txt");

	FILE *fprt = fopen(path, "r");
	if (!fprt) {
		fprintf(stderr, COLOR_RED COLOR_BOLD "error" COLOR_RESET ": missing data file\n");
		return 0;
	}

	char data[1024];
	int i = 1;

	while (fgets(data, sizeof(data), fprt)) {
		printf(COLOR_BLUE COLOR_BOLD "  %i" COLOR_RESET " %s", i++, data);
	}

	if (i == 1) {
		fprintf(stderr, COLOR_RED COLOR_BOLD "error" COLOR_RESET ": list is empty\n");
		printf(COLOR_CYAN COLOR_BOLD "info" COLOR_RESET ": " COLOR_BOLD "todo add \"task description\"" COLOR_RESET " - to add a task\n");
	} 

	fclose(fprt);
	return 0;
}

int handleTask(char *action, int taskNumber, char *description) {
	char path_main[512], path_tmp[512];
    getPath(path_main, ".todo_data.txt");
    getPath(path_tmp, ".todo_tmp.txt");

	int totalTasksNumber = getTasksNumber();

	if (taskNumber < 0 || taskNumber > totalTasksNumber ) {
		fprintf(stderr, COLOR_RED COLOR_BOLD "error" COLOR_RESET ": wrong task number\n");
		return 1;
	} 

    FILE *fptr = fopen(path_main, "r");
    FILE *fptr_tmp = fopen(path_tmp, "w");

    if (!fptr || !fptr_tmp) {
        if (fptr) fclose(fptr);
        if (fptr_tmp) fclose(fptr_tmp);
        return 1;
    }

    char buffer[MAX_LINE_LENGTH];
    int current_line = 1;
    int found = 0;

    while (fgets(buffer, MAX_LINE_LENGTH, fptr)) {
		if (current_line == taskNumber) {
			found = 1;
			if (strcmp(action, "done") == 0) {
				// skip
			} else if (strcmp(action, "edit") == 0) {
				description[0] = toupper(description[0]);
				fprintf(fptr_tmp, "%s\n", description);
			}
		} else {
			fputs(buffer, fptr_tmp);
		}
		current_line++;
    }

    fclose(fptr);
    fclose(fptr_tmp);
    remove(path_main);
    rename(path_tmp, path_main);

	if (!found) {
		fprintf(stderr, COLOR_RED COLOR_BOLD "error" COLOR_RESET ": task not found\n");
	}

	return 0;
}

int addTask(char *task) {
	if (strlen(task) > MAX_LINE_LENGTH) {
		fprintf(stderr, COLOR_RED COLOR_BOLD "error" COLOR_RESET ": task is too verbose\n");
		printf(COLOR_CYAN COLOR_BOLD "info" COLOR_RESET ": 256 chars\n");
		return 1;
	}

	char path[512];
	getPath(path, ".todo_data.txt");
	FILE *fprt = fopen(path, "a");

	int totalTasksNumber = getTasksNumber();
	if (totalTasksNumber >= MAX_TASK_NUMBER) {
		fprintf(stderr, COLOR_RED COLOR_BOLD "error" COLOR_RESET ": max amount of tasks achieved\n");
		printf(COLOR_CYAN COLOR_BOLD "info" COLOR_RESET ": complete at least one of existing tasks before adding next one\n");
		return 1;		
	}

	task[0] = toupper(task[0]);
	fprintf(fprt, "%s\n", task);
	fclose(fprt);
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc == 1 || strcmp(argv[1], "list") == 0) {
		showTasks();
		return 0;
	}

	if (strcmp(argv[1], "add") == 0) {
		if (argc < 3 || argc > 3) {
			fprintf(stderr, COLOR_RED COLOR_BOLD "error" COLOR_RESET ": missing description\n");
			printf(COLOR_CYAN COLOR_BOLD "info" COLOR_RESET ": " COLOR_BOLD "todo add \"task description\"" COLOR_RESET " - to add a task\n");
			return 1;
		}	
		addTask(argv[2]);
		return 0;
	}

	if (strcmp(argv[1], "done") == 0) {
		if (argc < 3) {
			fprintf(stderr, COLOR_RED COLOR_BOLD "error" COLOR_RESET ": missing task number\n");
			printf(COLOR_CYAN COLOR_BOLD "info" COLOR_RESET ": " COLOR_BOLD "todo done 1" COLOR_RESET " - to complete selected task" "\n");
			return 1;
		}
		handleTask("done", atoi(argv[2]), NULL);
		return 0;
	}

	if (strcmp(argv[1], "edit") == 0) {
		if (argc < 4) {
			fprintf(stderr, COLOR_RED COLOR_BOLD "error" COLOR_RESET ": not enough arguments\n");
			printf(COLOR_CYAN COLOR_BOLD "info" COLOR_RESET ": " COLOR_BOLD "todo edit 1 \"new description\"" COLOR_RESET " - to edit selected task\n");
			return 1;
		}
		handleTask("edit", atoi(argv[2]), argv[3]);
		return 0;	
	}

	return 0;
}

