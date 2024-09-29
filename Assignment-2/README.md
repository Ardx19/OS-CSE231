# Assignment-2

# Contributors:
    Aryan Dutt   - 2023143
    Aryan Dahiya - 2023142

# q1.c
    No dependencies or assumptions were used in this code

# q2.c
    Assumtions:
    The segment numbers (12-15) in the Virtual Address given are used to map to Code, Heap and Stack
    as no specific mapping of segment numbers was mentioned in the question. There's no universal rule 
    that segment numbers dircetly map to code,heap or stack but it was a mere assumption.
    Segment Numbers [0,5] map to Code segment
    Segment Numbers [6,10] map to Heap segment
    Segment Numbers [11,15] map to Stack segment
    (both inclusive)

# q3.c
    Assumptions:
    mapping() vpn->pfn : as vpn is the 4 MSB , toggle bit at second plcace, left shift by 2(x2^2),
                         add 13 and modulo by 64.

    pageTablemaker() : initialising page table entries in the page table array using the mapping() function

    TLB(): checks if vpn is present in tlbVpn[] if yes then returns index , else returns -1

    MMU(): gets vpn and offset by bit manipulation checks if offset is in bounds (i.e <16*1024),
            gets id of TLB() , returns pfn from pageTable if not found in TLB. Continues to update TLB

    updateTLB(): if not founcd in tlb then replaces the least recently used vpn and pfn, else updates tlbID 
                 if its the same as the index fetched

    main(): calls pageTablemaker() to initialize page table, takes in virtual addresses in an infinite loop
            unless encountered by an invalid input

# Contact Us
    aryan23143@iiitd.ac.in
    aryan23143@iiitd.ac.in