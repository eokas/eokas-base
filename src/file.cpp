
#include "file.h"
#include "string.h"

#if _EOKAS_OS == _EOKAS_OS_WIN32
#include <Windows.h>
#include <io.h>
#else
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

_BeginNamespace(eokas)

#if _EOKAS_OS == _EOKAS_OS_WIN32

static void FileTimeToTimeT(FILETIME* ft, time_t* tt)
{
	ULARGE_INTEGER uln;
	uln.LowPart = ft->dwLowDateTime;
	uln.HighPart = ft->dwHighDateTime;
	*tt = ((LONGLONG)(uln.QuadPart - 116444736000000000) / 10000000);
}

#endif

/*
=================================================================
== FileStream
=================================================================
*/
FileStream::FileStream(const String& fileName, const String& openMode)
	:mName(fileName),
	mMode(openMode),
	mHandle(nullptr)
{}

FileStream::~FileStream()
{
	this->close();
}

bool FileStream::open()
{
	if (mHandle != nullptr)
	{
		fseek(mHandle, 0, SEEK_SET);
		return true;
	}
	mHandle = fopen(mName.cstr(), mMode.cstr());
	return mHandle != nullptr;
}

void FileStream::close()
{
	if(mHandle != nullptr)
	{
		fclose(mHandle);
		mHandle = nullptr;
	}
}

bool FileStream::isOpen() const
{
	return mHandle != nullptr;
}

bool FileStream::readable() const
{
	return mMode.startsWith("r");
}

bool FileStream::writable() const
{
	return mMode.startsWith("w") || mMode.startsWith("a");
}

bool FileStream::eos() const
{
	return feof(mHandle) != 0;
}

size_t FileStream::pos() const
{
	return ftell(mHandle);
}

size_t FileStream::size() const
{
	long pos = ftell(mHandle);
	fseek(mHandle, 0, SEEK_END);
	long len = ftell(mHandle);
	fseek(mHandle, pos, SEEK_SET);
	return len;
}

size_t FileStream::read(void* data, size_t size)
{
	return fread(data, 1, size, mHandle);
}

size_t FileStream::write(void* data, size_t size)
{
	return fwrite(data, 1, size, mHandle);
}

bool FileStream::seek(int offset, int origin)
{
	return fseek(mHandle, offset, origin) == 0;
}

void FileStream::flush()
{
	fflush(mHandle);
}

FILE* FileStream::handle() const
{
	return mHandle;
}

/*
=================================================================
== File System Interface
=================================================================
*/
bool File::exists(const String& path)
{
#if _EOKAS_OS == _EOKAS_OS_WIN32
	return _access(path.cstr(), 0) == 0;
#else
	return access(path.cstr(), F_OK) == 0;
#endif
}

bool File::isFile(const String& path)
{
#if _EOKAS_OS == _EOKAS_OS_WIN32
	DWORD attr = GetFileAttributesA(path.cstr());
	if (attr == INVALID_FILE_ATTRIBUTES)
		return false;
	return !(attr & FILE_ATTRIBUTE_DIRECTORY);
#else
	if (access(path.cstr(), 0) == 0)
	{
		struct stat status;
		stat(path.cstr(), &status);
		return status.st_mode & S_IFREG;
	}
	return false;
#endif
}

bool File::isFolder(const String& path)
{
#if _EOKAS_OS == _EOKAS_OS_WIN32
	DWORD attr = GetFileAttributesA(path.cstr());
	if (attr == INVALID_FILE_ATTRIBUTES)
		return false;
	return !!(attr & FILE_ATTRIBUTE_DIRECTORY);
#else
	if (access(path.cstr(), 0) == 0)
	{
		struct stat status;
		stat(path.cstr(), &status);
		return status.st_mode & S_IFDIR; // FIXME: what about symlinks here?
	}
	return false;
#endif
}

FileList File::fileInfoList(const String& path)
{
	FileList list;
#if _EOKAS_OS == _EOKAS_OS_WIN32
    String search = File::combinePath(path, "*");
	WIN32_FIND_DATAA f;
	HANDLE h = FindFirstFileA(search.cstr(), &f);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			FileInfo info;
			info.name = f.cFileName;
			ULARGE_INTEGER n;
			n.LowPart = f.nFileSizeLow;
			n.HighPart = f.nFileSizeHigh;
			info.size = n.QuadPart;
			info.isFile = !(f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
			info.isHidden = !!(f.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN);
			FileTimeToTimeT(&f.ftLastAccessTime, &info.atime);
			FileTimeToTimeT(&f.ftLastWriteTime, &info.mtime);
			FileTimeToTimeT(&f.ftCreationTime, &info.ctime);
			list.push_back(info);

		} while (FindNextFileA(h, &f));
		FindClose(h);
	}
