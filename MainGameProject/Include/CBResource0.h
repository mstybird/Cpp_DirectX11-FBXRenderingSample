#pragma once
#include"MSLight.h"
#include<DXMath.hpp>
struct CBResource0 {
public:
	MSLight mLight;

	D3DXVECTOR4 vEye;//カメラ位置
	D3DXVECTOR4 vAmbient;//アンビエント光
	D3DXVECTOR4 vDiffuse;//ディフューズ色
	D3DXVECTOR4 vSpecular;//鏡面反射


};