//Word Jumblers
//Created by Avyukth Sai Rangarajan and Max Arimany


#include <FEHLCD.h> //All proteus display
#include <FEHImages.h> //Background imags
#include <stdio.h> //File reading
#include <windows.h> //Sleep()
#include <time.h> //Timer
#include <string.h> //Strings from file
#include <math.h> //Basic math, stats
#include <stdlib.h> //random generation


// Function declarations
void menuFunction(); //calls menue
void playScreen(); //calls on play screen
void statsScreen(); //calls stats
void creditsScreen(); //calls credit
void instructionsScreen(); //calls insturctions
void goBack(); //functions that loops back to menu from any other screen with a back arrow
void fiveRandomize(); //picks 4 radom 5 letter words 
void sixRandomize(); //picks 4 radom 6 letter words
void sevenRandomize(); //picks 4 radom 7 letter words
void scramble5(char Word[], char ScrambledWord[]); //Scambles the letters for each 5 letter words
void scramble6(char Word[], char ScrambledWord[]); //Scambles the letters for each 6 letter words
void scramble7(char Word[], char ScrambledWord[]); //Scambles the letters for each 7 letter words
void play5(char Word_1[], char Word_2[], char Word_3[], char Word_4[], char scrambledWord1[], char scrambledWord2[], char scrambledWord3[], char scrambledWord4[]);
void play6(char Word_1[], char Word_2[], char Word_3[], char Word_4[], char scrambledWord1[], char scrambledWord2[], char scrambledWord3[], char scrambledWord4[]);
void play7(char Word_1[], char Word_2[], char Word_3[], char Word_4[], char scrambledWord1[], char scrambledWord2[], char scrambledWord3[], char scrambledWord4[]);
void lose5();
void lose6();
void lose7();
void win5(int timer);
void win6(int timer);
void win7(int timer);


// Object requirement
struct random_letters {
    int first;
    int second;
    int third;
    int fourth;
    int fifth;
    int sixth;
    int seventh;
};


// Initialize global variables for stats that can be altered and called by any function
int fiveWins = 0;
int fivePR = 90;
int sixWins = 0;
int sixPR = 90;
int sevenWins = 0;
int sevenPR = 90;
        
//main function to start code
int main(void)
{
    menuFunction(); // Start the program by displaying the menu

    return 0;
}





void menuFunction()
{
    int running = 1; // State variable to manage the program flow (always on)
    float x_coor, y_coor; 
   

    while (running)
    {
        LCD.Clear(); // Clear any residual content
        LCD.SetBackgroundColor(WHITE);
        LCD.Update(); // Ensure the screen is refreshed
        Sleep(0.1); // Allow time for screen rendering

        // Menu background image importation and display
        FEHImage menuBackground;
        menuBackground.Open("menu_background.png");
        menuBackground.Draw(0, 0);
        LCD.Update(); // Force screen update after drawing background

        // Word declaration
        FEHIcon::Icon menuTitle[5];
        FEHIcon::Icon menuButtons[5];

        // Array creation
        char menu_labels[5][20] = {"", "Play", "Stats", "Credits", "Instructions"};
        char menu_title[5][20] = {"WORD JUMBLERS", "", "", "", ""};

        // Draw menu icons
        FEHIcon::DrawIconArray(menuTitle, 5, 1, 10, 10, 5, 5, menu_title, BLACK, BLACK);
        FEHIcon::DrawIconArray(menuButtons, 5, 1, 10, 10, 5, 5, menu_labels, BLACK, BLACK);

        LCD.Update(); // Ensure all icons are drawn
        Sleep(0.1);   // Small delay for screen rendering

        int inMenu = 1;
        while (inMenu)
        {
            if (LCD.Touch(&x_coor, &y_coor)) //constantly checks for a click and assigns it to vars
            {/*This is how we incorperate touch location detection. Each if statement now works to check where that touch occured*/
                Sleep(0.1); // Debounce the touch

                if (y_coor <= 98 && y_coor >= 54) //where play button is located
                {
                    // Go to Play screen
                    inMenu = 0;
                    menuBackground.Close(); // Clean up resources
                    playScreen();
                }
                else if (y_coor <= 142 && y_coor >= 98) //where stat button is located
                {
                    // Go to Stats screen
                    inMenu = 0;
                    menuBackground.Close(); // Clean up resources
                    statsScreen();
                }
                else if (y_coor <= 186 && y_coor >= 142) //where credit button is located
                {
                    // Go to Credits screen
                    inMenu = 0;
                    menuBackground.Close(); // Clean up resources
                    creditsScreen();
                }
                else if (y_coor <= 230 && y_coor >= 186) //where instructions button is located
                {
                    // Go to Instructions screen
                    inMenu = 0;
                    menuBackground.Close(); // Clean up resources
                    instructionsScreen();
                }
            }

            // Clear any lingering touch before continuing
            while (LCD.Touch(&x_coor, &y_coor));
        }
    }
}






void playScreen()
{
    // Reset stuff and initialize 
    LCD.Clear();
    LCD.SetBackgroundColor(WHITE);
    LCD.SetFontColor(BLACK); 
    LCD.Update(); 
    Sleep(0.1); 

    // Draw content for the Play screen
    LCD.WriteLine(" ");
    
    LCD.Update(); //Update the screen

    float x_coor, y_coor;

    // Draw "Back" button
    FEHImage backArrow;
    backArrow.Open("backArrow.png"); //import image
   
    backArrow.Draw(10, 5);
    //Draw difficulty buttons
    FEHIcon::Icon levelButtons[5];
    
    // Array creation
    char level_labels[5][20] = {"Choose Difficulty:", " ", "5 Letter Words", "6 Letter Words", "7 Letter Words"};
        
    // Draw menu icons
    FEHIcon::DrawIconArray(levelButtons, 5, 1, 10, 10, 5, 5, level_labels, WHITE, BLACK);

    LCD.Update();

    int waitingForClick = 1;
    while (waitingForClick)
    {
        if (LCD.Touch(&x_coor, &y_coor)) //constantly check for click
        {
            Sleep(0.2); // Debounce the touch
            if ((x_coor >= 5 && x_coor <= 45) && (y_coor >= 5 && y_coor <= 25)){ //back button
                waitingForClick = 0; //Go back
            }
            else if (y_coor <= 142 && y_coor >= 98){ //5 letters
                fiveRandomize();
                waitingForClick = 0;
            }
            else if (y_coor <= 186 && y_coor >= 142){ //6 letters
                sixRandomize();
                waitingForClick = 0;
            }
            else if (y_coor <= 230 && y_coor >= 186){ //7 letters
                sevenRandomize();
                waitingForClick = 0;
            }
        }
    }

    // Clear any lingering touch before changing functions
    while (LCD.Touch(&x_coor, &y_coor));
}






