//-----------------------------------------------------
//	DirectX �e�L�X�g�`��T���v��
//	Direct3D�̕`��֘A
//	2010.03.04 TMO
//-----------------------------------------------------

#include "stdafx.h"
#include "Graphic.h"

//�uD3DFVF_TEX1�v��1�̓e�N�X�`���̖���
#define D3DFVF_2DVERTEX (D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_DIFFUSE )
typedef struct _D3DVERTEX_
{   D3DVECTOR	pos;		//���_�̈ʒu
	float		rhw;		//1������Ƃ�
	D3DCOLOR    color;		//diffuse	�|���S���̐F�B��ʂ̕���/�S�̃t�F�[�h�Ƃ��e�N�X�`���摜�̃A���t�@�l�𐧌䂷��̂Ɏg����
	FLOAT       tu;			//�e�N�X�`���̍��W
	FLOAT       tv;			//�e�N�X�`���̍��W

} D3DVERTEX;

CGraphic*				glbl_graphic = NULL;

LPDIRECT3D9				g_pD3D = NULL;			//D3D�C���^�[�t�F�[�X D3D�f�o�C�X�����̂ɕK�v
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;	//D3D�f�o�C�X �`��ȂǍL�͈͂ŕK�v�ɂȂ�B
IDirect3DVertexBuffer9* g_pVB = NULL;			//�|���S���̃o�b�t�@

//���̃R�[�h����쐬���ꂽD3D�f�o�C�X���擾���邽�߂̂���
extern LPDIRECT3DDEVICE9 GetDevice()
{
	return g_pD3DDevice;
}

//CGraphic�̍쐬
CGraphic* CreateGraphic( HWND hWnd )
{
	glbl_graphic = new CGraphic();
	glbl_graphic->Create( hWnd );
	return glbl_graphic;
}

//CGraphic�̎擾
CGraphic* GetGraphic() { return glbl_graphic; }

//CGraphic�̉��
void ReleaseGraphic()
{
	if ( glbl_graphic ) {
		glbl_graphic->Destroy();
		delete glbl_graphic;
		glbl_graphic = NULL;
	}
}


CGraphic::CGraphic()
{
	if ( g_pD3D != NULL ) {
		//CGraphic�͂Q���Ȃ����Ƃɂ��Ă���B
		_ASSERT(0);
	}

	m_backColor = 0xFFFFFFFF;

	m_currDisplayMode = DisplayMode::WindowMode;

	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	
	g_pD3DDevice   = NULL;
}
CGraphic::~CGraphic()
{
	Destroy();
}

//CGraphic�̍쐬
void CGraphic::Destroy()
{
	//D3D�f�o�C�X���������
	if( g_pD3DDevice )
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//D3D�C���^�[�t�F�[�X���������
	if( g_pD3D )
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//CGraphic�̍쐬( Direct3D�̏��������� )
bool CGraphic::Create(HWND hWnd)
{
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if ( g_pD3D == NULL ) {
		return false;
	}
	
	//D3DPRESENT_PARAMETERS�\���̏������̂��߃f�B�X�v���C���[�h���擾����
	D3DDISPLAYMODE dmode;
	ZeroMemory( &dmode, sizeof( dmode ) );
	g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &dmode );
	
	//�f�o�C�X�̏�Ԃ��w�肷��
	D3DPRESENT_PARAMETERS	d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );

	switch ( m_currDisplayMode ) {
		case DisplayMode::FullScreen:
			//�t���X�N���[��
			d3dpp.Windowed         = FALSE;
			d3dpp.BackBufferWidth  = 640;
			d3dpp.BackBufferHeight = 480;
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
			d3dpp.BackBufferCount  = 1;
			d3dpp.SwapEffect       = D3DSWAPEFFECT_DISCARD;
			d3dpp.FullScreen_RefreshRateInHz = 60;
			break;
		case DisplayMode::WindowMode:
			//�E�B���h�E���[�h
			d3dpp.Windowed			= TRUE;
			d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;
			d3dpp.BackBufferFormat	= dmode.Format;
			d3dpp.BackBufferCount	= 1;
			break;
	}

	//�f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬����
	//�`�揈�����@�ƒ��_�������@�̓n�[�h�E�F�A���������Ή��ł���Ǝ��s����̂ŁA�������@�̑g�ݍ��킹��ς��čēx�쐬�����݂�
	HRESULT		hr;
	D3DDEVTYPE	devtype;
	DWORD		behaviorFlags;
	for ( int c = 0; c < 4; c++ ) {
		switch ( c ) {
            case 0:
				//�n�[�h�E�F�A�ŕ`�揈�����s��
				devtype			= D3DDEVTYPE_HAL;						
				//���_�������n�[�h�E�F�A�ōs��
				behaviorFlags	= D3DCREATE_HARDWARE_VERTEXPROCESSING;
				break;
			case 1:
				//�n�[�h�E�F�A�ŕ`�揈�����s��
				devtype			= D3DDEVTYPE_HAL;
				//���_�������\�t�g�E�F�A�ōs��
				behaviorFlags	= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
				break;
			case 2:
				//�\�t�g�E�F�A�ŕ`�揈�����s��
				devtype			= D3DDEVTYPE_REF;
				//���_�������n�[�h�E�F�A�ōs��
				behaviorFlags	= D3DCREATE_HARDWARE_VERTEXPROCESSING;
				break;
			case 3:
				//�\�t�g�E�F�A�ŕ`�揈�����s��
				devtype			= D3DDEVTYPE_REF;						
				//���_�������\�t�g�E�F�A�ōs��
				behaviorFlags	= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
				break;
		}
		
		hr = g_pD3D->CreateDevice(
						D3DADAPTER_DEFAULT,
						devtype,
						hWnd,
						behaviorFlags,
						&d3dpp,
						&g_pD3DDevice );
		
		
		if ( FAILED(hr) ) {
			continue;
		}
		break;
	}

	if ( FAILED(hr) ) {
		//�\�t�g�E�F�A�����ɂ��Ή����Ă��Ȃ�
		return false;
	}

	//�A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	//�L���������ł͎v���ʂ�ɓ��߂��Ȃ��͂��Ȃ̂ŁA�u�����f�B���O�W���̐ݒ���s���B
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	 
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); 

	//�e�N�X�`���̃A���t�@�ɁA�|���S���F���A���t�@�Ƃ��ĉ�������ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE );
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

	//�|���S���̍쐬
	g_pD3DDevice->CreateVertexBuffer(sizeof(D3DVERTEX)*4, 0, D3DFVF_2DVERTEX, D3DPOOL_MANAGED, &g_pVB, NULL );

	return true;
}

