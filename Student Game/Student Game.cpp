// Student Game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "iostream"
#include "string"
#include "stdlib.h"
#include "time.h"
#include "windows.h"
#include "mmsystem.h"

#include "gtest/gtest.h"

using namespace std;

int loc[4][2];
int total = 0;
// 0 student - S
// 1 ghost - O
// 2 treasure - T
// 3 goblin - G
// index 0 for x-axis, index 1 for y axis // op

string input, msg, inMsg;

bool endGame, treasure = false;

namespace student_game
{

	void map() {
		for (int x = 0; x < 10; x++) {
			for (int y = 0; y < 10; y++) {
				if (y == loc[0][0] && x == loc[0][1]) cout << " S";
				else if (y == loc[1][0] && x == loc[1][1]) cout << " O";
				else if (y == loc[2][0] && x == loc[2][1]) cout << " T";
				else if (y == loc[3][0] && x == loc[3][1]) cout << " G";
				else cout << " _";
			}
			cout << "\n";
		}

		//check

		// check ghost
		if (loc[0][0] == loc[1][0] && loc[0][1] == loc[1][1]) {
			cout << endl << "Killed by Ghost !!" << endl;
			cout << endl << "You Lose !!" << endl;
			endGame = true;
		}

		// check treasure
		if (loc[0][0] == loc[2][0] && loc[0][1] == loc[2][1]) {
			treasure = true; loc[2][1] = 99;

		}

		// check goblin
		if (loc[0][0] == loc[3][0] && loc[0][1] == loc[3][1]) {
			// have treasure
			if (treasure) {
				cout << endl << "You Win !!" << endl;
				endGame = true;
			}
			// don't have treasure'
			else {
				cout << endl << "Killed by Ghost !!" << endl;
				cout << endl << "You Lose !!" << endl;
				endGame = true;
			}
		}
	}

	void move(int &x, int &y, int xx, int yy, bool ghost) {

		x += xx;
		y += yy;

		//check if try to move out of bound
		if (x < 0 || x > 9 || y < 0 || y > 9) {
			if (ghost) { msg = "Ghost bump into a wall"; }
			else {
				msg += "\nNo more room to move !!";
			}
		}

		if (!ghost) inMsg = "Input direction : ";

		//correcting location
		if (x < 0) x = 0;
		if (x > 9) x = 9;
		if (y < 0) y = 0;
		if (y > 9) y = 9;

		system("cls");
		map();

	}

	void ghostMove()
	{
		int dir = rand() % 4;
		//0 = w, 1 = a , 2 = s, 3 = d

		//up = north
		if (dir == 0) {
			move(loc[1][0], loc[1][1], 0, -1, true);
		}
		//left = west
		else if (dir == 1) {
			move(loc[1][0], loc[1][1], -1, 0, true);
		}
		//down = south
		else if (dir == 2) {
			move(loc[1][0], loc[1][1], 0, 1, true);
		}
		//right = east
		else if (dir == 3) {
			move(loc[1][0], loc[1][1], 1, 0, true);
		}

	}
}


