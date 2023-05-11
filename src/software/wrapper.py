import ctypes
import numpy as np
import os
from data_utils import transform
from cnn import Cnn
import torch

if __name__ == "__main__":
    pass
    mylib = ctypes.CDLL(os.path.join(os.path.dirname(os.path.abspath(__file__)),'test.so'))
    cnn = Cnn()
    checkpoint = torch.load("model.pth")
    cnn.load_state_dict(checkpoint['model_state_dict'])
    cnn.eval()
    # for loop begin
    # read from c file
    mylib.main()

    with open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'rgb888img0'), 'r') as f:
        pixel_values = np.loadtxt(f)
    # postprocess
    in_tensor = transform(pixel_values)

    # CNN
    pred = cnn(in_tensor)
    score = pred
    print("score:", score)

    # send to c file
    #mylib.write_to_hex(score)

    # for loop end