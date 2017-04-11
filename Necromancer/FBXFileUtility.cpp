#include "stdafx.h"
#include "FBXFileUtility.h"

#include "Skeleton.h"
#include "SkeletonInstance.h"
#include "SceneNode.h"
#include "Mesh.h"
#include "Geometry.h"
#include "PIL.h"
#include "PILLLGI.h"
#include "NodeGeometryResource.h"
#include "HardwareDeformMesh.h"
#include "AnimationEngine.h"
#include "Necromancer.h"

namespace Necromancer{

	namespace{
		Transform get_transform(
			FbxNode* node)
		{
			auto fbx_local = node->EvaluateLocalTransform();
			auto fbx_quaternion = fbx_local.GetQ();
			auto fbx_rotation = fbx_local.GetR();
			auto fbx_translation = fbx_local.GetT();
			auto fbx_scale = fbx_local.GetS();
			Transform transform;
			float translation[] = {
				static_cast<float>(fbx_translation[0]),
				static_cast<float>(fbx_translation[1]),
				static_cast<float>(fbx_translation[2])
			};
			float rotation[] = {
				static_cast<float>(fbx_quaternion[0]),
				static_cast<float>(fbx_quaternion[1]),
				static_cast<float>(fbx_quaternion[2]),
				static_cast<float>(fbx_quaternion[3])
			};
			float scale[] = {
				static_cast<float>(fbx_scale[0]),
				static_cast<float>(fbx_scale[1]),
				static_cast<float>(fbx_scale[2])
			};
			float local[16];
			for (int i = 0; i < 4; ++i){
				for (int j = 0; j < 4; ++j){
					local[i * 4 + j] =
						static_cast<float>(fbx_local[i][j]);
				}
			}

			transform.set_translation(translation[0], translation[1], translation[2]);
			transform.set_rotation(rotation[0], rotation[1], rotation[2], rotation[3]);
			transform.set_scale(scale[0], scale[1], scale[2]);
			transform.recompute_matrix();

			return transform;
		}

		Vec3 caculate_position(const Vec3& pos, const Mat4& mat){
			Vec4 v4_pos(pos, 1.0f);
			Vec4 v4_result = v4_pos * mat;
			Vec3 result = v4_result.xyz / v4_result.w;
			return result;
		}

		Vec3 caculate_normal_tangent(const Vec3& in, const Mat4& mat){
			Vec4 v4_in(in, 0.0f);
			Vec4 v4_result = v4_in * mat;
			Vec3 result = v4_result.xyz;
			return result;
		}

		void load_texture_from_file(const char* file_name,
			float** texture_data, U32& texture_width, U32& texture_height)
		{
			FIBITMAP* dib(0);
			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			fif = FreeImage_GetFileType(file_name);
			if (fif == FIF_UNKNOWN)
				fif = FreeImage_GetFIFFromFilename(file_name);
			if (FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_Load(fif, file_name);

			U32 width = FreeImage_GetWidth(dib);
			U32 height = FreeImage_GetHeight(dib);

			FIBITMAP* rgbf_bitmap = FreeImage_ConvertToRGBF(dib);
			float* image_data = (float*)FreeImage_GetBits(rgbf_bitmap);

			float* rgba_data = new float[width * height * 4];
			for (U32 i = 0; i < height; ++i){
				for (U32 j = 0; j < width; ++j){
					U32 offset = (i * width + j) * 4;
					U32 image_offset = ((height - 1 - i) * width + j) * 3;
					rgba_data[offset + 0] = image_data[image_offset + 0];
					rgba_data[offset + 1] = image_data[image_offset + 1];
					rgba_data[offset + 2] = image_data[image_offset + 2];
					rgba_data[offset + 3] = 1.0f;
				}
			}

			FreeImage_Unload(dib);
			FreeImage_Unload(rgbf_bitmap);

			texture_width = width;
			texture_height = height;
			*texture_data = rgba_data;
		}

		union SkeletonElement{
			U32 u32_value;
			F32 f32_value;

			SkeletonElement(U32 value)
				:u32_value(value)
			{

			}
		};
	}

	int caculate_vertex_size(const String& format){
		int size = 3;
		if (format.find('N') != -1){
			size += 3;
		}
		if (format.find('C') != -1){
			size += 4;
		}
		if (format.find('A') != -1){
			size += 3;
		}
		if (format.find('S') != -1){
			size += 8;
		}
		if (format.find('2') != -1){
			size += 2;
		}

		return size;
	}

