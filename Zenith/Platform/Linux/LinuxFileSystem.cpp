#include "znpch.hpp"
#include "Zenith/Utilities/FileSystem.hpp"

#include "Zenith/Core/Application.hpp"

#include <GLFW/glfw3.h>

#include <sys/inotify.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include <nlohmann/json.hpp>

namespace Zenith {

	static std::filesystem::path s_PersistentStoragePath;

	FileStatus FileSystem::TryOpenFile(const std::filesystem::path& filepath)
	{
		int res = access(filepath.c_str(), F_OK);

		if (!res) return FileStatus::Success;

		switch (errno)
		{
			default: return FileStatus::OtherError;

			case ENOENT: [[fallthrough]];
			case ENOTDIR: return FileStatus::Invalid;

			case EPERM: [[fallthrough]];
			case EACCES: return FileStatus::Locked;
		}
	}

	bool FileSystem::WriteBytes(const std::filesystem::path& filepath, const Buffer& buffer)
	{
		std::ofstream stream(filepath, std::ios::binary | std::ios::trunc);

		if (!stream)
		{
			stream.close();
			return false;
		}

		stream.write((char*)buffer.Data, buffer.Size);
		stream.close();

		return true;
	}

	Buffer FileSystem::ReadBytes(const std::filesystem::path& filepath)
	{
		Buffer buffer;

		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
		ZN_CORE_ASSERT(stream);

		auto end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		auto size = end - stream.tellg();
		ZN_CORE_ASSERT(size != 0);

		buffer.Allocate((uint32_t)size);
		stream.read((char*)buffer.Data, buffer.Size);
		stream.close();

		return buffer;
	}

	std::filesystem::path FileSystem::GetPersistentStoragePath()
	{
		if (!s_PersistentStoragePath.empty())
			return s_PersistentStoragePath;

		const char* configHome = std::getenv("XDG_CONFIG_HOME");
		if (configHome)
			s_PersistentStoragePath = configHome;
		else
			s_PersistentStoragePath = std::getenv("HOME") + std::string("/.config");

		s_PersistentStoragePath /= "Zenith-Editor";

		if (!std::filesystem::exists(s_PersistentStoragePath))
			std::filesystem::create_directory(s_PersistentStoragePath);

		return s_PersistentStoragePath;
	}

	bool FileSystem::HasConfigValue(const std::string& key)
	{
		auto path = GetPersistentStoragePath() / "zenith.conf";
		if (!std::filesystem::exists(path)) return false;

		std::ifstream in(path);
		if (!in.is_open()) return false;

		// Read the config file
	}

	bool FileSystem::SetConfigValue(const std::string& key, const std::string& value)
	{
		auto path = GetPersistentStoragePath() / "zenith.conf";

		if (std::filesystem::exists(path))
		{
			std::ifstream in(path);
			if (in.is_open()) return false;
		}

		std::ofstream out(path);
		if (!out.is_open()) return false;
		// Write the config file
		return true;
	}

	std::string FileSystem::GetConfigValue(const std::string& key)
	{
		auto path = GetPersistentStoragePath() / "zenith.conf";
		if (!std::filesystem::exists(path)) return {};

		// Read the config file
	}

}
