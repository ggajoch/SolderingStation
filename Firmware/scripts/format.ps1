$build = "..\..\build\"

./compile_arm.ps1
./unit_tests.ps1
make -C $build\arm\ arm.format
make -C $build\arm\ unit_tests.format
make -C $build\x86 core.format
make -C $build\x86 libs.format