#else
	dirent* ptr = nullptr;
	DIR* dir = opendir(path.cstr());
	while ((ptr = readdir(dir)) != nullptr)
	{
		const char* name = ptr->d_name;
		struct stat status;
		stat((path + "/" + name).cstr(), &status);

		FileInfo info;
		info.name = name;
		info.size = status.st_size;
		info.isFile = (status.st_mode & S_IFREG) != 0;
		info.isHidden = false;
		info.atime = status.st_atime;
		info.mtime = status.st_mtime;
		info.ctime = status.st_ctime;

		list.push_back(info);
	}
	closedir(dir);
#endif
	return list;
}

StringList File::fileNameList(const String& path)
{
	StringList list;
#if _EOKAS_OS == _EOKAS_OS_WIN32
	String search = File::combinePath(path, "*");
	WIN32_FIND_DATAA f;
	HANDLE h = FindFirstFileA(search.cstr(), &f);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				const char* name = f.cFileName;
				list.push_back(name);
			}
		} while (FindNextFileA(h, &f));
		FindClose(h);
	}
#else
	dirent* ptr = nullptr;
	DIR* dir = opendir(path.cstr());
	while ((ptr = readdir(dir)) != nullptr)
	{
		const char* name = ptr->d_name;
		list.push_back(name);
	}
	closedir(dir);
#endif
	return list;
}

StringList File::folderNameList(const String& path)
{
	StringList list;
#if _EOKAS_OS == _EOKAS_OS_WIN32
    String search = File::combinePath(path, "*");
	WIN32_FIND_DATAA f;
	HANDLE h = FindFirstFileA(search.cstr(), &f);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				const char* name = f.cFileName;
				if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
					continue;
				list.push_back(name);
			}
		} while (FindNextFileA(h, &f));
		FindClose(h);
	}
#else
	dirent* ptr = nullptr;
	DIR* dir = opendir(path.cstr());
	while ((ptr = readdir(dir)) != nullptr)
	{
        const char* name = ptr->d_name;
		if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
			continue;
		list.push_back(name);
	}
	closedir(dir);
#endif
	return list;
}

String File::executingPath()
{
#if _EOKAS_OS == _EOKAS_OS_WIN32
	char buf[MAX_PATH];
	DWORD ret = GetModuleFileNameA(NULL, buf, MAX_PATH);
	if (ret == 0)
		return "";
	return buf;
#else
	char buf[PATH_MAX];
	size_t ret = readlink("/proc/self/exe", buf, PATH_MAX - 1);
	if (ret <= 0 || (ret > PATH_MAX - 1))
		return "";
	buf[ret] = '\0';
	return buf;
#endif
}

String File::absolutePath(const String& path)
{
#if _EOKAS_OS == _EOKAS_OS_WIN32
	char resolved_path[MAX_PATH] = { 0 };
	_fullpath(resolved_path, path.cstr(), MAX_PATH);
	return resolved_path;
#else
	char resolved_path[PATH_MAX] = { 0 };
	realpath(path.cstr(), resolved_path);
	return resolved_path;
#endif
}

String File::basePath(const String& path)
{
	size_t index1 = path.rfind('/');
	size_t index2 = path.rfind('\\');
	if (index1 == String::npos && index2 == String::npos)
		return "./";
	if (index1 == String::npos)
		return path.substr(0, index2);
	if (index2 == String::npos)
		return path.substr(0, index1);
	return path.substr(0, index1 < index2 ? index1 : index2);
}

String File::fileName(const String& path)
{
	size_t index1 = path.rfind('/');
	size_t index2 = path.rfind('\\');
	if (index1 == String::npos && index2 == String::npos)
		return path;
	if (index1 == String::npos)
		return path.substr(index2);
	if (index2 == String::npos)
		return path.substr(index1);
	return path.substr(index1 < index2 ? index1 : index2);
}

String File::fileNameWithoutExtension(const String& path)
{
	String name = File::fileName(path);
	size_t index = name.rfind('.');
	if (index == String::npos)
		return name;
	return name.substr(0, index);
}

String File::fileExtension(const String& path)
{
	String name = File::fileName(path);
	size_t index = name.rfind('.');
	if (index == String::npos)
		return "";
	return name.substr(index);
}

String File::combinePath(const String& path1, const String& path2)
{
    if(path1.isEmpty()) return path2;
    if(path2.isEmpty()) return path1;
    
    char end = path1.at(path1.length() - 1);
    if(end == '/' || end == '\\')
        return path1 + path2;
    
    return path1 + "/" + path2;
}


_EndNamespace(eokas)
