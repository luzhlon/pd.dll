
set_project 'pd'

add_rules('mode.debug')
add_rules('mode.release')

set_targetdir '$(buildir)/$(mode)/$(arch)'
set_objectdir '$(buildir)/.objs/$(mode)/$(arch)'
add_linkdirs '$(buildir)/$(mode)/$(arch)'

if is_mode 'debug' then
    add_cxxflags '/MTd'
else
    add_cxxflags '/MT'
end

target 'pd'
    set_kind 'shared'

    add_includedirs('src')

    add_files('src/dump_process.cpp')
    add_files('src/export_list.cpp')
    add_files('src/hash.cpp')
    add_files('src/module_list.cpp')
    add_files('src/pd.cpp')
    add_files('src/pe_exports.cpp')
    add_files('src/pe_hash_database.cpp')
    add_files('src/terminate_monitor_hook.cpp')
    add_files('src/pe_header.cpp')
    add_files('src/pe_imports.cpp')
    add_files('src/close_watcher.cpp')
    add_files('src/simple.cpp')
    add_files('src/stdafx.cpp')

    add_files('src/dllmain.cpp')

    add_defines('_UNICODE', 'UNICODE')
    add_cxxflags('/Zc:wchar_t')

    add_links('Shlwapi', 'Advapi32', 'Psapi')
