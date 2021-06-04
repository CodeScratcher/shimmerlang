# Shimmer Standard Library
This is the folder where all of the builtin libraries will go.
At the moment, there are only a few libraries, but more will be added soon.

## loops
### ```do_while(callback=func, cond=func)```
* repeat the following:
  * call callback
  * make sure cond evaluates to true
    * if not, exit

### ```while(cond=func, callback=func)```
* repeat the following:
  * make sure cond evaluates to true
    * if not, exit
  * call callback

### ```until(cond=func, callback=func)```
* repeat the following:
  * make sure cond evaluates to false
    * if not, exit
  * call callback

### ```for(init=func, cond=func, step=func, callback=func)```
* call init
* repeat the following:
    * make sure cond evaluates to false
      * if not, exit
    * call callback
    * call step

## math
### ```Nothing yet!```

## headtail
### ```car(linked_list)```
* returns the head of a linked list

### ```cdr(linked_list)```
* returns the tail of a linked list
## server
### ```handle(handler)```
* handler  must be function with specification: 
`(req) {} => string`
req is a hash map with data about the request.
the return value of the function is the response.