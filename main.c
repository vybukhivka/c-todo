#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256

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
		printf("List is empty.\n");
		return 0;
	}

	char data[1024];
	int i = 0;

	while (fgets(data, sizeof(data), fprt)) {
		i++;
	}

	if (i == 0) {
		printf("List is empty.\n");
	} else if (i > 1) {
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
		printf("List is empty.\n");
		return 0;
	}

	char data[1024];
	int i = 1;

	while (fgets(data, sizeof(data), fprt)) {
		printf("[%i] %s", i++, data);
	}

	if (i == 1) printf("List is empty.\n");

	fclose(fprt);
	return 0;
}

int handleTask(char *action, int taskNumber, char *description) {
	char path_main[512], path_tmp[512];
    getPath(path_main, ".todo_data.txt");
    getPath(path_tmp, ".todo_tmp.txt");

	int totalTasksNumber = getTasksNumber();

	if (taskNumber < 0 || taskNumber > totalTasksNumber ) {
		printf("Wrong task number\n");
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
				printf("Task %d marked completed.\n", taskNumber);
			} else if (strcmp(action, "edit") == 0) {
				description[0] = toupper(description[0]);
				fprintf(fptr_tmp, "%s\n", description);
				printf("Task %d updated.\n", taskNumber);
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
		printf("Task %d not found.\n", taskNumber);
	}

	return 0;
}

int addTask(char *task) {
	if (strlen(task) > MAX_LINE_LENGTH) {
		printf("Task is too verbose.\n");
		return 1;
	}

	char path[512];
	getPath(path, ".todo_data.txt");
	FILE *fprt = fopen(path, "a");

	int totalTasksNumber = getTasksNumber();
	if (totalTasksNumber >= 10) {
		printf("You have achieved the max number of tasks!\n");
		printf("Complete at least on of existing tasks before adding a new one.\n");
		return 1;		
	}

	task[0] = toupper(task[0]);
	fprintf(fprt, "%s\n", task);
	fclose(fprt);
	printf("Task added.\n");
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc == 1 || strcmp(argv[1], "list") == 0) {
		showTasks();
		return 0;
	}

	if (strcmp(argv[1], "add") == 0) {
		if (argc < 3 || argc > 3) {
			printf("Error: Provide a task description\n");
			printf("Like: todo add \"your task\"\n");
			return 1;
		}	
		addTask(argv[2]);
		return 0;
	}

	if (strcmp(argv[1], "done") == 0) {
		if (argc < 3) {
			printf("Error: Specify a task number\n");
			printf("Like: todo done 1\n");
			return 1;
		}
		handleTask("done", atoi(argv[2]), NULL);
		return 0;
	}

	if (strcmp(argv[1], "edit") == 0) {
		if (argc < 4) {
			printf("Error: You need to provide 4 arguments\n");
			printf("Like: todo edit 1 \"new deskricption""\n");
			return 1;
		}
		handleTask("edit", atoi(argv[2]), argv[3]);
		return 0;	
	}

	return 0;
}

