//#define _SHOW_LIGHT_NUMBER

#define TILE_SIZE 16

struct TileFrustum{
	float4 planes[4];
};

struct Light{
	float4 position;
	float4 direction;
	float4 color;
	float4 params;
	uint type;
	float costheta;
	float t1, t2;
	float4 pos_in_view;
};

struct SurfaceData{
	float3 position;
	float3 normal;
	float3 diffuse;
	float specular;
	uint shiness;
	float depth;
	float ambient;
	float view_z;
};

cbuffer PerFrameConstants:register(b0){
	uint g_buffer_width;
	uint g_buffer_height;
	uint group_size_x;
	uint group_size_y;
	float4 camera_params;
	float4x4 projection_matrix;
	float4x4 inverse_view_matrix;
}

cbuffer light_number:register(b1){
	uint4 g_light_number;
}

StructuredBuffer<Light> global_light : register(t0);
StructuredBuffer<TileFrustum> global_tile_frustums : register(t1);
SamplerState texture_sampler : register(s0);
Texture2D g_buffer[3] : register(t2);
Texture2D depth_buffer : register(t5);
Texture2D<uint> shadow_mask_map : register(t6);
RWTexture2D<float4> g_frame_buffer:register(u0);

groupshared uint min_z;
groupshared uint max_z;

groupshared uint tile_light_indices[128];
groupshared uint tile_light_number;

static const uint constant_result_base [] = {
	1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024
};

float3 decode_normal(float2 e){
	float2 tmp = e - e * e;
		float f = tmp.x + tmp.y;
	float m = sqrt(4.0f * f - 1.0f);

	float3 n;
	n.xy = m * (e * 4.0f - 2.0f);
	n.z = 3.0f - 8.0f * f;
	return n;
}

float3 caculate_view_position(uint2 screen_pos, float depth){
	float2 screen_pos_f;
	screen_pos_f.x = (float) screen_pos.x / (float) g_buffer_width;
	screen_pos_f.y = (float) screen_pos.y / (float) g_buffer_height;
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

	return view_position;
}

SurfaceData get_surface_data(uint2 screen_pos){
	SurfaceData data = (SurfaceData) 0;

	float4 diffuse_ambient =
		g_buffer[0].Load(uint3(screen_pos, 0));
	float4 normal_specular =
		g_buffer[1].Load(uint3(screen_pos, 0));
	data.depth = depth_buffer.Load(uint3(screen_pos, 0)).x;
	data.normal = decode_normal(normal_specular.xy);
	data.specular = normal_specular.z;
	data.shiness = asuint(normal_specular.w);
	data.diffuse = diffuse_ambient.xyz;
	data.ambient = diffuse_ambient.w;
	float4 position =
		float4(caculate_view_position(screen_pos, data.depth), 1.0f);
	data.view_z = position.z / position.w;
	position = mul(inverse_view_matrix, position);
	data.position = position.xyz / position.w;
	//data.position = caculate_view_position(screen_pos, data.depth);

	return data;
}

inline float3 caculate_brdf(SurfaceData surface_data,
	Light light)
{
	const uint shiness = 128;

	float3 result =
		float3(0.0, 0.0, 0.0);
	uint type = light.type;
	[branch]
	if (type == 0){
		//Point
		float3 l = surface_data.position - light.position.xyz;
			float distance = length(l);
		l = -normalize(l);
		if (distance < light.params[3]){
			float ldotn = dot(l, surface_data.normal);
			if (ldotn > 0.0f){
				float ra =
					1.0 / (light.params.x +
					light.params.y * distance +
					light.params.z * distance * distance);
				float k = ldotn * ra;
				float3 V =
					-normalize(surface_data.position.xyz);
				float3 R =
					l - surface_data.normal *
					(2.0 * dot(l, surface_data.normal));
				float dotVR = dot(V, R);
				if (dotVR > 0.0){
					k += pow(abs(dotVR), shiness) * ra;
				}
				result = k * light.color.xyz * surface_data.diffuse;
			}
		}
	}
	else if (type == 1){
		//Directional
		float3 l = -normalize(light.direction.xyz);
			float ldotn = dot(l, surface_data.normal);
		if (ldotn > 0.0f){
			float k = ldotn;
			result = k * light.color.xyz * surface_data.diffuse;
		}
		//result = float3(0.5f, 0.5f, 0.5f);
	}
	else if (type == 2){
		//Spot
		float3 l =
			surface_data.position - light.position.xyz;
		float distance = length(l);
		l = -normalize(l);
		float ndotd = dot(l, normalize(light.direction.xyz));
		if (distance < light.params[3]){
			float ldotn = dot(l, surface_data.normal);
			if (ldotn > 0.0f && ndotd > light.costheta){
				float ra =
					1.0 / (light.params.x +
					light.params.y * distance +
					light.params.z * distance * distance);
				float k = ldotn * ra;
				float3 V =
					-normalize(surface_data.position.xyz);
				float3 R =
					l - surface_data.normal *
					(2.0 * dot(l, surface_data.normal));
				float dotVR = dot(V, R);
				if (dotVR > 0.0){
					k += pow(abs(dotVR), shiness) * ra;
				}
				result = k * light.color.xyz * surface_data.diffuse;
			}
		}
	}
	else{
	}
	return result;
}

