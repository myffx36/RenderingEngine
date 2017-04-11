#include "stdafx.h"
#include "FSProcess.h"
#include "Exception.h"

namespace Necromancer{
	FSProcess::FSProcess(){
	}

	FSProcess::~FSProcess(){
	}

	void FSProcess::attach_uid(U32 uid){
		m_declaration_uid.push_back(uid);
	}

	void FSProcess::attach_process_string(const String& format,
		const String& context)
	{
		m_process_map[format] = context;
	}

	String FSProcess::process_string(const String& format){
		auto suitable_format = find_suitable_format(format);
		return m_process_map[suitable_format];
	}

	U32 FSProcess::caculate_simularity(const String& str1,
		const String& str2)
	{
		U32 result = 0;
		for(U32 i = 0;i < str1.size();++ i){
			for(U32 j = 0;j < str2.size();++ j){
				if(str1[i] == str2[j]){
					++ result;
					break;
				}
			}
		}

		return result;
	}

	String FSProcess::find_suitable_format(const String& format){
		U32 suitable_size = 0;
		String suitable_format = "";
		for(auto iter = m_process_map.begin();
			iter != m_process_map.end();++ iter)
		{
			String str = iter->first;
			U32 similarity = caculate_simularity(str, format);
			if(similarity == str.size()){
				if(similarity > suitable_size){
					suitable_format = str;
					suitable_size = similarity;
				}
			}
		}

		if(suitable_format == ""){
			throw Exception("unsupport format");
		}

		return suitable_format;
	}
}