	SkeletonPtr create_skeleton(FbxScene* fbx_scene){
		SkeletonPtr skeleton(new Skeleton());

		U32 n = fbx_scene->GetNodeCount();

		for (U32 i = 0; i < n; ++i){
			FbxNode* fbx_node = fbx_scene->GetNode(i);
			skeleton->add_bone(fbx_node->GetName());
		}

		return skeleton;
	}

	void load_skeleton_info(FbxMesh* fbx_mesh, U32 vert_index,
		U32* indices, F32* weights, SkeletonPtr skeleton)
	{
		U32 skin_count = fbx_mesh->GetDeformerCount();
		U32 k = 0;
		int vertex_count = fbx_mesh->GetControlPointsCount();
		for (U32 skin_index = 0; skin_index < skin_count; ++skin_index){
			FbxSkin* fbx_skin = (FbxSkin*)
				fbx_mesh->GetDeformer(skin_index, FbxDeformer::eSkin);
			int cluster_count = fbx_skin->GetClusterCount();
			for (int cluster_index = 0;
				cluster_index < cluster_count; ++cluster_index)
			{
				FbxCluster* fbx_cluster =
					fbx_skin->GetCluster(cluster_index);
				if (!fbx_cluster->GetLink())
					continue;

				String node_name(fbx_cluster->GetLink()->GetName());
				int control_point_indices_count = fbx_cluster->GetControlPointIndicesCount();
				int *control_point_indices = fbx_cluster->GetControlPointIndices();
				double* control_point_weights = fbx_cluster->GetControlPointWeights();
				for (int i = 0; i < control_point_indices_count; ++i){
					if (i >= vertex_count) continue;
					if (control_point_weights[i] == 0.0) continue;
					int index = control_point_indices[i];
					if (vert_index != index) continue;
					float weight = (float)control_point_weights[i];
					U32 skeleton_id = skeleton->get_bone_index(node_name);

					if (k < 4){
						weights[k] = weight;
						indices[k] = skeleton_id;
						++k;
					}
					else{
						std::cout << "vert " << index << " has more than 4 bones associated" << std::endl;
						break;
					}
				}//for control_point_index
			}//for clusters
		}//for skins
	}

	MaterialPtr create_material_from_fbx_material(
		FbxSurfaceMaterial* fbx_material)
	{
		if (fbx_material == nullptr){
			return nullptr;
		}
		MaterialPtr material(new Material());

		if (fbx_material->GetClassId().Is(FbxSurfacePhong::ClassId)){
			auto phong_material =
				static_cast<FbxSurfacePhong*>(fbx_material);
			auto fbx_ambient = phong_material->Ambient.Get();
			auto fbx_diffuse = phong_material->Diffuse.Get();
			auto fbx_specular = phong_material->Specular.Get();
			auto fbx_emissive = phong_material->Emissive.Get();
			float shininess = static_cast<float>(
				phong_material->Shininess.Get());

			Vec3 ambient(static_cast<float>(fbx_ambient[0]),
				static_cast<float>(fbx_ambient[1]),
				static_cast<float>(fbx_ambient[2]));
			Vec3 diffuse(static_cast<float>(fbx_diffuse[0]),
				static_cast<float>(fbx_diffuse[1]),
				static_cast<float>(fbx_diffuse[2]));
			Vec3 specular(static_cast<float>(fbx_specular[0]),
				static_cast<float>(fbx_specular[1]),
				static_cast<float>(fbx_specular[2]));

			material->set_ambient(ambient);
			material->set_diffuse(diffuse);
			material->set_specular(specular);
		}
		else if (fbx_material->GetClassId().Is(FbxSurfaceLambert::ClassId)){
			auto lambert_material =
				static_cast<FbxSurfaceLambert*>(fbx_material);
			auto fbx_ambient = lambert_material->Ambient.Get();
			auto fbx_diffuse = lambert_material->Diffuse.Get();
			auto fbx_emissive = lambert_material->Emissive.Get();

			Vec3 ambient(static_cast<float>(fbx_ambient[0]),
				static_cast<float>(fbx_ambient[1]),
				static_cast<float>(fbx_ambient[2]));
			Vec3 diffuse(static_cast<float>(fbx_diffuse[0]),
				static_cast<float>(fbx_diffuse[1]),
				static_cast<float>(fbx_diffuse[2]));
			Vec3 specular;

			material->set_ambient(ambient);
			material->set_diffuse(diffuse);
			material->set_specular(specular);
		}

		//Texture file
		FbxProperty fbx_property;
		std::string file_name;
		FbxTexture* fbx_texture =
			fbx_material->GetSrcObject<FbxTexture>();
		bool find = false;
		fbx_property =
			fbx_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		if (fbx_property.IsValid()){
			const int texture_count =
				fbx_property.GetSrcObjectCount<FbxFileTexture>();
			if (texture_count){
				const FbxFileTexture* fbx_texture =
					fbx_property.GetSrcObject<FbxFileTexture>();
				if (fbx_texture){
					find = true;
					file_name = fbx_texture->GetFileName();
				}
			}
		}

		if (find){
			U32 width, height;
			float* texture_data;
			load_texture_from_file(file_name.c_str(),
				&texture_data, width, height);

			PIL* pil = PIL::get_instance();
			PILLLGI* llgi = pil->pil_llgi();
			Tex2DPtr texture =
				llgi->create_texture_2d(width, height,
				NECROMANCER_TEXTURE_R32G32B32A32, texture_data);
			material->attach_texture(0, texture);
		}

		return material;
	}

