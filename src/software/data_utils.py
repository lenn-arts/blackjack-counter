import os
import torch
import pandas as pd
#from skimage import io, transform
import numpy as np
import matplotlib.pyplot as plt
from torch.utils.data import Dataset, DataLoader
from torchvision import transforms, utils
import PIL.Image
import numpy as np

def transform(input:np.ndarray):
    transform = transforms.Compose(
                [transforms.ToTensor(),
                 
                 transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
                 transforms.Resize((100,100))])
    image = transform(input)
    return image


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
        
        def get_label_from_filename_custom_set(fn):
            if len(fn.split("/")[-1]) < 12:
                number_map = {str(k):k-1 for k in list(range(1,14))}
                #number_map.update({"J":9,"Q":10,"K":11,"A":12})
                color_map = {"club":0,"diamond":1,"heart":2,"spade":3}
                card_name = fn.split("/")[-2]
                #print("CARD_NAME: ", card_name)
                if "club" in card_name:
                    color_str = "club"
                elif "diamond" in card_name:
                    color_str = "diamond"
                elif "heart" in card_name:
                    color_str = "heart"
                elif "spade" in card_name:
                    color_str = "spade"
                #number = fn[0] if fn[1] != "0" else "10"
                number = card_name[len(color_str):]
                label = color_map[color_str]*13+number_map[number]
            else: # joker
                label = 52
            return label

        self.transform = transform
        self.root_dir = root_dir
        self.img_dir = os.path.join(root_dir, "Images/Images/")
        self.img_dir = os.path.join(root_dir, "")
        self.dirs = sorted(os.listdir(self.img_dir))
        self.dirs = [mydir for mydir in self.dirs if "club" in mydir or "diamond" in mydir or "heart" in mydir or "spade" in mydir]
        print("self.dirs", self.dirs)
        self.ann_dir = os.path.join(root_dir, "Annotations/Annotations/")
        #self.filenames = sorted(os.listdir(self.img_dir))
        self.filenames = []
        for i, mydir in enumerate(self.dirs):
            to_append = sorted(os.listdir(os.path.join(self.img_dir, mydir)))
            to_append = [os.path.join(self.img_dir, mydir, img_fn) for img_fn in to_append if "bin" not in img_fn and "9" not in img_fn and "10" not in img_fn and "." not in img_fn]
            self.filenames.append(to_append)
        out = []
        for fns in self.filenames:
            out = out + fns
        self.filenames = out
        #print("filenames ",self.filenames)
        #self.labels = [get_label_from_filename(fn) for fn in self.filenames]
        self.labels = [get_label_from_filename_custom_set(fn) for fn in self.filenames]
        print(np.unique(self.labels))
        #print([f'{fn.split("/")[-2:]}\n' for fn in self.filenames])
        
        # generate train and test
        self.filenames_train = []
        self.len_train = 25
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
        elif ds_type is "all":
            self.filenames = self.filenames
        else: 
            raise Exception("unvalid dataset subset type")
        self.labels = [get_label_from_filename_custom_set(fn) for fn in self.filenames]
        
        print(len(self.filenames))
        #index_debug = torch.randint(len(self.filenames), size=(100,)).numpy()
        #print(index_debug)
        #self.filenames = np.asarray(self.filenames)[index_debug]
        #self.labels = np.asarray(self.labels)[index_debug]

        #print(self.filenames)

    def __len__(self):
        return len(self.filenames)
    
    def __getitem__(self, index):
        if torch.is_tensor(index):
            index = index.tolist()

        file_name = self.filenames[index]
        #print(file_name)
        #image = PIL.Image.open(os.path.join(self.img_dir, file_name)).convert("RGB")
        f = None        
        with open(file_name, 'r') as f:
            image = np.loadtxt(f) #[h, w, 3]
        
        image = image.reshape((480,640,3)).astype(np.uint8)
        #print("max, min", np.max(image), np.min(image))

        if self.transform:
            transform = transforms.Compose(
                [transforms.ToTensor(),
                 
                 transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5)),
                 transforms.Resize((100,100))])
            image = transform(image)

        if image.size()[0]==1:
            image=image.repeat([3,1,1])

        #print(image.shape, image.max(), image.min())
        sample = {'image': image, 'label': self.labels[index], 'filename': file_name}

        return sample


    def resize_save(self, target_dir):
      if not os.path.exists(target_dir):
        os.mkdir(target_dir)
      transform = transforms.Compose(
                [transforms.ToTensor(),
                 transforms.Resize((512,512)),
                 transforms.ToPILImage()])
      for fn in self.filenames:
        image = PIL.Image.open(os.path.join(self.img_dir, fn)).convert("RGB")        
        image = transform(image)
        if not os.path.exists(os.path.join(target_dir, fn)):
          image.save(os.path.join(target_dir, fn))  
      print("done saving resized images")

if __name__=="__main__":
    PlayingCardsSet("/Users/lennartschulze/Downloads/Embedded Systems/project/blackjack-counter/Data/CustomSet")