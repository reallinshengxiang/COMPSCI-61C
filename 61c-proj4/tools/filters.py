import numpy as np

from framework import Matrix, FILTER_MULTIPLIER


def create_gaussian_filter(size: int = 5, sigma: float = 2.0) -> np.ndarray:
    # Based on https://medium.com/@akumar5/computer-vision-gaussian-filter-from-scratch-b485837b6e09
    gaussian_filter = np.zeros((size, size), dtype=np.float32)
    m = size // 2
    n = size // 2

    for x in range(-m, m + 1):
        for y in range(-n, n + 1):
            x1 = 2 * np.pi * (sigma ** 2)
            x2 = np.exp(-(x ** 2 + y ** 2)/(2 * sigma ** 2))
            gaussian_filter[x + m, y + n] = (1 / x1) * x2
    return gaussian_filter / np.sum(gaussian_filter)


class BlurFilter:
    @staticmethod
    def create(size: int = 17, sigma: float = 7) -> Matrix:
        assert size % 2 == 1, "Size must be odd"
        gaussian_filter = create_gaussian_filter(size, sigma)
        return Matrix(size, size, list((gaussian_filter.flatten() * FILTER_MULTIPLIER).astype(np.uint32)))


class SharpenFilter:
    @staticmethod
    def create(size: int = 3, sigma: float = 1) -> Matrix:
        assert size % 2 == 1, "Size must be odd"
        gaussian_filter = create_gaussian_filter(size, sigma)
        gaussian_filter = gaussian_filter * -1
        gaussian_filter[size // 2, size //
                        2] = gaussian_filter[size // 2, size // 2] + 2

        return Matrix(size, size, list((gaussian_filter.flatten() * FILTER_MULTIPLIER).astype(np.uint32)))
