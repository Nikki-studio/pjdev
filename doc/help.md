# pjdev

## table of contents

> has 3 modes

- [modes](#modes)
  - [dir: this is the directory navigation mode.](#dir)
  - [cmd: this is the dedicated command mode.](#cmd)
  - [rwx this is the main editor mode.](#rwx)

## modes

> modes are layers that are built so as to properly control or get what you want without messing up
> in pjdev one can togglemodes using key `esc`

### dir

> This is the mode that helps you navigate directories easily.

### cmd

> this mode helps you write and execute commands via a temporary `bash` (recommended) or `batch` scripts

### rwx

> this is the mode dedicated in reading, writing and executing of source code (compilation scrips)
> if one focuses on a certain file, stages changes but does not write the changes to the subject file, but instead focuses on another file, the changes are 'journaled' inside the project subfolder `.pjdev` which then prompts the user to write the chages.
> if incase the changes are not written. the 'journal' remains and asks the user on the next time the project is opened.
