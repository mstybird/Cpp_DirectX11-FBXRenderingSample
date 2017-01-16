#pragma once
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>
#include<string>
#include<unordered_map>
#include"DXMath.hpp"

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



	//Effekseer�ꌳ�Ǘ��p�f�[�^�x�[�X
	class EffectDatabase {
	public:
		//�G�t�F�N�g�f�[�^�x�[�X�̔j��
		~EffectDatabase();
		//db�̏�����
		void Initialize(::Effekseer::Manager*&aManager);
		//�G�t�F�N�g�t�@�C����o�^����
		bool Load(const char*aFileName, const int32_t aRegistID);
		//�o�^�ς݃G�t�F�N�g
		::Effekseer::Effect* Get(const int32_t aID)const;
		//�G�t�F�N�g�����݂��邩���ׂ�
		bool IsExist(const int32_t aID)const;
		//�o�^�ς݂̃G�t�F�N�g���폜����
		void CleanAll();
		//�w�肵��ID�����G�t�F�N�g���폜����
		void Clean(const int32_t aID);

		void Release();

	private:
		//����ID,�G�t�F�N�g
		std::unordered_map<int32_t, ::Effekseer::Effect*>mDatabase;
		::Effekseer::Manager* mManagerPtr;
	};

	//�G�t�F�N�g�p�J����
	struct EffectCamera {
		::Effekseer::Vector3D mEye;
		::Effekseer::Vector3D mLookAt;
		::Effekseer::Vector3D mUp;
		void SetDXCamera(DXCamera*aCamera);
		//		::Effekseer::Matrix44 GetLookAtLH();
		::Effekseer::Matrix44 GetLookAtLH();

	};

	struct EffectProjection {
		float mAngle;
		float mAspect;
		float mNear;
		float mFar;
		void SetDXProjection(DXProjection*aProjection);
		//		::Effekseer::Matrix44 GetPerspectiveFovLH();
		::Effekseer::Matrix44 GetPerspectiveFovLH();

	};

	class EfkManager;
	class EfkObject {
	public:
		void SetEffect(::Effekseer::Effect*& aEffect);
		void SetManager(EfkManager*aManager);

		void SetPosition(const ::Effekseer::Vector3D& aPosition);
		void AddPosition(const ::Effekseer::Vector3D& aPosition);
		void SetRotation(const ::Effekseer::Vector3D& aPosition);

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
		~EfkManager();
		void Initialize(::EffekseerRenderer::Renderer*&aRenderer, const int aInstanceMax = 2000);
		::Effekseer::Manager*& GetManager();
		::Effekseer::Manager* mManager;
		void Update();

		void Release();
	};

	class EfkRenderer {
	public:
		~EfkRenderer();

		void Initialize(ID3D11Device*aDevice, ID3D11DeviceContext*aDeviceContext, const int aDrawMax = 2000);

		::EffekseerRenderer::Renderer*& GetRenderer();

		void RenderAll(EfkManager*aManager);
		//		void Render(const int32_t aID);

		void SetProjection(EffectProjection* aProjection);
		void SetCamera(EffectCamera* aProjection);
		void SetCamera(DXCamera*aCamera);
		void Release();

	private:
		::EffekseerRenderer::Renderer* mRenderer;

	};
}