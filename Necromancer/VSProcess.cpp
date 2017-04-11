#include "stdafx.h"
#include "VSProcess.h"
#include "Exception.h"

namespace Necromancer{
	VSProcess::VSProcess(){
	}

	VSProcess::~VSProcess(){
	}

	void VSProcess::attach_uid(U32 uid){
		m_declaration_uid.push_back(uid);
	}

	void VSProcess::attach_process_string(const String& format,
		const String& context)
	{
		m_process_map[format] = context;
	}

	String VSProcess::process_string(const String& format){
		auto suitable_format = find_suitable_format(format);
		return m_process_map[suitable_format];
	}

	void VSProcess::set_output_format(const String& format,
		const String& output_format)
	{
		m_output_format[format] = output_format;
	}

	String VSProcess::output_format(const String& format){
		auto suitable_format = find_suitable_format(format);
		return m_output_format[suitable_format];
	}

	U32 VSProcess::caculate_simularity(const String& str1,
		const String& str2)
	{
		//TOOD a bug: str1[i] == str2[i] no means
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

	String VSProcess::find_suitable_format(const String& format){
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
		return suitable_format;
	}
}