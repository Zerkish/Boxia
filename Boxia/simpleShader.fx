
cbuffer perObject
{
  float4x4 world;
  float4x4 projection;
  float4x4 view;
};

cbuffer perFrame
{
  float3 cameraPosition;
};

struct VS_INPUT
{
  float3 iPos : POSITION0;
  float3 iNorm : NORMAL0;
  float4 matDiffuse : COLOR0;
  float4 matSpecular : COLOR1;
  
};

struct PS_INPUT
{
  float4 oPos : SV_POSITION;
  float4 matDiffuse : COLOR0;
  float4 matSpecular : COLOR1;
  float4 oNorm : NORMAL0;
  float4 worldPos : POSITION2;
};

float4 ambientColor = float4(0.25f, 0.25f, 0.25f, 1.0f);

float4 diffuseDir = normalize(float4(-0.0, 1.0f, 0.0f, 0.0f));
float4 diffuseColor = float4(0.10f, 0.12f, 0.10f, 1.0f);

// Point Light test
float4 pointLightPos = float4(0, 1, 0, 0);
float4 pointColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
float a0 = 0.55;
float a1 = 0.025;
float a2 = 0.01f;


PS_INPUT VS(VS_INPUT input)
{
  PS_INPUT output;
  output.oPos = mul(float4(input.iPos, 1.0f), world);
  output.oPos = mul(output.oPos, view);
  output.oPos = mul(output.oPos, projection);
  output.oNorm = normalize(mul(float4(input.iNorm, 0.0f), world));
  output.worldPos = mul(float4(input.iPos, 1.0f), world);

  output.matDiffuse = input.matDiffuse;
  output.matSpecular = input.matSpecular;
  return output;
}

float4 PS(PS_INPUT input) : SV_TARGET
{
  float dInten = dot(diffuseDir, input.oNorm);
  float sInten = 0;
  if(dInten > 0)
  {
    sInten = pow(max(dot(normalize(float4(cameraPosition, 0) - input.worldPos), reflect(-diffuseDir, input.oNorm)), 0), input.matSpecular.a);
  }

  float4 retCol = ambientColor * input.matDiffuse +
                  dInten * diffuseColor * input.matDiffuse +
                  sInten * diffuseColor * input.matSpecular;
  
  float dist = distance(pointLightPos, input.worldPos);
  float4 pL = normalize((pointLightPos - input.worldPos) / dist);
  float pDInten = max(dot(pL, input.oNorm), 0);
  float pSInten = 0;

  if(pDInten > 0)
  {
    pSInten = pow(max(dot(normalize(float4(cameraPosition, 0) - input.worldPos), reflect(-pL, input.oNorm)), 0), input.matSpecular.a);
  }
  
  float4 pCol = (
    pointColor * input.matDiffuse + 
    pDInten * pointColor * input.matDiffuse + 
    pSInten * pointColor * input.matSpecular) / 
    (a0 + a1 * dist + a2 * dist *dist);

  return saturate(pCol + retCol);
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