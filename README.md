# Console Commands Extender

## Build Dependencies
* [SKSE64](https://skse.silverlock.org/)
* [CommonLibSSE](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE)

## End User Dependencies
* [SKSE64](https://skse.silverlock.org/)

## Console commands
<img width=120/> | Help
--- | ---
**Command** | `"Help" <expr>`
**Expression** | `<exp> ::= <empty> | <matchstring> | <matchstring> <filter> | <matchstring> <filter> <form-type>`
| `<matchstring> ::= <string> ; The string to filter results with`
| `<filter> ::= <integer>`<br>`; 0 - All``; 1 - Functions`<br>`; 3 - Globals`<br>`; 4 - Forms`
| `<form-type> ::= <string> ; The form type to filter form results with`
**Example Usage** | `help iron 4 armo`
**Description** | Performs a substring search on all forms by name and editor ID
<img width=120/> | Clear
--- | ---
**Command** | `"Clear"`
**Example Usage** | `clear`
**Description** | Clears the console output
