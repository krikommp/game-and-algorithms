#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

constexpr int xoffset = 10;
constexpr int yoffset = 10;

class BreakOut : public olc::PixelGameEngine
{
public:
	BreakOut()
	{
		sAppName = "TUTORIAL - BreakOut Clone";
	}

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Erase previous frame
		Clear(olc::DARK_BLUE);

		// Draw Boundary
		DrawLine(xoffset, yoffset, ScreenWidth() - xoffset, yoffset, olc::YELLOW);
		DrawLine(xoffset, yoffset, xoffset, ScreenHeight() - yoffset, olc::YELLOW);
		DrawLine(ScreenWidth() - xoffset, yoffset, ScreenWidth() - xoffset, ScreenHeight() - yoffset, olc::YELLOW);
		return true;
	}
};

int main()
{
	BreakOut demo;
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();
	return 0;
}
