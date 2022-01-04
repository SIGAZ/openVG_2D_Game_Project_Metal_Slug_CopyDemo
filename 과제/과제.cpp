//  C++ 윈도우 프로그래밍 - 구정현/1992.10.07
//  openVG를 사용한 게임 제작 - 메탈 슬러그 모작
//
//  제작 환경: window 10 x64
//
//  게임 실행 방법
//  실행하면 openVG 메세지 확인 버튼 누름
//  로고가 로딩되면 엔터를 눌러 게임 시작
//
//  이동키: 오른쪽 화살표 키보드
//  미사일: 스페이스바
//  폭탄: Z키
//
//  적 비행기나 적 탱크에 충돌하면 게임 종료


#include "stdafx.h"
#include "과제.h"

#include "egl/egl.h"
#include "vg/openvg.h"
#include "vgLoadImage.h"
#include "DXUTSound.h"


#define Window_Width 1200
#define Window_Height 600
#define Background_Width 4700

#define MAX_LOADSTRING 100
#define KEY_DOWN(code) ((GetAsyncKeyState(code)&0x8000)?1:0)

EGLDisplay display;
EGLSurface surface;
EGLContext context;

VGImage logo; // 타이틀 이미지
VGImage end; // 타이틀 이미지
VGImage background; //배경이미지 변수
VGImage plane_i; //내 비행기 이미지 변수
VGImage enemy_pi; //적 비행기 이미지 변수
VGImage enemy_ti; //적 탱크 이미지 변수
VGImage missile_i; //미사일 이미지 변수
VGImage bomb_i; //폭탄 이미지 변수
VGImage enemy_boss_i; //적 보스 이미지 변수

struct Plane
{
	int x;//x좌표
	int y;//y좌표
} plane; //내 비행기 스트럭쳐

struct Missile
{
	int x;//x좌표
	int y;//y좌표
	bool m;//미사일
} missiles[100]; //미사일 스트럭쳐

struct Bomb
{
	int x;//x좌표
	int y;//y좌표
	float vx; //속도 x
	float vy; //속도 y
	bool m;//미사일
} bomb[100]; //폭탄 스트럭쳐

struct enemy_p
{
	int x;//x좌표
	int y;//y좌표
	bool m;
} enemy_p[100]; //적 비행기 스트럭쳐

struct enemy_t
{
	int x;//x좌표
	int y;//y좌표
	bool m;
} enemy_t[100]; //적 탱크 스트럭쳐

struct enemy_boss
{
	int x;
	int y;
	int hp;
	bool m;
	bool life;
}enemy_boss; //적 보스 스트럭처

bool a, b, c, d, e, f, g, h; //내 비행기와 적 충돌 체크 변수
bool aa, bb, cc, dd, ee, ff, gg, hh; // 미사일, 폭탄과 적 충돌 체크 변수
bool boa, bob, boc, bod; //내 비행기와 적 보스 충돌 체크
bool boaa, bobb, bocc, bodd, boee, boff, bogg, bohh; // 미사일, 폭탄과 적 충돌 체크 변수
bool logo_flag;
bool Boss_clear;

int Boss_Wait = 5;

int cx; //카메라 좌표

void timerProc();
void draw();
void title();
//void ending();

RECT rect;

CSoundManager* sm = NULL;
CSound* crashSound = NULL;


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // 기본 메시지 루프입니다.
	DWORD lastTime = GetTickCount();
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, 1))
		{
			if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT) break;
		}
		DWORD curTime = GetTickCount();
		if (curTime - lastTime>32) // 30 frame per second
		{
			lastTime = lastTime + 33;
			timerProc();
		}
	}

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//


