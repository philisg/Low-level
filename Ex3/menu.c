#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define MAX 4
#define MIN 1
#define IGNORE 0
#define IGNORED 1
#define LIST_MAX 1
#define LIST_MIN 0

struct MenuOptions {
	char  option1[25];
	char  option2[25];
	char  option3[25];
	char  option4[25];
};

struct MenuSettings {
	int opcodes[4];
	int limit[4];
	int key[4];
};

struct PartStatus {
	int buttonPressed;
	int variable;
};

int menuAlive = 1;

int position[] = { 1, 1, 1, 1 };
int startup = 1;

menuIteration = 0;

void makeArrow(int positionID, int newPosition);
int mainMenu(int menuPosition, struct MenuOptions stringOfOptions);
struct PartStatus checkKeyInput(char key, char keyVal, int argument2, int argument2Val, int variable, int Opcode);
struct PartStatus takeInput(int menuPosition, struct MenuSettings MenuSettings);

int mainMenu(int menuPosition, struct MenuOptions stringOfOptions) {
	struct MenuOptions mainMenuOptions;
	struct PartStatus mainMenuStatus;
	struct MenuSettings MainMenuSettings;
	struct MenuSettings MainMenuSettings2;

	MainMenuSettings.key[0] = 'w';
	MainMenuSettings.key[1] = 's';
	MainMenuSettings.key[2] = 'd';
	MainMenuSettings.key[3] = 'a';

	MainMenuSettings.opcodes[0] = 2;
	MainMenuSettings.opcodes[1] = 1;
	MainMenuSettings.opcodes[2] = 3;
	MainMenuSettings.opcodes[3] = 4;

	MainMenuSettings2.key[0] = 'w';
	MainMenuSettings2.key[1] = 's';
	MainMenuSettings2.key[2] = 'd';
	MainMenuSettings2.key[3] = 'a';

	MainMenuSettings2.opcodes[0] = 2;
	MainMenuSettings2.opcodes[1] = 1;
	MainMenuSettings2.opcodes[2] = 3;
	MainMenuSettings2.opcodes[3] = 4;

	mainMenuStatus = takeInput(menuPosition, MainMenuSettings);
	mainMenuOptions = stringOfOptions;

	if (mainMenuStatus.buttonPressed || startup == 1) {
		
		system("cls");
		makeArrow(1, position[menuPosition]);
		printf(stringOfOptions.option1);
		makeArrow(2, position[menuPosition]);
		printf(stringOfOptions.option2);
		makeArrow(3, position[menuPosition]);
		printf(stringOfOptions.option3);
		makeArrow(4, position[menuPosition]);
		printf(stringOfOptions.option4);
		//printf(" Position is: %d \r \ n", position[menuPosition]);
	}
	startup = 0;
	return position[menuPosition];
}

struct PartStatus takeInput(int menuPosition, struct MenuSettings MenuSettingsInput) {
	struct PartStatus KeyboardStatus;
	KeyboardStatus.buttonPressed = 0;
	KeyboardStatus.variable = 0;

	if (_kbhit()) {
		int key = _getch();
		//printf("I'm pressing %d \n", key);
		switch (key) {
		case 'w':
			KeyboardStatus = checkKeyInput(key, MenuSettingsInput.key[0], position[menuPosition], MIN, position[menuPosition], MenuSettingsInput.opcodes[0]);
			break;
		case 's':
			KeyboardStatus = checkKeyInput(key, MenuSettingsInput.key[1], position[menuPosition], MAX, position[menuPosition], MenuSettingsInput.opcodes[1]);
			break;
		case 'd':
			KeyboardStatus = checkKeyInput(key, MenuSettingsInput.key[2], menuIteration, LIST_MAX, position[menuPosition], MenuSettingsInput.opcodes[2]);
			break;
		case 'a':
			KeyboardStatus = checkKeyInput(key, MenuSettingsInput.key[3], menuIteration, LIST_MIN, position[menuPosition], MenuSettingsInput.opcodes[3]);
			break;
		default:
			printf("Illegal Case, wrong button pressed");
		}
		position[menuPosition] = KeyboardStatus.variable;
		return KeyboardStatus;
	}
	return KeyboardStatus;
}

void makeArrow(int positionID, int newPosition) {
	if (positionID == newPosition) {
		printf(" => ");
	}
	else {
		printf("    ");
	}
}


int main() {


	struct MenuOptions MenuOptions1;
	struct MenuOptions MenuOptions2;

	strcpy_s(MenuOptions1.option1, 25, "Menu item 1 \r \n");
	strcpy_s(MenuOptions1.option2, 25, "Menu item 2 \r \n");
	strcpy_s(MenuOptions1.option3, 25, "Menu item 3 \r \n");
	strcpy_s(MenuOptions1.option4, 25, "Menu item 4 \r \n");

	strcpy_s(MenuOptions2.option1, 25, "Menu item 5 \r \n");
	strcpy_s(MenuOptions2.option2, 25, "Menu item 6 \r \n");
	strcpy_s(MenuOptions2.option3, 25, "Menu item 7 \r \n");
	strcpy_s(MenuOptions2.option4, 25, "Menu item 8 \r \n");


	for (;;) {
		startup = 1;
		while (menuIteration == 0) {
			(mainMenu(0, MenuOptions1));
		}
		startup = 1;
		while (menuIteration == 1) {
			(mainMenu(1, MenuOptions2));
		}
	}
}

struct PartStatus checkKeyInput(char key, char keyVal, int argument2, int argument2Val, int variable, int Opcode) {
	// Acts as a multi purpouse software debounce, checking once again that the correct button is pressed.
	// Opcode Table
	//
	// Opcode     |     Action
	// ----------------------------------
	// 1		   |	 Increments by 1
	// 2		   |     Decrements by 1
	// 3		   |	 Returns a 0

int status;
struct PartStatus PartStatusFunction;
PartStatusFunction.variable = variable;

if (key == keyVal && argument2 != argument2Val) {
	PartStatusFunction.buttonPressed = 1;
	if (Opcode == 1)
	{
		PartStatusFunction.variable++;
		//printf("I'm in opcode 1");
	}

	else if (Opcode == 2)
	{
		PartStatusFunction.variable--;
		//printf("I'm in opcode 2");
	}
	else if (Opcode == 3)
	{
		//PartStatusFunction.variable = 0;
		//printf("I'm in opcode 3");
		menuIteration++;
	}
	else if (Opcode == 4)
	{
		//PartStatusFunction.variable = 0;
		//printf("I'm in opcode 4");
		menuIteration--;
	}
}
else {
	PartStatusFunction.buttonPressed = 0;
}
return PartStatusFunction;
}