void fiveRandomize(){

    FILE *fiveLetterWords;
    fiveLetterWords = fopen("5letterwords.txt", "r"); //read file word banks
    
    char five_list[488][6];

    for(int i = 0; i < 488; i++){  //scan the whole bank into a large array, each row with a unique row
            fscanf(fiveLetterWords, "%5s", &five_list[i]);
    }
    fclose(fiveLetterWords);

    
    char Word_1[6], Word_2[6], Word_3[6], Word_4[6];
    Word_1[5] = '\0'; Word_2[5] = '\0'; Word_3[5] = '\0'; Word_4[5] = '\0';

    int i = rand() % 488; //Generate a number from 0-487

    for (int n = 0; n < 5; n++){
        Word_1[n] = five_list[i][n];
    } //Copy random word to first word var
    printf("\nWord 1: %s\n", Word_1);

    i = rand() % 488; //Generate a number from 0-487

    for (int n = 0; n < 5; n++){
        Word_2[n] = five_list[i][n];
    } //Copy random word to second word var
    printf("Word 2: %s\n", Word_2);

    i = rand() % 488; //Generate a number from 0-487

    for (int n = 0; n < 5; n++){
        Word_3[n] = five_list[i][n];
    } //Copy random word to third word var
    printf("Word 3: %s\n", Word_3);

    i = rand() % 488; //Generate a number from 0-487

    for (int n = 0; n < 5; n++){
        Word_4[n] = five_list[i][n];
    } //Copy random word to fourth word var
    printf("Word 4: %s\n", Word_4);

    char scrambledWord1[6], scrambledWord2[6], scrambledWord3[6], scrambledWord4[6];

    //Scramble Words
    scramble5(Word_1, scrambledWord1);
    scramble5(Word_2, scrambledWord2);
    scramble5(Word_3, scrambledWord3);
    scramble5(Word_4, scrambledWord4);

    printf("\n1: %s\n", scrambledWord1);
    printf("2: %s\n", scrambledWord2);
    printf("3: %s\n", scrambledWord3);
    printf("4: %s\n", scrambledWord4);

    // Call function that plays the game for 5 letters
    play5(Word_1, Word_2, Word_3, Word_4, scrambledWord1, scrambledWord2, scrambledWord3, scrambledWord4);
}





void play5(char Word_1[], char Word_2[], char Word_3[], char Word_4[], char scrambledWord1[], char scrambledWord2[], char scrambledWord3[], char scrambledWord4[]){
    /*Set new screen*/
    LCD.Clear();
    LCD.SetBackgroundColor(MEDIUMAQUAMARINE); 
    LCD.SetFontColor(BLACK);
    LCD.Clear();
    LCD.Update();

    //x: integer number of pixels in x direction (min 0, max 319)
    //y: integer number of pixels in y direction (min 0, max 239)
    // LCD.WriteAt(string, x, y);
    /* Word 1       Word 3
       Word 2       Word 4*/  //reference for imaging when coding
    LCD.WriteAt(scrambledWord1, 5, 60);
    LCD.WriteAt(scrambledWord2, 5, 150);
    LCD.WriteAt(scrambledWord3, 180, 60);
    LCD.WriteAt(scrambledWord4, 180, 150);

    // Word declaration
    FEHIcon::Icon answerBox1[5];
    FEHIcon::Icon answerBox2[5];
    FEHIcon::Icon answerBox3[5];
    FEHIcon::Icon answerBox4[5];

    // Array creation
    char answer_labels[5][20] = {"", "", "", "", ""};



    // Draw menu icons
    //FEHIcon::DrawIconArray(answerBox, #rows, #columnd, y, y wack var, x, x wack var, etc, etc, etc)
    FEHIcon::DrawIconArray(answerBox1, 1, 5, 80, 130, 5, 190, answer_labels, BLACK, BLACK);
    FEHIcon::DrawIconArray(answerBox2, 1, 5, 170, 40, 5, 190, answer_labels, BLACK, BLACK);
    FEHIcon::DrawIconArray(answerBox3, 1, 5, 80, 130, 180, 15, answer_labels, BLACK, BLACK);
    FEHIcon::DrawIconArray(answerBox4, 1, 5, 170, 40, 180, 15, answer_labels, BLACK, BLACK);
    //Total box width is 125, height is 30

    LCD.Update();

    //Initialize and display a starting timer
    int timer = 90;
    long int starttime = time(NULL);
    LCD.WriteAt(":", 277, 5);
    LCD.WriteAt(timer, 285, 5);
    LCD.Update();

    int Right_word1 = 1, Right_word2 = 1, Right_word3 = 1, Right_word4 = 1;
    char Word_guess1[6] = "", Word_guess2[6] = "", Word_guess3[6] = "", Word_guess4[6] = "";
    Word_guess1[5] = '\0'; Word_guess2[5] = '\0'; Word_guess3[5] = '\0'; Word_guess4[5] = '\0'; 
    
    while(timer > 0 && (Right_word1 != 0 || Right_word2 != 0 || Right_word3 != 0 || Right_word4 != 0)){
        /*Timer incrementation*/
        if ((time(NULL) - starttime) >= 1){ //Constantly checks if one second has passed
            LCD.SetFontColor(MEDIUMAQUAMARINE);
            LCD.DrawRectangle(285, 5, 30, 20);
            LCD.FillRectangle(285, 5, 30, 20);
            timer--;
            LCD.SetFontColor(BLACK);
            LCD.WriteAt(":", 277, 5);
            LCD.WriteAt(timer, 285, 5);
            LCD.Update();
            starttime = time(NULL);
        } /*This whole if statement updates the timer var and display by subing 1 every sec*/


        //user imputs change scrabled words values
        char click_value[2];
        click_value[1] = '\0';
        int x_coor, y_coor;
        if (LCD.Touch(&x_coor, &y_coor)) //Checks for click
        {
            Sleep(0.2); //Debounce

            /*Assign click from first word*/
            if ((x_coor >= 5 && x_coor < 18) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[0];
            }
            else if ((x_coor >= 18 && x_coor < 31) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[1];
            }
            else if ((x_coor >= 31 && x_coor < 44) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[2];
            }
            else if ((x_coor >= 44 && x_coor < 57) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[3];
            }
            else if ((x_coor >= 57 && x_coor < 70) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[4];
            }

            /*Assign click from second word*/
            else if ((x_coor >= 5 && x_coor < 18) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[0];
            }
            else if ((x_coor >= 18 && x_coor < 31) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[1];
            }
            else if ((x_coor >= 31 && x_coor < 44) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[2];
            }
            else if ((x_coor >= 44 && x_coor < 57) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[3];
            }
            else if ((x_coor >= 57 && x_coor < 70) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[4];
            }

            /*Assign click from third word*/
            else if ((x_coor >= 180 && x_coor < 193) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[0];
            }
            else if ((x_coor >= 193 && x_coor < 206) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[1];
            }
            else if ((x_coor >= 206 && x_coor < 219) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[2];
            }
            else if ((x_coor >= 219 && x_coor < 232) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[3];
            }
            else if ((x_coor >= 232 && x_coor < 245) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[4];
            }

            /*Assign click from fourth word*/
            else if ((x_coor >= 180 && x_coor < 193) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[0];
            }
            else if ((x_coor >= 193 && x_coor < 206) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[1];
            }
            else if ((x_coor >= 206 && x_coor < 219) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[2];
            }
            else if ((x_coor >= 219 && x_coor < 232) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[3];
            }
            else if ((x_coor >= 232 && x_coor < 245) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[4];
            }

            // Print to box 1
            else if ((x_coor >= 5 && x_coor < 30) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(6, 81, 23, 28);
                LCD.FillRectangle(6, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 10, 85);
            }
            else if ((x_coor >= 30 && x_coor < 55) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(31, 81, 23, 28);
                LCD.FillRectangle(31, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 35, 85);
            }
            else if ((x_coor >= 55 && x_coor < 80) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(56, 81, 23, 28);
                LCD.FillRectangle(56, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 60, 85);
            }
            else if ((x_coor >= 80 && x_coor < 105) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(81, 81, 23, 28);
                LCD.FillRectangle(81, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 85, 85);
            }
            else if ((x_coor >= 105 && x_coor < 130) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(106, 81, 23, 28);
                LCD.FillRectangle(106, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 110, 85);
            }

            // Print to box 2
            else if ((x_coor >= 5 && x_coor < 30) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(6, 171, 23, 28);
                LCD.FillRectangle(6, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 10, 175);
            }
            else if ((x_coor >= 30 && x_coor < 55) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(31, 171, 23, 28);
                LCD.FillRectangle(31, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 35, 175);
            }
            else if ((x_coor >= 55 && x_coor < 80) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(56, 171, 23, 28);
                LCD.FillRectangle(56, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 60, 175);
            }
            else if ((x_coor >= 80 && x_coor < 105) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(81, 171, 23, 28);
                LCD.FillRectangle(81, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 85, 175);
            }
            else if ((x_coor >= 105 && x_coor < 130) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(106, 171, 23, 28);
                LCD.FillRectangle(106, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 110, 175);
            }

            // Print to box 3
            else if ((x_coor >= 180 && x_coor < 205) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(181, 81, 23, 28);
                LCD.FillRectangle(181, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 185, 85);
            }
            else if ((x_coor >= 205 && x_coor < 230) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(206, 81, 23, 28);
                LCD.FillRectangle(206, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 210, 85);
            }
            else if ((x_coor >= 230 && x_coor < 255) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(231, 81, 23, 28);
                LCD.FillRectangle(231, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 235, 85);
            }
            else if ((x_coor >= 255 && x_coor < 280) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(256, 81, 23, 28);
                LCD.FillRectangle(256, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 260, 85);
            }
            else if ((x_coor >= 280 && x_coor < 305) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(281, 81, 23, 28);
                LCD.FillRectangle(281, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 285, 85);
            }

            // Print to box 4
            else if ((x_coor >= 180 && x_coor < 205) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(181, 171, 23, 28);
                LCD.FillRectangle(181, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 185, 175);
            }
            else if ((x_coor >= 205 && x_coor < 230) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(206, 171, 23, 28);
                LCD.FillRectangle(206, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 210, 175);
            }
            else if ((x_coor >= 230 && x_coor < 255) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(231, 171, 23, 28);
                LCD.FillRectangle(231, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 235, 175);
            }
            else if ((x_coor >= 255 && x_coor < 280) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(256, 171, 23, 28);
                LCD.FillRectangle(256, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 260, 175);
            }
            else if ((x_coor >= 280 && x_coor < 305) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(281, 171, 23, 28);
                LCD.FillRectangle(281, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 285, 175);
            }
        }

        //Checks if correct
        Right_word1 = strcmp(Word_guess1, Word_1);
        Right_word2 = strcmp(Word_guess2, Word_2);
        Right_word3 = strcmp(Word_guess3, Word_3);
        Right_word4 = strcmp(Word_guess4, Word_4);
    }
    Sleep(300); //Pause a sec before opening next page
    if (timer == 0){ //You lost screen
        LCD.Clear();
        lose5();
    }
    else{
        LCD.Clear();
        win5(timer);
    }
}



