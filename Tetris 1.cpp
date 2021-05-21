#include <iostream>
#include <thread>
#include <Windows.h>


using namespace std;

wstring tetromino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
int nScreenWidth = 80;
int nScreenHeight = 30;
unsigned char *pField = nullptr;

int Rotate(int px, int py, int r) // so the rotate function works as a loop because
{								  // 0 % 4 = 0, 
								  // 1 % 4 = 1,
								  // 2 % 4 = 2..., 4 % 4 = 0, 5 % 4 = 1... and so on.
	switch (r % 4)
	{
	case 0: return (py * 4 + px);
	case 1: return 12 + py - (px * 4);  
	case 2: return 15 - (py * 4) - px;
	case 3: return 3 - py + (px * 4);
	}

	return 0;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY) //nPosX and nPosY is the top left part of the nTetromino
{
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++)
		{

			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px); 

			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
						return false; // fail on first hit
				}
			}

		}
	return true;

}
int main()
{

	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L"....");

	tetromino[2].append(L"..X.");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

	tetromino[3].append(L"..X.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".X..");
	tetromino[3].append(L"....");

	tetromino[4].append(L".XX.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"....");
	tetromino[4].append(L"....");

	tetromino[5].append(L".X..");
	tetromino[5].append(L".X..");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"....");

	tetromino[6].append(L".X..");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L"..X.");
	tetromino[6].append(L"....");

	pField = new unsigned char[nFieldHeight * nFieldWidth]; // Create playing field buffer

	for (int x = 0; x < nFieldWidth; x++)
		for (int y = 0; y < nFieldHeight; y++)
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;

	wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	bool bGameOver = false;

	int nCurrentPiece = 0; // What current piece is falling 

	int nCurrentRotation = 0; // Is it rotated and to what angle

	int nCurrentX = nFieldWidth / 2; // It falls from the center

	int nCurrentY = 0; // At the top
	
	bool bKey[5];
	bool bRotateHold = false; // This so called flag suggests whether the users is holding the rotate key or not. 
							  // Because of the 50ms time laps when we are holding the key the program tries to enforce a rotaion.

	int nSpeed = 15;
	int nSpeedCounter = 0;
	bool bForceDown = false;

	while (!bGameOver)
	{
		// GAME TIMING ========================================= for consistent playing experience
		this_thread::sleep_for(50ms); // Game Tick
		nSpeedCounter++;
		bForceDown = (nSpeedCounter == nSpeed);


		// INPUT ===============================================
		for (int k = 0; k < 4; k++)                              // R  L    D Z
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z\x26"[k]))) != 0;

		// GAME LOGIC ========================================== shapes falling, collision detection, scoring

		nCurrentX += (bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;
		nCurrentX -= (bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;
		nCurrentY += (bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;


		if (bKey[3])
		{
			nCurrentRotation += (!bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1 , nCurrentX, nCurrentY));
			bRotateHold = true;
		}
		else
		{
			bRotateHold = false;
		}
		 
		if (bForceDown)
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
				nCurrentY++; // It can so do it!
			else
			{
				// Lock the current piece in to the field
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
							pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;

				// Check have we got any lines

				// Chose next piece

				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;

				// If piece does not fit
				bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}

			nSpeedCounter = 0;
		}
	
		// RENDER OUTPUT ======================================= 


		// Draw Field
		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];

		// Draw Current Piece
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
				if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;


		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);

	}



	return 0;
}