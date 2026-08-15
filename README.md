> [!WARNING]
> this is project is purely for learing purposes and in no way supposed to be used in a real setting.

# makkro
basic interpreted language for writing macros in the X window system.

## makkro's stance on semicolons
as any other *real* language, makkro uses semicolons.

## functions
there aren't (yet) any of these fancy features of moders languages, like loops, expressions, self declared functions or control flow,
instead there are just these basic functions:

### calling a function
functions are called with the function name followed by an arrow ('->') and the function parameters, even if the function doesnt have any params, makkro still needs the arrow to know to call the function

example: 
```makkro
str -> "echo hello makkro";
key -> "Return";
```
run it with: `./makkro file.makkro`

### mouse functions
- `mov -> <x: int> <y: int>`: moves cursor to \<x\> \<y\> on the screen
- `but -> <button: int>`: presses mouse button \<button\> (left click = 1, middle click = 2, right click = 3)
- `hbut -> <button: int>`: holds \<button\>
- `rbut -> <button: int>`: releases \<button\>
- `get -> None`: prints pointer position to stdout 

### keyboard functions
- `str -> <string: str>`: writes \<string\>
- `key -> <key: str>`: presses \<key\>
- `hkey -> <key: str>`: holds \<key\>
- `rkey -> <key: str>`: releases \<key\>

### misc functions
- `sleep -> <seconds: int>`: sleeps for \<seconds\>
- `delay -> <ms: int>`: sleeps for \<ms\> milliseconds

