//  C++ ������ ���α׷��� - ������/1992.10.07
//  openVG�� ����� ���� ���� - ��Ż ������ ����
//
//  ���� ȯ��: window 10 x64
//
//  ���� ���� ���
//  �����ϸ� openVG �޼��� Ȯ�� ��ư ����
//  �ΰ� �ε��Ǹ� ���͸� ���� ���� ����
//
//  �̵�Ű: ������ ȭ��ǥ Ű����
//  �̻���: �����̽���
//  ��ź: ZŰ
//
//  �� ����⳪ �� ��ũ�� �浹�ϸ� ���� ����


#include "stdafx.h"
#include "����.h"

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

VGImage logo; // Ÿ��Ʋ �̹���
VGImage end; // Ÿ��Ʋ �̹���
VGImage background; //����̹��� ����
VGImage plane_i; //�� ����� �̹��� ����
VGImage enemy_pi; //�� ����� �̹��� ����
VGImage enemy_ti; //�� ��ũ �̹��� ����
VGImage missile_i; //�̻��� �̹��� ����
VGImage bomb_i; //��ź �̹��� ����
VGImage enemy_boss_i; //�� ���� �̹��� ����

struct Plane
{
	int x;//x��ǥ
	int y;//y��ǥ
} plane; //�� ����� ��Ʈ����

struct Missile
{
	int x;//x��ǥ
	int y;//y��ǥ
	bool m;//�̻���
} missiles[100]; //�̻��� ��Ʈ����

struct Bomb
{
	int x;//x��ǥ
	int y;//y��ǥ
	float vx; //�ӵ� x
	float vy; //�ӵ� y
	bool m;//�̻���
} bomb[100]; //��ź ��Ʈ����

struct enemy_p
{
	int x;//x��ǥ
	int y;//y��ǥ
	bool m;
} enemy_p[100]; //�� ����� ��Ʈ����

struct enemy_t
{
	int x;//x��ǥ
	int y;//y��ǥ
	bool m;
} enemy_t[100]; //�� ��ũ ��Ʈ����

struct enemy_boss
{
	int x;
	int y;
	int hp;
	bool m;
	bool life;
}enemy_boss; //�� ���� ��Ʈ��ó

bool a, b, c, d, e, f, g, h; //�� ������ �� �浹 üũ ����
bool aa, bb, cc, dd, ee, ff, gg, hh; // �̻���, ��ź�� �� �浹 üũ ����
bool boa, bob, boc, bod; //�� ������ �� ���� �浹 üũ
bool boaa, bobb, bocc, bodd, boee, boff, bogg, bohh; // �̻���, ��ź�� �� �浹 üũ ����
bool logo_flag;
bool Boss_clear;

int Boss_Wait = 5;

int cx; //ī�޶� ��ǥ

void timerProc();
void draw();
void title();
//void ending();

RECT rect;

CSoundManager* sm = NULL;
CSound* crashSound = NULL;


// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//