void sixRandomize(){

    FILE* sixLetterWords;
    sixLetterWords = fopen("6letterwords.txt", "r"); //read file word banks

    char six_list[448][7];
    
    for(int i = 0; i < 448; i++){ //scan the whole bank into a large array, each row with a unique row
        fscanf(sixLetterWords, "%6s", &six_list[i]);  
    }
    fclose(sixLetterWords);
    
    char Word_1[7], Word_2[7], Word_3[7], Word_4[7];
    Word_1[6] = '\0'; Word_2[6] = '\0'; Word_3[6] = '\0'; Word_4[6] = '\0';

    int i = rand() % 448; //Generate a number from 0-447

    for (int n = 0; n < 6; n++){
        Word_1[n] = six_list[i][n];
    } //Copy random word to first word var
    printf("\n1: %c%c%c%c%c%c\n", Word_1[0], Word_1[1], Word_1[2], Word_1[3], Word_1[4], Word_1[5]);

    i = rand() % 448; //Generate a number from 0-447

    for (int n = 0; n < 6; n++){
        Word_2[n] = six_list[i][n];
    } //Copy random word to second word var
    printf("2: %c%c%c%c%c%c\n", Word_2[0], Word_2[1], Word_2[2], Word_2[3], Word_2[4], Word_2[5]);

    i = rand() % 448; //Generate a number from 0-447

    for (int n = 0; n < 6; n++){
        Word_3[n] = six_list[i][n];
    } //Copy random word to third word var
    printf("3: %c%c%c%c%c%c\n", Word_3[0], Word_3[1], Word_3[2], Word_3[3], Word_3[4], Word_3[5]);

    i = rand() % 448; //Generate a number from 0-447

    for (int n = 0; n < 6; n++){
        Word_4[n] = six_list[i][n];
    } //Copy random word to fourth word var
    printf("4: %c%c%c%c%c%c\n", Word_4[0], Word_4[1], Word_4[2], Word_4[3], Word_4[4], Word_4[5]);

    char scrambledWord1[7], scrambledWord2[7], scrambledWord3[7], scrambledWord4[7];

    //Scramble Words
    scramble6(Word_1, scrambledWord1);
    scramble6(Word_2, scrambledWord2);
    scramble6(Word_3, scrambledWord3);
    scramble6(Word_4, scrambledWord4);

    printf("\n1: %s\n", scrambledWord1);
    printf("2: %s\n", scrambledWord2);
    printf("3: %s\n", scrambledWord3);
    printf("4: %s\n", scrambledWord4);

    // Call function that plays the game for 6 letters
    play6(Word_1, Word_2, Word_3, Word_4, scrambledWord1, scrambledWord2, scrambledWord3, scrambledWord4);
}