//�`��O�ɌĂ�
void CGraphic::ClearAndBegin()
{
	//��ʃN���A
	g_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, m_backColor, 1.0f, 0 );

	//�f�o�C�X�ɕ`��J�n��m�点��
	HRESULT hr;
	hr = g_pD3DDevice->BeginScene();
}

//�`���ɌĂ�
void CGraphic::EndAndPresent()
{
	//�f�o�C�X�ɕ`��I����m�点��
	g_pD3DDevice->EndScene();
	
	//��ʂ��o�b�N�o�b�t�@�Ɠ���ւ���B
	g_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

//�F�̓�������`��`��B�������ł��܂��B����/�S�̂̃t�F�[�h�Ɏg����
void CGraphic::DrawColorRect( float left, float top, float right, float bottom, DWORD color )
{
	D3DVERTEX *d3dv;
	g_pVB->Lock(0 , 0 , (void**)&d3dv , 0);
	d3dv[0].pos.x = left;
	d3dv[0].pos.y = top;
	d3dv[0].pos.z = 0;
	d3dv[0].rhw = 1;
	d3dv[0].color = color;
	d3dv[0].tu = 0.0f;
	d3dv[0].tv = 0.0f;

	d3dv[1] = d3dv[0];
	d3dv[1].pos.x = right;
	d3dv[1].pos.y = top;
	d3dv[1].tu = 1.0f;
	d3dv[1].tv = 0.0f;

	d3dv[2] = d3dv[0];
	d3dv[2].pos.x = left;
	d3dv[2].pos.y = bottom;
	d3dv[2].tu = 0.0f;
	d3dv[2].tv = 1.0f;

	d3dv[3] = d3dv[0];
	d3dv[3].pos.x = right;
	d3dv[3].pos.y = bottom;
	d3dv[3].tu = 1.0f;
	d3dv[3].tv = 1.0f;

	g_pVB->Unlock();
	
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetVertexShader(NULL);
	g_pD3DDevice->SetFVF( D3DFVF_2DVERTEX );
	g_pD3DDevice->SetStreamSource( 0, g_pVB, 0, sizeof(D3DVERTEX) );
	g_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

}

//�`��
void CGraphic::Draw( float left, float top, float right, float bottom, DWORD alphaColor )
{
	//�w��ʒu���̂܂܂ŕ`���ƃ|���S���̋��E�ł킸���Ȃ��ꂪ�������܂��B���������_���g�p���Ă��邽�߂��Ƃ��E�E�E�B
	//���_���W�����炷���Ƃł��ꂢ�ɕ`����B
	left	-= 0.5f;
	top		-= 0.5f;
	right	-= 0.5f;
	bottom	-= 0.5f;

	D3DVERTEX *d3dv;
	g_pVB->Lock(0 , 0 , (void**)&d3dv , 0);
	d3dv[0].pos.x = left;
	d3dv[0].pos.y = top;
	d3dv[0].pos.z = 0;
	d3dv[0].rhw = 1;
	d3dv[0].color = alphaColor;
	d3dv[0].tu = 0.0f;
	d3dv[0].tv = 0.0f;

	d3dv[1] = d3dv[0];
	d3dv[1].pos.x = right;
	d3dv[1].pos.y = top;
	d3dv[1].tu = 1.0f;
	d3dv[1].tv = 0.0f;

	d3dv[2] = d3dv[0];
	d3dv[2].pos.x = left;
	d3dv[2].pos.y = bottom;
	d3dv[2].tu = 0.0f;
	d3dv[2].tv = 1.0f;

	d3dv[3] = d3dv[0];
	d3dv[3].pos.x = right;
	d3dv[3].pos.y = bottom;
	d3dv[3].tu = 1.0f;
	d3dv[3].tv = 1.0f;

	g_pVB->Unlock();
	
	g_pD3DDevice->SetVertexShader(NULL);
	g_pD3DDevice->SetFVF( D3DFVF_2DVERTEX );
	g_pD3DDevice->SetStreamSource( 0, g_pVB, 0, sizeof(D3DVERTEX) );
	g_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
}

void CGraphic::Draw( float left, float top, float right, float bottom, CTexture* texture, DWORD alphaColor )
{
	texture->SetCurrentUse();
	Draw( left, top, right, bottom, alphaColor );
}