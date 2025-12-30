#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int showTasks(void) {
    FILE *fprt;
    int position; 
    char data[100];
    fprt = fopen("data.txt", "r");
    fseek(fprt, 0, SEEK_END);
    position = ftell(fprt);

    if (position != 0) {
	fseek(fprt, 0, SEEK_SET);
	while (fgets(data, sizeof(data), fprt)) {
	    printf("content: %s", data);
	}
	fclose(fprt);
	return 0;
    } else {
	printf("There is no items in to-do list.\n");
	fclose(fprt);
	return 1;
    }
}

void compliteTask(void) {
	printf("compliteTask\n");
	printf("list of tasks\n");
}

void editTask(void) {
	printf("editTask\n");
	printf("list of tasks\n");
}

int createDataFile(void) {
    FILE *fprt;
    if ((fprt = fopen("data.txt", "r"))) {
	fclose(fprt);
	printf("Data file exists.\n");
	return 1;
    }
    fprt = fopen("data.txt", "w");
    fclose(fprt);
    printf("Data file created.\n");
    return 0;
}

void appendToFile(char *task) {
	FILE *fprt;
	fprt = fopen("data.txt", "a");
	fprintf(fprt, "\n");
	fprintf(fprt, task);
	fclose(fprt);
}

int main(void) {
	char input[30];
	char allowedInputs[4][10] = {"list", "add", "complite", "edit"};
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

	if (strcmp(input, "list\n")) {
		showTasks();
	}

	return 0;
}