HWND hWnd;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, L"프로그래밍 클라이언트 포트폴리오 게임", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, Window_Width, Window_Height, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_CREATE:
	{
		display = eglGetDisplay(GetDC(hWnd));
		eglInitialize(display, NULL, NULL);
		eglBindAPI(EGL_OPENVG_API);
		EGLint conf_list[] = { EGL_RED_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_BLUE_SIZE, 8,
			EGL_ALPHA_SIZE, 8,
			EGL_NONE };
		EGLConfig config;
		EGLint    num_config;
		eglChooseConfig(display, conf_list, &config, 1, &num_config);

		surface = eglCreateWindowSurface(display, config, hWnd, NULL);
		context = eglCreateContext(display, 0, NULL, NULL);

		//이미지 생성 및 변수 대입
		logo = vgLoadImage(TEXT("logo.png")); //배경 이미지
		//end = vgLoadImage(TEXT("end.jpg")); //배경 이미지
		background = vgLoadImage(TEXT("background.png")); //배경 이미지
		plane_i = vgLoadImage(TEXT("plane.png")); //내 비행기 이미지
		enemy_pi = vgLoadImage(TEXT("enemy_plane.png")); // 적 비행기 이미지
		enemy_ti = vgLoadImage(TEXT("enemy_tank.png")); //적 탱크 이미지
		missile_i = vgLoadImage(TEXT("missile.png")); //미사일 이미지
		bomb_i = vgLoadImage(TEXT("bomb.png")); //폭탄 이미지
		enemy_boss_i = vgLoadImage(TEXT("enemy_boss.png")); //폭탄 이미지

		for (int i = 0; i < 100; i++)
		{
			missiles[i].m = false;
			bomb[i].m = false;
			enemy_p[i].m = false;
			enemy_t[i].m = false;
		} // 적과 미사일 초기화

		enemy_boss.hp = 20;
		enemy_boss.life = false;
		enemy_boss.m = false;

		logo_flag = false;
		Boss_clear = false;
		cx = 0; //카메라 좌표 초기화

		sm = new CSoundManager();
		sm->Initialize(hWnd, DSSCL_NORMAL);
		sm->SetPrimaryBufferFormat(2, 22050, 16);

		sm->Create(&crashSound, TEXT("background_sound.wav"), 0, GUID_NULL);
		crashSound->Play(0, DSBPLAY_LOOPING);

		sm->Create(&crashSound, TEXT("missile_sound.wav"), 0, GUID_NULL);
		crashSound->Play(0);


	}

	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
	}
	break;

	case WM_KEYDOWN:
	{
		//동기식 입력 처리
		if (wParam == 'Z')
		{
			for (int i = 0; i < 100; i++)
				if (bomb[i].m == false)
				{
					bomb[i].m = true;
					bomb[i].x = plane.x;
					bomb[i].y = plane.y;
					bomb[i].vx = 0; //초기속도 0
					bomb[i].vy = 10; //초기속도 10
					break;
				}
		}
		break;
	}
	break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


