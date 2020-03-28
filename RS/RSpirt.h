#pragma once
#include"System.h"
class RSpirt
{
public:

	XMMATRIX GetWorldXMMATRIX();


	bool LoadContent();
	void UnLoadContent();

	void Update(float dt);
	void Render();

	HRESULT CreateVertices(float width,float height);
private:

};

