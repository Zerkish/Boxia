
cbuffer perObject
{
  float4x4 world;
  float4x4 projection;
  float4x4 view;
};

struct VS_INPUT
{
  float3 iPos : POSITION0;
  float4 iCol : COLOR0;
  float3 iNorm : NORMAL0;
};

struct PS_INPUT
{
  float4 oPos : SV_POSITION;
  float4 oCol : COLOR0;
  float3 oNorm : NORMAL0;
};

float4 ambientColor = float4(0.45f, 0.45f, 0.45f, 1.0f);

float3 diffuseDir = normalize(float3(0.15, -0.25f, 0.75));
float4 diffuseColor = float4(0.20f, 0.25f, 0.20f, 1.0f);
float diffuseIntensity = 0.1f;


PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output;
  output.oPos = mul(float4(input.iPos, 1.0f), world);
  output.oPos = mul(output.oPos, view);
  output.oPos = mul(output.oPos, projection);
  output.oNorm = normalize(mul(float4(input.iNorm, 0.0f), world).xyz);
  

  output.oCol = input.iCol;
  return output;

  //if(oPos.y > 0) oCol = float4(0, 1, 0, 1);
}

float4 PS(PS_INPUT input) : SV_TARGET
{  
  float4 aColor = input.oCol;
  aColor = input.oCol * ambientColor;
 
  aColor += saturate(diffuseColor * max(saturate(dot(-diffuseDir, input.oNorm)), -1));
  return aColor;
}


technique10 Renderer
{
  pass P0
  {
    SetVertexShader(CompileShader(vs_4_0, VS()));
    SetGeometryShader(NULL);
    SetPixelShader(CompileShader(ps_4_0, PS()));
  }
}