void timerProc()
{
	cx += 5; //5프레임씩 이동

	if (cx >= Window_Width)
		cx = 0; //윈도우 사이즈 넘어가면 0으로 초기화

	// 비동기식 입력 처리
	if (KEY_DOWN(VK_UP) && plane.y < (Window_Height - 100))
		plane.y += 10;

	if (KEY_DOWN(VK_DOWN) && plane.y > 0)
		plane.y -= 10;

	if (KEY_DOWN(VK_RIGHT) && plane.x < (Window_Width - 100))
		plane.x += 10;

	if (KEY_DOWN(VK_LEFT) && plane.x > 0)
		plane.x -= 10;

	//비동기식 미사일 발사
	if (KEY_DOWN(VK_SPACE)) {
		for (int i = 0; i < 100; i++) {
			if (missiles[i].m == false)
			{
				missiles[i].m = true;
				missiles[i].x = plane.x;
				missiles[i].y = plane.y;
				break;
			}
		}
	}

	// 미사일 객체 발사 & 창밖 처리
	for (int i = 0; i < 100; i++) {
		if (missiles[i].m == true)
		{
			missiles[i].x += 20;

			if (missiles[i].x > Window_Width) //가로 최대크기 넘어가면 초기화
			{
				missiles[i].m = false;
			}
		}
	}

	// 폭탄 객체 발사 & 창밖 처리
	for (int i = 0; i < 100; i++)
	{
		if (bomb[i].m)
		{
			bomb[i].vx += 0.1f; // x속도는 0.1
			bomb[i].vy += -1; // y속도는 -1
			bomb[i].x += bomb[i].vx; //속도를 좌표에 넣어준다.
			bomb[i].y += bomb[i].vy;

			if (bomb[i].y < -32) //이미지가 넘어가면 소멸
			{
				bomb[i].m = false;
			}
		}
	}



	//적 탱크 생성
	if (rand() % 60 == 3) //2초에 한번씩
	{
		for (int i = 0; i < 100; i++)
		{
			if (enemy_t[i].m == false)
			{
				enemy_t[i].m = true;
				enemy_t[i].y = 50; //y좌표는 50
				enemy_t[i].x = Window_Width; //윈도우 오른쪽 끝에 생성
				break;
			}
		}
	}

	//적 탱크 이동, 삭제 등
	for (int i = 0; i < 100; i++)
	{
		if (enemy_t[i].m == true)
			enemy_t[i].x -= 5; //탱크 생성시 이동속도는 x 좌표 -5

		if (enemy_t[i].x < -80)
			enemy_t[i].m = false; // 만약 윈도우 사이즈를 넘어가면 초기화
	}




	//적 비행기 생성
	if (rand() % 30 == 3) //1초에 한번
	{
		for (int i = 0; i < 100; i++)
		{
			if (enemy_p[i].m == false)
			{
				enemy_p[i].m = true;
				enemy_p[i].y = (rand() % 400 + 110); //y 좌표는 110~510
				enemy_p[i].x = Window_Width; // 윈도우 오른쪽 끝에 생성
				break;
			}
		}
	}

	//적 비행기 이동, 삭제 등
	for (int i = 0; i < 100; i++)
	{
		if (enemy_p[i].m == true)
			enemy_p[i].x -= 10; //비행기 생성시 이동속도는 x 좌표 -10

		if (enemy_p[i].x < -80)
			enemy_p[i].m = false; // 만약 윈도우 사이즈를 넘어가면 초기화
	}

	/*
	//보스 비행기 생성, 이동
	if (enemy_boss.life == true) //0일 경우 생성
	{
		if (enemy_boss.m == false)
		{
			enemy_boss.m = true;
			enemy_boss.y = 200; //y 좌표는 200
			enemy_boss.x = Window_Width; // 윈도우 오른쪽 끝에 생성
			enemy_boss.x -= 10; //비행기 생성시 이동속도는 x 좌표 -10
		}
	}
	*/


	//내 비행기와 충돌 체크 부분
	for (int i = 0; i < 100; i++)
	{
		//내 비행기와 적 비행기 충돌 체크
		a = plane.x + 86 < enemy_p[i].x;
		b = enemy_p[i].x + 86 < plane.x;
		c = plane.y + 60 < enemy_p[i].y;
		d = enemy_p[i].y + 60 < plane.y;

		//내 비행기와 적 탱크 충돌 체크
		e = plane.x + 86 < enemy_t[i].x;
		f = enemy_t[i].x + 86 < plane.x;
		g = plane.y + 60 < enemy_t[i].y;
		h = enemy_t[i].y + 60 < plane.y;

		// 내 비행기와 적 비행기 충돌, 프로그램 종료
		if (enemy_p[i].m == true && !(a || b || c || d))
			exit(0);

		// 내 비행기와 적 탱크 충돌, 프로그램 종료
		if (enemy_t[i].m == true && !(e || f || g || h))
			exit(0);
	}

	/*
	//보스와 충돌체크
	if (enemy_boss.life == true)
	{
		//내 비행기와 적 보스 충돌 체크
		boa = plane.x + 512 < enemy_boss.x;
		bob = enemy_boss.x + 512 < plane.x;
		boc = plane.y + 364 < enemy_boss.y;
		bod = enemy_boss.y + 364 < plane.y;

		// 내 비행기와 적 보스 충돌, 프로그램 종료
		if (enemy_boss.m == true && !(boa || bob || boc || bod))
		{
			exit(0);
		}
	}
	*/



	//미사일과 적 비행기 충돌 체크
	for (int i = 0; i < 100; i++)
	{
		if (missiles[i].m) //미사일 i번째 발사가 되면
		{
			for (int j = 0; j < 100; j++)
			{
				if (enemy_p[j].m) // 생성된 j번째 적과 비교
				{
					aa = missiles[i].x + 32 < enemy_p[j].x;
					bb = enemy_p[j].x + 86 < missiles[i].x;
					cc = missiles[i].y + 14 < enemy_p[j].y;
					dd = enemy_p[j].y + 60 < missiles[i].y;

					if (!(aa || bb || cc || dd))
					{
						missiles[i].m = false;
						enemy_p[j].m = false;
						Boss_Wait -= 1;
					}
				}
			}
		}
	}

	//폭탄과 적 탱크 충돌 체크
	for (int i = 0; i < 100; i++)
	{
		if (bomb[i].m) //폭탄 i번째 발사가 되면
		{
			for (int j = 0; j < 100; j++)
			{
				if (enemy_t[j].m) // 생성된 j번째 적과 비교
				{
					ee = bomb[i].x + 14 < enemy_t[j].x;
					ff = enemy_t[j].x + 86 < bomb[i].x;
					gg = bomb[i].y + 32 < enemy_t[j].y;
					hh = enemy_t[j].y + 60 < bomb[i].y;

					if (!(ee || ff || gg || hh))
					{
						bomb[i].m = false;
						enemy_t[j].m = false;
						Boss_Wait -= 1;
					}
				}
			}
		}
	}


	/*
	//미사일과 적 보스 충돌 체크
	for (int i = 0; i < 100; i++)
	{
		if (missiles[i].m) //미사일 i번째 발사가 되면
		{
			if (enemy_boss.m) // 생성된 적 보스와 비교
			{
				boaa = missiles[i].x + 32 < enemy_boss.x;
				bobb = enemy_boss.x + 512 < missiles[i].x;
				bocc = missiles[i].y + 14 < enemy_boss.y;
				bodd = enemy_boss.y + 364 < missiles[i].y;

				if (!(boaa || bobb || bocc || bodd))
				{
					missiles[i].m = false;
					enemy_boss.hp -= 1;
				}
			}

		}
	}


	//폭탄과 적 보스 충돌 체크
	for (int i = 0; i < 100; i++)
	{
		if (bomb[i].m) //폭탄 i번째 발사가 되면
		{
			if (enemy_t[1].m) // 생성된 j번째 적과 비교
			{
				boee = bomb[i].x + 14 < enemy_boss.x;
				boff = enemy_boss.x + 512 < bomb[i].x;
				bogg = bomb[i].y + 32 < enemy_boss.y;
				bohh = enemy_boss.y + 364 < bomb[i].y;

				if (!(boee || boff || bogg || bohh))
				{
					bomb[i].m = false;
					enemy_boss.hp -= 1;
				}
			}

		}
	}

	if (Boss_Wait == 0)
	{
		enemy_boss.life = true;
	}

	if (enemy_boss.hp == 0)
	{
		//enemy_boss.life = false;
		Boss_clear == true;
		ending();
	}
	*/
	title();
	draw();

}

