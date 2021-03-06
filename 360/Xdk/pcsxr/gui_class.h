#include <xtl.h>
#include <xboxmath.h>
#include <xui.h>
#include <xuiapp.h>
#include <string>
#include <vector>
#include <algorithm>


class FileBrowserProvider {
protected:
	struct _FILE_INFO {
		std::wstring filename;
		std::wstring displayname;
		bool isDir;
	};

	std::wstring currentDir;
	std::vector<_FILE_INFO> fileList;
	 
protected:

	static bool compare (const _FILE_INFO a, const _FILE_INFO b) {
		
		/* If one is a file and one is a directory the directory is first. */
		if(a.isDir && !b.isDir) return true;
        if(!a.isDir && b.isDir) return false;

		return a.displayname < b.displayname;
	}

	void Sort() {
		std::sort(fileList.begin(), fileList.end(), compare);
	}
	
public:
	void Init() {
		currentDir = L"game:";
	}

	void AddParentEntry(std::wstring currentDir) {
		_FILE_INFO finfo;
		wchar_t lastLetter = currentDir[currentDir.length() - 1];
		if (lastLetter == L':') {
			finfo.isDir = true;
			finfo.displayname = L"DeviceList";
			finfo.filename = L"/";

			fileList.push_back(finfo);
			return;
		}

		unsigned int p = currentDir.rfind(L"\\");
		currentDir = currentDir.substr(0,  p);

		finfo.isDir = true;
		finfo.displayname = L"..";
		finfo.filename = currentDir;

		fileList.push_back(finfo);
	}

	HRESULT AddDevicesList() {
		// Free file list
		fileList.clear();

		_FILE_INFO finfo;
		finfo.isDir = true;
		
		finfo.filename = L"game:";
		finfo.displayname = L"[GAME]";
		fileList.push_back(finfo);

		finfo.filename = L"usb0:";
		finfo.displayname = L"[USB0]";
		fileList.push_back(finfo);

		finfo.filename = L"hdd0:";
		finfo.displayname = L"[HDD0]";
		fileList.push_back(finfo);

		finfo.filename = L"hdd1:";
		finfo.displayname = L"[HDD1]";
		fileList.push_back(finfo);
		
		finfo.filename = L"hdd2:";
		finfo.displayname = L"[HDD2]";
		fileList.push_back(finfo);
		
		finfo.filename = L"hdd3:";
		finfo.displayname = L"[HDD3]";
		fileList.push_back(finfo);
		
		finfo.filename = L"hddx:";
		finfo.displayname = L"[HDDX]";
		fileList.push_back(finfo);

		return S_OK;
	}

	HRESULT UpdateDirList(std::wstring currentDir = L"game:") {
		WIN32_FIND_DATA ffd;
		std::string szDir;

		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD dwError=0;		

		if(currentDir == L"/") {
			AddDevicesList();
			return S_OK;
		}
		// Free file list
		fileList.clear();

		AddParentEntry(currentDir);

		get_string(currentDir, szDir);
		szDir = szDir + "\\*";

		// Look for files in current directory
		hFind = FindFirstFile(szDir.c_str(), &ffd);

		if (INVALID_HANDLE_VALUE == hFind) 
		{
			return S_FALSE;
		} 

		// List all the files in the directory with some info about them.
		do
		{
			_FILE_INFO finfo;
			finfo.isDir = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)?true:false;
			get_wstring(ffd.cFileName, finfo.displayname);
			finfo.filename = currentDir + L"\\" + finfo.displayname;

			// :s
			if (finfo.isDir) {
				finfo.displayname = L"["+finfo.displayname+L"]";
			}

			fileList.push_back(finfo);

		}
		while (FindNextFile(hFind, &ffd) != 0);

		FindClose(hFind);

		Sort();

		return S_OK;
	}

	DWORD Size() {
		return this->fileList.size();
	}

	LPCWSTR At(unsigned int i) {
		if (i >= 0 && i < Size()) {
			return this->fileList[i].displayname.c_str();
		} else {
			return L"";
		}
	}

	LPCWSTR Filename(unsigned int i) {
		if (i >= 0 && i < Size()) {
			return this->fileList[i].filename.c_str();
		} else {
			return L"";
		}
	}

	bool IsDir(unsigned int i) {
		if (i >= 0 && i < Size()) {
			return this->fileList[i].isDir;
		} else {
			return false;
		}
	}
};