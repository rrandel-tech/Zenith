#include "znpch.hpp"
#include "Entity.hpp"

namespace Zenith {

	Entity::Entity(const std::string& name)
		: m_Name(name), m_Transform(1.0f)
	{

	}

	Entity::~Entity()
	{

	}

}