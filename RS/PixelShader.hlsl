#include"Shader.hlsli"
float4 PS_Main(VS_OUTPUT input) : SV_TARGET
{
	return input.Color;
}