void play6(char Word_1[], char Word_2[], char Word_3[], char Word_4[], char scrambledWord1[], char scrambledWord2[], char scrambledWord3[], char scrambledWord4[]){
    /*Set new screen*/
    LCD.Clear();
    LCD.SetBackgroundColor(MEDIUMAQUAMARINE); 
    LCD.SetFontColor(BLACK);
    LCD.Clear();
    LCD.Update();

    //x: integer number of pixels in x direction (min 0, max 319)
    //y: integer number of pixels in y direction (min 0, max 239)
    // LCD.WriteAt(string, x, y);
    /* Word 1       Word 3
       Word 2       Word 4*/
    LCD.WriteAt(scrambledWord1, 5, 60);
    LCD.WriteAt(scrambledWord2, 5, 150);
    LCD.WriteAt(scrambledWord3, 180, 60);
    LCD.WriteAt(scrambledWord4, 180, 150);

    // Word declaration
    FEHIcon::Icon answerBox1[6];
    FEHIcon::Icon answerBox2[6];
    FEHIcon::Icon answerBox3[6];
    FEHIcon::Icon answerBox4[6];

    // Array creation
    char answer_labels[6][20] = {"", "", "", "", "", ""};



    // Draw menu icons
    //FEHIcon::DrawIconArray(answerBox, #rows, #columnd, y, y wack var, x, x wack var, etc, etc, etc)
    FEHIcon::DrawIconArray(answerBox1, 1, 6, 80, 130, 5, 165, answer_labels, BLACK, BLACK);
    FEHIcon::DrawIconArray(answerBox2, 1, 6, 170, 40, 5, 165, answer_labels, BLACK, BLACK);
    FEHIcon::DrawIconArray(answerBox3, 1, 6, 80, 130, 165, 5, answer_labels, BLACK, BLACK);
    FEHIcon::DrawIconArray(answerBox4, 1, 6, 170, 40, 165, 5, answer_labels, BLACK, BLACK);
    //Total box width is 150, height is 30

    LCD.Update();

    //Initialize and display a starting timer
    int timer = 90;
    long int starttime = time(NULL);
    LCD.WriteAt(":", 277, 5);
    LCD.WriteAt(timer, 285, 5);
    LCD.Update();

    int Right_word1 = 1, Right_word2 = 1, Right_word3 = 1, Right_word4 = 1;
    char Word_guess1[7] = "", Word_guess2[7] = "", Word_guess3[7] = "", Word_guess4[7] = "";
    Word_guess1[6] = '\0'; Word_guess2[6] = '\0'; Word_guess3[6] = '\0'; Word_guess4[6] = '\0'; 
    
    while(timer > 0 && (Right_word1 != 0 || Right_word2 != 0 || Right_word3 != 0 || Right_word4 != 0)){
        /*Timer incrementation*/
        if ((time(NULL) - starttime) >= 1){ //Constantly checks if one second has passed
            LCD.SetFontColor(MEDIUMAQUAMARINE);
            LCD.DrawRectangle(285, 5, 30, 20);
            LCD.FillRectangle(285, 5, 30, 20);
            timer--;
            LCD.SetFontColor(BLACK);
            LCD.WriteAt(":", 277, 5);
            LCD.WriteAt(timer, 285, 5);
            LCD.Update();
            starttime = time(NULL);
        } /*This whole if statement updates the timer var and display by subing 1 every sec*/


        //user imputs change scrabled words values
        char click_value[2];
        click_value[1] = '\0';
        int x_coor, y_coor;
        if (LCD.Touch(&x_coor, &y_coor)) //Checks for click
        {
            Sleep(0.2); //Debounce

            /*Assign click from first word*/
            if ((x_coor >= 5 && x_coor < 18) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[0];
            }
            else if ((x_coor >= 18 && x_coor < 31) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[1];
            }
            else if ((x_coor >= 31 && x_coor < 44) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[2];
            }
            else if ((x_coor >= 44 && x_coor < 57) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[3];
            }
            else if ((x_coor >= 57 && x_coor < 70) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[4];
            }
            else if ((x_coor >= 70 && x_coor < 83) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[5];
            }

            /*Assign click from second word*/
            else if ((x_coor >= 5 && x_coor < 18) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[0];
            }
            else if ((x_coor >= 18 && x_coor < 31) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[1];
            }
            else if ((x_coor >= 31 && x_coor < 44) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[2];
            }
            else if ((x_coor >= 44 && x_coor < 57) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[3];
            }
            else if ((x_coor >= 57 && x_coor < 70) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[4];
            }
            else if ((x_coor >= 70 && x_coor < 83) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[5];
            }

            /*Assign click from third word*/
            else if ((x_coor >= 180 && x_coor < 193) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[0];
            }
            else if ((x_coor >= 193 && x_coor < 206) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[1];
            }
            else if ((x_coor >= 206 && x_coor < 219) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[2];
            }
            else if ((x_coor >= 219 && x_coor < 232) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[3];
            }
            else if ((x_coor >= 232 && x_coor < 245) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[4];
            }
            else if ((x_coor >= 245 && x_coor < 258) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[5];
            }

            /*Assign click from fourth word*/
            else if ((x_coor >= 180 && x_coor < 193) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[0];
            }
            else if ((x_coor >= 193 && x_coor < 206) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[1];
            }
            else if ((x_coor >= 206 && x_coor < 219) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[2];
            }
            else if ((x_coor >= 219 && x_coor < 232) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[3];
            }
            else if ((x_coor >= 232 && x_coor < 245) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[4];
            }
            else if ((x_coor >= 245 && x_coor < 258) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[5];
            }

            // Print to box 1
            else if ((x_coor >= 5 && x_coor < 30) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(6, 81, 23, 28);
                LCD.FillRectangle(6, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 10, 85);
            }
            else if ((x_coor >= 30 && x_coor < 55) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(31, 81, 23, 28);
                LCD.FillRectangle(31, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 35, 85);
            }
            else if ((x_coor >= 55 && x_coor < 80) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(56, 81, 23, 28);
                LCD.FillRectangle(56, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 60, 85);
            }
            else if ((x_coor >= 80 && x_coor < 105) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(81, 81, 23, 28);
                LCD.FillRectangle(81, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 85, 85);
            }
            else if ((x_coor >= 105 && x_coor < 130) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(106, 81, 23, 28);
                LCD.FillRectangle(106, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 110, 85);
            }
            else if ((x_coor >= 130 && x_coor < 155) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[5] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(131, 81, 23, 28);
                LCD.FillRectangle(131, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 135, 85);
            }

            // Print to box 2
            else if ((x_coor >= 5 && x_coor < 30) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(6, 171, 23, 28);
                LCD.FillRectangle(6, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 10, 175);
            }
            else if ((x_coor >= 30 && x_coor < 55) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(31, 171, 23, 28);
                LCD.FillRectangle(31, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 35, 175);
            }
            else if ((x_coor >= 55 && x_coor < 80) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(56, 171, 23, 28);
                LCD.FillRectangle(56, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 60, 175);
            }
            else if ((x_coor >= 80 && x_coor < 105) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(81, 171, 23, 28);
                LCD.FillRectangle(81, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 85, 175);
            }
            else if ((x_coor >= 105 && x_coor < 130) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(106, 171, 23, 28);
                LCD.FillRectangle(106, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 110, 175);
            }
            else if ((x_coor >= 130 && x_coor < 155) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[5] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(131, 171, 23, 28);
                LCD.FillRectangle(131, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 135, 175);
            }

            // Print to box 3
            else if ((x_coor >= 165 && x_coor < 190) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(166, 81, 23, 28);
                LCD.FillRectangle(166, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 170, 85);
            }
            else if ((x_coor >= 190 && x_coor < 215) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(191, 81, 23, 28);
                LCD.FillRectangle(191, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 195, 85);
            }
            else if ((x_coor >= 215 && x_coor < 240) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(216, 81, 23, 28);
                LCD.FillRectangle(216, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 220, 85);
            }
            else if ((x_coor >= 240 && x_coor < 265) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(241, 81, 23, 28);
                LCD.FillRectangle(241, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 245, 85);
            }
            else if ((x_coor >= 265 && x_coor < 290) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(266, 81, 23, 28);
                LCD.FillRectangle(266, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 270, 85);
            }
            else if ((x_coor >= 290 && x_coor < 315) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[5] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(291, 81, 23, 28);
                LCD.FillRectangle(291, 81, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 295, 85);
            }

            // Print to box 4
            else if ((x_coor >= 165 && x_coor < 190) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(166, 171, 23, 28);
                LCD.FillRectangle(166, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 170, 175);
            }
            else if ((x_coor >= 190 && x_coor < 215) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(191, 171, 23, 28);
                LCD.FillRectangle(191, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 195, 175);
            }
            else if ((x_coor >= 215 && x_coor < 240) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(216, 171, 23, 28);
                LCD.FillRectangle(216, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 220, 175);
            }
            else if ((x_coor >= 240 && x_coor < 265) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(241, 171, 23, 28);
                LCD.FillRectangle(241, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 245, 175);
            }
            else if ((x_coor >= 265 && x_coor < 290) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(266, 171, 23, 28);
                LCD.FillRectangle(266, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 270, 175);
            }
            else if ((x_coor >= 290 && x_coor < 315) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[5] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(291, 171, 23, 28);
                LCD.FillRectangle(291, 171, 23, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 295, 175);
            }
        }

        //Checks if correct
        Right_word1 = strcmp(Word_guess1, Word_1);
        Right_word2 = strcmp(Word_guess2, Word_2);
        Right_word3 = strcmp(Word_guess3, Word_3);
        Right_word4 = strcmp(Word_guess4, Word_4);
    }
    Sleep(300); //Pause a sec before opening next page
    if (timer == 0){ //You lost screen
        LCD.Clear();
        lose6();
    }
    else{
        LCD.Clear();
        win6(timer);
    }
}





