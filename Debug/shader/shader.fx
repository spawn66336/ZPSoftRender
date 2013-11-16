
#include "global.fx"

//顶点着色器输入
struct VS_INPUT
{
	float4 f4Pos : POSITION0;
	float3 f3Norm : NORMAL0;
	float2 f2Tex  : TEXCOORD0; 
	float3 f3Binormal : TEXCOORD1;
	float3 f3Tangent : TEXCOORD2;
};

//顶点着色器输出
struct VS_OUTPUT
{
	float4 f4Pos : POSITION0;
	float3 f3Norm : NORMAL0;
	float2 f2Tex : TEXCOORD0; 
	float3 f3LightDir : TEXCOORD1;
	float3 f3ViewDir : TEXCOORD2;
};

//像素着色器输入
struct PS_INPUT
{
	float3 f3Norm : NORMAL0;
	float2 f2Tex : TEXCOORD0; 
	float3 f3LightDir : TEXCOORD1;
	float3 f3ViewDir : TEXCOORD2;
};

//像素着色器输出
struct PS_OUTPUT
{ 
	float4 f4Color : COLOR0; 
};


//材质结构体
struct Material_t
{ 
	float4 f4Diff;
	float4 f4Amb;
	float4 f4Spec;
	float4 f4Emis;
	float  fPower;
};

 
float4x4 m4World;
uniform float3 f3LightPos;
sampler2D diffuseTex;
sampler2D normalTex;
uniform Material_t g_Material;



VS_OUTPUT NormalMapShadingVS( VS_INPUT input )
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


PS_OUTPUT NormalMapShadingPS( PS_INPUT input )
{
	PS_OUTPUT output;
	float3 f3Normal = normalize( ( tex2D( normalTex, input.f2Tex ).xyz * 2.0f ) - 1.0f );
	float4 f4DiffColor = tex2D( diffuseTex , input.f2Tex );
	float3 f3ViewDir = normalize( input.f3ViewDir );
	float  fDiffFactor   = max( 0.0f , dot( f3Normal, normalize(input.f3LightDir) ) ); 
	float3 f3H     = normalize( input.f3LightDir + input.f3ViewDir );
	float  fSpecFactor    = max( 0.0f, dot( f3Normal , f3H ) );
	
    float4 f4TotalAmbient   = g_Material.f4Amb * f4DiffColor; 
    float4 f4TotalDiffuse   = g_Material.f4Diff * fDiffFactor * f4DiffColor; 
    float4 f4TotalSpecular  = g_Material.f4Spec * pow( fSpecFactor, g_Material.fPower );
   
	output.f4Color = saturate( f4TotalAmbient + f4TotalDiffuse + f4TotalSpecular ); 
	return output;
}



VS_OUTPUT PhongShadingVS( VS_INPUT input )
{
	VS_OUTPUT output;
	//构造从本地空间到屏幕空间的变换矩阵
	float4x4 worldView = mul( m4World , m4View );
	float4x4 worldViewProj = mul( worldView , m4Proj );  
	
	 
 	float3 f3NormalInView = normalize( mul( input.f3Norm , worldView ) );  
	float4 f4ObjPosInView = mul( input.f4Pos , worldView );
	
 
	output.f4Pos = mul( input.f4Pos , worldViewProj ); 
	output.f3Norm = f3NormalInView;
	output.f2Tex = input.f2Tex;
	output.f3LightDir = f3LightPos - f4ObjPosInView.xyz;
	output.f3ViewDir = -f4ObjPosInView.xyz;
	 
	return output;
}


PS_OUTPUT PhongShadingPS( PS_INPUT input )
{
	PS_OUTPUT output;
	float3 f3Normal = input.f3Norm;
	f3Normal = normalize( f3Normal );
	float4 f4DiffColor = tex2D( diffuseTex , input.f2Tex );
	float3 f3ViewDir = normalize( input.f3ViewDir );
	float  fDiffFactor   = max( 0.0f , dot( f3Normal, normalize(input.f3LightDir) ) ); 
	float3 f3H     = normalize( input.f3LightDir + input.f3ViewDir );
	float  fSpecFactor    = max( 0.0f, dot( f3Normal , f3H ) );
	
    float4 f4TotalAmbient   = g_Material.f4Amb * f4DiffColor; 
    float4 f4TotalDiffuse   = g_Material.f4Diff * fDiffFactor * f4DiffColor; 
    float4 f4TotalSpecular  = g_Material.f4Spec * pow( fSpecFactor, g_Material.fPower );
   
	output.f4Color = saturate( f4TotalAmbient + f4TotalDiffuse + f4TotalSpecular ); 
	return output;
}

technique NormalMapShading
{
	pass P0
	{
		VertexShader = compile vs_3_0 NormalMapShadingVS();
		PixelShader  = compile ps_3_0 NormalMapShadingPS();
	}
}

technique PhongShading
{
	pass P0
	{
		VertexShader = compile vs_3_0 PhongShadingVS();
		PixelShader  = compile ps_3_0 PhongShadingPS();
	}
}
