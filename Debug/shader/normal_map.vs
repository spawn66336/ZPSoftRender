struct VS_INPUT
{
	float4 f4Pos : POSITION0;
	float3 f3Norm : NORMAL0;
	float2 f2Tex  : TEXCOORD0; 
	float3 f3Binormal : TEXCOORD1;
	float3 f3Tangent : TEXCOORD2;
};

struct VS_OUTPUT
{
	float4 f4Pos : POSITION0;
	float3 f3Norm : NORMAL0;
	float2 f2Tex : TEXCOORD0; 
	float3 f3LightDir : TEXCOORD1;
	float3 f3ViewDir : TEXCOORD2;
};
 
 
uniform float4x4 m4World;
uniform float4x4 m4View;
uniform float4x4 m4Proj;
uniform float3 f3LightPos;

VS_OUTPUT main( VS_INPUT input )
{
	VS_OUTPUT output;
	
	//构造从本地空间到屏幕空间的变换矩阵
	float4x4 worldView = mul( m4World , m4View );
	float4x4 worldViewProj = mul( worldView , m4Proj );  
	
	
	float4 f4ObjPosInView = mul( input.f4Pos , worldView );
	
 	float3 f3NormalInView = normalize( mul( input.f3Norm , worldView ) );
	float3 f3BinormalInView = normalize( mul( input.f3Binormal , worldView ) );
	float3 f3TangentInView = normalize( mul( input.f3Tangent , worldView ) );
	
	//将光线变换到视觉空间内
	float3 f3LightDirInView = f3LightPos - f4ObjPosInView.xyz;
	float3 f3ViewDirInView = -f4ObjPosInView.xyz;
	

	 
	output.f4Pos = mul( input.f4Pos , worldViewProj ); 
	output.f3Norm = f3NormalInView;
	output.f2Tex = input.f2Tex;
	
	output.f3ViewDir.x  = dot( f3TangentInView, f3ViewDirInView );
	output.f3ViewDir.y  = dot( f3BinormalInView, f3ViewDirInView );
	output.f3ViewDir.z  = dot( f3NormalInView, f3ViewDirInView );
   
	output.f3LightDir.x  = dot( f3TangentInView, f3LightDirInView );
	output.f3LightDir.y  = dot( f3BinormalInView, f3LightDirInView );
	output.f3LightDir.z  = dot( f3NormalInView, f3LightDirInView );
	
	return output;
}