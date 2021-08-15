#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <vector>
#include <map>

/**
 * 定义一个圆数据
 */
struct sBall {
	/**
	 * 圆心坐标
	 */
	float px, py;
	/**
	 * 速度方向
	 */
	float vx, vy;
	/**
	 * 加速度方向
	 */
	float ax, ay;
	/**
	 * 半径
	 */
	float radius;


	std::size_t id;
};

using namespace std;
// Override base class with your custom functionality
class BallCollision : public olc::PixelGameEngine
{
public:
	BallCollision()
	{
		// Name your application
		sAppName = "BallCollision";
	}
private:
	vector<sBall> vecBalls;
	sBall* pCurrentSelectedBall = nullptr;

	void AddBall(float x, float y, float radius) {
		sBall ball;
		ball.px = x;
		ball.py = y;
		ball.ax = 0.f;
		ball.ay = 0.f;
		ball.vx = 0.f;
		ball.vy = 0.f;
		ball.radius = radius;
		ball.id = vecBalls.size();
		
		vecBalls.emplace_back(ball);
	}

public:
	bool OnUserCreate() override
	{ 

		float fDefaultRad = 8.f;
		for (int i = 0; i < 10; ++i) {
			AddBall(rand() % ScreenWidth(), rand() % ScreenHeight(), fDefaultRad);
		}
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// 判断两圆是否相交
		auto DoCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2) {
			 return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < (r1 + r2) * (r1 + r2);
		};

		// 判断点是否在圆内
		auto IsPointInCircle = [](float x1, float y1, float r1, float px, float py) {
			return fabs((x1 - px) * (x1 - px) + (y1 - py) * (y1 - py)) < (r1 * r1);
		};

		// 鼠标点击选中圆
		if (GetMouse(0).bPressed) {
			pCurrentSelectedBall = nullptr;
			for (auto& ball : vecBalls) {
				if (IsPointInCircle(ball.px, ball.py, ball.radius, GetMouseX(), GetMouseY())) {
					pCurrentSelectedBall = &ball;
					break;
				}
			}
		}

		// 鼠标保持点击状态
		if (GetMouse(0).bHeld) {
			if (pCurrentSelectedBall != nullptr) {
				pCurrentSelectedBall->px = GetMouseX();
				pCurrentSelectedBall->py = GetMouseY();
			}
		}

		// 鼠标取消点击
		if (GetMouse(0).bReleased) {
			pCurrentSelectedBall = nullptr;
		}

		// 碰撞记录
		vector<pair<sBall*, sBall*>> vecCollidingPairs; 

		// 更新小球位置
		for (auto& ball : vecBalls) {
			ball.vx += ball.ax * fElapsedTime;
			ball.vy += ball.ay * fElapsedTime;
			ball.px += ball.vx * fElapsedTime;
			ball.py += ball.vy * fElapsedTime;

			// 边界判断
			if (ball.px < 0) ball.px += float(ScreenWidth());
			if (ball.px >= ScreenWidth()) ball.px -= float(ScreenWidth());
			if (ball.py < 0) ball.px += float(ScreenHeight());
			if (ball.py >= ScreenHeight()) ball.py -= float(ScreenHeight());
		}

		// 碰撞计算
		for (auto& ball : vecBalls) {
			for (auto& target : vecBalls) {
				if (ball.id != target.id) {
					if (DoCirclesOverlap(ball.px, ball.py, ball.radius, target.px, target.py, target.radius)) {
						// 将碰撞物体加入到队列中
						vecCollidingPairs.push_back({&ball, &target});
						// 计算发生碰撞时，两球的距离
						float fDistance = sqrtf((ball.px - target.px) * (ball.px - target.px) + (ball.py - target.py) * (ball.py - target.py));
						// 两球相交距离的一半
						float fOverlap = 0.5f * (fDistance - target.radius - ball.radius);

						// 将小球向碰撞的相反方向移动
						ball.px -= fOverlap * (ball.px - target.px) / fDistance;
						ball.py -= fOverlap * (ball.py - target.py) / fDistance;

						target.px += fOverlap * (ball.px - target.px) / fDistance;
						target.py += fOverlap * (ball.py - target.py) / fDistance;
					}
				}
			}
		}

		Clear(olc::DARK_BLUE);

		for (const auto& ball : vecBalls) {
			DrawCircle(int(ball.px), int(ball.py), int(ball.radius));
		}

		for (auto c : vecCollidingPairs) {
			DrawLine(c.first->px, c.first->py, c.second->px, c.second->py, olc::RED);
		}

		return true;
	}
};

int main()
{
	BallCollision demo;
	if (demo.Construct(160, 120, 8, 8))
		demo.Start();
	return 0;
}