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
private:
	float fBatPos = 20.f;
	float fBatWidth = 40.f;

	olc::vf2d vBall = { 200.f, 200.f };		// 小球位置
	olc::vf2d vBallVel = { 100.f, 100.f };		// 小球方向

	float fBatSpeed = 250.f;				// 每秒钟移动的距离

private:
	void UserController(float fElapsedTime) {
		if (GetKey(olc::Key::LEFT).bHeld) {
			fBatPos -= fBatSpeed * fElapsedTime;
		}
		if (GetKey(olc::Key::RIGHT).bHeld) {
			fBatPos += fBatSpeed * fElapsedTime;
		}

		if (fBatPos < 11.f) { fBatPos = 11.f; }
		if (fBatPos + fBatWidth >= (ScreenWidth() - 10.f)) { fBatPos = (ScreenWidth() - 10.f) - fBatWidth; }
	}

	void UpdateBall(float fElapsedTime) {
		vBall += vBallVel * fElapsedTime;

		// 发生碰撞就往相反方向继续运动
		if (vBall.x <= 10.f) {
			vBallVel.x *= -1.f; 
		}
		if (vBall.y <= 10.f) {
			vBallVel.y *= -1.f;
		}
		if (vBall.x >= float(ScreenWidth() - 10.f)) {
			vBallVel.x *= -1.f;
		}
		// 检查是否碰撞到Bat
		if (vBall.y >= (float(ScreenHeight()) - 20.0f) && (vBall.x > fBatPos) && (vBall.x <= (fBatPos + fBatWidth))) {
			vBallVel.y *= -1.f;
		}

		// 如果没有接到小球
		if (vBall.y > ScreenHeight()) {
			vBall = { 200.f, 200.f };
			float fAngle = (float(rand()) / float(RAND_MAX)) * 2.f * 3.14159f;
			vBallVel = { 100.f * cos(fAngle), 100.f * sin(fAngle) };
		}
	}


public:
	bool OnUserCreate() override
	{
		srand(time(0));
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
		
		// Draw Fill Rect
		FillRect(int(fBatPos), ScreenHeight() - 20, int(fBatWidth), 10, olc::GREEN);

		// Draw Ball
		FillCircle(vBall, 5, olc::CYAN);

		UserController(fElapsedTime);
		UpdateBall(fElapsedTime);
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
