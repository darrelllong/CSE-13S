#!/usr/bin/env python3

from heap import heap_sort
from insert import insertion_sort
from quick import quick_sort
from shell import shell_sort
from batcher import batcher_sort
import unittest, random

class TestSorts(unittest.TestCase):
    def setUp(self):
        self.sorts = [ insertion_sort, shell_sort, quick_sort, heap_sort, batcher_sort ]

    def assertSorted(self, A: list):
        for i in range(1, len(A)):
            self.assertTrue(A[i - 1] <= A[i])

    def test_sorts_zero(self):
        for sort in self.sorts:
            A = []
            sort(A)
            self.assertSorted(A)

    def test_sorts_one(self):
        for sort in self.sorts:
            A = [1]
            sort(A)
            self.assertSorted(A)

    def test_sorts_many(self):
        for sort in self.sorts:
            A = [random.randint(0, 1 << 24) for _ in range(0, 1000)]
            sort(A)
            self.assertSorted(A)

if __name__ == '__main__':
    unittest.main()
