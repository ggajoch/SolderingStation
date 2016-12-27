file(GLOB_RECURSE allDriversFiles
	Drivers/*.c
)

foreach(file ${allDriversFiles})
	set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "-w")
endforeach(x)
