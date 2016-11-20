//-----------------------------------------------------
//	DirectX �e�L�X�g�`��T���v��
//	2010.03.04 TMO
//-----------------------------------------------------

#include "stdafx.h"
#include "Graphic.h"
#include "KeyDownCheck.h"

class MyApp
{
	//�e�N�X�`���摜�������e�L�X�g�f�[�^
	CTexture	m_texture;
	TextGraphic m_text;
	
	//�L�[���̓`�F�b�N
	KeyDownChk	m_chkNumA;
	KeyDownChk	m_chkNumS;
	KeyDownChk	m_chkNumD;
	KeyDownChk	m_chkNumF;
	KeyDownChk	m_chkNumZ;
	KeyDownChk	m_chkNumX;
	KeyDownChk	m_chkNumC;

public:
	//�T���v���A�v���̏���������
	void Init(HWND hWnd)
	{
		//DirectX������
		CreateGraphic( hWnd );

		//�w�i�F��
		GetGraphic()->m_backColor = 0x22222222;

		//�w�i�摜�̍쐬
		m_texture.Load(TEXT(".\\image\\sakura.jpg"));

		//�t�H���g�̏���
		LOGFONT	logFont;
		::ZeroMemory( &logFont, sizeof( logFont ) );
	    logFont.lfHeight = 20;	//�t�H���g�T�C�Y
		logFont.lfWidth = 0;
		logFont.lfEscapement = 0;
		logFont.lfOrientation = 0;
		logFont.lfWeight = FW_EXTRABOLD;
		logFont.lfItalic = 0;
		logFont.lfUnderline = 0;
		logFont.lfStrikeOut = 0;
		logFont.lfCharSet   = SHIFTJIS_CHARSET;
		logFont.lfOutPrecision = OUT_TT_ONLY_PRECIS;
		logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logFont.lfQuality = PROOF_QUALITY;
		logFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;

#ifdef UNICODE
		//wcscpy_s�̑������͕���(�z��)��(�o�b�t�@�T�C�Y���ƃf�o�b�K���I�����Ȃ������肷��)
		wcscpy_s( logFont.lfFaceName, LF_FACESIZE, _T("�l�r ����") );
#else
		strcpy_s( logFont.lfFaceName, LF_FACESIZE, _T("�l�r ����") );
#endif	
		
		//�e�L�X�g�摜�̍쐬
		m_text.Create(
			_T("�����\���̃e�X�g�v���O�����ł��B\n�r�W���A���m�x���݂����ɂ��Ă݂܂����B\n")
			_T("�ꕶ�����ƂɃe�N�X�`���������ĕ`������܂��B�w��G���A����͂ݏo��Ɛ܂�Ԃ��Ă܂���B\n")
			_T("\n")
			_T("Z�L�[:����\n")
			_T("X�L�[:������\n")
			_T("C�L�[:�E��\n")
			_T("\n")
			_T("A�L�[:�e�L�X�g�ʏ�`��\n")
			_T("S�L�[:�e�L�X�g������\n")
			_T("D�L�[:�e�L�X�g�����i�������Ȃ��Ȃ�܂��j\n")
			_T("\n")
			_T("F�L�[:�����F��Ԃɂ���\n")
			,50,50,640-50,480-50//�e�L�X�g�̕`��G���A
			,logFont );


		//�L�[�_�E���`�F�b�N�̓o�^
		m_chkNumA.SetKey('A');
		m_chkNumS.SetKey('S');
		m_chkNumD.SetKey('D');
		m_chkNumF.SetKey('F');
		m_chkNumZ.SetKey('Z');
		m_chkNumX.SetKey('X');
		m_chkNumC.SetKey('C');

	}

	void Roop()
	{
		//�L�[���͂ɂ�鏈��
		if ( m_chkNumA.Actioned() ) {
			m_text.SetTextAlpha(0xffffffff);
		} else
		if ( m_chkNumS.Actioned() ) {
			m_text.SetTextAlpha(0x88888888);
		} else
		if ( m_chkNumD.Actioned() ) {
			m_text.SetTextAlpha(0x00000000);
		} else
		if ( m_chkNumF.Actioned() ) {
			//�����̐F�𔒂ō���Ă���̂ŁA�A���t�@�ȊO�̃r�b�g���������ĐԂ��e�L�X�g�ɂ��邱�Ƃ��ł��܂��B
			m_text.SetTextAlpha(0xffff0000);
		} else
		if ( m_chkNumZ.Actioned() ) {
			m_text.SetTextAlign(TextAlign::AlignLeft);
		} else
		if ( m_chkNumX.Actioned() ) {
			m_text.SetTextAlign(TextAlign::AlignCenter);
		} else
		if ( m_chkNumC.Actioned() ) {
			m_text.SetTextAlign(TextAlign::AlignRight);
		}

		//�ȍ~�`�揈��------------

		GetGraphic()->ClearAndBegin();

		//�w�i�`��
		GetGraphic()->Draw( 0, 0, 640, 480, &m_texture, 0xffffffff );

		//�w�i�𕔕��I�ɈÂ����邽�߂ɔ������̍�����`��
		GetGraphic()->DrawColorRect(50,50,640-50, 480-50,0xaa000000);
	
		//�e�L�X�g�`��
		m_text.Draw();

		GetGraphic()->EndAndPresent();
	}

	//�T���v���A�v���̉������
	void Release()
	{
		//�e�N�X�`���̉��
		m_text.Release();

		//DirectX�̉��
		ReleaseGraphic();
	}

};
static MyApp myApp;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		//WM_PAINT�ł͕`�悵�܂���B
		//ValidateRect���g���Ǝ�����WM_PAINT���Ă΂�Ȃ��Ȃ�B�i�t���X�N���[���\���̎������j
		ValidateRect( hWnd, NULL );
		break;
	case WM_DESTROY:
		myApp.Release();//�T���v���A�v���������
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MSG msg;
	
	//�E�B���h�E�N���X�̓o�^
	WNDCLASSEX wcex;
	memset( &wcex, 0, sizeof(wcex) );
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= TEXT("wcDrawSample");

	if( !RegisterClassEx(&wcex) ) {
		return FALSE;
	}

	HWND hWnd;
	hWnd = CreateWindow(TEXT("wcDrawSample"), TEXT("�`��T���v��"), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
	CW_USEDEFAULT, 0, 640, 480, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	
	{
		//�N���C�A���g�T�C�Y��640��480�ɂ���B
		//���݃E�B���h�E�T�C�Y��640��480�Ȃ̂ŃN���C�A���g�T�C�Y���擾���Ęg��^�C�g���������߂�
		RECT rect;
		GetClientRect( hWnd, &rect );
		int ofsW = 640 - ( rect.right - rect.left );
		int ofsH = 480 - ( rect.bottom - rect.top );
		SetWindowPos( hWnd, NULL, 0, 0, 640+ofsW, 480+ofsH, SWP_NOMOVE | SWP_NOZORDER );
	}

	//�T���v���A�v���̏�����
	myApp.Init(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	for (;;) {
		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
			
			if ( GetMessage( &msg, NULL, 0, 0 ) > 0) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else {
				break;
			}

		} else {
			//�Q�[���̏����������ł���
			myApp.Roop();
		}
	}

	return (int) msg.wParam;
}