HWND hWnd;

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindowW(szWindowClass, L"���α׷��� Ŭ���̾�Ʈ ��Ʈ������ ����", WS_OVERLAPPEDWINDOW,
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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
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

		//�̹��� ���� �� ���� ����
		logo = vgLoadImage(TEXT("logo.png")); //��� �̹���
		//end = vgLoadImage(TEXT("end.jpg")); //��� �̹���
		background = vgLoadImage(TEXT("background.png")); //��� �̹���
		plane_i = vgLoadImage(TEXT("plane.png")); //�� ����� �̹���
		enemy_pi = vgLoadImage(TEXT("enemy_plane.png")); // �� ����� �̹���
		enemy_ti = vgLoadImage(TEXT("enemy_tank.png")); //�� ��ũ �̹���
		missile_i = vgLoadImage(TEXT("missile.png")); //�̻��� �̹���
		bomb_i = vgLoadImage(TEXT("bomb.png")); //��ź �̹���
		enemy_boss_i = vgLoadImage(TEXT("enemy_boss.png")); //��ź �̹���

		for (int i = 0; i < 100; i++)
		{
			missiles[i].m = false;
			bomb[i].m = false;
			enemy_p[i].m = false;
			enemy_t[i].m = false;
		} // ���� �̻��� �ʱ�ȭ

		enemy_boss.hp = 20;
		enemy_boss.life = false;
		enemy_boss.m = false;

		logo_flag = false;
		Boss_clear = false;
		cx = 0; //ī�޶� ��ǥ �ʱ�ȭ

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
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
	}
	break;

	case WM_KEYDOWN:
	{
		//����� �Է� ó��
		if (wParam == 'Z')
		{
			for (int i = 0; i < 100; i++)
				if (bomb[i].m == false)
				{
					bomb[i].m = true;
					bomb[i].x = plane.x;
					bomb[i].y = plane.y;
					bomb[i].vx = 0; //�ʱ�ӵ� 0
					bomb[i].vy = 10; //�ʱ�ӵ� 10
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
	cx += 5; //5�����Ӿ� �̵�

	if (cx >= Window_Width)
		cx = 0; //������ ������ �Ѿ�� 0���� �ʱ�ȭ

	// �񵿱�� �Է� ó��
	if (KEY_DOWN(VK_UP) && plane.y < (Window_Height - 100))
		plane.y += 10;

	if (KEY_DOWN(VK_DOWN) && plane.y > 0)
		plane.y -= 10;

	if (KEY_DOWN(VK_RIGHT) && plane.x < (Window_Width - 100))
		plane.x += 10;

	if (KEY_DOWN(VK_LEFT) && plane.x > 0)
		plane.x -= 10;

	//�񵿱�� �̻��� �߻�
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

	// �̻��� ��ü �߻� & â�� ó��
	for (int i = 0; i < 100; i++) {
		if (missiles[i].m == true)
		{
			missiles[i].x += 20;

			if (missiles[i].x > Window_Width) //���� �ִ�ũ�� �Ѿ�� �ʱ�ȭ
			{
				missiles[i].m = false;
			}
		}
	}

	// ��ź ��ü �߻� & â�� ó��
	for (int i = 0; i < 100; i++)
	{
		if (bomb[i].m)
		{
			bomb[i].vx += 0.1f; // x�ӵ��� 0.1
			bomb[i].vy += -1; // y�ӵ��� -1
			bomb[i].x += bomb[i].vx; //�ӵ��� ��ǥ�� �־��ش�.
			bomb[i].y += bomb[i].vy;

			if (bomb[i].y < -32) //�̹����� �Ѿ�� �Ҹ�
			{
				bomb[i].m = false;
			}
		}
	}



	//�� ��ũ ����
	if (rand() % 60 == 3) //2�ʿ� �ѹ���
	{
		for (int i = 0; i < 100; i++)
		{
			if (enemy_t[i].m == false)
			{
				enemy_t[i].m = true;
				enemy_t[i].y = 50; //y��ǥ�� 50
				enemy_t[i].x = Window_Width; //������ ������ ���� ����
				break;
			}
		}
	}

	//�� ��ũ �̵�, ���� ��
	for (int i = 0; i < 100; i++)
	{
		if (enemy_t[i].m == true)
			enemy_t[i].x -= 5; //��ũ ������ �̵��ӵ��� x ��ǥ -5

		if (enemy_t[i].x < -80)
			enemy_t[i].m = false; // ���� ������ ����� �Ѿ�� �ʱ�ȭ
	}




	//�� ����� ����
	if (rand() % 30 == 3) //1�ʿ� �ѹ�
	{
		for (int i = 0; i < 100; i++)
		{
			if (enemy_p[i].m == false)
			{
				enemy_p[i].m = true;
				enemy_p[i].y = (rand() % 400 + 110); //y ��ǥ�� 110~510
				enemy_p[i].x = Window_Width; // ������ ������ ���� ����
				break;
			}
		}
	}

	//�� ����� �̵�, ���� ��
	for (int i = 0; i < 100; i++)
	{
		if (enemy_p[i].m == true)
			enemy_p[i].x -= 10; //����� ������ �̵��ӵ��� x ��ǥ -10

		if (enemy_p[i].x < -80)
			enemy_p[i].m = false; // ���� ������ ����� �Ѿ�� �ʱ�ȭ
	}

	/*
	//���� ����� ����, �̵�
	if (enemy_boss.life == true) //0�� ��� ����
	{
		if (enemy_boss.m == false)
		{
			enemy_boss.m = true;
			enemy_boss.y = 200; //y ��ǥ�� 200
			enemy_boss.x = Window_Width; // ������ ������ ���� ����
			enemy_boss.x -= 10; //����� ������ �̵��ӵ��� x ��ǥ -10
		}
	}
	*/


	//�� ������ �浹 üũ �κ�
	for (int i = 0; i < 100; i++)
	{
		//�� ������ �� ����� �浹 üũ
		a = plane.x + 86 < enemy_p[i].x;
		b = enemy_p[i].x + 86 < plane.x;
		c = plane.y + 60 < enemy_p[i].y;
		d = enemy_p[i].y + 60 < plane.y;

		//�� ������ �� ��ũ �浹 üũ
		e = plane.x + 86 < enemy_t[i].x;
		f = enemy_t[i].x + 86 < plane.x;
		g = plane.y + 60 < enemy_t[i].y;
		h = enemy_t[i].y + 60 < plane.y;

		// �� ������ �� ����� �浹, ���α׷� ����
		if (enemy_p[i].m == true && !(a || b || c || d))
			exit(0);

		// �� ������ �� ��ũ �浹, ���α׷� ����
		if (enemy_t[i].m == true && !(e || f || g || h))
			exit(0);
	}

	/*
	//������ �浹üũ
	if (enemy_boss.life == true)
	{
		//�� ������ �� ���� �浹 üũ
		boa = plane.x + 512 < enemy_boss.x;
		bob = enemy_boss.x + 512 < plane.x;
		boc = plane.y + 364 < enemy_boss.y;
		bod = enemy_boss.y + 364 < plane.y;

		// �� ������ �� ���� �浹, ���α׷� ����
		if (enemy_boss.m == true && !(boa || bob || boc || bod))
		{
			exit(0);
		}
	}
	*/



	//�̻��ϰ� �� ����� �浹 üũ
	for (int i = 0; i < 100; i++)
	{
		if (missiles[i].m) //�̻��� i��° �߻簡 �Ǹ�
		{
			for (int j = 0; j < 100; j++)
			{
				if (enemy_p[j].m) // ������ j��° ���� ��
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

	//��ź�� �� ��ũ �浹 üũ
	for (int i = 0; i < 100; i++)
	{
		if (bomb[i].m) //��ź i��° �߻簡 �Ǹ�
		{
			for (int j = 0; j < 100; j++)
			{
				if (enemy_t[j].m) // ������ j��° ���� ��
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
	//�̻��ϰ� �� ���� �浹 üũ
	for (int i = 0; i < 100; i++)
	{
		if (missiles[i].m) //�̻��� i��° �߻簡 �Ǹ�
		{
			if (enemy_boss.m) // ������ �� ������ ��
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


	//��ź�� �� ���� �浹 üũ
	for (int i = 0; i < 100; i++)
	{
		if (bomb[i].m) //��ź i��° �߻簡 �Ǹ�
		{
			if (enemy_t[1].m) // ������ j��° ���� ��
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

	/* �������� §�ٸ� �Ʒ� �ڵ�
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
			//��ũ �̹��� ����
			if (enemy_t[i].m == true)
			{
				vgLoadIdentity();
				vgTranslate(enemy_t[i].x, enemy_t[i].y);
				vgDrawImage(enemy_ti);

			}
		}

		for (int i = 0; i < 100; i++)
		{
			//����� �̹��� ����
			if (enemy_p[i].m == true)
			{
				vgLoadIdentity();
				vgTranslate(enemy_p[i].x, enemy_p[i].y);
				vgDrawImage(enemy_pi);
			}
		}
	}

	/*
	//���� �̹��� ����
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
		//�̻��� �̹��� ����
		if (missiles[i].m == true)
		{
			vgLoadIdentity();
			vgTranslate(missiles[i].x, missiles[i].y);
			vgDrawImage(missile_i);
		}
	}

	for (int i = 0; i < 100; i++)
	{
		//��ź �̹��� ����
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