void sevenRandomize(){
    FILE* sevenLetterWords;
    sevenLetterWords = fopen("7letterwords.txt", "r"); //read file word banks
    char seven_list[366][8];
    
    for(int i = 0; i < 366; i++){   //scan the whole bank into a large array, each row with a unique word
        fscanf(sevenLetterWords, "%7s", &seven_list[i]);       
    }
    fclose(sevenLetterWords);

    char Word_1[8], Word_2[8], Word_3[8], Word_4[8];
    Word_1[7] = '\0'; Word_2[7] = '\0'; Word_3[7] = '\0'; Word_4[7] = '\0';

    int i = rand() % 366; //Generate a number from 0-365

    for (int n = 0; n < 7; n++){
        Word_1[n] = seven_list[i][n];
    } //Copy random word to first word var
    printf("\n1: %c%c%c%c%c%c%c\n", Word_1[0], Word_1[1], Word_1[2], Word_1[3], Word_1[4], Word_1[5], Word_1[6]);

    i = rand() % 366; //Generate a number from 0-365

    for (int n = 0; n < 7; n++){
        Word_2[n] = seven_list[i][n];
    } //Copy random word to second word var
    printf("2: %s\n", Word_2);

    i = rand() % 366; //Generate a number from 0-365

    for (int n = 0; n < 7; n++){
        Word_3[n] = seven_list[i][n];
    } //Copy random word to third word var
    printf("3: %c%c%c%c%c%c%c\n", Word_3[0], Word_3[1], Word_3[2], Word_3[3], Word_3[4], Word_3[5], Word_3[6]);

    i = rand() % 366; //Generate a number from 0-365

    for (int n = 0; n < 7; n++){
        Word_4[n] = seven_list[i][n];
    } //Copy random word to fourth word var
    printf("4: %c%c%c%c%c%c%c\n", Word_4[0], Word_4[1], Word_4[2], Word_4[3], Word_4[4], Word_4[5], Word_4[6]);

    char scrambledWord1[8], scrambledWord2[8], scrambledWord3[8], scrambledWord4[8];

    //Scramble Words
    scramble7(Word_1, scrambledWord1);
    scramble7(Word_2, scrambledWord2);
    scramble7(Word_3, scrambledWord3);
    scramble7(Word_4, scrambledWord4);

    printf("\n1: %s\n", scrambledWord1);
    printf("2: %s\n", scrambledWord2);
    printf("3: %s\n", scrambledWord3);
    printf("4: %s\n", scrambledWord4);

    // Call function that plays the game for 7 letters
    play7(Word_1, Word_2, Word_3, Word_4, scrambledWord1, scrambledWord2, scrambledWord3, scrambledWord4);
}



