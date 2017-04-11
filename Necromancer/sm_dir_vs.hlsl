cbuffer matrices:register(b0){
	float4x4 world;
	float4x4 view;
	float4x4 projection;
}
struct VSInput{
	float3 pos : POSITION;
};
struct PSInput{
	float4 pos		: POSITION;
	float4 v_pos	: SV_POSITION;
};
PSInput main(VSInput input)
{
	PSInput result;
	result = (PSInput) 0;
	result.pos = mul(world, float4(input.pos, 1.0f));
	result.v_pos = mul(view, result.pos);
	result.v_pos = mul(projection, result.v_pos);
	return result;
}