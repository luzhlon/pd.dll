
#include <Windows.h>
#include <string>
#include "dump_process.h"

using namespace std;

static string ExePath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string::size_type pos = string(buffer).find_last_of("\\/");
    return string(buffer).substr(0, pos);
}

const std::wstring s2ws(const std::string& src)
{
    std::locale sys_locale("");

    const char* data_from = src.c_str();
    const char* data_from_end = src.c_str() + src.size();
    const char* data_from_next = 0;

    wchar_t* data_to = new wchar_t[src.size() + 1];
    wchar_t* data_to_end = data_to + src.size() + 1;
    wchar_t* data_to_next = 0;

    wmemset( data_to, 0, src.size() + 1 );

    typedef std::codecvt<wchar_t, char, mbstate_t> convert_facet;
    mbstate_t in_state = { 0 };
    auto result = std::use_facet<convert_facet>(sys_locale).in(
    	in_state, data_from, data_from_end, data_from_next,
    	data_to, data_to_end, data_to_next );
    if( result == convert_facet::ok )
    {
    	std::wstring dst = data_to;
    	delete[] data_to;
    	return dst;
    }
    else
    {
    	printf( "convert error!\n" );
    	delete[] data_to;
    	return std::wstring(L"");
    }
}

extern "C" __declspec(dllexport) const char * DumpModule(void *address, char *output_path)
{
    if (!address || !output_path)
    {
        return "Invalid Argument";
    }

    auto clean_database = ExePath() + "\\" + "clean.hashes";
    auto db = new pe_hash_database((char*)clean_database.c_str());

    PD_OPTIONS options;
    options.ImportRec = true;
    options.ForceGenHeader = false;
    options.Verbose = false;
    options.EntryPointOverride = -1;
    options.ReconstructHeaderAsDll = false;
    options.DumpChunks = true;
    options.NumberOfThreads = 16; // Default 16 threads
    options.set_output_path(output_path);

    auto dp = new dump_process(GetCurrentProcessId(), db, &options, true);
    dp->dump_region(reinterpret_cast<__int64>(address));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }
    return TRUE;
}
