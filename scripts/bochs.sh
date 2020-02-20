src=$(git rev-parse --show-toplevel)
name=TerraOS

bochs 'magic_break: enabled=1' 'boot:cdrom' "ata0-slave: type=cdrom, path=${src}/bin/$1/${name}.iso, status=inserted" 'display_library: x, options="gui_debug"'