void play7(char Word_1[], char Word_2[], char Word_3[], char Word_4[], char scrambledWord1[], char scrambledWord2[], char scrambledWord3[], char scrambledWord4[]){
    /*Set new screen*/
    LCD.Clear();
    LCD.SetBackgroundColor(MEDIUMAQUAMARINE); 
    LCD.SetFontColor(BLACK);
    LCD.Clear();
    LCD.Update();

    //x: integer number of pixels in x direction (min 0, max 319)
    //y: integer number of pixels in y direction (min 0, max 239)
    // LCD.WriteAt(string, x, y);
    /* Word 1       Word 3
       Word 2       Word 4*/
    LCD.WriteAt(scrambledWord1, 5, 60);
    LCD.WriteAt(scrambledWord2, 5, 150);
    LCD.WriteAt(scrambledWord3, 180, 60);
    LCD.WriteAt(scrambledWord4, 180, 150);

    // Word declaration
    FEHIcon::Icon answerBox1[7];
    FEHIcon::Icon answerBox2[7];
    FEHIcon::Icon answerBox3[7];
    FEHIcon::Icon answerBox4[7];

    // Array creation
    char answer_labels[7][20] = {"", "", "", "", "", "", ""};



    // Draw menu icons
    //FEHIcon::DrawIconArray(answerBox, #rows, #columnd, y, y wack var, x, x wack var, etc, etc, etc)
    FEHIcon::DrawIconArray(answerBox1, 1, 7, 80, 130, 3, 163, answer_labels, BLACK, BLACK);
    FEHIcon::DrawIconArray(answerBox2, 1, 7, 170, 40, 3, 163, answer_labels, BLACK, BLACK);
    FEHIcon::DrawIconArray(answerBox3, 1, 7, 80, 130, 163, 3, answer_labels, BLACK, BLACK);
    FEHIcon::DrawIconArray(answerBox4, 1, 7, 170, 40, 163, 3, answer_labels, BLACK, BLACK);
    //Total box width is 154, height is 30, 22 per 

    LCD.Update();

    //Initialize and display a starting timer
    int timer = 90;
    long int starttime = time(NULL);
    LCD.WriteAt(":", 277, 5);
    LCD.WriteAt(timer, 285, 5);
    LCD.Update();

    int Right_word1 = 1, Right_word2 = 1, Right_word3 = 1, Right_word4 = 1;
    char Word_guess1[8] = "", Word_guess2[8] = "", Word_guess3[8] = "", Word_guess4[8] = "";
    Word_guess1[7] = '\0'; Word_guess2[7] = '\0'; Word_guess3[7] = '\0'; Word_guess4[7] = '\0'; 
    
    while(timer > 0 && (Right_word1 != 0 || Right_word2 != 0 || Right_word3 != 0 || Right_word4 != 0)){
        /*Timer incrementation*/
        if ((time(NULL) - starttime) >= 1){ //Constantly checks if one second has passed
            LCD.SetFontColor(MEDIUMAQUAMARINE);
            LCD.DrawRectangle(285, 5, 30, 20);
            LCD.FillRectangle(285, 5, 30, 20);
            timer--;
            LCD.SetFontColor(BLACK);
            LCD.WriteAt(":", 277, 5);
            LCD.WriteAt(timer, 285, 5);
            LCD.Update();
            starttime = time(NULL);
        } /*This whole if statement updates the timer var and display by subing 1 every sec*/


        //user imputs change scrabled words values
        char click_value[2];
        click_value[1] = '\0';
        int x_coor, y_coor;
        if (LCD.Touch(&x_coor, &y_coor)) //Checks for click
        {
            Sleep(0.2); //Debounce

            /*Assign click from first word*/
            if ((x_coor >= 5 && x_coor < 18) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[0];
            }
            else if ((x_coor >= 18 && x_coor < 31) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[1];
            }
            else if ((x_coor >= 31 && x_coor < 44) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[2];
            }
            else if ((x_coor >= 44 && x_coor < 57) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[3];
            }
            else if ((x_coor >= 57 && x_coor < 70) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[4];
            }
            else if ((x_coor >= 70 && x_coor < 83) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[5];
            }
            else if ((x_coor >= 83 && x_coor < 96) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord1[6];
            }

            /*Assign click from second word*/
            else if ((x_coor >= 5 && x_coor < 18) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[0];
            }
            else if ((x_coor >= 18 && x_coor < 31) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[1];
            }
            else if ((x_coor >= 31 && x_coor < 44) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[2];
            }
            else if ((x_coor >= 44 && x_coor < 57) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[3];
            }
            else if ((x_coor >= 57 && x_coor < 70) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[4];
            }
            else if ((x_coor >= 70 && x_coor < 83) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[5];
            }
            else if ((x_coor >= 83 && x_coor < 96) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord2[6];
            }

            /*Assign click from third word*/
            else if ((x_coor >= 180 && x_coor < 193) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[0];
            }
            else if ((x_coor >= 193 && x_coor < 206) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[1];
            }
            else if ((x_coor >= 206 && x_coor < 219) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[2];
            }
            else if ((x_coor >= 219 && x_coor < 232) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[3];
            }
            else if ((x_coor >= 232 && x_coor < 245) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[4];
            }
            else if ((x_coor >= 245 && x_coor < 258) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[5];
            }
            else if ((x_coor >= 258 && x_coor < 271) && (y_coor >= 60 && y_coor <= 73)){
                click_value[0] = scrambledWord3[6];
            }

            /*Assign click from fourth word*/
            else if ((x_coor >= 180 && x_coor < 193) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[0];
            }
            else if ((x_coor >= 193 && x_coor < 206) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[1];
            }
            else if ((x_coor >= 206 && x_coor < 219) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[2];
            }
            else if ((x_coor >= 219 && x_coor < 232) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[3];
            }
            else if ((x_coor >= 232 && x_coor < 245) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[4];
            }
            else if ((x_coor >= 245 && x_coor < 258) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[5];
            }
            else if ((x_coor >= 258 && x_coor < 271) && (y_coor >= 150 && y_coor <= 163)){
                click_value[0] = scrambledWord4[6];
            }

            // Print to box 1
            else if ((x_coor >= 3 && x_coor < 25) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(4, 81, 20, 28);
                LCD.FillRectangle(4, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 7, 85);
            }
            else if ((x_coor >= 25 && x_coor < 47) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(26, 81, 20, 28);
                LCD.FillRectangle(26, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 29, 85);
            }
            else if ((x_coor >= 47 && x_coor < 69) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(48, 81, 20, 28);
                LCD.FillRectangle(48, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 51, 85);
            }
            else if ((x_coor >= 69 && x_coor < 91) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(70, 81, 20, 28);
                LCD.FillRectangle(70, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 73, 85);
            }
            else if ((x_coor >= 91 && x_coor < 113) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(92, 81, 20, 28);
                LCD.FillRectangle(92, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 95, 85);
            }
            else if ((x_coor >= 113 && x_coor < 135) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[5] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(114, 81, 20, 28);
                LCD.FillRectangle(114, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 117, 85);
            }
            else if ((x_coor >= 135 && x_coor < 157) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess1[6] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(136, 81, 20, 28);
                LCD.FillRectangle(136, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 139, 85);
            }

            // Print to box 2
            else if ((x_coor >= 3 && x_coor < 25) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(4, 171, 20, 28);
                LCD.FillRectangle(4, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 7, 175);
            }
            else if ((x_coor >= 25 && x_coor < 47) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(26, 171, 20, 28);
                LCD.FillRectangle(26, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 29, 175);
            }
            else if ((x_coor >= 47 && x_coor < 69) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(48, 171, 20, 28);
                LCD.FillRectangle(48, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 51, 175);
            }
            else if ((x_coor >= 69 && x_coor < 91) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(70, 171, 20, 28);
                LCD.FillRectangle(70, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 73, 175);
            }
            else if ((x_coor >= 91 && x_coor < 113) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(92, 171, 20, 28);
                LCD.FillRectangle(92, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 95, 175);
            }
            else if ((x_coor >= 113 && x_coor < 135) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[5] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(114, 171, 20, 28);
                LCD.FillRectangle(114, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 117, 175);
            }
            else if ((x_coor >= 135 && x_coor < 157) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess2[6] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(136, 171, 20, 28);
                LCD.FillRectangle(136, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 139, 175);
            }

            // Print to box 3
            else if ((x_coor >= 163 && x_coor < 185) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(164, 81, 20, 28);
                LCD.FillRectangle(164, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 167, 85);
            }
            else if ((x_coor >= 185 && x_coor < 207) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(186, 81, 20, 28);
                LCD.FillRectangle(186, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 189, 85);
            }
            else if ((x_coor >= 207 && x_coor < 229) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(208, 81, 20, 28);
                LCD.FillRectangle(208, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 211, 85);
            }
            else if ((x_coor >= 229 && x_coor < 251) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(230, 81, 20, 28);
                LCD.FillRectangle(230, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 233, 85);
            }
            else if ((x_coor >= 251 && x_coor < 273) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(252, 81, 20, 28);
                LCD.FillRectangle(252, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 255, 85);
            }
            else if ((x_coor >= 273 && x_coor < 295) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[5] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(274, 81, 20, 28);
                LCD.FillRectangle(274, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 277, 85);
            }
            else if ((x_coor >= 295 && x_coor < 317) && (y_coor >= 80 && y_coor <= 110)){
                Word_guess3[6] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(296, 81, 20, 28);
                LCD.FillRectangle(296, 81, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 299, 85);
            }

            // Print to box 4
            else if ((x_coor >= 163 && x_coor < 185) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[0] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(164, 171, 20, 28);
                LCD.FillRectangle(164, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 167, 175);
            }
            else if ((x_coor >= 185 && x_coor < 207) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[1] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(186, 171, 20, 28);
                LCD.FillRectangle(186, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 189, 175);
            }
            else if ((x_coor >= 207 && x_coor < 229) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[2] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(208, 171, 20, 28);
                LCD.FillRectangle(208, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 211, 175);
            }
            else if ((x_coor >= 229 && x_coor < 251) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[3] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(230, 171, 20, 28);
                LCD.FillRectangle(230, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 233, 175);
            }
            else if ((x_coor >= 251 && x_coor < 273) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[4] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(252, 171, 20, 28);
                LCD.FillRectangle(252, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 255, 175);
            }
            else if ((x_coor >= 273 && x_coor < 295) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[5] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(274, 171, 20, 28);
                LCD.FillRectangle(274, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 277, 175);
            }
            else if ((x_coor >= 295 && x_coor < 317) && (y_coor >= 170 && y_coor <= 200)){
                Word_guess4[6] = click_value[0];
                LCD.SetFontColor(MEDIUMAQUAMARINE);
                LCD.DrawRectangle(296, 171, 20, 28);
                LCD.FillRectangle(296, 171, 20, 28);
                LCD.SetFontColor(BLACK);
                LCD.WriteAt(click_value[0], 299, 175);
            }
        }

        //Checks if correct
        Right_word1 = strcmp(Word_guess1, Word_1);
        Right_word2 = strcmp(Word_guess2, Word_2);
        Right_word3 = strcmp(Word_guess3, Word_3);
        Right_word4 = strcmp(Word_guess4, Word_4);
    }
    
    Sleep(300); //Pause a sec before opening next page
    if (timer == 0){ //You lost screen
        LCD.Clear();
        lose7();
    }
    else{
        LCD.Clear();
        win7(timer);
    }
}






