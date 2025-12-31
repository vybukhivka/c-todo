#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256

char toUpper(char c) {
	char output = c;
	if (output >= 'a' && output <= 'z') {
		output -= 32;
	}
	return output;
}

int showTasks(void) {
    FILE *fprt;
    int position; 
	int i = 1;
    char data[1024];
    fprt = fopen("data.txt", "r");
    fseek(fprt, 0, SEEK_END);
    position = ftell(fprt);

	if (position != 0) {
		fseek(fprt, 0, SEEK_SET);
		while (fgets(data, sizeof(data), fprt)) {
			printf("[%i]: %s", i++, data);
		}
		fclose(fprt);
		return 0;
	} else {
		printf("There is no items in to-do list.\n");
		fclose(fprt);
		return 1;
	}
}

int handleTask(char *action) {
	FILE *fptr, *fptr_tmp;
    fptr = fopen("data.txt", "r");
	char buffer[MAX_LINE_LENGTH];
	int taskNumber;
	int current_line = 1;
	char editedTask[MAX_LINE_LENGTH];

	showTasks();

	printf("Select task by number: ");
	scanf("%d", &taskNumber);

	// cleaning the buffer
	int c;
	while((c = getchar()) != '\n' && c != EOF);

	if (strcmp(action, "done") == 0) {
		fptr_tmp = fopen("data-tmp.txt", "w");
		if (fptr_tmp == NULL) {
			printf("Error: Can't open a temp file: data-tmp.txt\n");
			fclose(fptr_tmp);
			return 1;
		}

		while (fgets(buffer, MAX_LINE_LENGTH, fptr) != NULL) {
			if (current_line != taskNumber) {
				fputs(buffer, fptr_tmp);
			}
			current_line++;
		}
		printf("Task %d is completed\n", taskNumber);
	} else if (strcmp(action, "edit") == 0) {
		printf("Write your task: ");
		fgets(editedTask, sizeof(editedTask), stdin);
		size_t length = strlen(editedTask);
		editedTask[0] = toUpper(editedTask[0]);

		fptr_tmp = fopen("data-tmp.txt", "w");
		if (fptr_tmp == NULL) {
			printf("Error: Can't open a temp file: data-tmp.txt\n");
			fclose(fptr_tmp);
			return 1;
		}

		while (fgets(buffer, MAX_LINE_LENGTH, fptr) != NULL) {
			if (current_line != taskNumber) {
				fputs(buffer, fptr_tmp);
			} else {
				fputs(editedTask, fptr_tmp);
			}
			current_line++;
		}
	} 

	fclose(fptr);
	fclose(fptr_tmp);

	if (remove("data.txt") != 0) {
		printf("Error: Can't delete the original file.\n");
		return 1;
	}

	if (rename("data-tmp.txt", "data.txt") != 0) {
		printf("Error: Can't rename the temp file.\n");
		return 1;
	}

	return 0;
}

int createDataFile(void) {
	FILE *fprt;
	if ((fprt = fopen("data.txt", "r"))) {
		fclose(fprt);
		return 1;
	}
	fprt = fopen("data.txt", "w");
	fclose(fprt);
	printf("Data file created.\n");
	return 0;
}

void addTask(void) {
	FILE *fprt;
	fprt = fopen("data.txt", "a");

	// get user input
	char task[128];
	printf("Write your task: ");
	fgets(task, sizeof(task), stdin);
	size_t length = strlen(task);
	task[length - 1] = '\0';
	task[0] = toUpper(task[0]);

	// append to the data file 
	fprintf(fprt, "%s\n", task);
	fclose(fprt);
}

int main(void) {
	char input[30];
	char allowedInputs[4][10] = {"list", "add", "done", "edit"};
	int isInputAllowed = 0;

	createDataFile();

	do {
		printf("input: ");
		fgets(input, sizeof(input), stdin);
		size_t length = strlen(input);
		input[length - 1] = '\0';

		for (int i = 0; i < 4; i++) {
			int comparisonResult = strcmp(input, allowedInputs[i]);
			if (comparisonResult == 0) {
				isInputAllowed = 1;
				break;
			}
		}

		if (isInputAllowed == 0) {
			printf("You input in not allowed\n");
		}
	} while (isInputAllowed == 0);

	if (strcmp(input, "list") == 0) {
		showTasks();
	}

	if (strcmp(input, "add") == 0) {
		addTask();
		showTasks();
	}

	if (strcmp(input, "edit") == 0) {
		handleTask("edit");
	}

	if (strcmp(input, "done") == 0) {
		handleTask("done");
	}

	return 0;
}

