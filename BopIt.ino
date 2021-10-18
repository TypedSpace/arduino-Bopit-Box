// Game discription: BOPIT
// The game is simply BopIt the player is given a limited amount of time
// to complete the given instruction on the LCD, if he fails
// he loses the game, if he succeeds he gets points and continues with the game
// as the game progresses the player has less and less time to complete the insturction
// and the rate at which events occur becomes much more rapid


#include <string.h>; // makes working with strings much easier
#include <LiquidCrystal.h>;

const int RS = 11, EN = 12, D4 = 2, D5 = 3, D6 = 4, D7 = 5;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

#define joyX A0
#define joyY A1

String events[5] = {
    "up",
    "down",
    "left",
    "right"};

void setup()
{
    Serial.begin(9600);
    lcd.begin(16, 2); // set up number of columns and rows
    lcd.setCursor(0,0);
    lcd.print("Bop It");
}

String getJoyStickPosition(int x, int y)
{

    x = map(x, 0, 1000, 1, 3);
    y = map(y, 0, 1000, 1, 3);

    // Serial.print(x);
    // Serial.print(y);
    // Serial.print("\t");
    // Serial.print(x - y);

    switch (x - y)
    {
    case -1:
    {
        if (x == 1)
        {
            return "up";
        }
        else
        {
            return "left";
        }
    }
    case 1:
    {
        if (x == 3)
        {
            return "down";
        }
        else
        {
            return "right";
        }
    }
    default:
    {
        return "center";
    }
    }
};
void handlePoints(int amount, bool Reset) {
    lcd.setCursor(9, 1);
    lcd.print(amount);

    if (Reset) {
        lcd.print("------");
    }

}

// Game pre-sets

int score = 0;
int timePerRound = 3; // seconds
int timeBetweenRounds = 5; // seconds

bool hasWon;

void loop() {

    int randomNumber = rand() % 4;
    String chosenEvent = events[randomNumber];

    lcd.setCursor(0, 2);
    lcd.print(chosenEvent);

    for (int i; timePerRound/0.05 > i; i++) {
        delay(50);

        int VRx = analogRead(joyX);
        int VRy = analogRead(joyY);
        String joyStickPos = getJoyStickPosition(VRx, VRy);

        if (joyStickPos == chosenEvent) {
            handlePoints(score, false);
            hasWon = true;
            break;
        } else if (joyStickPos != "center") {
            handlePoints(score, true);
            hasWon = false;
            break;
        }
    }

    if (hasWon) {
        lcd.setCursor(0, 2);
        lcd.print("You passed!");
        score = score + 1;
    } else {
        lcd.setCursor(0, 2);
        lcd.print("You Lost :(");
        score = 0;
    }

    delay(timeBetweenRounds * 1000);

}