int main(int argc, char **argv)
{
	srand(time(NULL));

	// Run UT, otherwise play the game
	if (argc > 1 && !strcmp(argv[1], "test"))
	{
		// Mute output for clarity
		if (argc > 2 && !strcmp(argv[2], "short"))
		{
			cout.rdbuf(NULL);
		}
		::testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
	}

	//init
	//rand student
	loc[0][0] = rand() % 10;
	loc[0][1] = rand() % 10;
	//rand ghost
	loc[1][0] = rand() % 8 + 1;
	loc[1][1] = rand() % 8 + 1;
	//rand treasure
	loc[2][0] = rand() % 10;
	loc[2][1] = rand() % 10;
	//rand goblin
	loc[3][0] = rand() % 10;
	loc[3][1] = rand() % 10;

	inMsg = "Input direction : ";

	//run game
	do {
		student_game::ghostMove();

		//check near goblin
		total = abs(loc[0][0] - loc[3][0]) + abs(loc[0][1] - loc[3][1]);
		if (total == 1) { cout << "mumbling about Treasure" << endl; }

		//check near ghost
		total = abs(loc[0][0] - loc[1][0]) + abs(loc[0][1] - loc[1][1]);
		if (total == 1) { cout << "falling coins" << endl; }

		cout << msg << endl;
		msg = "";

		//get input
		if (!endGame) {
		cout << inMsg;
		cin >> input;
		}


		//up = north
		if (input.compare("N")==0 ){
			student_game::move(loc[0][0], loc[0][1], 0, -1, false);
		}
		//left = west
		else if (input.compare("W") == 0 ) {
			student_game::move(loc[0][0], loc[0][1], -1, 0, false);
		}
		//down = south
		else if (input.compare("S") == 0 ) {
			student_game::move(loc[0][0], loc[0][1], 0, 1, false);
		}
		//right = east
		else if (input.compare("E") == 0 ) {
			student_game::move(loc[0][0], loc[0][1], 1, 0, false);
		}
		else { inMsg = "Please enter N,S,W,E : "; }

	} while (!endGame);
	system("pause");

	return 0;
}

//--------------------------------------------------------------------------------------
//                                    UNIT TESTS
//--------------------------------------------------------------------------------------

// Just make sure all works
TEST(Dummy, Dummy)
{
	EXPECT_EQ(true, true);
}

// Test random movements, make sure object stays inside the wall
TEST(MoveTest, RandomWall)
{
	for (int i = 0; i < 100; ++i)
	{
		student_game::move(loc[0][0], loc[0][1], (rand() % 20) - 10, (rand() % 20) - 10, false);
		cout << endl;

		EXPECT_LE(loc[0][0], 9);
		EXPECT_LE(loc[0][1], 9);
		EXPECT_GE(loc[0][0], 0);
		EXPECT_GE(loc[0][1], 0);
		msg.clear();
	}
}

// Test random movements, make sure object stays inside the wall
TEST(MoveTest, RandomWallGhost)
{
	int x = 0, y = 0;
	for (int i = 0; i < 100; ++i)
	{
		student_game::move(loc[1][0], loc[1][1], (rand() % 20) - 10, (rand() % 20) - 10, true);
		cout << endl;

		EXPECT_LE(loc[1][0], 9);
		EXPECT_LE(loc[1][1], 9);
		EXPECT_GE(loc[1][0], 0);
		EXPECT_GE(loc[1][1], 0);
		msg.clear();
	}
}

// Test random movements, make sure ghost always moves by 1 or 0 units (if against the wall)
TEST(GhostMoveTest, GhostMove)
{
	loc[1][0] = 0;
	loc[1][1] = 0;
	for (int i = 0; i < 200; ++i)
	{
		const int x_old = loc[1][0];
		const int y_old = loc[1][1];
		student_game::ghostMove();
		cout << endl;

		const int distance = abs(x_old - loc[1][0]) + abs(y_old - loc[1][1]);
		EXPECT_LE(distance, 1);
		EXPECT_GE(distance, 0);
	}
}

// Test movement by one unit
TEST(MoveTest, MoveByOne)
{
	int x = 0, y = 0;

	student_game::move(x, y, 1, 0, false);
	cout << endl;
	EXPECT_EQ(x, 1);
	EXPECT_EQ(y, 0);

	student_game::move(x, y, 0, 1, false);
	cout << endl;
	EXPECT_EQ(x, 1);
	EXPECT_EQ(y, 1);

	student_game::move(x, y, 1, 1, false);
	cout << endl;
	EXPECT_EQ(x, 2);
	EXPECT_EQ(y, 2);

	student_game::move(x, y, -1, 0, false);
	cout << endl;
	EXPECT_EQ(x, 1);
	EXPECT_EQ(y, 2);

	student_game::move(x, y, 0, -1, false);
	cout << endl;
	EXPECT_EQ(x, 1);
	EXPECT_EQ(y, 1);

	student_game::move(x, y, -1, -1, false);
	cout << endl;
	EXPECT_EQ(x, 0);
	EXPECT_EQ(y, 0);
}

