#include"shader.hlsli"
VS_OUTPUT VS_Main( VS_OUTPUT input)
{
	input.Pos = mul(input.Pos,World);
	input.Pos = mul(input.Pos,View);
	input.Pos = mul(input.Pos,Projection);	
	return input;
}