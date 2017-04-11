cbuffer light:register(b1){
	float4 light_position;
}
struct PSInput{
	float4 pos		: POSITION;
	float4 v_pos	: SV_POSITION;
};
float main(PSInput input) : SV_TARGET
{
	float depth = input.v_pos.z / input.v_pos.w;
	return depth * 0.993;
}