void statsScreen()
{
    // Ensure proper initialization of the screen
    LCD.Clear();
    LCD.SetBackgroundColor(WHITE);
    LCD.SetFontColor(BLACK);
    LCD.Update();
    Sleep(0.1);

    // Draw content for the Stats screen
    
    LCD.WriteAt("STATS", 120, 10);
    
    //Write all stats
    LCD.WriteAt("Wins for 5 Letters: ", 5, 35);
    LCD.WriteAt("Quickest Solve (5): ", 5, 70);
    LCD.WriteAt("Wins for 6 Letters: ", 5, 105);
    LCD.WriteAt("Quickest Solve (6): ", 5, 140);
    LCD.WriteAt("Wins for 7 Letters: ", 5, 175);
    LCD.WriteAt("Quickest Solve (7): ", 5, 210);
    LCD.WriteAt(fiveWins, 250, 35);
    LCD.WriteAt(sixWins, 250, 105);
    LCD.WriteAt(sevenWins, 250, 175);

    //This makes sure that if you haven't solve that difficulty yet, a "pr" doesn't show
    if(fivePR != 90){
    LCD.WriteAt(fivePR, 250, 70);
    }
    
    if(sixPR != 90){
    LCD.WriteAt(sixPR, 250, 140);
    }
    
    if(sevenPR != 90){
    LCD.WriteAt(sevenPR, 250, 210);
    }
    

    // Wait for the back button
    goBack();
}





void creditsScreen()
{
    // Ensure proper initialization of the screen
    LCD.Clear();
    LCD.SetBackgroundColor(WHITE);
    LCD.SetFontColor(BLACK);
    LCD.Update();
    Sleep(0.1);

    // Draw content for the Credits screen
    LCD.WriteLine(" ");
    LCD.WriteLine("         CREDITS");
    LCD.WriteLine(" ");
    LCD.WriteLine("       Created by");
    LCD.WriteLine("  Avyukth Sai Rangarajan");
    LCD.WriteLine("     and Max Arimany");
    LCD.Update();

    // Wait for the back button
    goBack();
}



void instructionsScreen()
{
    // Ensure proper initialization of the screen
    LCD.Clear();
    LCD.SetBackgroundColor(WHITE);
    LCD.SetFontColor(BLACK);
    LCD.Update();
    Sleep(0.1);

    // Draw content for the Credits screen
    LCD.WriteLine("       Instructions:");
    LCD.WriteLine(" ");
    LCD.WriteLine("After picking the ");
    LCD.WriteLine("difficulty, your job is");
    LCD.WriteLine("to unscramble the four");
    LCD.WriteLine("words given to you in 90");
    LCD.WriteLine("seconds.");
    LCD.WriteLine(" ");
    LCD.WriteLine("Click the letters to ");
    LCD.WriteLine("select them and click");
    LCD.WriteLine("into the box to place it.");
    LCD.WriteLine(" ");
    LCD.WriteLine("Have fun!");
    LCD.Update();

    // Wait for the back button
    goBack();
}




void goBack()
{
    float x_coor, y_coor;

    // import "Back" button
    FEHImage backArrow;
    backArrow.Open("backArrow.png");
   
   // Draw button
    backArrow.Draw(10, 5);
    LCD.Update();

    int waitingForBack = 1;
    while (waitingForBack) //Wait until click
    {
        if (LCD.Touch(&x_coor, &y_coor)) //checks for click
        {
            Sleep(0.1); // Debounce the touch
            if ((x_coor >= 5 && x_coor <= 45) && (y_coor >= 5 && y_coor <= 25))
            {
                waitingForBack = 0; // Exit when "Back" is pressed
            }
        }
    }

    // Clear any lingering touch before returning
    while (LCD.Touch(&x_coor, &y_coor));
}




//scrambles up 4 different 5-letter words
void scramble5(char Word[], char ScrambledWord[]){
    struct random_letters letter; //Initialize struct

    letter.first = rand() % 5;
    ScrambledWord[0] = Word[letter.first];
 
    do{
        letter.second =  rand() % 5;    
    }while(letter.second == letter.first);
    ScrambledWord[1] = Word[letter.second];

    do{
        letter.third = rand() % 5;
    }while(letter.third == letter.first || letter.third == letter.second);
    ScrambledWord[2] = Word[letter.third];

    do{
        letter.fourth = rand() % 5;
    }while(letter.fourth == letter.first || letter.fourth == letter.second || letter.fourth == letter.third);
    ScrambledWord[3] = Word[letter.fourth];

    do{
        letter.fifth = rand() % 5;
    }while(letter.fifth == letter.first || letter.fifth == letter.second || letter.fifth == letter.third || letter.fifth == letter.fourth);
    ScrambledWord[4] = Word[letter.fifth];

    ScrambledWord[5] = '\0';
}

//scrambles up 4 different 6-letter words
void scramble6(char Word[], char ScrambledWord[]){
    struct random_letters letter; //Initialize struct

    letter.first = rand() % 6;
    ScrambledWord[0] = Word[letter.first];
 
    do{
        letter.second =  rand() % 6;    
    }while(letter.second == letter.first);
    ScrambledWord[1] = Word[letter.second];

    do{
        letter.third = rand() % 6;
    }while(letter.third == letter.first || letter.third == letter.second);
    ScrambledWord[2] = Word[letter.third];

    do{
        letter.fourth = rand() % 6;
    }while(letter.fourth == letter.first || letter.fourth == letter.second || letter.fourth == letter.third);
    ScrambledWord[3] = Word[letter.fourth];

    do{
        letter.fifth = rand() % 6;
    }while(letter.fifth == letter.first || letter.fifth == letter.second || letter.fifth == letter.third || letter.fifth == letter.fourth);
    ScrambledWord[4] = Word[letter.fifth];

    do{
        letter.sixth = rand() % 6;
    }while(letter.sixth == letter.first || letter.sixth == letter.second || letter.sixth == letter.third || letter.sixth == letter.fourth || letter.sixth == letter.fifth);
    ScrambledWord[5] = Word[letter.sixth];

    ScrambledWord[6] = '\0';
}

//scrambles up 4 different 7-letter words

void scramble7(char Word[], char ScrambledWord[]){
    struct random_letters letter; //Initialize struct

    letter.first = rand() % 7;
    ScrambledWord[0] = Word[letter.first];
 
    do{
        letter.second =  rand() % 7;    
    }while(letter.second == letter.first);
    ScrambledWord[1] = Word[letter.second];

    do{
        letter.third = rand() % 7;
    }while(letter.third == letter.first || letter.third == letter.second);
    ScrambledWord[2] = Word[letter.third];

    do{
        letter.fourth = rand() % 7;
    }while(letter.fourth == letter.first || letter.fourth == letter.second || letter.fourth == letter.third);
    ScrambledWord[3] = Word[letter.fourth];

    do{
        letter.fifth = rand() % 7;
    }while(letter.fifth == letter.first || letter.fifth == letter.second || letter.fifth == letter.third || letter.fifth == letter.fourth);
    ScrambledWord[4] = Word[letter.fifth];

    do{
        letter.sixth = rand() % 7;
    }while(letter.sixth == letter.first || letter.sixth == letter.second || letter.sixth == letter.third || letter.sixth == letter.fourth || letter.sixth == letter.fifth);
    ScrambledWord[5] = Word[letter.sixth];

    do{
        letter.seventh = rand() % 7;
    }while(letter.seventh == letter.first || letter.seventh == letter.second || letter.seventh == letter.third || letter.seventh == letter.fourth || letter.seventh == letter.fifth || letter.seventh == letter.sixth);
    ScrambledWord[6] = Word[letter.seventh];

    ScrambledWord[7] = '\0';
}


