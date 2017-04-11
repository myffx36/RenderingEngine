cbuffer matrices:register(b0){
	float4x4 world;
	float4x4 view;
	float4x4 projection;
}
struct VSInput{
	float3 pos : POSITION;
};
struct GSInput{
	float4 pos	: POSITION;
};
GSInput main(VSInput input)
{
	GSInput result;
	result = (GSInput) 0;
	result.pos = mul(world, float4(input.pos, 1.0f));
	return result;
}