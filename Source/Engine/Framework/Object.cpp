#include "Object.h"

namespace kiko{
	CLASS_DEFINITION(Object)

	void Object::Read(const rapidjson::Value& value){
		READ_DATA(value, name);
	}
}
