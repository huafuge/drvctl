
set_project 'drvctl'

add_rules('mode.debug')
add_rules('mode.release')

target 'drvctl'
    set_kind 'binary'

    add_files('src/*.cpp')
    add_includedirs 'src'
