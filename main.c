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

void clearScreen(void) {
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif 
}

char toUpper(char c) {
	return (c >= 'a' && c <= 'z') ? c - 32 : c;
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
		printf("[%i]: %s", i++, data);
	}

	if (i == 1) printf("List is empty.\n");

	fclose(fprt);
	return 0;
}

int handleTask(char *action) {
	char path_main[512], path_tmp[512];
	getPath(path_main, ".todo_data.txt");
	getPath(path_tmp, ".todo_tmp.txt");
	FILE *fptr = fopen(path_main, "r");
	FILE *fptr_tmp = fopen(path_tmp, "w");

	clearScreen();

	if(!fptr || !fptr_tmp) {
		printf("Error opening the fiels.\n");
		if(fptr) fclose(fptr);
		if(fptr_tmp) fclose(fptr_tmp);
		return 1;
	}

	char buffer[MAX_LINE_LENGTH], editedTask[MAX_LINE_LENGTH];
	int taskNumber, current_line = 1;

	showTasks();
	printf("\nSelect task by number: ");
	scanf("%d", &taskNumber);
	while(getchar() != '\n');

	if (strcmp(action, "edit") == 0) {
		printf("Write replacement: ");
		fgets(editedTask, sizeof(editedTask), stdin);
		editedTask[strcspn(editedTask, "\n")] = 0;
		editedTask[0] = toUpper(editedTask[0]);
	}

	while (fgets(buffer, MAX_LINE_LENGTH, fptr)) {
		if (current_line == taskNumber) {
			if (strcmp(action, "edit") == 0) {
				fprintf(fptr_tmp, "%s\n", editedTask);
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
	clearScreen();
	return 0;
}

void addTask(void) {
	char path[512];
	getPath(path, ".todo_data.txt");
	FILE *fprt = fopen(path, "a");

	clearScreen();
	showTasks();

	char task[MAX_LINE_LENGTH];
	printf("\nWrite your task: ");
	fgets(task, sizeof(task), stdin);
	task[strcspn(task, "\n")] = 0;
	task[0] = toUpper(task[0]);

	fprintf(fprt, "%s\n", task);
	fclose(fprt);
}

int main(void) {
	char input[30];
	int running = 1;

	while (running) {
		clearScreen();

		showTasks();
		printf("\nCommands: add, edit, done, or quit.\nInput: ");
		if (fgets(input, sizeof(input), stdin) == NULL) break;
		input[strcspn(input, "\n")] = 0;

		if (strcmp(input, "add") == 0) addTask();
		else if (strcmp(input, "edit") == 0) handleTask("edit");
		else if (strcmp(input, "done") == 0) handleTask("done");
		else if (strcmp(input, "quit") == 0) running = 0;
	} 

	return 0;
}

