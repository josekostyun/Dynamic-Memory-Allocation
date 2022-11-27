# Dynamic-Memory-Allocation
 Program to estimate how many monsters of each type trainers can expect to capture in each region using dynamic memory allocation, pointers, and pointer arrays.

Several small monster trainers have come to you for advice regarding expeditions they're planning into various regions.
You are writing a program to estimate how many monsters of each type they can expect to capture in each region.


• You've got a Small Monster Index that tells you the name, type, and relative commonality of all the small
monsters in question.

    o(A monster’s absolute commonality is the same in each region. A monster’s relative commonality will
      change region by region as calculations are performed – we’ll show you how that works shortly.)

• You've also got an atlas that tells you about the relevant regions and which small monsters are present in them.

• Each trainer tells you which regions they're visiting, and how many monsters they intend to capture per region.

• To estimate the number of a given monster M a trainer will capture in a region R:
    
    o Divide the relative population of M in R by R's total relative population.
    
    o Multiply the result by the total number of captures the trainer intends per region.
    
    o Round this result to the nearest integer. .5 rounds up, so you can use round() and its friends. Note
      that this can result in a total slightly different than the trainer intended!