	// Get the geometry offset to a node. It is never inherited by the children.
	FbxAMatrix GetGeometry(FbxNode* pNode)
	{
		const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

		return FbxAMatrix(lT, lR, lS);
	}

	Mat4 get_bind_inverse_matrix(FbxMesh* fbx_mesh, FbxCluster* fbx_cluster){

		FbxAMatrix joint_init_global;
		fbx_cluster->GetTransformLinkMatrix(joint_init_global);


		// Compute the shift of the link relative to the reference.
		FbxAMatrix fbx_result = joint_init_global.Inverse();

		Mat4 result;
		for (int i = 0; i < 4; ++i){
			for (int j = 0; j < 4; ++j){
				result[i][j] = static_cast<F32>(fbx_result[i][j]);
			}
		}

		return result;
	}


	Mat4 get_skinning_matrix(FbxNode* fbx_node, F32 time){
		Mat4 matrix;
		FbxTime fbx_time;
		fbx_time.SetSecondDouble(time);
		auto fbx_node_global = fbx_node->EvaluateGlobalTransform(fbx_time);

		for (U32 i = 0; i < 4; ++i){
			for (U32 j = 0; j < 4; ++j){
				matrix[i][j] =
					static_cast<float>(fbx_node_global[i][j]);
			}
		}

		if (fbx_node->GetName() == String("Bone002")){
			int x = 0;
			++x;
		}

		return matrix;
	}

