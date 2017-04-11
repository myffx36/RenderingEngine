cbuffer light:register(b1){
	float4 light_position;
}
struct PSInput{
	float4 pos		: POSITION;
	float4 v_pos	: SV_POSITION;
	uint rt_index	: SV_RenderTargetArrayIndex;
};
float main(PSInput input) : SV_TARGET
{
	float distance = length(input.pos.xyz / input.pos.w - light_position.xyz);
	return distance;
}