#include "znpch.hpp"
#include "Zenith/Utilities/FileSystem.hpp"

#include "Zenith/Core/Application.hpp"

#include <GLFW/glfw3.h>

#include <sys/inotify.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include <filesystem>
#include <thread>

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

		s_PersistentStoragePath = HasEnvironmentVariable("ZENITH_DIR") ? GetEnvironmentVariable("ZENITH_DIR") : "..";
		s_PersistentStoragePath /= "Zenith-Editor";

		if (!std::filesystem::exists(s_PersistentStoragePath))
			std::filesystem::create_directory(s_PersistentStoragePath);

		return s_PersistentStoragePath;
	}

	bool FileSystem::HasEnvironmentVariable(const std::string& key)
	{
		return !GetEnvironmentVariable(key).empty();
	}

	bool FileSystem::SetEnvironmentVariable(const std::string& key, const std::string& value)
	{
		int result = setenv(key.c_str(), value.c_str(), 1);
		if (result != 0)
		{
			return false;
		}

		std::filesystem::path zshrcPath = std::getenv("HOME");
		zshrcPath /= ".zshrc";

		std::ofstream zshrcFile(zshrcPath, std::ios::app);
		if (!zshrcFile.is_open())
		{
			return false;
		}

		zshrcFile << std::format("\nexport {}={}\n", key, value);
		zshrcFile.close();

		return true;
	}

	std::string FileSystem::GetEnvironmentVariable(const std::string& key)
	{
		const char* value = getenv(key.c_str());
		if (value)
			return std::string(value);
		else
			return {};
	}

}
