// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include "Point.h"
using namespace std;
// 콘솔창 띄우기
//#ifdef UNICODE
//#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
//#else
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
//#endif

// 창 크기
#define CLIENT_WIDTH		800
#define CLIENT_HEIGHT		600

// 사각형 크기
#define RECT_SIZE 50

// 보드판 줄
#define WIDTH_LINE     CLIENT_WIDTH / RECT_SIZE
#define HEIGHT_LINE    CLIENT_HEIGHT / RECT_SIZE

// 객체 갯수
#define PLAYER_NUM				10
#define ITEM_ALLNUM				100
#define ITEM_NUM				50
#define VIRUS_ALLNUM			15
#define VIRUS_NUM				5
#define TRAP_NUM				7

#define NOMALSIZE 10
#define SPEED 1600
#define PLAYER_RANGE 100		// 바이러스와 플레이어 사이의 거리 범위
#define VIRUS_RANGE 100			// 바이러스와 바이러스 사이의 거리 범위
#define TRAP_SIZE 20
#define PI 3.141592f
#define MAX_SIZE 100
// Scene
enum class ENUM_SCENEID :UINT
{
	LOGOSCENE,
	GAMESCENE,
	END
};

// Target
enum class ENUM_TARGETID :UINT
{
	NON_TARGET,
	PLAYER_TARGET,
	VIRUSTARGET,
	ITEM_TARGET
};

template <class T>
void Swap(T*& a, T*& b)
{
	T* temp = a;
	a = b;
	b = temp;
}
