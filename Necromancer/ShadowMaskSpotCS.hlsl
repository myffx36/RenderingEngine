#define TILE_SIZE 16

Texture2D<float> texture_slot0 : register(t0);
RWTexture2D<uint> g_out_texture:register(u0);
SamplerState default_sampler : register(s0);
Texture2D g_buffer[3] : register(t2);
Texture2D depth_buffer : register(t5);

cbuffer cb0:register(b0){
	uint width;
	uint height;
	uint result_base;
	uint light_index;
}

cbuffer cb1:register(b1){
	float4x4 inverse_view_matrix;
	float4x4 projection_matrix;
}

cbuffer cb2 : register(b2){
	float4x4 shadow_map_matrix;
}

cbuffer cb3 : register(b3){
	float4 light_position;
}

float3 get_world_position(uint2 screen_pos){
	float depth =
		depth_buffer.Load(uint3(screen_pos, 0)).x;
	float2 screen_pos_f;
	screen_pos_f.x = (float) screen_pos.x / (float) width;
	screen_pos_f.y = (float) screen_pos.y / (float) height;
	screen_pos_f.x = 2.0f * screen_pos_f.x - 1.0f;
	screen_pos_f.y = 1.0f - 2.0f * screen_pos_f.y;

	float view_space_z = projection_matrix._34 /
		(projection_matrix._33 - projection_matrix._43 * depth);
	float2 screen_space_ray = float2(screen_pos_f.x / projection_matrix._11,
		screen_pos_f.y / projection_matrix._22);
	float3 view_position;
	view_position.z = -view_space_z;
	view_position.xy = screen_space_ray * view_position.z
		* projection_matrix._43;
	float4 position =
		mul(inverse_view_matrix, float4(view_position, 1.0f));
	float3 result_position =
		position.xyz / position.w;

	return result_position;
}

[numthreads(16, 16, 1)]
void main(uint3 group_id          : SV_GroupID,
	uint3 dispatch_thread_id : SV_DispatchThreadID,
	uint3 group_thread_id : SV_GroupThreadID,
	uint group_index : SV_GroupIndex)
{
	uint2 screen_pos =
		group_id.xy * TILE_SIZE + group_thread_id.xy;
	float3 world_position =
		get_world_position(screen_pos);
	float4 pos_in_light =
		mul(shadow_map_matrix, float4(world_position, 1.0f));

	if (light_index == 0){
		g_out_texture[screen_pos] = 0;
	}

	float2 texcoord;
	texcoord.x = pos_in_light.x / pos_in_light.w;
	texcoord.y = pos_in_light.y / pos_in_light.w;
	texcoord.x = (texcoord.x + 1) / 2.0f;
	texcoord.y = 1.0f - ((texcoord.y + 1) / 2.0f);
	if ((texcoord.x < 0.0f)||(texcoord.x > 1.0f)||(texcoord.y < 0.0f)||(texcoord.y > 1.0f)){
		return;
	}
	float shadow_map_distance = texture_slot0.Gather(default_sampler, texcoord).x * 1.015;
	float actual_depth = length(world_position - 
		(light_position.xyz / light_position.w));

	//g_out_texture[screen_pos] = actual_depth * 100;

	if (actual_depth >= shadow_map_distance){
		g_out_texture[screen_pos] += result_base;
	}
}