	SceneNode* load_fbx_mesh(FbxMesh* fbx_mesh, const SkeletonPtr& skeleton,
		const SkeletonInstancePtr& skeleton_instance)
	{
		SceneNode* scene_node = new SceneNode();
		FbxNode* fbx_node = fbx_mesh->GetNode();
		FbxAMatrix fbx_global = fbx_node->EvaluateGlobalTransform();
		Mat4 global;

		for (int i = 0; i < 4; ++i){
			for (int j = 0; j < 4; ++j){
				global[i][j] = static_cast<F32>(fbx_global[i][j]);
			}
		}

		const int polygon_count = fbx_mesh->GetPolygonCount();

		FbxLayerElementArrayTemplate<int>* material_indices = nullptr;
		FbxGeometryElement::EMappingMode material_mapping_mode = FbxGeometryElement::eNone;

		Vector<F32>::Type vertices_vec[32];
		Vector<U32>::Type indices_vec[32];
		if (fbx_mesh->GetElementMaterial())
		{
			material_indices = &fbx_mesh->GetElementMaterial()->GetIndexArray();
			material_mapping_mode = fbx_mesh->GetElementMaterial()->GetMappingMode();
		}

		const int TRIANGLE_VERTEX_COUNT = 3;
		const int VERTEX_STRIDE = 3;
		const int NORMAL_STRIDE = 3;
		const int TANGENT_STRIDE = 3;
		const int UV_STRIDE = 2;
		const int SKELETON_STRIDE = 8;

		int skin_flag = 0;

		int pos_start_pos = 0;
		int normal_start_pos = 0;
		int tangent_start_pos = 0;
		int texture_start_pos = 0;
		int skin_start_pos = 0;

		bool all_by_control_point = true;
		bool has_normal = fbx_mesh->GetElementNormalCount() > 0;
		bool has_uv = fbx_mesh->GetElementUVCount() > 0;
		bool has_tangent = fbx_mesh->GetElementTangentCount() > 0;
		bool has_skin = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
		int vertex_size = VERTEX_STRIDE;
		normal_start_pos = 3;
		tangent_start_pos = 3;
		texture_start_pos = 3;
		skin_start_pos = 3;
		if (has_normal){
			vertex_size += NORMAL_STRIDE;
			tangent_start_pos += 3;
			texture_start_pos += 3;
			skin_start_pos += 3;
		}
		if (has_tangent){
			vertex_size += TANGENT_STRIDE;
			texture_start_pos += 3;
			skin_start_pos += 3;
		}
		if (has_uv){
			vertex_size += UV_STRIDE;
			skin_start_pos += 2;
		}
		if (has_skin){
			vertex_size += SKELETON_STRIDE;
		}

		FbxGeometryElement::EMappingMode normal_mapping_mode = FbxGeometryElement::eNone;
		FbxGeometryElement::EMappingMode uv_mapping_mode = FbxGeometryElement::eNone;
		if (has_normal){
			normal_mapping_mode = fbx_mesh->GetElementNormal(0)->GetMappingMode();
			if (normal_mapping_mode == FbxGeometryElement::eNone){
				has_normal = false;
			}
			if (has_normal && normal_mapping_mode != FbxGeometryElement::eByControlPoint){
				all_by_control_point = false;
			}
		}
		if (has_uv){
			uv_mapping_mode = fbx_mesh->GetElementUV(0)->GetMappingMode();
			if (uv_mapping_mode == FbxGeometryElement::eNone){
				has_uv = false;
			}
			if (has_uv && uv_mapping_mode != FbxGeometryElement::eByControlPoint){
				all_by_control_point = false;
			}
		}

		// Allocate the array memory, by control point or by polygon vertex.
		int polygon_vertex_count = fbx_mesh->GetControlPointsCount();
		if (!all_by_control_point){
			polygon_vertex_count = polygon_count * TRIANGLE_VERTEX_COUNT;
		}
		int vertices_size = polygon_vertex_count * vertex_size;
		int indices_size = polygon_count * TRIANGLE_VERTEX_COUNT;
		std::vector<float> vertices(vertices_size);
		std::vector<unsigned long> indices(indices_size);

		FbxStringList uv_names;
		fbx_mesh->GetUVSetNames(uv_names);
		const char * uv_name = NULL;
		if (has_uv && uv_names.GetCount()){
			uv_name = uv_names[0];
		}

		// Populate the array with vertex attribute, if by control point.
		const FbxVector4 * lControlPoints = fbx_mesh->GetControlPoints();
		FbxVector4 current_vertex;
		FbxVector4 current_normal;
		FbxVector4 current_tangent;
		FbxVector2 current_uv;
		if (all_by_control_point){
			const FbxGeometryElementNormal * normal_element = nullptr;
			const FbxGeometryElementUV * uv_element = nullptr;
			const FbxGeometryElementTangent* tangent_element = nullptr;
			if (has_normal){
				normal_element = fbx_mesh->GetElementNormal(0);
			}
			if (has_uv){
				uv_element = fbx_mesh->GetElementUV(0);
			}
			if (has_tangent){
				tangent_element = fbx_mesh->GetElementTangent(0);
			}
			for (int i = 0; i < polygon_vertex_count; ++i){
				// Save the vertex position.
				current_vertex = lControlPoints[i];
				Vec3 pos(static_cast<F32>(current_vertex[0]), 
						 static_cast<F32>(current_vertex[1]), 
						 static_cast<F32>(current_vertex[2]));
				//covert to the global space
				pos = caculate_position(pos, global);

				vertices[i * vertex_size] = pos[0];
				vertices[i * vertex_size + 1] = pos[1];
				vertices[i * vertex_size + 2] = pos[2];

				// Save the normal.
				if (has_normal){
					int normal_index = i;
					if (normal_element->GetReferenceMode() ==
						FbxLayerElement::eIndexToDirect)
					{
						normal_index =
							normal_element->GetIndexArray().GetAt(i);
					}
					current_normal =
						normal_element->GetDirectArray().GetAt(i);
					Vec3 normal(static_cast<F32>(current_normal[0]), 
								static_cast<F32>(current_normal[1]), 
								static_cast<F32>(current_normal[2]));
					//covert to global space
					normal = caculate_normal_tangent(normal, global);
					vertices[i * vertex_size + normal_start_pos + 0] = normal[0];
					vertices[i * vertex_size + normal_start_pos + 1] = normal[1];
					vertices[i * vertex_size + normal_start_pos + 2] = normal[2];
				}

				//save the tangent
				if (has_tangent){
					int tangent_index = i;
					if (tangent_element->GetReferenceMode() ==
						FbxLayerElement::eIndexToDirect)
					{
						tangent_index =
							tangent_element->GetIndexArray().GetAt(i);
					}
					current_tangent =
						tangent_element->GetDirectArray().GetAt(i);
					Vec3 tangent(static_cast<F32>(current_tangent[0]), 
								 static_cast<F32>(current_tangent[1]), 
								 static_cast<F32>(current_tangent[2]));
					//convert to gloal space
					tangent = caculate_normal_tangent(tangent, global);

					vertices[i * vertex_size + tangent_start_pos + 0] = tangent[0];
					vertices[i * vertex_size + tangent_start_pos + 1] = tangent[1];
					vertices[i * vertex_size + tangent_start_pos + 2] = tangent[2];
				}

				// Save the UV.
				if (has_uv)
				{
					int uv_index = i;
					if (uv_element->GetReferenceMode() == FbxLayerElement::eIndexToDirect){
						uv_index = uv_element->GetIndexArray().GetAt(i);
					}
					current_uv = uv_element->GetDirectArray().GetAt(uv_index);
					vertices[i * vertex_size + texture_start_pos + 0] =
						static_cast<float>(current_uv[0]);
					vertices[i * vertex_size + texture_start_pos + 1] =
						1.0f - static_cast<float>(current_uv[1]);
				}

				if (has_skin){
					F32 weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
					U32 indices[4] = { 0, 0, 0, 0 };
					load_skeleton_info(fbx_mesh, i, indices, weights, skeleton);
					vertices[i * vertex_size + skin_start_pos + 0] = SkeletonElement(indices[0]).f32_value;
					vertices[i * vertex_size + skin_start_pos + 1] = SkeletonElement(indices[1]).f32_value;
					vertices[i * vertex_size + skin_start_pos + 2] = SkeletonElement(indices[2]).f32_value;
					vertices[i * vertex_size + skin_start_pos + 3] = SkeletonElement(indices[3]).f32_value;
					vertices[i * vertex_size + skin_start_pos + 4] = weights[0];
					vertices[i * vertex_size + skin_start_pos + 5] = weights[1];
					vertices[i * vertex_size + skin_start_pos + 6] = weights[2];
					vertices[i * vertex_size + skin_start_pos + 7] = weights[3];
					/*vertices[i * vertex_size + skin_start_pos + 0] = SkeletonElement(0).f32_value;
					vertices[i * vertex_size + skin_start_pos + 1] = SkeletonElement(0).f32_value;
					vertices[i * vertex_size + skin_start_pos + 2] = SkeletonElement(0).f32_value;
					vertices[i * vertex_size + skin_start_pos + 3] = SkeletonElement(0).f32_value;
					vertices[i * vertex_size + skin_start_pos + 4] = 1.0f;
					vertices[i * vertex_size + skin_start_pos + 5] = 0.0f;
					vertices[i * vertex_size + skin_start_pos + 6] = 0.0f;
					vertices[i * vertex_size + skin_start_pos + 7] = 0.0f;*/
				}
			}

		}

		for (int i = 0; i < polygon_count; ++i){

			int material_index = 0;
			if (material_indices && material_mapping_mode == FbxGeometryElement::eByPolygon)
			{
				material_index = material_indices->GetAt(i);
			}

			for (int j = 0; j < TRIANGLE_VERTEX_COUNT; ++j){
				const int control_point_index = fbx_mesh->GetPolygonVertex(i, j);

				if (all_by_control_point){
					U32 size = static_cast<U32>(
						indices_vec[material_index].size());

					indices_vec[material_index].push_back(size);
					for (int i = 0; i < vertex_size; ++i){
						vertices_vec[material_index].push_back(
							vertices[control_point_index * vertex_size + i]);
					}
				}
				else{
					U32 size = static_cast<U32>(
						indices_vec[material_index].size());

					indices_vec[material_index].push_back(size);
					current_vertex = lControlPoints[control_point_index];
					Vec3 pos(static_cast<F32>(current_vertex[0]), 
							 static_cast<F32>(current_vertex[1]), 
							 static_cast<F32>(current_vertex[2]));
					//convert to global space
					pos = caculate_position(pos, global);
					vertices_vec[material_index].push_back(pos[0]);
					vertices_vec[material_index].push_back(pos[1]);
					vertices_vec[material_index].push_back(pos[2]);
					if (has_normal){
						fbx_mesh->GetPolygonVertexNormal(i, j, current_normal);
						Vec3 normal(static_cast<F32>(current_normal[0]), 
									static_cast<F32>(current_normal[1]), 
									static_cast<F32>(current_normal[2]));
						//convert to global
						normal = caculate_normal_tangent(normal, global);
						vertices_vec[material_index].push_back(normal[0]);
						vertices_vec[material_index].push_back(normal[1]);
						vertices_vec[material_index].push_back(normal[2]);
					}

					if (has_tangent){
						/*fbx_mesh->getPolygonvertex
						fbx_mesh->GetPolygonVertexNormal(i, j, current_normal);

						vertices[i * vertex_size + tangent_start_pos + 0] = static_cast<float>(current_tangent[0]);
						vertices[i * vertex_size + tangent_start_pos + 1] = static_cast<float>(current_tangent[1]);
						vertices[i * vertex_size + tangent_start_pos + 2] = static_cast<float>(current_tangent[2]);*/
					}

					if (has_uv){
						bool unmap_flag;
						fbx_mesh->GetPolygonVertexUV(
							i, j, uv_name, current_uv, unmap_flag);
						vertices_vec[material_index].push_back(
							static_cast<float>(current_uv[0]));
						vertices_vec[material_index].push_back(
							1.0f - static_cast<float>(current_uv[1]));
					}

					if (has_skin){
						F32 weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
						U32 indices[4] = { 0, 0, 0, 0 };
						load_skeleton_info(fbx_mesh,
							control_point_index, indices, weights, skeleton);
						vertices_vec[material_index].push_back(SkeletonElement(indices[0]).f32_value);
						vertices_vec[material_index].push_back(SkeletonElement(indices[1]).f32_value);
						vertices_vec[material_index].push_back(SkeletonElement(indices[2]).f32_value);
						vertices_vec[material_index].push_back(SkeletonElement(indices[3]).f32_value);
						vertices_vec[material_index].push_back(weights[0]);
						vertices_vec[material_index].push_back(weights[1]);
						vertices_vec[material_index].push_back(weights[2]);
						vertices_vec[material_index].push_back(weights[3]);
					}
				}
			}
		}


		PIL* pil = PIL::get_instance();
		PILLLGI* llgi = pil->pil_llgi();

		String vertex_format = String("P");
		if (fbx_mesh->GetElementNormalCount() >= 1)
			vertex_format += "N";
		if (fbx_mesh->GetElementVertexColorCount() >= 1)
			vertex_format += "C";
		if (fbx_mesh->GetElementTangentCount() >= 1)
			vertex_format += "A";
		if (fbx_mesh->GetTextureUVCount() >= 1)
			vertex_format += "2";
		if (fbx_mesh->GetDeformerCount(FbxDeformer::eSkin) >= 1){
			vertex_format += "S";
		}

		for (U32 i = 0; i < 32; ++i){
			if (indices_vec[i].size() > 0 && vertices_vec[i].size() > 0){
				VBPtr vertex_buffer = llgi->create_vertex_buffer(
					static_cast<U32>(vertices_vec[i].size() * sizeof(float)), 
					static_cast<U32>(vertex_size * sizeof(float)),
					0, vertex_format, vertices_vec[i].data());
				IBPtr index_buffer = llgi->create_index_buffer(
					static_cast<U32>(indices_vec[i].size()), 
					indices_vec[i].data());

				MeshPtr mesh;
				if (fbx_mesh->GetDeformerCount(FbxDeformer::eSkin) >= 1){
					HardwareDeformMesh* hdm = new HardwareDeformMesh();
					mesh.reset(hdm);
					hdm->set_skeleton_instance(skeleton_instance);

					I32 skin_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
					for (I32 skin_index = 0; skin_index < skin_count; ++skin_index){
						FbxSkin* fbx_skin =
							(FbxSkin*)fbx_mesh->GetDeformer(skin_index,
							FbxDeformer::eSkin);
						I32 cluster_count = fbx_skin->GetClusterCount();
						for (I32 cluster_index = 0; cluster_index < cluster_count; ++cluster_index){
							FbxCluster* fbx_cluster = fbx_skin->GetCluster(cluster_index);
							Mat4 mat;
							U32 matrix_index = skeleton->get_bone_index(
								fbx_cluster->GetLink()->GetName());

							Mat4 inverse_bind = get_bind_inverse_matrix(fbx_mesh, fbx_cluster);
							hdm->set_matrix(matrix_index, inverse_bind);
						}
					}
				}
				else{
					mesh.reset(new Mesh());
				}
				mesh->set_vertex_buffer(vertex_buffer);
				mesh->set_index_buffer(index_buffer);
				mesh->set_geometry_type(NECROMANCER_GEOMETRY_MESH);
				FbxSurfaceMaterial* fbx_material =
					fbx_mesh->GetNode()->GetMaterial(i);
				MaterialPtr material =
					create_material_from_fbx_material(fbx_material);
				GeometryPtr geometry(new Geometry());
				geometry->set_mesh(mesh);
				geometry->set_material(material);

				NodeGeometryResource* node_geometry_resource =
					new NodeGeometryResource(geometry);
				SceneNode* geometry_node = new SceneNode();
				geometry_node->set_node_resource(node_geometry_resource);
				scene_node->attach_child(geometry_node);
			}
		}

		return scene_node;
	}

