#pragma once

class Input
{
public:
	static int horizontal;
	static int vertical;

	static void InitInput(void* windowPtr);
	static void UpdateInput();
	static bool GetKey(int key);
private:
	Input();
};