# Shimmer Standard Library (WIP)
This is the folder where all of the builtin libraries will go.
At the moment, there are only a few libraries, but more will be added soon.

## loops
#### ```do_while(callback: () => void {}, cond: () => bool {})```
* repeat the following:
  * call callback
  * make sure cond evaluates to true
    * if not, exit

#### ```while(cond: () => bool {}, callback: () => void {})```
* repeat the following:
  * make sure cond evaluates to true
    * if not, exit
  * call callback

#### ```until(cond: () => bool {}, callback: () => void {})```
* repeat the following:
  * make sure cond evaluates to false
    * if not, exit
  * call callback

#### ```for(init: () => void {}, cond: () => bool {}, step: () => void {}, callback: () => void {})```
* call init
* repeat the following:
    * make sure cond evaluates to false
      * if not, exit
    * call callback
    * call step

## math
#### ```Nothing yet!```

## linkedlist
#### ```car(l: linked_list)```
* returns the head of a linked list

#### ```cdr(l: linked_list)```
* returns the tail of a linked list

## server
### ```handle(handler: (req) => string {})```
`req` is a hash map with data about the request.
The return value of the function is the response.