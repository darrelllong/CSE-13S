import unittest, random

def insertion_sort(A: list):
    for i in range(1, len(A)):
        j = i
        temp = A[i]
        while j > 0 and temp < A[j - 1]:
            A[j] = A[j - 1]
            j -= 1
        A[j] = temp

class TestSort(unittest.TestCase):
    def assertSorted(self, A: list):
        for i in range(1, len(A)):
            self.assertTrue(A[i - 1], A[i])

    def test_sorts_zero(self):
        A = []
        insertion_sort(A)
        self.assertSorted(A)

    def test_sorts_one(self):
        A = [1]
        insertion_sort(A)
        self.assertSorted(A)

    def test_sorts_many(self):
        A = [random.randint(0, 1 << 24) for _ in range(0, 1000)]
        insertion_sort(A)
        self.assertSorted(A)

if __name__ == '__main__':
    unittest.main()
