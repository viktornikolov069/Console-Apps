#include <conio.h> 
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//-------------------------------------------------------------------------------------------               
// I have heard that using the hole namespace with all it's components is a bad practice
// but in this case, mainly for ease of use I will use it.
//-------------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------------               
// GLOBALS
//-------------------------------------------------------------------------------------------

// Player names
string Player_1;
string Player_2;

// Player scores
int nPBulls = 0;
int nPCows = 0;

// The numbers that the players choose in the begining
char P1Number[5];
char P2Number[5];

int P1Attempts = 0;
int P2Attempts = 0;

// The guess number of each player
char PGuess[5];

int GameCouter = 0;

//-------------------------------------------------------------------------------------------     
// FUNCTION PROTOTYPING           
//-------------------------------------------------------------------------------------------               

// Shows you the rules
void ReadRules();

// The LoadStats function loads the data saved by the SaveData functions
void LoadStats();

// The SaveDataInGame function saves the data that is changing while the game is running
void SaveDataInGame(string player, char* guess, int bulls, int cows, int attempt);

// The CheckNumber function checks if the entered number falls within the requirements
bool CheckNumber(char* aP);

// The GuessNumber function compares the guess number of Player_1 with the original number of Player_2
// After comparison the function gives values to the Cows and Bulls variables
void GuessNumber(char* aPG, char* aPN);

// The TryToGuess function is a wrapper function it uses the functions: CheckNumber, GuessNumber, SaveDataInGame
void TryToGuess(string sPlayerA, string sPlayerB, char* aPNumber, int& nPAttempts);

// PlayGame has lots of game realeated text inside it also uses the TryToGuess function
void PlayGame();

// The SaveDataBeforeGame function saves the: player names, player starting numbers, game counter 
// The (counter) parameter keeps track of the number of new games played
void SaveDataBeforeGame(string Player1, string Player2, char* P1Number, char* P2Number, int counter);

// The ChooseNumber functon let's the player choose the number that his opponent will have to guess
void ChooseNumber(char* aP);

// The IniateGame function calls the ChooseNumber and the PlayGame functions
void InitiateGame();


//-------------------------------------------------------------------------------------------    
// I have tried to sort the functions in a hierarchical order.         
//-------------------------------------------------------------------------------------------

int main()
{
    char CHOICE[10];

    while (CHOICE[0] != 'q')
    {
        cout << "\n\t\t=============== Main Menu ================";
        cout << "\n\t\t|                                        |";
        cout << "\n\t\t|          (R)ead the Rules              |";
        cout << "\n\t\t|          (N)ew Game                    |";
        cout << "\n\t\t|          (L)oad Stats                  |";
        cout << "\n\t\t|          (Q)uit                        |";
        cout << "\n\t\t|                                        |";
        cout << "\n\t\t==========================================" << "\n\n";;

        cin >> CHOICE;

        switch (CHOICE[0])
        {
        case 'r': ReadRules(); break;
        case 'n': InitiateGame(); break;
        case 'l': LoadStats(); break;
        case 'q': break;
        default: "\n\tInvalid input!";
        }
        system("PAUSE");
        system("cls");
    }
}

//-------------------------------------------------------------------------------------------      
//-------------------------------------------------------------------------------------------    

void InitiateGame()
{
    GameCouter++;
    int P1Attempts = 0;
    int P2Attempts = 0;
    cout << "\n\t\tPlayer 1. Please enter your name.\n";
    cin >> Player_1;

    cout << "\t\tPlayer 2. Please enter your name.\n";
    cin >> Player_2;

    cout << "\t\t" << Player_1 << " please enter your number.\n";
    ChooseNumber(P1Number);

    cout << "\t\t" << Player_2 << " please enter your number.\n";
    ChooseNumber(P2Number);

    SaveDataBeforeGame(Player_1, Player_2, P1Number, P2Number, GameCouter);

    PlayGame();
}

//-------------------------------------------------------------------------------------------        
// The function _getch() as well as the <conio.h> library are usually not recommended to be used 
// because of security reasons and also because not all compilers support them thus limiting 
// code portability. 
// 
// But I decided to use them becuase they solve a certain problem of the game.
//-------------------------------------------------------------------------------------------

void ChooseNumber(char* aP)
{
    for (int i = 0; i < 4; i++)
    {
        aP[i] = _getch(); // (_getch) = Reading one character at a time without having to press the Enter key

        cout << '*';
    }
    cout << endl;
    if (!CheckNumber(aP))
    {
        ChooseNumber(aP);
    }

}

//-------------------------------------------------------------------------------------------               
//-------------------------------------------------------------------------------------------    

