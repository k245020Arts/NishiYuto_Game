#include "time.h"
#include <Windows.h>
#include <DxLib.h>
namespace {
	LARGE_INTEGER freq;
	LARGE_INTEGER current;
	float deltaTime;
	float refreshRate;
	// 直近の処理時間を保存
	const int BUF_SIZE = 30;
	float timeBuf[BUF_SIZE];
	int readP;
	int writeP;
	float rate;
	float debugTimeRate;
};

void Time::Init()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&current);

	HDC hdc = GetDC(GetMainWindowHandle());	// デバイスコンテキストの取得
	refreshRate = (float)GetDeviceCaps(hdc, VREFRESH);	// リフレッシュレートの取得
	ReleaseDC(GetMainWindowHandle(), hdc);	// デバイスコンテキストの解放

	readP = 0;
	writeP = 0;
	debugTimeRate = 1.0f;
	rate = 1.0f;
}

void Time::Refresh()
{
	LARGE_INTEGER last = current;
	QueryPerformanceCounter(&current);
	float dt = static_cast<float >(current.QuadPart - last.QuadPart) / freq.QuadPart;
	timeBuf[writeP] = dt;
	writeP = (writeP+1) % BUF_SIZE;
	if (writeP == readP) {
		readP = (readP + 1) % BUF_SIZE;
	}
	float sum = 0;
	int num = 0;
	for (int i = readP; i != writeP; i = (i + 1) % BUF_SIZE) {
		sum += timeBuf[i];
		num++;
	}
	if (num>BUF_SIZE/2) {
		float ave = sum / num;
		if (dt >= ave*2)
			deltaTime = ave*2;
		else
			deltaTime = dt;
	}
	else
		deltaTime = dt;
}

float Time::DeltaTime()
{
	return deltaTime;
}

void Time::Release()
{
}

void Time::ChangeDeltaRate(float _rate)
{
	rate = _rate;
}

float Time::DeltaTimeRate()
{
	float r = DeltaTime() * rate * debugTimeRate;
	return r;
}

float Time::DebugSlowTimeSet(float _rate)
{
	debugTimeRate = _rate;
	return debugTimeRate;
}

float Time::GetTimeRate()
{
	return rate;
}