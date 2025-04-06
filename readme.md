# Recom


## Functionality 
This application recompiles your code on file save and displays the output

## Usage 
```bash
recom "go run ." --d=somedir 
``` 
will watch the changes in the directorie and when change will re run the command

```bash 
recom "go run somedir/main.go" --f=somedir/main.go
``` 
will recompile main.go on change


#### TODO: 
- [ ] Add the functionality of adding folders to not watch for changes