void PlayGame()
{
    TryToGuess(Player_1, Player_2, P2Number, P1Attempts);

    if (nPBulls == 4)
    {
        cout << "\t" << Player_1 << " has 4 Bulls and will win if " << Player_2 << " fails to guess correctly.\n";

        TryToGuess(Player_2, Player_1, P1Number, P2Attempts);

        if (nPBulls == 4)
        {
            cout << "\n\t\tThe game has ended in a tie.\n";
            cout << "\t\t" << Player_1 << " ended in " << P1Attempts << " attempts.\n";
            cout << "\t\t" << Player_2 << " ended in " << P2Attempts << " attempts.\n";
            return;
        }
        else
        {
            cout << "\n\t\t" << Player_1 << " won the game in " << P2Attempts << " attempts.\n";
            return;
        }
    }
    else
    {
        cout << "\t\t" << Player_1 << " has Bulls = " << nPBulls << " Cows = " << nPCows << "\n";
    }

    TryToGuess(Player_2, Player_1, P1Number, P2Attempts);

    if (nPBulls == 4)
    {
        cout << "\t\t" << Player_2 << " has 4 Bulls and will win if " << Player_1 << " fails to guess correctly.\n";

        TryToGuess(Player_1, Player_2, P2Number, P1Attempts);

        if (nPBulls == 4)
        {
            cout << "\n\t\tThe game has ended in a tie.\n";
            cout << "\t\t" << Player_1 << " ended in " << P1Attempts << " attempts.\n";
            cout << "\t\t" << Player_2 << " ended in " << P2Attempts << " attempts.\n";
            return;
        }
        else
        {
            cout << "\t\t" << Player_2 << " won the game in " << P2Attempts << " attempts.\n";
        }
    }
    else
    {
        cout << "\t\t" << Player_2 << " has Bulls = " << nPBulls << " Cows = " << nPCows << "\n";
        PlayGame();
    }
}

//-------------------------------------------------------------------------------------------           
// The comments bellow are some thoughts of mine and are releated to the TryToGuess function. 
//-------------------------------------------------------------------------------------------


/* Why does &nPAttempts work and *nPAttempts doesn't.
   Why does *nPNumber work and &nPNumber doesn't.
   So P2Number and P1Number are char[] and they are in some way addresses. That's why when you pass them to the parameter *nPAttempts because of (*) it's expecting an address and that is why it works. *nPAttempts means expecting an adress.
   *nPNumber means expecting an adress.
   &nPAttempts means to take address. Because of this we can change the value of P1/P2Attempts. Because we are using the adress directly.
   If we try to write the parameter *nPAttempts it will be waiting for an adress but the value that we will be giving it is P1/P2Attempts which is an integer value not an adress and that's why it will give us an error in the function call.
   If we want to pass P1/P2Attempts to the parameter *nPAttempts which in this case will be waiting for an adress then we must write the passing value in this way &P1/&P2Attempts which will then be an adress.
*/


//-------------------------------------------------------------------------------------------               
//-------------------------------------------------------------------------------------------          


void TryToGuess(string sPlayerA, string sPlayerB, char* aPNumber, int& nPAttempts)
{
    cout << "\n\t\t" << sPlayerA << " Try to guess " << sPlayerB << "'s number.\n";
    cin >> PGuess;

    if (!CheckNumber(PGuess))
    {
        TryToGuess(sPlayerA, sPlayerB, aPNumber, nPAttempts);
    }
    else
    {
        nPAttempts++;
        GuessNumber(PGuess, aPNumber);
        SaveDataInGame(sPlayerA, PGuess, nPBulls, nPCows, nPAttempts);
    }
}

//-------------------------------------------------------------------------------------------               
//-------------------------------------------------------------------------------------------

void GuessNumber(char* aPG, char* aPN)
{
    // aPG = (array) Player Guess
    // aPN = (array) Player Number

    int nCows = 0;
    int nBulls = 0;
    for (int i = 0; i < 4; i++)
    {
        if (aPG[i] == aPN[i])
        {
            nBulls++;
        }
        else
        {
            for (int x = 0; x < 4; x++)
            {
                if (aPG[i] == aPN[x])
                {
                    nCows++;
                    break;
                }
            }
        }
    }
    nPBulls = nBulls;
    nPCows = nCows;
}

//-------------------------------------------------------------------------------------------               
//-------------------------------------------------------------------------------------------             


bool CheckNumber(char* aP)
{
    // aP = (array) Player
    if (strlen(aP) > 4)
    {
        cout << "\t\tEnter a 4 digit number!!!\n";
        cout << "\t\tTry again.\n";
        return false;
    }
    for (int i = 0; i < 4; i++)
    {
        if (!isdigit(aP[i]))
        {
            cout << "\t\tUse only digits!!!\n";
            cout << "\t\tTry again.\n";
            return false;
        }
        for (int x = 0; x < 4; x++)
        {
            if (i != x)
            {
                if (aP[i] == aP[x])
                {
                    cout << "\t\tDo not repeat digits!!!\n";
                    cout << "\t\tTry again.\n";
                    return false;
                }
            }

        }
    }
    if (aP[0] == '0')
    {
        cout << "\t\tYour number should not begin with 0!!!\n";
        cout << "\t\tTry again.\n";
        return false;
    }
    return true;
}

