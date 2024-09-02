#include "app.h"

#include <windows.h>
#include <Shlobj.h>

#include "imgui/imgui.h"

ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y);
}

bool operator==(const ImVec4& lhs, const ImVec4& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(ImVec4)) == 0;
}

bool operator!=(const ImVec4& lhs, const ImVec4& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(ImVec4)) != 0;
}


bool open_file_dialog(std::string& select_file_path) {
	IFileDialog* pfd = NULL;
	auto hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
	DWORD flags;
	hr = pfd->GetOptions(&flags);
	hr = pfd->SetOptions(flags | FOS_FORCEFILESYSTEM | FOS_ALLOWMULTISELECT);
	COMDLG_FILTERSPEC file_types[] = {
		{ L"NGR Niagara Performance File", L"*.niagaraperf" },
	};
	hr = pfd->SetFileTypes(ARRAYSIZE(file_types), file_types);
	hr = pfd->SetFileTypeIndex(1);

	hr = pfd->Show(NULL);

	IShellItem* item = nullptr;
	hr = pfd->GetResult(&item);

	if (item == nullptr) {
		return false;
	}

	struct CoTaskMemFreeScope {
		CoTaskMemFreeScope(LPVOID p)
			:ptr(p) {

		}

		~CoTaskMemFreeScope() {
			if (ptr) {
				CoTaskMemFree(ptr);
			}
		}

		LPVOID ptr;
	};

	LPWSTR wpath = NULL;
	CoTaskMemFreeScope _(wpath);
	hr = item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &wpath);

	if (wpath == NULL) {
		return false;
	}

	DWORD dwMinSize = 0;
	dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, wpath, -1, NULL, 0, NULL, FALSE);
	if (dwMinSize == 0) {
		return false;
	}

	select_file_path.resize(dwMinSize + 1);
	WideCharToMultiByte(CP_OEMCP, NULL, wpath, -1, select_file_path.data(), dwMinSize, NULL, FALSE);

	return true;
}


void ImApp::init() {
    mainWindow = new MyMainWindow();
}

void ImApp::quit() {
    _DeletePointer(mainWindow);
}

void ImApp::tick() {
	mainWindow->render(0);
}
