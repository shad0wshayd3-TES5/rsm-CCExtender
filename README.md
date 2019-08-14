# Console Commands Extender

## Build Dependencies
* [SKSE64](https://skse.silverlock.org/)
* [CommonLibSSE](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE)

## End User Dependencies
* [SKSE64](https://skse.silverlock.org/)

## Console commands
<img width=120/> | <img width=120/>
--- | ---
<img width=120/> | <img width=120/>
**Command** | `"> <file-name>"`
**Expression** | `<file-name> ::= <string-no-space> | """ <string-with-space> """
<img width=120/> | `<string-no-space> ::= <string> ; Cannot contain spaces
<img width=120/> | `<string-with-space> ::= <string> | <string> " " <string-with-space> ; Can contain spaces`
**Example Usage** | `help belethor 4 > "output.txt"`
**Description** | Pipes output from a console command to the indicated file. File names with spaces must be wrapped in parenthesis. Files are created relative to Skyrim's directory.
**Command** | `"Help" <expr>`
**Expression** | `<expr> ::= <empty> \| <matchstring> \| <matchstring> <filter> \| <matchstring> <filter> <form-type>`
<img width=120/> | `<matchstring> ::= <string> ; The string to filter results with`
<img width=120/> | `<filter> ::= <integer>`<br>`; 0 - All`<br>`; 1 - Functions`<br>`; 3 - Globals`<br>`; 4 - Forms`
<img width=120/> | `<form-type> ::= <string> ; The form type to filter form results with`
**Example Usage** | `help iron 4 armo`
**Description** | Performs a substring search on all forms by name and editor ID
<img width=120/> | <img width=120/>
**Command** | `"Clear"`
**Example Usage** | `clear`
**Description** | Clears the console output
