#pragma once
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>
#include<string>
#include<unordered_map>
#include"../../DXCamera.h"
#include"../../DXProjection.h"

#if _DEBUG
#pragma comment(lib, "VS2015/Debug/Effekseer.lib" )
#pragma comment(lib, "VS2015/Debug/EffekseerRendererDX11.lib" )
#pragma comment(lib, "VS2015/Debug/EffekseerSoundXAudio2.lib" )
#else
#pragma comment(lib, "VS2015/Release/Effekseer.lib" )
#pragma comment(lib, "VS2015/Release/EffekseerRendererDX11.lib" )
#pragma comment(lib, "VS2015/Release/EffekseerSoundXAudio2.lib" )
#endif

namespace Comfort {
	


	//Effekseer���[�_
	class EffectDatabase {
	public:
		void Initialize(::Effekseer::Manager*&aManager);
		//�G�t�F�N�g�t�@�C����o�^����
		bool Load(const char*aFileName, const int32_t aRegistID);
		//�o�^�ς݃G�t�F�N�g
		::Effekseer::Effect* Get(const int32_t aID)const;
		bool IsExist(const int32_t aID)const;
		void CleanAll();
		void Clean(const int32_t aID);
	private:
		//����ID,�G�t�F�N�g
		std::unordered_map<int32_t, ::Effekseer::Effect*>mDatabase;
		::Effekseer::Manager* mManagerPtr;
	};

	struct EffectCamera {
		::Effekseer::Vector3D mEye;
		::Effekseer::Vector3D mLookAt;
		::Effekseer::Vector3D mUp;
		void SetDXCamera(DXCamera*aCamera);
//		::Effekseer::Matrix44 GetLookAtLH();
		::Effekseer::Matrix44 GetLookAtRH();

	};

	struct EffectProjection {
		float mAngle;
		float mWidth;
		float mHeight;
		float mNear;
		float mFar;
		void SetDXProjection(DXProjection*aProjection);
//		::Effekseer::Matrix44 GetPerspectiveFovLH();
		::Effekseer::Matrix44 GetPerspectiveFovRH();

	};

	class EfkManager;
	class EfkObject {
	public:
		void SetEffect(::Effekseer::Effect*& aEffect);
		void SetManager(EfkManager*aManager);

		void SetPosition(const ::Effekseer::Vector3D& aPosition);
		void AddPosition(const ::Effekseer::Vector3D& aPosition);

		void Play();
		void Stop();
	private:
		::Effekseer::Effect* mEffect;
		::Effekseer::Handle mHandle = -1;
		::Effekseer::Vector3D mPosition;	//�G�t�F�N�g���W
		//�Đ��o�^�����}�l�[�W��������
		::Effekseer::Manager* mParentManager;


	};

	class EfkManager {
	public:
		void Initialize( ::EffekseerRenderer::Renderer*&aRenderer, const int aInstanceMax = 2000);
		::Effekseer::Manager*& GetManager();
		::Effekseer::Manager* mManager;
		void Update();
	};

	class EfkRenderer {
	public:
		void Initialize(ID3D11Device*aDevice, ID3D11DeviceContext*aDeviceContext, const int aDrawMax = 2000);

		::EffekseerRenderer::Renderer*& GetRenderer();

		void RenderAll(EfkManager*aManager);
//		void Render(const int32_t aID);

		void SetProjection(EffectProjection* aProjection);
		void SetCamera(EffectCamera* aProjection);
	private:
		::EffekseerRenderer::Renderer* mRenderer;

	};


	////���ۂɎg���G�t�F�N�g
	//class Effect {

	//public:
	//	//�����_���쐬
	//	bool CreateRenderer(ID3D11Device*&aDevice, ID3D11DeviceContext*&aDeviceContext, const int aDrawMax = 2000);
	//	//�G�t�F�N�g�}�l�[�W���쐬
	//	bool CreateManager(const int aInstanceMax = 2000);
	//	//�����_�����O�@�\�ݒ�
	//	void SetRenderFunction();
	//	//�e�N�X�`���ǂݍ��݋@�\�ݒ�
	//	void SetTextureLoader();

	//	//���Đ��p�C���X�^���X����
	//	bool CreateSound(IXAudio2*&aAudio, const int32_t a1ChSoundCount = 16, const int32_t a2ChSoundCount = 16);
	//	//�Đ��@�\��ݒ�
	//	void SetSoundPlayer();
	//	void SetSoundLoader();
	//	
	//	void SetEyePosition(const ::Effekseer::Vector3D& aEye);
	//	void SetLookPosition(const ::Effekseer::Vector3D& aLook);
	//	void SetUpVector(const ::Effekseer::Vector3D& aUp);
	//	//�G�t�F�N�g�̍��W�ݒ�
	//	void SetLocation(const ::Effekseer::Vector3D& aLocation);

	//	bool Load(const char* aFileName);
	//	void Play();
	//	void Stop();
	//	void Release();

	//	void Update();
	//	void Render();
	//public:
	//	::Effekseer::Manager* mManager;
	//	::EffekseerRenderer::Renderer* mRenderer;
	//	::EffekseerSound::Sound* mSound;
	//	::Effekseer::Effect* mEffect;
	//	::Effekseer::Handle mHandle = -1;
	//	::Effekseer::Vector3D mPosition;	//�G�t�F�N�g���W
	//	EffectCamera mCamera;
	//	EffectProjection mProjection;
	//};

}