inline bool in_frustum(Light light, float4 frustums[6]){
	if (light.type == 1) return true;
	bool result = true;
	[unroll]
	for (int i = 0; i < 6; ++i){
		float d = dot(frustums[i], float4(light.pos_in_view.xyz, 1.0f));
		result = result && (d >= -light.params[3]);
	}
	return result;
}

[numthreads(TILE_SIZE, TILE_SIZE, 1)]
void main(uint3 group_id          : SV_GroupID,
	uint3 dispatch_thread_id : SV_DispatchThreadID,
	uint3 group_thread_id : SV_GroupThreadID,
	uint group_index : SV_GroupIndex)
{

	uint2 screen_pos =
		group_id.xy * TILE_SIZE + group_thread_id.xy;
	SurfaceData surface_data = get_surface_data(screen_pos);

	float min_z_sample = min(camera_params.y, surface_data.view_z);
	float max_z_sample = max(camera_params.x, surface_data.view_z);

	//init shared memory
	if (group_index == 0){
		tile_light_number = 0;
		min_z = 0x7F7FFFFF; // max float
		max_z = 0;
	}

	GroupMemoryBarrierWithGroupSync();

	if (max_z_sample >= min_z_sample){
		InterlockedMin(min_z, asuint(min_z_sample));
		InterlockedMax(max_z, asuint(max_z_sample));
	}

	GroupMemoryBarrierWithGroupSync();

	//make frustum
	float min_tile_z = asfloat(min_z);
	float max_tile_z = asfloat(max_z);

	float4 frustum_planes[6];
	uint frustum_index =
		group_id.x * group_size_y + group_id.y;
	TileFrustum tile_frustum = global_tile_frustums[frustum_index];
	frustum_planes[0] = tile_frustum.planes[0];
	frustum_planes[1] = tile_frustum.planes[1];
	frustum_planes[2] = tile_frustum.planes[2];
	frustum_planes[3] = tile_frustum.planes[3];
	frustum_planes[4] =
		float4(0.0f, 0.0f, 1.0f, -min_tile_z);
	frustum_planes[5] =
		float4(0.0f, 0.0f, -1.0f, max_tile_z);

	//tile-based culling
	//[unroll(4)]
	for (uint i = group_index; i < g_light_number[0]; i += (TILE_SIZE * TILE_SIZE)){
		if (in_frustum(global_light[i], frustum_planes) &&
			(tile_light_number < 128))
		{
			uint tmp;
			InterlockedAdd(tile_light_number, 1, tmp);
			tile_light_indices[tmp] = i;
		}
	}

	GroupMemoryBarrierWithGroupSync();

	//shading
	uint light_number = tile_light_number;
	float4 color =
		float4(0.0f, 0.0f, 0.0f, 1.0f);

#ifdef _SHOW_LIGHT_NUMBER
	float tmp =
		(float) light_number / g_light_number[0];
	color = float4(tmp, tmp, tmp, 1.0f);
#else
	//shadow
	uint mask = shadow_mask_map[screen_pos];
	[unroll]
	for (uint k = 0; k < light_number; ++k){
		uint index = tile_light_indices[k];
		[branch]
		if ((index >= g_light_number[1]) ||
			((mask & constant_result_base[k]) == 0))
		{
			Light light = global_light[index];
			color.xyz += caculate_brdf(
				surface_data, light);
		}
	}
	//no-shadow

	//color.xyz += surface_data.diffuse * 0.1f;
#endif
	//uint mask = shadow_mask_map[screen_pos];
	//float c = (mask) / 1.0f;
	//g_frame_buffer[screen_pos] = float4(c, c, c, 1.0f);
	g_frame_buffer[screen_pos] = color;
	//g_frame_buffer[screen_pos] = float4(max_z_sample / 500.0f,
	//	max_z_sample / 500.0f, max_z_sample / 500.0f, 1.0f);
}