	SceneNode* load_scene_node(FbxNode* fbx_node, SceneNode* parent,
		SkeletonPtr skeleton, const SkeletonInstancePtr& skeleton_instance)
	{
		auto node_attribute = fbx_node->GetNodeAttribute();
		SceneNode* result;

		if (node_attribute){
			auto node_attribute_type = node_attribute->GetAttributeType();
			if (node_attribute_type == FbxNodeAttribute::eMesh){
				FbxMesh* fbx_mesh = static_cast<FbxMesh*>(node_attribute);
				result = load_fbx_mesh(fbx_mesh, skeleton, skeleton_instance);
				if (parent != nullptr)
					parent->attach_child(result);
			}
			else if (node_attribute_type == FbxNodeAttribute::eLight){
				result = new SceneNode(parent);
			}
			else{
				result = new SceneNode(parent);
			}
		}
		else{
			result = new SceneNode(parent);
		}

		//Transform transform = get_transform(fbx_node);
		//result->set_local_transform(transform);
		result->set_name(fbx_node->GetName());

		return result;
	}


	AnimationPtr load_animation(FbxScene* fbx_scene, SkeletonPtr skeleton){
		const F32 sample_time = 0.05f;

		FbxArray<FbxString*> fbx_anim_stack_names;
		fbx_scene->FillAnimStackNameArray(fbx_anim_stack_names);
		auto fbx_stack_name = fbx_anim_stack_names.GetAt(0);
		FbxAnimStack* fbx_anim_stack = fbx_scene->FindMember<FbxAnimStack>(
			fbx_anim_stack_names[0]->Buffer());
		FbxAnimLayer* fbx_anim_layer = fbx_anim_stack->GetMember<FbxAnimLayer>();
		fbx_scene->SetCurrentAnimationStack(fbx_anim_stack);
		FbxTakeInfo* fbx_take_info = fbx_scene->GetTakeInfo(*fbx_stack_name);
		FbxTime fbx_start;
		FbxTime fbx_stop;

		if (fbx_take_info){
			fbx_start = fbx_take_info->mLocalTimeSpan.GetStart();
			fbx_stop = fbx_take_info->mLocalTimeSpan.GetStop();
		}
		else{
			FbxTimeSpan fbx_time_span;
			fbx_scene->GetGlobalSettings().
				GetTimelineDefaultTimeSpan(fbx_time_span);
			fbx_start = fbx_time_span.GetStart();
			fbx_stop = fbx_time_span.GetStop();
		}

		//Time info
		U32 frame_number, matrix_number;
		matrix_number = skeleton->bone_num();
		frame_number = 1;
		F32 t = 0.0f;
		while (t < fbx_stop.GetSecondDouble()){
			t += sample_time;
			++frame_number;
		}

		AnimationPtr animation(new Animation(
			String(*fbx_stack_name), matrix_number, frame_number));

		t = 0.0f;
		U32 k = 0;
		while (t < fbx_stop.GetSecondDouble()){
			animation->set_frame_time(k, t);
			t += sample_time;
			++k;
		}
		animation->set_frame_time(k,
			static_cast<float>(fbx_stop.GetSecondDouble()));

		//matrices
		U32 n = fbx_scene->GetNodeCount();
		for (U32 i = 0; i < n; ++i){
			FbxNode* fbx_node = fbx_scene->GetNode(i);
			String node_name(fbx_node->GetName());
			U32 bone_index = skeleton->get_bone_index(node_name);

			F32 t = 0.0f;
			U32 k = 0;
			while (t < fbx_stop.GetSecondDouble()){
				Mat4 matrix = get_skinning_matrix(fbx_node, t);
				animation->set_matrix(bone_index, k, matrix);
				t += sample_time;
				++k;
			}
			Mat4 matrix = get_skinning_matrix(
				fbx_node, static_cast<float>(
				fbx_stop.GetSecondDouble()));
			animation->set_matrix(bone_index, k, matrix);
		}

		animation->set_duration(
			static_cast<float>(fbx_stop.GetSecondDouble()));

		return animation;
	}