//-------------------------------------------------------------------------------------------               
//-------------------------------------------------------------------------------------------            

void SaveDataInGame(string player, char* guess, int bulls, int cows, int attempt)
{
    try
    {
        ofstream DATAFILE;
        DATAFILE.open("DATA2.FILE", ios::app); // with ios::app (Append mode) you can save new data without deleating the previous data.

        DATAFILE << "Name = " << player << "\n";
        DATAFILE << "Guess = " << guess << "\n";
        DATAFILE << "Bulls = " << bulls << "\n";
        DATAFILE << "Cows = " << cows << "\n";
        DATAFILE << "Attempt = " << attempt << "\n";


        DATAFILE.close();
    }
    catch (exception X)
    {
        cout << "\n\tFile Error! Could not SAVE PERSON.\n";
    }
}

//-------------------------------------------------------------------------------------------               
//-------------------------------------------------------------------------------------------            

void SaveDataBeforeGame(string Player1, string Player2, char* P1Number, char* P2Number, int counter)
{
    try
    {
        ofstream DATAFILE;
        DATAFILE.open("DATA2.FILE", ios::app); // with ios::app (Append mode) you can save new data without deleating the previous data.

        DATAFILE << "Player 1 = " << Player1 << "\n";
        DATAFILE << "Player 1 Number = " << P1Number << "\n";
        DATAFILE << "Player 2 = " << Player2 << "\n";
        DATAFILE << "Player 2 Number = " << P2Number << "\n";
        DATAFILE << "Game = " << counter << "\n";

        DATAFILE.close();
    }
    catch (exception X)
    {
        cout << "\n\tFile Error! Could not SAVE PERSON.\n";
    }
}

//-------------------------------------------------------------------------------------------               
//-------------------------------------------------------------------------------------------               

void LoadStats()
{
    try
    {
        ifstream DATAFILE;
        DATAFILE.open("DATA2.FILE", ios::in);
        string line;
        cin.ignore();
        while (getline(DATAFILE, line))
        {
            cout << line << '\n';
        }
        DATAFILE.close();
    }
    catch (exception X)
    {
        cout << "\n\tFile Error! Unable to LOAD data.";
    }
}

void ReadRules()
{
    cout << "\n\t\t ======================The Rules =======================";
    cout << "\n\t\t|              Welcome to Cows and Bulls.               |";
    cout << "\n\t\t|  Cows and Bulls is a two player game.                 |";
    cout << "\n\t\t|  Each player should enter a 4-four digit secret number|";
    cout << "\n\t\t|  (without the other player seeing it).                |";
    cout << "\n\t\t|  -The digits must all be different                    |";
    cout << "\n\t\t|  -and the first digit cannot be 0-zero.               |";
    cout << "\n\t\t|  Then in turn the players try to guess                |";
    cout << "\n\t\t|  their opponent's number afterwhich                   |";
    cout << "\n\t\t|  they get a number of mathes.                         |";
    cout << "\n\t\t|  If the matching digits are in their                  |";
    cout << "\n\t\t|  right positions, they are \"bulls\".                   |";
    cout << "\n\t\t|  If in different positions                            |";
    cout << "\n\t\t|  they are \"cows\".                                     |";
    cout << "\n\t\t|                                                       |";
    cout << "\n\t\t|  Example:                                             |";
    cout << "\n\t\t|  -> Secret Number = 1462                              |";
    cout << "\n\t\t|  -> Opponent's try = 1234                             |";
    cout << "\n\t\t|  -> Answer: 1 bull and 2 cows.                        |";
    cout << "\n\t\t|                                                       |";
    cout << "\n\t\t|  (The bull is \"1\" the cows are \"2\" and \"4\")           |";
    cout << "\n\t\t|  The fisrt player to reveal the other's secret number |";
    cout << "\n\t\t|  in the fewest guesses wins only if the other player  |";
    cout << "\n\t\t|  fails to guess the number on the next turn           |";
    cout << "\n\t\t|  otherwise it's a tie.                                |";
    cout << "\n\t\t =======================================================\n";
}

//-------------------------------------------------------------------------------------------
// FINAL NOTES
//-------------------------------------------------------------------------------------------      
// Further improvements are needed.  
// 
// 1. All of the code is in a single cpp file which is not good.
// 
// 2. It would be much better if I could use a data structure like a class to store my data in 
//    insted of using global variables but I'm still running into problems with that.
//  
// 3. I have not been able to implement the saving of progress after closing the console.
// 
// 4. Displaying the loaded data from the LoadStats function is not good.
// 
// 5. I haven't done the computer player function yet.
//------------------------------------------------------------------------------------------- 