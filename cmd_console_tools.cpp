/* -----------------------------------------

	 ���ļ�����Ҫ�ύ���������Ķ�

   ----------------------------------------- */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <tchar.h>
#include <string.h>
#include <Windows.h>
#include "cmd_console_tools.h"
using namespace std;

/* --------------------------------------------------------------------------------------- */
/*   �˴�����������̬ȫ�ֱ�����Ŀ�������к���������Ҫ�ٴ� HANDLE ������ʹ�ⲿ����ʱ������  */
/*   ע��Ϊ��ֹͬ������̬ȫ��һ�㽨���__��Ϊ��������ʼ                                    */
/* --------------------------------------------------------------------------------------- */
static const HANDLE __hout = GetStdHandle(STD_OUTPUT_HANDLE); // ȡ��׼����豸��Ӧ�ľ��
static const HANDLE __hin = GetStdHandle(STD_INPUT_HANDLE);	  // ȡ��׼�����豸��Ӧ�ľ��

/***************************************************************************
  �������ƣ�
  ��    �ܣ������system("cls")һ���Ĺ��ܣ���Ч�ʸ�
  ���������
  �� �� ֵ��
  ˵    �������������Ļ���������������ǿɼ���������(ʹ�õ�ǰ��ɫ)
***************************************************************************/
void cct_cls(void)
{
	COORD coord = {0, 0};
	CONSOLE_SCREEN_BUFFER_INFO binfo; /* to get buffer info */
	DWORD num;

	/* ȡ��ǰ��������Ϣ */
	GetConsoleScreenBufferInfo(__hout, &binfo);
	/* ����ַ� */
	FillConsoleOutputCharacter(__hout, (TCHAR)' ', binfo.dwSize.X * binfo.dwSize.Y, coord, &num);
	/* ������� */
	FillConsoleOutputAttribute(__hout, binfo.wAttributes, binfo.dwSize.X * binfo.dwSize.Y, coord, &num);

	/* ���ص�(0,0) */
	SetConsoleCursorPosition(__hout, coord);
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����ָ������ɫ
  ���������const int bg_color ������ɫ��0-15��
			const int fg_color ��ǰ��ɫ��0-15��
  �� �� ֵ��
  ˵    ������ɫ��ȡֵ�ɱ���ɫ+ǰ��ɫ��ɣ���16��
			fg_color��0-7    ��   ��   ��   ��   ��   ��   ��   ��
					  8-15 ���� ���� ���� ���� ���� ���� ���� ����
			bg_color��0-7    ��   ��   ��   ��   ��   ��   ��   ��
					  8-15 ���� ���� ���� ���� ���� ���� ���� ����
			���յ���ɫΪ ����ɫ*16+ǰ��ɫ
***************************************************************************/
void cct_setcolor(const int bg_color, const int fg_color)
{
	SetConsoleTextAttribute(__hout, bg_color * 16 + fg_color);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������һ��setcolor��ǰ��ɫ�ͱ���ɫ
  ���������int &bg_color      �����صı���ɫ��0-15��
			int &fg_color      �����ص�ǰ��ɫ��0-15��
  �� �� ֵ��
  ˵    �����β��е�&��ʾ���ã������»ὲ���������βε�ֵ���Է��ظ�ʵ��
***************************************************************************/
void cct_getcolor(int &bg_color, int &fg_color)
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(__hout, &binfo);

	bg_color = binfo.wAttributes / 16;
	fg_color = binfo.wAttributes % 16;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������ƶ���ָ��λ��
  ���������const int X       ��X�����꣨�У�
			const int Y       ��Y�����꣨�У�
  �� �� ֵ��
  ˵    ������Ļ���Ͻ�����Ϊ(0,0)����cmd���ڵĴ�Сδ������������£�Win10Ϊ��
			����x�ᣬ��Ӧ��(0-119)
			����y�ᣬ��Ӧ��(0-29)
***************************************************************************/
void cct_gotoxy(const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(__hout, coord);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡ��ǰ�������λ�õ�����ֵ
  ���������int &x            ��ȡ�õ�X�����꣨�У�
			int &y            ��ȡ�õ�Y�����꣨�У�
  �� �� ֵ��
  ˵    �����β��е�&��ʾ���ã������»ὲ���������βε�ֵ���Է��ظ�ʵ��
***************************************************************************/
void cct_getxy(int &x, int &y)
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(__hout, &binfo);

	x = binfo.dwCursorPosition.X;
	y = binfo.dwCursorPosition.Y;

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ù��״̬����ʾ/����ʾ/ȫ��/���/���ߵȣ�
  ���������const int option��Ҫ���õĹ��״̬
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void cct_setcursor(const int options)
{
	CONSOLE_CURSOR_INFO cursor_info;

	switch (options)
	{
	case CURSOR_VISIBLE_FULL:
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 100;
		break;
	case CURSOR_VISIBLE_HALF:
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 50;
		break;
	case CURSOR_INVISIBLE:
		cursor_info.bVisible = 0;
		cursor_info.dwSize = 1;
		break;
	case CURSOR_VISIBLE_NORMAL:
	default: // ȱʡ��ʾ��꣬����
		cursor_info.bVisible = 1;
		cursor_info.dwSize = 25;
		break;
	}
	SetConsoleCursorInfo(__hout, &cursor_info);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ָ��λ�ã���ָ����ɫ����ʾһ���ַ����ɴ�
  ���������const int X       ��X�����꣨�У�
			const int Y       ��Y�����꣨�У�
			const char ch     ��Ҫ������ַ�ֵ
			const int bg_color������ɫ��ȱʡΪCOLOR_BLACK��
			const int fg_color������ɫ��ȱʡΪCOLOR_WHITE��
			const int rpt     ���ظ�������ȱʡΪ1��
  �� �� ֵ��
  ˵    ����X��Y�ķ�Χ�μ�gotoxy������˵��
***************************************************************************/
void cct_showch(const int X, const int Y, const char ch, const int bg_color, const int fg_color, const int rpt)
{
	int i;

	cct_gotoxy(X, Y);
	cct_setcolor(bg_color, fg_color);

	/* ѭ��n�Σ���ӡ�ַ�ch */
	for (i = 0; i < rpt; i++)
		putchar(ch);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ָ��λ�ã���ָ����ɫ����ʾһ���ַ���
  ���������const int X       ��X�����꣨�У�
			const int Y       ��Y�����꣨�У�
			const char *str   ��Ҫ������ַ���
			const int bg_color������ɫ��ȱʡΪCOLOR_BLACK��
			const int fg_color������ɫ��ȱʡΪCOLOR_WHITE��
			const int rpt     ���ظ�������ȱʡΪ1��
			const int max_len ��-1����ʾ�����Ƴ��ȣ���strlen(str)*rpt��ʵ�ʴ�ӡ��
  �� �� ֵ��
  ˵    �����������������ʱ���ţ���ָ��ѧϰ��ɺ����Ķ�
***************************************************************************/
void cct_showstr(const int X, const int Y, const char *str, const int bg_color, const int fg_color, int rpt, int maxlen)
{
	const char *p;
	int i, rpt_count = 0;

	cct_gotoxy(X, Y);
	cct_setcolor(bg_color, fg_color);

	/* ���ȿ���str==NULL / str="" �����
	   1�����maxlen��-1/0����ֱ�ӷ��أ�ʲô������ӡ
	   2�����maxlen>0������maxlen���ո���� */
	if (str == NULL || str[0] == 0)
	{
		for (i = 0; i < maxlen; i++) // ���maxlen��-1��0��ѭ����ִ�У�ֱ�ӷ���
			putchar(' ');
		return;
	}

	/* ֮�е��ˣ���str��NULL/str!=""�����(��strlenһ��>0) */
	if (rpt <= 0)
		rpt = 1; // ��ֹ�������

	if (maxlen < 0)
		maxlen = strlen(str) * rpt; // δ����maxlen��Ϊԭʼ����

	for (i = 0, p = str; i < maxlen; i++, p++)
	{ // �ظ�rpt�Σ�ÿ������ַ������������ڻ��߿�ʱ������ɸ�"�T"�����
		if (*p == 0)
		{
			p = str; // ���p�Ѿ���\0����ص�ͷ���˴��ѱ�֤strlen(str)>0����һ�������ݣ�
			rpt_count++;
		}
		putchar(rpt_count < rpt ? *p : ' '); // ���������rpt�������ÿո����
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���ָ��λ�ã���ָ����ɫ����ʾһ���ַ���
  ���������const int X       ��X�����꣨�У�
			const int Y       ��Y�����꣨�У�
			const int num     ��Ҫ�����intֵ
			const int bg_color������ɫ��ȱʡΪCOLOR_BLACK��
			const int fg_color������ɫ��ȱʡΪCOLOR_WHITE��
			const int rpt     ���ظ�������ȱʡΪ1��
�� �� ֵ��
˵    ����
***************************************************************************/
void cct_showint(const int X, const int Y, const int num, const int bg_color, const int fg_color, const int rpt)
{
	int i;

	cct_gotoxy(X, Y);
	cct_setcolor(bg_color, fg_color);
	for (i = 0; i < rpt; i++) // �ظ�rpt�Σ�ÿ������ַ������������ڻ��߿�ʱ������ɸ�"�T"�����
		cout << num;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ı�cmd���ڵĴ�С���������Ĵ�С
  ���������const int cols         ���µ�����
			const int lines        ���µ�����
			const int buffer_cols  ���µĻ���������
			const int buffer_lines ���µĻ���������
  �� �� ֵ��
  ˵    �������������û������������ô��ڴ�С��
			���������ڴ�С���ڵ�ǰ��������δ����ǰ��������ʧ��
***************************************************************************/
void cct_setconsoleborder(int set_cols, int set_lines, int set_buffer_cols, int set_buffer_lines)
{
	/* ȡ��ǰϵͳ�����Ĵ��ڵ��������ֵ */
	COORD max_coord;
	max_coord = GetLargestConsoleWindowSize(__hout); /* .X �� .Y �ֱ��Ǵ��ڵ��к��е����ֵ */

	/* �������ô��ڵ����еķǷ�ֵ */
	if (set_cols <= 0 || set_lines <= 0)
		return;
	if (set_cols > max_coord.X)
		set_cols = max_coord.X;
	if (set_lines > max_coord.Y)
		set_lines = max_coord.Y;

	/* ���ô��ڵ����д�С����0��ʼ��0 ~ lines-1, 0 ~ cols-1��*/
	SMALL_RECT rect;
	rect.Top = 0;
	rect.Bottom = set_lines - 1;
	rect.Left = 0;
	rect.Right = set_cols - 1;

	/* ���û����������д�С(ȱʡ��С�ڴ���ֵ���봰��ֵһ��) */
	COORD cr;
	cr.X = (set_buffer_cols == -1 || set_buffer_cols < set_cols) ? set_cols : set_buffer_cols;		// δ�����������ֵС��set_cols����set_cols��δ��������
	cr.Y = (set_buffer_lines == -1 || set_buffer_lines < set_lines) ? set_lines : set_buffer_lines; // δ�����������ֵС��set_lines����set_lines��δ��������

	/* ȡ��ǰ���ڼ��������Ĵ�С(����getconsoleborder) */
	int cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines;
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(__hout, &binfo);

	cur_cols = binfo.srWindow.Right - binfo.srWindow.Left + 1;	// �ɼ����ڵ�����
	cur_lines = binfo.srWindow.Bottom - binfo.srWindow.Top + 1; // �ɼ����ڵ�����
	cur_buffer_cols = binfo.dwSize.X;							// ������������
	cur_buffer_lines = binfo.dwSize.Y;							// ������������

	cct_cls();
	/* ����˳��(��֤���ô��ڴ�Сʱ���ֻ���������ֵ>����ֵ) */
	if (cr.X <= cur_buffer_cols)
	{
		if (cr.Y <= cur_buffer_lines)
		{
			SetConsoleWindowInfo(__hout, true, &rect); // ���ô���
			SetConsoleScreenBufferSize(__hout, cr);	   // ���û�����
		}
		else
		{ // cr.Y > cur_buffer_lines����Ҫ�û��������������
			COORD tmpcr;
			tmpcr.X = cur_buffer_cols;
			tmpcr.Y = cr.Y;
			SetConsoleScreenBufferSize(__hout, tmpcr); // ���û�����

			SetConsoleWindowInfo(__hout, true, &rect); // ���ô���
			SetConsoleScreenBufferSize(__hout, cr);	   // ���û�����
		}
	}
	else
	{ // cr.X > cur_buffer_cols
		if (cr.Y >= cur_buffer_lines)
		{
			SetConsoleScreenBufferSize(__hout, cr);	   // ���û�����
			SetConsoleWindowInfo(__hout, true, &rect); // ���ô���
		}
		else
		{ // cr.Y < cur_buffer_lines
			COORD tmpcr;
			tmpcr.X = cr.X;
			tmpcr.Y = cur_buffer_lines;
			SetConsoleScreenBufferSize(__hout, tmpcr); // ���û�����

			SetConsoleWindowInfo(__hout, true, &rect); // ���ô���
			SetConsoleScreenBufferSize(__hout, cr);	   // ���û�����
		}
	}

	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡ��ǰcmd���ڵĴ�С����
  ���������int &cols         ����ǰ���ڵ�����-����ֵ
			int &lines        ����ǰ���ڵ�����-����ֵ
			int &buffer_cols  ����ǰ������������-����ֵ
			int &buffer_lines ����ǰ������������-����ֵ
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void cct_getconsoleborder(int &cols, int &lines, int &buffer_cols, int &buffer_lines)
{
	CONSOLE_SCREEN_BUFFER_INFO binfo;
	GetConsoleScreenBufferInfo(__hout, &binfo);

	cols = binfo.srWindow.Right - binfo.srWindow.Left + 1;	// �ɼ����ڵ�����
	lines = binfo.srWindow.Bottom - binfo.srWindow.Top + 1; // �ɼ����ڵ�����
	buffer_cols = binfo.dwSize.X;							// ������������
	buffer_lines = binfo.dwSize.Y;							// ������������
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�ȡ��ǰcmd���ڵı���
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void cct_getconsoletitle(char *title, int maxbuflen)
{
	GetConsoleTitleA(title, maxbuflen); // ������Ƿ�Խ�硢�Ƿ��пռ�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void cct_setconsoletitle(const char *title)
{
	SetConsoleTitleA(title);
}

/***************************************************************************
�������ƣ�
��    �ܣ�����ʹ�����
���������const HANDLE hin ��cmd����������
�� �� ֵ��
˵    ����ĳЩcmd���ڿ������ִ�к󣬿��ܻ�ȡ�����֧�֣�����ñ��������ٴμ���
***************************************************************************/
void cct_enable_mouse(void)
{
	DWORD Mode;

	GetConsoleMode(__hin, &Mode);					  /* ȡ�ÿ���̨ԭ����ģʽ */
	SetConsoleMode(__hin, Mode | ENABLE_MOUSE_INPUT); // �������֧�֣�����ԭ����֧����꣬�ټ�Ҳû����
}

/***************************************************************************
�������ƣ�
��    �ܣ�����ʹ�����
���������const HANDLE hin ��cmd����������
�� �� ֵ��
˵    ����ĳЩcmd���ڿ������ִ�к󣬿��ܻ�ȡ�����֧�֣�����ñ��������ٴμ���
***************************************************************************/
void cct_disable_mouse(void)
{
	DWORD Mode;

	GetConsoleMode(__hin, &Mode);						 /* ȡ�ÿ���̨ԭ����ģʽ */
	SetConsoleMode(__hin, Mode & (~ENABLE_MOUSE_INPUT)); // ȥ�����֧�֣����ԭ���Ѳ�֧����꣬����Ҳû����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�����갴��
  ���������
  �� �� ֵ��
  ˵    ��������˵��������궨���ļ�
			01.typedef struct _MOUSE_EVENT_RECORD      //����¼��ṹ��
			02.{
			03.    COORD dwMousePosition;      //��ǰ����ڿ���̨���ڻ�������λ��
			04.    DWORD dwButtonState;        //��갴����״̬
			05.    DWORD dwControlKeyState;    //���Ƽ�״̬
			06.    DWORD dwEventFlags;         //����¼�����
			07.} MOUSE_EVENT_RECORD;
			08.
			09.������갴��״̬dwButtonState���ܵ�ֵ��
			10.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			11.FROM_LEFT_1ST_BUTTON_PRESSED        ����ߵ�����������      һ����˵����������
			12.FROM_LEFT_2ND_BUTTON_PRESSED        ����ڶ�������������    һ����˵������м������ǹ��ּ�
			13.FROM_LEFT_3RD_BUTTON_PRESSED        �������������������
			14.FROM_LEFT_4TH_BUTTON_PRESSED        ������ĸ�����������
			15.RIGHTMOST_BUTTON_PRESSED            ���ұߵ�����������      һ����˵������Ҽ�
			16.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			17.���Ƽ�״̬dwControlKeyState������¼���һ��
			18.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			19.ENHANCED_KEY        ��չ��������
			20.LEFT_ALT_PRESSED    ��Alt��������
			21.LEFT_CTRL_PRESSED   ��Ctrl��������
			22.RIGHT_ALT_PRESSED   ��Alt��������
			23.RIGHT_CTRL_PRESSED  ��Ctrl��������
			24.NUMLOCK_ON          ������������
			25.SCROLLLOCK_ON       ������������
			26.CAPSLOCK_ON         ��д��������
			27.SHIFT_PRESSED       Shift��������
			28.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			29.����¼�����dwEventFlags�����¼���
			30.~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			31.DOUBLE_CLICK            ˫������һ��ֻ��Ϊ��ͨ�����¼����ڶ�������Ϊ˫���¼�
			32.MOUSE_HWHEELED          ˮƽ�������ƶ�
			33.MOUSE_MOVED             ����ƶ�
			34.MOUSE_WHEELED           ��ֱ�������ƶ�
			35.0                       ������м������»����ͷ�
***************************************************************************/
int cct_read_keyboard_and_mouse(int &MX, int &MY, int &MAction, int &keycode1, int &keycode2)
{
	static int MX_old = -1, MY_old = -1, MAction_old = MOUSE_ONLY_MOVED;
	INPUT_RECORD InputRec;
	DWORD res;
	COORD crPos;

	while (1)
	{
		/* ��hin�ж�����״̬��������ꡢ���̵ȣ� */
		ReadConsoleInput(__hin, &InputRec, 1, &res);

		/* �����¼���Ҫ��������¼����������������Ŀ�������޷������� */
		if (InputRec.EventType == KEY_EVENT)
		{
			keycode1 = 0x00;
			keycode2 = 0x00;
			if (InputRec.Event.KeyEvent.bKeyDown)
			{ // ֻ�ڰ���ʱ�жϣ�����ʱ���ж�
				/* ���е��������ɲο�������ַ��
					https://baike.baidu.com/item/%E8%99%9A%E6%8B%9F%E9%94%AE%E7%A0%81/9884611?fr=aladdin
					��Ӧͷ�ļ���c:\Program Files (x86)\Windows Kits\10\Include\�汾��\um\WinUser.h (��VS2017Ϊ����ȱʡ·��)
					Ŀǰֻ�����ĸ���ͷ��������Ŀ����������� */
				switch (InputRec.Event.KeyEvent.wVirtualKeyCode)
				{
				case VK_UP:
					keycode1 = 0xe0;
					keycode2 = KB_ARROW_UP; // ģ�� _getch()��ʽ���ص��������룬�ֱ���224��0xE0����72��0x48��
					break;
				case VK_DOWN:
					keycode1 = 0xe0;
					keycode2 = KB_ARROW_DOWN; // ģ�� _getch()��ʽ���ص��������룬�ֱ���224��0xE0����80��0x50��
					break;
				case VK_LEFT:
					keycode1 = 0xe0;
					keycode2 = KB_ARROW_LEFT; // ģ�� _getch()��ʽ���ص��������룬�ֱ���224��0xE0����75��0x4B��
					break;
				case VK_RIGHT:
					keycode1 = 0xe0;
					keycode2 = KB_ARROW_RIGHT; // ģ�� _getch()��ʽ���ص��������룬�ֱ���224��0xE0����77��0x4D��
					break;
				default:
					break;
				} // end of switch

				/* �Ǽ�ͷ��ֱ�ӷ���ASCII��ʽ��Fn��Insert��Delete�Ⱦ�δ������ */
				if (keycode1 == 0)
					keycode1 = InputRec.Event.KeyEvent.uChar.AsciiChar;

				return CCT_KEYBOARD_EVENT;
			} // end of if (KEYDOWN)
		}	  // end of if (�����¼�)

		/* ����¼� */
		if (InputRec.EventType == MOUSE_EVENT)
		{
			/* �ӷ����ж����ָ�뵱ǰ������ */
			crPos = InputRec.Event.MouseEvent.dwMousePosition;
			MX = crPos.X;
			MY = crPos.Y;

			if (InputRec.Event.MouseEvent.dwEventFlags == MOUSE_MOVED)
			{ // ����ƶ�
				/* ���ʼ����MOUSE_MOVED�¼������겻�䣬����Ϊ��MOUSE_MOVED */
				if (MX_old == MX && MY_old == MY && MAction_old == MOUSE_ONLY_MOVED)
					continue;

				/* λ�ñ仯���¼���� */
				MX_old = MX;
				MY_old = MY;
				MAction = MOUSE_ONLY_MOVED;
				MAction_old = MAction;
				return CCT_MOUSE_EVENT;
			}
			MAction_old = MOUSE_NO_ACTION; // �÷�MOUSE_ONLY_MOVEDֵ����

			if (InputRec.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED)
			{ // �����ƶ�
				/* https://docs.microsoft.com/en-us/windows/console/mouse-event-record-str
					The vertical mouse wheel was moved.
					If the high word of the dwButtonState member contains a positive value, the wheel was rotated forward, away from the user.
					Otherwise, the wheel was rotated backward, toward the user. */
				if (InputRec.Event.MouseEvent.dwButtonState & 0x80000000) // ��λΪ1������
					MAction = MOUSE_WHEEL_MOVED_DOWN;
				else
					MAction = MOUSE_WHEEL_MOVED_UP;
				return CCT_MOUSE_EVENT;
			}

			if (InputRec.Event.MouseEvent.dwButtonState == (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED))
			{ // ͬʱ�������Ҽ�
				MAction = MOUSE_LEFTRIGHT_BUTTON_CLICK;
				return CCT_MOUSE_EVENT;
			}
			else if (InputRec.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{ // �������
				if (InputRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
					MAction = MOUSE_LEFT_BUTTON_DOUBLE_CLICK;
				else
					MAction = MOUSE_LEFT_BUTTON_CLICK;
				return CCT_MOUSE_EVENT;
			}
			else if (InputRec.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{ // �����Ҽ�
				if (InputRec.Event.MouseEvent.dwEventFlags == DOUBLE_CLICK)
					MAction = MOUSE_RIGHT_BUTTON_DOUBLE_CLICK;
				else
					MAction = MOUSE_RIGHT_BUTTON_CLICK;
				return CCT_MOUSE_EVENT;
			}
			else if (InputRec.Event.MouseEvent.dwButtonState == FROM_LEFT_2ND_BUTTON_PRESSED)
			{ // ���¹���
				MAction = MOUSE_WHEEL_CLICK;
				return CCT_MOUSE_EVENT;
			}
			else // ����������������
				continue;
		} // end of if(����¼�)
	}	  // end of while(1)

	return CCT_MOUSE_EVENT; // �˾�Ӧ��ִ�в�����Ϊ����ĳЩ������������ȫ��֧���󣬼�
}

typedef BOOL(WINAPI *PROCSETCONSOLEFONT)(HANDLE, DWORD);
typedef BOOL(WINAPI *PROCGETCONSOLEFONTINFO)(HANDLE, BOOL, DWORD, PCONSOLE_FONT_INFO);
typedef COORD(WINAPI *PROCGETCONSOLEFONTSIZE)(HANDLE, DWORD);
typedef DWORD(WINAPI *PROCGETNUMBEROFCONSOLEFONTS)();
typedef BOOL(WINAPI *PROCGETCURRENTCONSOLEFONT)(HANDLE, BOOL, PCONSOLE_FONT_INFO);
typedef BOOL(WINAPI *PROCSetBufferSize)(HANDLE hConsoleOutput, COORD dwSize);
typedef HWND(WINAPI *PROCGETCONSOLEWINDOW)();

#if 0 // �������룬0��ʾ�ر�
PROCSETCONSOLEFONT SetConsoleFont;
PROCGETCONSOLEFONTINFO GetConsoleFontInfo;
PROCGETCONSOLEFONTSIZE GetConsoleFontSize;
PROCGETNUMBEROFCONSOLEFONTS GetNumberOfConsoleFonts;
PROCGETCURRENTCONSOLEFONT GetCurrentConsoleFont;
#endif

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int cct_getfontinfo(void)
{
	HMODULE hKernel32 = GetModuleHandleA("kernel32");
	if (hKernel32 == 0)
		return -1;
	PROCSETCONSOLEFONT SetConsoleFont = (PROCSETCONSOLEFONT)GetProcAddress(hKernel32, "SetConsoleFont");
	PROCGETCONSOLEFONTINFO GetConsoleFontInfo = (PROCGETCONSOLEFONTINFO)GetProcAddress(hKernel32, "GetConsoleFontInfo");
	PROCGETCONSOLEFONTSIZE GetConsoleFontSize = (PROCGETCONSOLEFONTSIZE)GetProcAddress(hKernel32, "GetConsoleFontSize");
	PROCGETNUMBEROFCONSOLEFONTS GetNumberOfConsoleFonts = (PROCGETNUMBEROFCONSOLEFONTS)GetProcAddress(hKernel32, "GetNumberOfConsoleFonts");
	PROCGETCURRENTCONSOLEFONT GetCurrentConsoleFont = (PROCGETCURRENTCONSOLEFONT)GetProcAddress(hKernel32, "GetCurrentConsoleFont");
	//	PROCSetBufferSize SetConsoleBufSize = (PROCSetBufferSize)GetProcAddress(hKernel32,"SetConsoleScreenBufferSize");
	//	PROCGETCONSOLEWINDOW GetConsoleWindow = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow");

	CONSOLE_FONT_INFOEX infoex;
	char fontname[64];
	CONSOLE_FONT_INFO cur_f;
	int nFontNum;

	/* ȡ��ǰ��������ƣ�cmd��Ŀǰ�����֣�Terminal(��������)��������*/
	infoex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(__hout, 1, &infoex);
	WideCharToMultiByte(CP_ACP, 0, infoex.FaceName, -1, fontname, sizeof(fontname), NULL, NULL);
	cout << "��ǰ���壺" << fontname << endl;

	/* ��ӡ��ǰ������ֺ���Ϣ */
	nFontNum = GetNumberOfConsoleFonts();
	cout << "    �� �� �� �� ��" << nFontNum << endl;

	/* ȡ��ǰ���ֺ����� */
	GetCurrentConsoleFont(__hout, 0, &cur_f);
	cout << "    ��ǰ�ֺ���ţ�" << cur_f.nFont << endl;
	cout << "            ���ȣ�" << cur_f.dwFontSize.X << " pixels" << endl;
	cout << "            �߶ȣ�" << cur_f.dwFontSize.Y << " pixels" << endl;

	return 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ı�������ڵ��ֺ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void cct_setconsolefont(const int font_no)
{
	HMODULE hKernel32 = GetModuleHandleA("kernel32");
	if (hKernel32 == 0)
		return;

	PROCSETCONSOLEFONT SetConsoleFont = (PROCSETCONSOLEFONT)GetProcAddress(hKernel32, "SetConsoleFont");
	/* font_no width high
	   0       3     5
	   1       4     6
	   2       5     8
	   3       6     8
	   4       8     8
	   5       16    8
	   6       5     12
	   7       6     12
	   8       7     12
	   9       8     12
	   10      16    12
	   11      8     16
	   12      12    16
	   13      8     18
	   14      10    18
	   15      10    20 */
	SetConsoleFont(__hout, font_no);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ı�������ڵ����弰��С
  ���������
  �� �� ֵ��
  ˵    ����GBK�����cmd����ֻ֧��"��������"��"������"���֣�
			����fontnameʱ����"������"ȫ����Ϊȱʡ����(Terminal-"��������")
***************************************************************************/
void cct_setfontsize(const char *fontname, const int high, const int width)
{
	CONSOLE_FONT_INFOEX infoex = {0};

	infoex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	infoex.dwFontSize.X = width;   // ������ȣ�����Truetype���壬����Ҫ���ȣ�Ϊ0���ɣ����ڵ������壬�������Ϊ0����ѡָ���߶ȴ��ڵĿ���
	infoex.dwFontSize.Y = high;	   // ����߶�
	infoex.FontWeight = FW_NORMAL; // ����� wingdi.h

	MultiByteToWideChar(CP_ACP, 0, fontname, -1, infoex.FaceName, LF_FACESIZE);
	SetCurrentConsoleFontEx(__hout, NULL, &infoex);

	return;
}
