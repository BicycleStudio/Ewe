cbuffer ConstantBuffer : register( b0 )
{
  matrix matView;
  matrix matProjection;
}

struct VInput {
  float3 position   : POSITION;
};
struct VOutput {
  float4 position   : SV_POSITION;
};

VOutput vsDefault(VInput input) {
  VOutput output = (VOutput) 0;
  output.position = mul( input.position, matView );
  output.position = mul( output.position, matProjection );
 
  return output;
}
float4 psDefault(VOutput input) : SV_Target{
  return float4(0.25f, 0.55f, 0.65f, 1.0f);
}