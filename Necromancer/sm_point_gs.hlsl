cbuffer matrices:register(b0){
	float4x4 world;
	float4x4 view;
	float4x4 projection;
}
cbuffer rt_num_cb : register(b2){
	uint rt_num, dummy1, dummy2, dummy3;
}
struct GSInput{
	float4 pos	: POSITION;
};
struct PSInput{
	float4 pos		: POSITION;
	float4 v_pos	: SV_POSITION;
	uint rt_index	: SV_RenderTargetArrayIndex;
};
static const float4x4 right = float4x4(
	0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);
static const float4x4 left = float4x4(
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);
static const float4x4 up = float4x4(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);
static const float4x4 down = float4x4(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, -1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);
static const float4x4 front = float4x4(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);
static const float4x4 back = float4x4(
	-1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f);
[maxvertexcount(18)]
void main(
	triangle GSInput input[3],
	inout TriangleStream< PSInput > output
	)
{
	PSInput tmp;
	int i = 0;
	[unroll]
	for (i = 0; i < 3; ++i){
		tmp.pos = input[i].pos;
		tmp.v_pos = mul(view, input[i].pos);
		tmp.v_pos = mul(right, tmp.v_pos);
		tmp.v_pos = mul(projection, tmp.v_pos);
		tmp.rt_index = 0;
		output.Append(tmp);
	}
	output.RestartStrip();
	[unroll]
	for (i = 0; i < 3; ++i){
		tmp.pos = input[i].pos;
		tmp.v_pos = mul(view, input[i].pos);
		tmp.v_pos = mul(left, tmp.v_pos);
		tmp.v_pos = mul(projection, tmp.v_pos);
		tmp.rt_index = 1;
		output.Append(tmp);
	}
	output.RestartStrip();
	[unroll]
	for (i = 0; i < 3; ++i){
		tmp.pos = input[i].pos;
		tmp.v_pos = mul(view, input[i].pos);
		tmp.v_pos = mul(up, tmp.v_pos);
		tmp.v_pos = mul(projection, tmp.v_pos);
		tmp.rt_index = 2;
		output.Append(tmp);
	}
	output.RestartStrip();
	[unroll]
	for (i = 0; i < 3; ++i){
		tmp.pos = input[i].pos;
		tmp.v_pos = mul(view, input[i].pos);
		tmp.v_pos = mul(down, tmp.v_pos);
		tmp.v_pos = mul(projection, tmp.v_pos);
		tmp.rt_index = 3;
		output.Append(tmp);
	}
	output.RestartStrip();
	[unroll]
	for (i = 0; i < 3; ++i){
		tmp.pos = input[i].pos;
		tmp.v_pos = mul(view, input[i].pos);
		tmp.v_pos = mul(front, tmp.v_pos);
		tmp.v_pos = mul(projection, tmp.v_pos);
		tmp.rt_index = 4;
		output.Append(tmp);
	}
	output.RestartStrip();
	[unroll]
	for (i = 0; i < 3; ++i){
		tmp.pos = input[i].pos;
		tmp.v_pos = mul(view, input[i].pos);
		tmp.v_pos = mul(back, tmp.v_pos);
		tmp.v_pos = mul(projection, tmp.v_pos);
		tmp.rt_index = 5;
		output.Append(tmp);
	}
	output.RestartStrip();
}