	SubScenePtr create_sub_scene_from_fbx_file(const char* file_name){
		FbxManager* fbx_manager;
		FbxScene* fbx_scene;
		FbxImporter* fbx_importer;

		fbx_manager = FbxManager::Create();
		FbxIOSettings* ios = FbxIOSettings::Create(fbx_manager, IOSROOT);
		fbx_manager->SetIOSettings(ios);
		fbx_scene = FbxScene::Create(fbx_manager, "My Scene");
		fbx_importer = FbxImporter::Create(fbx_manager, "");
		fbx_importer->Initialize(file_name);
		FbxGeometryConverter* fbx_converter = new FbxGeometryConverter(fbx_manager);

		SubScenePtr sub_scene(new SubScene());

		if (fbx_importer->Import(fbx_scene)){
			fbx_converter->Triangulate(fbx_scene, true);
			FbxNode* root = fbx_scene->GetRootNode();

			SkeletonPtr skeleton = create_skeleton(fbx_scene);
			SkeletonInstancePtr skeleton_instance(new SkeletonInstance(skeleton));

			std::queue<FbxNode*> queue;
			std::queue<SceneNode*> parent_queue;
			queue.push(root);
			parent_queue.push(nullptr);
			SceneNode* root_node = nullptr;
			U32 node_index = 0;
			while (!queue.empty()){
				FbxNode* node = queue.front();
				queue.pop();
				SceneNode* parent_node = parent_queue.front();
				parent_queue.pop();

				SceneNode* scene_node = load_scene_node(
					node, parent_node, skeleton, skeleton_instance);
				if (root_node == nullptr)
					root_node = scene_node;
				//sub_scene->set_node(node_index++, scene_node);

				for (int i = 0; i < node->GetChildCount(); ++i){
					queue.push(node->GetChild(i));
					parent_queue.push(scene_node);
				}
			}
			sub_scene->set_root_node(root_node);


			AnimationPtr animation = load_animation(fbx_scene, skeleton);
			AnimationInstancePtr animation_instance(
				new AnimationInstance(animation));
			skeleton_instance->attach_animation_instance(animation_instance);
			AnimationEngine* ae = Necromancer::get_instance()->animation_engine();
			ae->attach_skeleton_instance(skeleton_instance);
		}
		else{

		}

		return sub_scene;
	}

