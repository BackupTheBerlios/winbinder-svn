/*******************************************************************************

 WINBINDER - A native Windows binding

 Copyright � 2004-2006 Hypervisual - see LICENSE.TXT for details
 Author: Rubem Pechansky (http://www.hypervisual.com/winbinder/contact.php)

 Implements HyperLink control

*******************************************************************************/

//----------------------------------------------------------------- DEPENDENCIES

#include "wb.h"

//-------------------------------------------------------------------- CONSTANTS

#define COLOR_HYPERLINK		RGB(0, 0, 192)

//---------------------------------------------------------- FUNCTION PROTOTYPES

extern WNDPROC lpfnHyperLinkProcOld;

static BOOL DrawHyperLink(HDC hdc, HWND hwnd, LPRECT lprc, COLORREF color);

//-------------------------------------------------------------------- VARIABLES

static BOOL bUnderline = FALSE;

//----------------------------------------------------------- EXPORTED FUNCTIONS

// Code adapted from Neal Stublen, thanks to Davide
// http://www.codeguru.com/Cpp/controls/staticctrl/article.php/c5803/

LRESULT CALLBACK HyperLinkProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg) {

		case WM_PAINT:
			{
				HDC hdc;
				PAINTSTRUCT ps;

				PWBOBJ pwbobj = wbGetWBObj(hwnd);

				if(!pwbobj)
					break;
				hdc = BeginPaint(hwnd, &ps);
				DrawHyperLink(hdc, hwnd, &ps.rcPaint,
				  pwbobj->lparam == CLR_INVALID ? COLOR_HYPERLINK : pwbobj->lparam);
				EndPaint(hwnd, &ps);
			}
			return 0;

		case WM_MOUSEMOVE:

			if(GetCapture() != hwnd) {
				bUnderline = TRUE;
				InvalidateRect(hwnd, NULL, FALSE);
				SetCapture(hwnd);
			} else {
				RECT rect;
				POINT pt;

				GetWindowRect(hwnd, &rect);
				pt.x = LOWORD(lParam);
				pt.y = HIWORD(lParam);
				ClientToScreen(hwnd, &pt);

				if(!PtInRect(&rect, pt)) {
					bUnderline = FALSE;
					InvalidateRect(hwnd, NULL, FALSE);
					ReleaseCapture();
				}
			}
			break;

		case WM_SETCURSOR:
			{
				HCURSOR hCursor;

				hCursor = LoadCursor(NULL, IDC_HAND);
				if(hCursor)
					SetCursor(hCursor);
			}
			return 0;
	}

	return CallWindowProc(lpfnHyperLinkProcOld, hwnd, msg, wParam, lParam);
}

//------------------------------------------------------------ PRIVATE FUNCTIONS

/*static HFONT GetUnderlineFont(HFONT hOrigFont)
{
	HFONT hUnderFont;
	LOGFONT lf;

	GetObject(hOrigFont, sizeof(LOGFONT), &lf);
	lf.lfUnderline = TRUE;
	hUnderFont = CreateFontIndirect(&lf);
	return hUnderFont;
}
*/

static BOOL DrawHyperLink(HDC hdc, HWND hwnd, LPRECT lprc, COLORREF color)
{
	char szString[1024];
	int nLen;
	COLORREF clOld;
	HFONT hFont, hfOld;
	HBRUSH hbr, hbrOld;
	BOOL bRet;

	GetWindowText(hwnd, szString, 1023);
	nLen = strlen(szString);
	if(!nLen)
		return FALSE;

	// Draw a background rectangle

	hbr = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	hbrOld = SelectObject(hdc, hbr);
	PatBlt(hdc, lprc->left, lprc->top, lprc->right - lprc->left, lprc->bottom - lprc->top,
	  PATCOPY);
	SelectObject(hdc, hbrOld);
	DeleteObject(hbr);

	// Prepare to draw text

	hFont = (HFONT)SendMessage(hwnd, WM_GETFONT, 0, 0);
	clOld = SetTextColor(hdc, color);
	SetBkMode(hdc, TRANSPARENT);
	hfOld = SelectObject(hdc, hFont);

	// Draw the text

	bRet = DrawTextEx(hdc, (PSZ)szString, nLen, lprc, DT_LEFT | DT_SINGLELINE, NULL);

	// Draw a line under the text

	if(bUnderline) {
		SIZE siz;

		if(GetTextExtentPoint32(hdc, (PSZ)szString, nLen, &siz)) {
			wbDrawLine(hdc, lprc->left, lprc->top + siz.cy - 1,
			  lprc->left + siz.cx, lprc->top + siz.cy - 1, color, 0, 0);
		}
	}

	// Restore the original context

	SelectObject(hdc, hfOld);
	SetBkMode(hdc, OPAQUE);
	SetTextColor(hdc, clOld);

	return bRet;
}

//------------------------------------------------------------------ END OF FILE