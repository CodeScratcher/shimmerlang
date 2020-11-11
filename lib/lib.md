# Shimmer Standard Library
This is the folder where all of the builtin libraries will go.
At the moment, there are only a few libraries, but more will be added soon.

## loops
### ```do_while(callback=func, cond=bool)```
* repeat the following:
  * call callback
  * make sure cond is true
    * if not, exit

### ```while(cond=bool, callback=func)```
* repeat the following:
  * make sure cond is true
    * if not, exit
  * call callback

### ```until(cond=bool, callback=func)```
* repeat the following:
  * make sure cond is false
    * if not, exit
  * call callback

### ```for(init=func, cond=bool, step=func, callback=func)```
* call init
* repeat the following:
    * make sure cond is false
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