void win5(int timer){
    //increment wins
    fiveWins++;

    //if the new time is less than the previous PR, make this the new PR
    if(90-timer < fivePR){
        fivePR = 90-timer;
    }
    LCD.Clear();
    LCD.SetBackgroundColor(MEDIUMAQUAMARINE); 
    LCD.SetFontColor(BLACK);
    LCD.Clear();
    LCD.Update();
    Sleep(2.0);

    LCD.WriteAt("You win!", 100,100);
    
    LCD.DrawRectangle(50, 180, 80, 40);
    LCD.WriteAt("Menu", 63, 192);

    LCD.DrawRectangle(190, 180, 80, 40);
    LCD.WriteAt("Replay", 193, 192);

    float x_coor, y_coor;

    
    
    int waitingForClick = 1;
    while (waitingForClick)
    {
        if (LCD.Touch(&x_coor, &y_coor))
        {
            Sleep(0.1); // Debounce the touch
            if ((x_coor >= 50 && x_coor <= 130) && (y_coor >= 180 && y_coor <= 220))
            {
              waitingForClick = 0;
            }
            if((x_coor >= 190 && x_coor <= 270) && (y_coor >= 180 && y_coor <= 220)){
                fiveRandomize();

            }
        }
    } 
}
void lose5(){
    LCD.Clear();
    LCD.SetBackgroundColor(MEDIUMAQUAMARINE); 
    LCD.SetFontColor(BLACK);
    LCD.Clear();
    LCD.Update();
    Sleep(2.0);

    LCD.WriteAt("You lost.", 100,100);
    
    LCD.DrawRectangle(50, 180, 80, 40);
    LCD.WriteAt("Menu", 63, 192);

    LCD.DrawRectangle(190, 180, 80, 40);
    LCD.WriteAt("Replay", 193, 192);

    float x_coor, y_coor;

    
    
    int waitingForClick = 1;
    while (waitingForClick)
    {
        if (LCD.Touch(&x_coor, &y_coor))
        {
            Sleep(0.1); // Debounce the touch
            if ((x_coor >= 50 && x_coor <= 130) && (y_coor >= 180 && y_coor <= 220))
            {
                waitingForClick = 0;
            }
            if((x_coor >= 190 && x_coor <= 270) && (y_coor >= 180 && y_coor <= 220)){
                fiveRandomize();

            }
        }
    }
}



void win6(int timer){
    sixWins++;
    if(90-timer < sixPR){
        sixPR = 90-timer;
    }
    LCD.Clear();
    LCD.SetBackgroundColor(MEDIUMAQUAMARINE); 
    LCD.SetFontColor(BLACK);
    LCD.Clear();
    LCD.Update();
    Sleep(2.0);

    LCD.WriteAt("You win!", 100,100);
    
    LCD.DrawRectangle(50, 180, 80, 40);
    LCD.WriteAt("Menu", 63, 192);

    LCD.DrawRectangle(190, 180, 80, 40);
    LCD.WriteAt("Replay", 193, 192);

    float x_coor, y_coor;

    
    
    int waitingForClick = 1;
    while (waitingForClick)
    {
        if (LCD.Touch(&x_coor, &y_coor))
        {
            Sleep(0.1); // Debounce the touch
            if ((x_coor >= 50 && x_coor <= 130) && (y_coor >= 180 && y_coor <= 220))
            {
              waitingForClick = 0;
            }
            if((x_coor >= 190 && x_coor <= 270) && (y_coor >= 180 && y_coor <= 220)){
                sixRandomize();

            }
        } 
    }
}
void lose6(){
    LCD.Clear();
    LCD.SetBackgroundColor(MEDIUMAQUAMARINE); 
    LCD.SetFontColor(BLACK);
    LCD.Clear();
    LCD.Update();
    Sleep(2.0);

    LCD.WriteAt("You lost.", 100,100);
    
    LCD.DrawRectangle(50, 180, 80, 40);
    LCD.WriteAt("Menu", 63, 192);

    LCD.DrawRectangle(190, 180, 80, 40);
    LCD.WriteAt("Replay", 193, 192);

    float x_coor, y_coor;

    
    
    int waitingForClick = 1;
    while (waitingForClick)
    {
        if (LCD.Touch(&x_coor, &y_coor))
        {
            Sleep(0.1); // Debounce the touch
            if ((x_coor >= 50 && x_coor <= 130) && (y_coor >= 180 && y_coor <= 220))
            {
              waitingForClick = 0;
            }
            if((x_coor >= 190 && x_coor <= 270) && (y_coor >= 180 && y_coor <= 220)){
                sixRandomize();

            }
        } 
    }
}




void win7(int timer){
    if(90-timer < sevenPR){
        sevenPR = 90-timer;
    }
    sevenWins++;
    LCD.Clear();
    LCD.SetBackgroundColor(MEDIUMAQUAMARINE); 
    LCD.SetFontColor(BLACK);
    LCD.Clear();
    LCD.Update();
    Sleep(2.0);

    LCD.WriteAt("You win!", 100,100);
    
    LCD.DrawRectangle(50, 180, 80, 40);
    LCD.WriteAt("Menu", 63, 192);

    LCD.DrawRectangle(190, 180, 80, 40);
    LCD.WriteAt("Replay", 193, 192);

    float x_coor, y_coor;

    
    
    int waitingForClick = 1;
    while (waitingForClick)
    {
        if (LCD.Touch(&x_coor, &y_coor))
        {
            Sleep(0.1); // Debounce the touch
            if ((x_coor >= 50 && x_coor <= 130) && (y_coor >= 180 && y_coor <= 220))
            {
              waitingForClick = 0;
            }
            if((x_coor >= 190 && x_coor <= 270) && (y_coor >= 180 && y_coor <= 220)){
                sevenRandomize();

            }
        }   
    }
}
void lose7(){
    LCD.Clear();
    LCD.SetBackgroundColor(MEDIUMAQUAMARINE); 
    LCD.SetFontColor(BLACK);
    LCD.Clear();
    LCD.Update();
    Sleep(2.0);

    LCD.WriteAt("You lost.", 100,100);
    
    LCD.DrawRectangle(50, 180, 80, 40);
    LCD.WriteAt("Menu", 63, 192);

    LCD.DrawRectangle(190, 180, 80, 40);
    LCD.WriteAt("Replay", 193, 192);

    float x_coor, y_coor;

    
    
    int waitingForClick = 1;
    while (waitingForClick)
    {
        if (LCD.Touch(&x_coor, &y_coor))
        {
            Sleep(0.1); // Debounce the touch
            if ((x_coor >= 50 && x_coor <= 130) && (y_coor >= 180 && y_coor <= 220))
            {
              waitingForClick = 0;
            }
            if((x_coor >= 190 && x_coor <= 270) && (y_coor >= 180 && y_coor <= 220)){
                sevenRandomize();

            }
        } 
    }
}