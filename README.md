# TaskList

# Description

This is a task management app which supports three features: adding, listing, and deleting tasks. You can add a task with a given priority to the list 
(which is saved to mytask.dat) and it will be sorted in order from the lowest number to the highest. There is a list feature as well that simply prints the list 
out for the user to see. The delete feature just removes a task and resorts the list.

# Usage

$ todo add 10 clean bedroom 
$ todo list

>10 clean bedroom 1000

$ todo add 3 read
$ todo list

>3 read 1001
>10 clean bedroom 1000

$ todo delete 1000
$ todo list

>3 read 1001
