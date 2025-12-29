#include <stdio.h>
#include <string.h>

void showTasks(void) {
	printf("this is a todo app\n");
}

void compliteTask(void) {
	printf("compliteTask\n");
	printf("list of tasks\n");
}

void editTask(void) {
	printf("editTask\n");
	printf("list of tasks\n");
}

int main(void) {
	char input[30];
	char allowedInputs[4][10] = {"list", "add", "complite", "edit"};
	int isInputAllowed = 0;

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

		if (isInputAllowed == 1) {
			printf("You good to go\n");
		} else {
			printf("You input in not allowed\n");
		}
	} while (isInputAllowed == 0);

	return 0;
}

