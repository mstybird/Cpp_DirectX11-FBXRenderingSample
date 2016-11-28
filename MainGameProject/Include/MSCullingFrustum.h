#pragma once

class DX11RenderResource;
class MSFbxManager;
class DXProjection;
//視錐台クラス
class MSCullingFrustum {
public:
	//ターゲットがキャラクターの視界内か調べる
	/*
		判定には境界球を用いる(境界球が作成されていない場合は内部で自動的に作られる
	*/
	bool IsCullingWorld(
		DX11RenderResource&pCameraResource,
		DX11RenderResource&pTargetResource
	);
};