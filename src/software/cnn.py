import numpy as np
import os

from data_utils import PlayingCardsSet

if __name__ == "__main__":
    ds = PlayingCardsSet(os.path.join(os.path.dirname(os.path.abspath(__file__)),"../../Data/PlayingCards"))
    print(len(ds))
    for sample in ds:
        print(sample["label"], sample["filename"])