import os
import torch
import pandas as pd
#from skimage import io, transform
import numpy as np
import matplotlib.pyplot as plt
from torch.utils.data import Dataset, DataLoader
from torchvision import transforms, utils
import PIL.Image

class PlayingCardsSet(Dataset):
    def __init__(self, root_dir, ds_type="train", transform=True) -> None:
        super().__init__()

        def get_label_from_filename(fn):
            if len(fn.split(".jpg")[0]) < 6:
                number_map = {str(k):k-2 for k in list(range(2,11))}
                number_map.update({"J":9,"Q":10,"K":11,"A":12})
                color_map = {"C":0,"D":1,"H":2,"S":3}
                number = fn[0] if fn[1] != "0" else "10"
                color = fn[1] if fn[1] != "0" else fn[2]
                label = number_map[number]+color_map[color]*13
            else: # joker
                label = 52
            return label

        self.transform = transform
        self.root_dir = root_dir
        self.img_dir = os.path.join(root_dir, "Images/Images/")
        self.ann_dir = os.path.join(root_dir, "Annotations/Annotations/")
        self.filenames = sorted(os.listdir(self.img_dir))
        self.labels = [get_label_from_filename(fn) for fn in self.filenames]

        # generate train and test
        self.filenames_train = []
        self.len_train = 40
        self.filenames_test = []
        current_label = self.labels[0]
        next_label = self.labels[0]
        i_it = 0 
        while i_it < len(self.filenames):
            counter = 0
            current_label = next_label
            while current_label == next_label:
                current_label = next_label
                if counter < self.len_train:
                    self.filenames_train.append(self.filenames[i_it])
                else:
                    self.filenames_test.append(self.filenames[i_it])
                counter += 1
                i_it += 1
                next_label = self.labels[i_it] if i_it < len(self.filenames) else "none"
        if ds_type is "train":
            self.filenames = self.filenames_train
        elif ds_type is "test":
            self.filenames = self.filenames_test
        else: 
            raise Exception("unvalid dataset subset type")
        self.labels = [get_label_from_filename(fn) for fn in self.filenames]
        

        print(self.filenames)

    def __len__(self):
        return len(self.filenames)
    
    def __getitem__(self, index):
        if torch.is_tensor(index):
            index = index.tolist()

        file_name = self.filenames[index]
        image = PIL.Image.open(os.path.join(self.img_dir, file_name)).convert("RGB")        

        if self.transform:
            transform = transforms.Compose(
                [transforms.ToTensor(),
                 transforms.Resize((400,400)),
                transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])
            image = transform(image)

        if image.size()[0]==1:
            image=image.repeat([3,1,1])

        sample = {'image': image, 'label': self.labels[index], 'filename': file_name}

        return sample