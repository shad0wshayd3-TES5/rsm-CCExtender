# Console Commands Extender

## Build Dependencies
* [SKSE64](https://skse.silverlock.org/)
* [Json2Settings](https://github.com/Ryan-rsm-McKenzie/Json2Settings)
* [CommonLibSSE](https://github.com/Ryan-rsm-McKenzie/CommonLibSSE)

## End User Dependencies
* [SKSE64](https://skse.silverlock.org/)

## Settings
Setting | Description
--- | ---
`betaCommentFileName` | The file path to print beta comment output to.

## Console commands
<img width=120/> | <img width=120/>
--- | ---
<img width=120/> | <img width=120/>
**Command** | `"> " <file-name>`
**Expression** | `<file-name> ::= <string-no-space> \| """ <string-with-space> """`
<img width=120/> | `<string-no-space> ::= <string> ; Cannot contain spaces`
<img width=120/> | `<string-with-space> ::= <string> \| <string> " " <string-with-space> ; Can contain spaces`
**Example Usage** | `help belethor 4 > "output.txt"`
**Description** | Pipes output from a console command to the indicated file. File names with spaces must be wrapped in parenthesis. Files are created relative to Skyrim's directory.
<img width=120/> | <img width=120/>
**Command** | `<betacomment> " " <comment>`
**Expression** | `<betacomment> ::= "BetaComment" \| "BC"`
<img width=120/> | `<comment> ::= <string> ; The comment string`
**Example Usage** | `bc "this item does not belong here"`
**Description** | Logs a comment to the beta comment file with the given string. Comments are in the following format `USERNAME:	(YYYY-MM-DD HH:MM:SS GMT)	SOURCE-FILE	[CELL-FORM-ID: CELL-EDITOR-ID (CELL-X, CELL-Y)]	[REF-FORM-ID: REF-EDITOR-ID (REF-X, REF-Y, REF-Z)]	"COMMENT"`.
<img width=120/> | <img width=120/>
**Command** | `"Clear"`
**Example Usage** | `clear`
**Description** | Clears the console output.
<img width=120/> | <img width=120/>
**Command** | `"Help" <expr>`
**Expression** | `<expr> ::= <empty> \| " " <matchstring> \| " " <matchstring> " " <filter> \| " " <matchstring> " " <filter> " " <form-type>`
<img width=120/> | `<matchstring> ::= <string> ; The string to filter results with`
<img width=120/> | `<filter> ::= <integer>`<br>`; 0 - All`<br>`; 1 - Functions`<br>`; 3 - Globals`<br>`; 4 - Forms`
<img width=120/> | `<form-type> ::= <string> ; The form type to filter form results with`
**Example Usage** | `help iron 4 armo`
**Description** | Performs a substring search on all forms by name and editor ID.
<img width=120/> | <img width=120/>
**Command** | `<setperkpoints> " " <numperkpoints>`
**Expression** | `<setperkpoints> ::= "SetPerkPoints" \| "SPP"`
<img width=120/> | `<numperkpoints> ::= <integer> ; The number of perk points`
**Example Usage** | `spp 10`
**Description** | Sets the player's perk points to the specified number.
