// Posterize: 画面の色に gParam.color を掛ける（gView.time・gView.screenSize も使える）
#include "FullScreen.hlsli"
#include "Common.hlsli"
#include "View.hlsli"

struct Param
{
    float iLevel;
};

ConstantBuffer<Param> gParam : register(b0);
ConstantBuffer<View> gView : register(b1);
Texture2D<float4> gTexture : register(t0);

float4 main(VertexShaderOutput input) : SV_TARGET
{
    float4 output;
    float2 uv = input.texCoord;
    
    float4 color = gTexture.Sample(gLinearClamp, uv);
    output.a = color.a;
    color = floor(color * gParam.iLevel) / gParam.iLevel;
   
    // Output to screen
    output.rgb = color.rgb;
    
    return output;
}
