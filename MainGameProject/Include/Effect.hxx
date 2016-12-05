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



	//Effekseer一元管理用データベース
	class EffectDatabase {
	public:
		~EffectDatabase();
		void Initialize(::Effekseer::Manager*&aManager);
		//エフェクトファイルを登録する
		bool Load(const char*aFileName, const int32_t aRegistID);
		//登録済みエフェクト
		::Effekseer::Effect* Get(const int32_t aID)const;
		bool IsExist(const int32_t aID)const;
		void CleanAll();
		void Clean(const int32_t aID);

		void Release();

	private:
		//識別ID,エフェクト
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
		::Effekseer::Vector3D mPosition;	//エフェクト座標
											//再生登録したマネージャが入る
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

		void Release();

	private:
		::EffekseerRenderer::Renderer* mRenderer;

	};
}