// Test endgame condition
TEST(EndGameTest, GhostFalse)
{
	endGame = false;
	loc[3][0] = 9;
	loc[3][1] = 9;

	loc[0][0] = 0;
	loc[0][1] = 0;
	loc[1][0] = 1;
	loc[1][1] = 1;
	student_game::map();
	cout << endl;
	EXPECT_FALSE(endGame);

	endGame = false;
	loc[0][0] = 2;
	loc[0][1] = 3;
	loc[1][0] = 2;
	loc[1][1] = 2;
	student_game::map();
	cout << endl;
	EXPECT_FALSE(endGame);
}

// Test endgame condition
TEST(EndGameTest, GhostTrue)
{
	endGame = false;
	loc[3][0] = 9;
	loc[3][1] = 9;

	loc[0][0] = 1;
	loc[0][1] = 0;
	loc[1][0] = 1;
	loc[1][1] = 0;
	student_game::map();
	cout << endl;
	EXPECT_TRUE(endGame);

	loc[0][0] = 4;
	loc[0][1] = 5;
	loc[1][0] = 4;
	loc[1][1] = 5;
	student_game::map();
	cout << endl;
	EXPECT_TRUE(endGame);
}

// Test endgame condition - at the goblin
TEST(EndGameTest, Goblin)
{
	endGame = false;
	treasure = false;
	loc[1][0] = 1;
	loc[1][1] = 1;

	loc[3][0] = 5;
	loc[3][1] = 6;
	loc[0][0] = 5;
	loc[0][1] = 6;

	student_game::map();
	cout << endl;
	EXPECT_TRUE(endGame);

	endGame = false;
	treasure = true;
	loc[0][0] = 5;
	loc[0][1] = 6;
	loc[1][0] = 5;
	loc[1][1] = 6;
	student_game::map();
	cout << endl;
	EXPECT_TRUE(endGame);
}

// Test treasure condition
TEST(TreasureTest, TreasureTrue)
{
	endGame = false;
	treasure = false;
	loc[1][0] = 1;
	loc[1][1] = 0;
	loc[3][0] = 9;
	loc[3][1] = 9;


	loc[2][0] = 5;
	loc[2][1] = 6;
	loc[0][0] = 5;
	loc[0][1] = 6;

	student_game::map();
	cout << endl;
	EXPECT_TRUE(treasure);
	EXPECT_EQ(loc[2][1], 99);

	loc[2][0] = 1;
	loc[2][1] = 3;
	loc[0][0] = 1;
	loc[0][1] = 3;
	student_game::map();
	cout << endl;
	EXPECT_TRUE(treasure);
	EXPECT_EQ(loc[2][1], 99);

	EXPECT_FALSE(endGame);
}

// Test treasure condition
TEST(TreasureTest, TreasureFalse)
{
	endGame = false;
	treasure = false;
	loc[1][0] = 1;
	loc[1][1] = 0;
	loc[3][0] = 9;
	loc[3][1] = 9;


	loc[2][0] = 6;
	loc[2][1] = 6;
	loc[0][0] = 5;
	loc[0][1] = 6;

	student_game::map();
	cout << endl;
	EXPECT_FALSE(treasure);
	EXPECT_EQ(loc[2][1], 6);

	loc[2][0] = 1;
	loc[2][1] = 3;
	loc[0][0] = 1;
	loc[0][1] = 4;
	student_game::map();
	cout << endl;
	EXPECT_FALSE(treasure);
	EXPECT_EQ(loc[2][1], 3);

	EXPECT_FALSE(endGame);
}