void draw()
{
	int B_WIDTH = Window_Width;
	int k = cx / B_WIDTH;

	vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);

	/* 동적으로 짠다면 아래 코드
	int n=(int)(SCREEN_WIDTH/IMAGE_WIDTH)+2;
	int k=cx/IMAGE_WIDTH;

	for( i=0; i<n; i++ )
	{
		vgLoadIdentity();
		vgTranslate( (k+i)*IMAGE_WIDTH-cx, 0 );
		vgDrawImage( backgroundImage );
	}
	}*/

	vgLoadIdentity();
	vgTranslate(k * B_WIDTH - cx, 0);
	vgDrawImage(background);
	vgLoadIdentity();
	vgTranslate((k + 1) * B_WIDTH - cx, 0);
	vgDrawImage(background);

	vgLoadIdentity();
	vgTranslate(plane.x, plane.y);
	vgDrawImage(plane_i);

	if (enemy_boss.life == false)
	{
		for (int i = 0; i < 100; i++) {
			//탱크 이미지 생성
			if (enemy_t[i].m == true)
			{
				vgLoadIdentity();
				vgTranslate(enemy_t[i].x, enemy_t[i].y);
				vgDrawImage(enemy_ti);

			}
		}

		for (int i = 0; i < 100; i++)
		{
			//비행기 이미지 생성
			if (enemy_p[i].m == true)
			{
				vgLoadIdentity();
				vgTranslate(enemy_p[i].x, enemy_p[i].y);
				vgDrawImage(enemy_pi);
			}
		}
	}

	/*
	//보스 이미지 생성
	if (enemy_boss.life == true)
	{
		if (enemy_boss.m == true)
		{
			vgLoadIdentity();
			vgTranslate(enemy_boss.x, enemy_boss.y);
			vgDrawImage(enemy_boss_i);
		}
	}
	*/

	for (int i = 0; i < 100; i++)
	{
		//미사일 이미지 생성
		if (missiles[i].m == true)
		{
			vgLoadIdentity();
			vgTranslate(missiles[i].x, missiles[i].y);
			vgDrawImage(missile_i);
		}
	}

	for (int i = 0; i < 100; i++)
	{
		//폭탄 이미지 생성
		if (bomb[i].m == true)
		{
			vgLoadIdentity();
			vgTranslate(bomb[i].x, bomb[i].y);
			vgDrawImage(bomb_i);
		}
	}
	eglSwapBuffers(display, surface);
}

void title()
{
	while (logo_flag != true)
	{
		vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);

		vgLoadIdentity();
		vgTranslate(0, 0);
		vgDrawImage(logo);

		if (KEY_DOWN(VK_RETURN) == true)
		{
			logo_flag = true;
		}

		eglSwapBuffers(display, surface);
	}
}