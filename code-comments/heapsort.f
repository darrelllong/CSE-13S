C BSD 2-Clause License
C
C Copyright (c) 1982, Darrell Long
C All rights reserved.
C
C Redistribution and use in source and binary forms, with or without
C modification, are permitted provided that the following conditions are met:
C
C 1. Redistributions of source code must retain the above copyright notice, this
C    list of conditions and the following disclaimer.
C
C 2. Redistributions in binary form must reproduce the above copyright notice,
C    this list of conditions and the following disclaimer in the documentation
C    and/or other materials provided with the distribution.
C
C THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
C AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
C IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
C DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
C FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
C DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
C SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
C CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
C OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
C OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


	subroutine HEAP_SORT(SEQUENCE, FIRST, LAST)
C
C Heap_Sort - Sort an array of items using a generalized heap-sort
C             technique.
	integer    SEQUENCE(*), FIRST, LAST
	integer    LEAF
	call BUILD_HEAP(SEQUENCE, FIRST, LAST)
	do LEAF = LAST, FIRST + 1, -1.
	   call SWAP(SEQUENCE(FIRST), SEQUENCE(LEAF))
	   call FIX_HEAP(SEQUENCE, FIRST, LEAF - 1)
	end do
	end

	subroutine BUILD_HEAP(SEQUENCE, FIRST, LAST)
C
C Build_Heap - Construct a heap from an array of items.
C
	integer   SEQUENCE(*), FIRST, LAST
	integer   FATHER
	do FATHER = (LAST / 2), FIRST, -1)
	   call FIX_HEAP(SEQUENCE, FATHER, LAST)
	end do
	end

	subroutine FIX_HEAP(SEQUENCE, FIRST, LAST)
C
C Fix_Heap - Repair a subheap.
C	integer    SEQUENCE(*), FIRST, LAST
	integer    FATHER, GREAT
	integer    MAX_CHILD
	logical    FOUND
	FOUND  = .false.
	FATHER = FIRST
	GREAT  = MAX_CHILD(SEQUENCE, FATHER, LAST)
	do while ((FATHER .le. (LAST / 2)) .and. .not. FOUND)
	   if (SEQUENCE(FATHER) .lt. SEQUENCE(GREAT)) then
	      call SWAP(SEQUENCE(FATHER), SEQUENCE(GREAT))
	      FATHER = GREAT
	      GREAT  = MAX_CHILD(SEQUENCE, FATHER, LAST)
	   else
	      FOUND = .true.
	   end if
	end do
	end

	integer function MAX_CHILD(SEQUENCE, FIRST, LAST)
C
C Max_Child - Find the largest of the two children of a node rooted at
C             FIRST.
C
	integer	SEQUENCE(*), FIRST, LAST
	integer LEFT, RIGHT
	LEFT  = FIRST * 2
	RIGHT = LEFT  + 1
	MAX_CHILD = LEFT
	if (RIGHT .le. LAST) then
	   if (SEQUENCE(RIGHT) .gt. SEQUENCE(LEFT)) then
	      MAX_CHILD = RIGHT
	   end if
	end if
	end

	subroutine SWAP(ARG_A, ARG_B)
C
C Swap - Swap two items.
C
	integer    ARG_A, ARG_B
	integer    TEMP
	TEMP  = ARG_A
	ARG_A = ARG_B
	ARG_B = TEMP
	end
