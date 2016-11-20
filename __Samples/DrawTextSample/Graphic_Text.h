//-----------------------------------------------------
//	DirectX �e�L�X�g�`��T���v��
//	�e�L�X�g�摜�̍쐬�A�`��Ȃ�
//	2010.03.04 TMO
//-----------------------------------------------------
#pragma once

//TextGraphic��TextuteTextLine�������P�������̉摜�f�[�^
struct TxtureChar
{
	LPDIRECT3DTEXTURE9	m_txtr;	//�e�N�X�`���摜
	int					m_wid;	//������
};

//�s����
enum TextAlign
{
	AlignLeft,
	AlignRight,
	AlignCenter
};

//TextGraphic������1�s���̃e�L�X�g
class TextuteTextLine : public std::vector<TxtureChar>
{
public:
	TextAlign	m_align;	//�s����
	int			m_lineWidth;//�P�s�̕�(�s�N�Z��)

	TextuteTextLine();
	~TextuteTextLine();
};

//Graphic�@�\�������e�L�X�g�f�[�^
class TextGraphic
{
	std::vector< TextuteTextLine* > m_lineAry;//�s���ƂɃf�[�^�������܂��B

	IRECT	m_rect;			//�`��͈�
	int		m_fontsize;		//�t�H���g�T�C�Y
	DWORD	m_alphaColor;	//�A���t�@�F( �`�悷��|���S����Diffuse�F )

public:

	TextGraphic();
	virtual ~TextGraphic();

	//�e�L�X�g�摜���쐬
	void Create( LPCTSTR txt, int left, int top, int right, int bottom, LOGFONT& logFont );
	
	//�s���낦�̎w��(�S�s)
	void SetTextAlign( TextAlign align );

	//�s���낦�̎w��(�C�ӂ̕����s)
	void SetTextAlign(
		unsigned int lineIdx,	//�J�n�ʒu
		unsigned int lineIdxTo,	//�I���ʒu
		TextAlign align );

	//�e�L�X�g�̃A���t�@�l(�`�悷��|���S����Diffuse�F)�ύX
	void SetTextAlpha(DWORD alpha);

	void Release();
	void Draw();
};