	NECFBXFile::NECFBXFile(const char* file_name){
		FbxManager* fbx_manager;
		FbxScene* fbx_scene;
		FbxImporter* fbx_importer;

		fbx_manager = FbxManager::Create();
		FbxIOSettings* ios = FbxIOSettings::Create(fbx_manager, IOSROOT);
		fbx_manager->SetIOSettings(ios);
		fbx_scene = FbxScene::Create(fbx_manager, "My Scene");
		fbx_importer = FbxImporter::Create(fbx_manager, "");
		fbx_importer->Initialize(file_name);
		FbxGeometryConverter* fbx_converter = new FbxGeometryConverter(fbx_manager);

		SubScenePtr sub_scene(new SubScene());

		if (fbx_importer->Import(fbx_scene)){
			fbx_converter->Triangulate(fbx_scene, true);
			FbxNode* root = fbx_scene->GetRootNode();

			SkeletonPtr skeleton = create_skeleton(fbx_scene);
			SkeletonInstancePtr skeleton_instance(new SkeletonInstance(skeleton));

			std::queue<FbxNode*> queue;
			std::queue<SceneNode*> parent_queue;
			queue.push(root);
			parent_queue.push(nullptr);
			SceneNode* root_node = nullptr;
			U32 node_index = 0;
			while (!queue.empty()){
				FbxNode* node = queue.front();
				queue.pop();
				SceneNode* parent_node = parent_queue.front();
				parent_queue.pop();

				SceneNode* scene_node = load_scene_node(
					node, parent_node, skeleton, skeleton_instance);
				if (root_node == nullptr)
					root_node = scene_node;
				//sub_scene->set_node(node_index++, scene_node);

				for (int i = 0; i < node->GetChildCount(); ++i){
					queue.push(node->GetChild(i));
					parent_queue.push(scene_node);
				}
			}
			sub_scene->set_root_node(root_node);

			AnimationPtr animation = load_animation(fbx_scene, skeleton);
			AnimationInstancePtr animation_instance(
				new AnimationInstance(animation));
			skeleton_instance->attach_animation_instance(animation_instance);
			AnimationEngine* ae = Necromancer::get_instance()->animation_engine();
			ae->attach_skeleton_instance(skeleton_instance);
		}
		else{

		}

		m_sub_scene = sub_scene;
	}

	NECFBXFile::~NECFBXFile(){

	}
}