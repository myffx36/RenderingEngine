#pragma once

#include "Container.hpp"
#include "CommonUtility.h"

namespace Necromancer{
	class VSProcess{
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, uid);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(String, name);
		DEF_DATA_MEMBER_AND_GETTER_SETTER(U32, priority);
	public:
		~VSProcess();
		VSProcess();

		void attach_uid(U32);
		const List<U32>::Type& declaration_list() const{
			return m_declaration_uid;
		}
		void attach_process_string(const String& format,
			const String& context);
		String process_string(const String& format);

		void set_output_format(const String& format,
			const String& output);
		String output_format(const String& format);
	private:
		U32 caculate_simularity(const String& str1,
			const String& str2);
		String find_suitable_format(const String&);
	private:
		List<U32>::Type m_declaration_uid;
		Map<String, String>::Type m_process_map;
		Map<String, String>::Type m_output_format;
	};
}

