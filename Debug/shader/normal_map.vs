uniform float4x4 m4World;
uniform float4x4 m4View;
uniform float4x4 m4Proj;

struct VS_INPUT
{
	float4 f4Pos : POSITION0;
	float3 f3Norm : NORMAL0;
	float2 f2Tex  : TEXCOORD0; 
};

struct VS_OUTPUT
{
	float4 f4Pos : POSITION0;
	float3 f3Norm : NORMAL0;
	float2 f2Tex : TEXCOORD0;
};

VS_OUTPUT main( VS_INPUT input )
{
	VS_OUTPUT output;
	
	float4x4 worldViewProj = mul( m4World , m4View );
	worldViewProj = mul( worldViewProj  , m4Proj );
	
	output.f4Pos = mul( input.f4Pos , worldViewProj ); 
	output.f3Norm = mul( input.f3Norm , m4View );
	output.f2Tex = input.f2Tex;
	
	return output;
}