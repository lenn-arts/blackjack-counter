import numpy as np
import os

import torch
import torch.nn as nn
import torch.nn.functional as F

from data_utils import PlayingCardsSet

class Cnn(nn.Module):
    def __init__(self) -> None:
        super().__init__()
        act = nn.ReLU()
        self.layers = []
        self.layers.append(nn.Conv2d(3, 10, 30))
        self.layers.append(nn.MaxPool2d(2, 2))
        #self.layers.append(act)
        self.layers.append(nn.Conv2d(10, 30, 20))
        self.layers.append(nn.MaxPool2d(2, 2))
        #self.layers.append(act)
        self.layers.append(nn.Conv2d(30, 600, 10))
        self.layers.append(nn.MaxPool2d(2, 2))
        #self.layers.append(act)
        self.layers.append(nn.Linear(68450,1000))
        self.layers.append(act)
        self.layers.append(nn.Linear(1000,200))
        self.layers.append(act)
        self.layers.append(nn.Linear(200,100))
        self.layers.append(act)
        self.layers.append(nn.Linear(100,53))
        self.layers = nn.ModuleList(self.layers)
        print(self)

    def forward(self, input):
        output = input
        first_linear = False
        for i_layer, layer in enumerate(self.layers):
            if first_linear is False and isinstance(layer, nn.Linear):
                output = torch.flatten(output, 1)
                first_linear = True
            output = layer(output)
        return output

def train():
    src_path = os.path.join(os.path.dirname(os.path.abspath(__file__)),"../../Data/PlayingCards")
    datasets = {}
    datasets["train"] = PlayingCardsSet(src_path, "train")
    datasets["test"] = PlayingCardsSet(src_path, "test")
    print([len(dsi) for dsi in datasets.values()])

    dataloaders = {}
    batch_size = 8
    dataloaders["train"] = torch.utils.data.DataLoader(datasets["train"], batch_size=batch_size,
                                         shuffle=True, num_workers=4)
    dataloaders["test"] = torch.utils.data.DataLoader(datasets["test"], batch_size=batch_size,
                                         shuffle=False, num_workers=4)
    #print(next(iter(dataloaders["train"]))["image"].shape)

    model = Cnn()
    #print([param.shape for param in model.parameters()])

    loss_criterion = nn.CrossEntropyLoss() # includes softmax on output of the network
    optimizer = torch.optim.SGD(model.parameters(), lr=0.0001, momentum=0.9)

    num_epochs = 5
    for epoch in range(num_epochs):
        running_loss = 0.0
        running_acc = 0.0
        for i_batch, batch in enumerate(dataloaders["train"]):
            # get batch data
            gt_labels = batch["label"]
            print(gt_labels)
            
            # reset gradients to zero
            optimizer.zero_grad()

            # forward (get prediction)
            pred = model(batch["image"])
            print(model)
            pred_labels = torch.argmax(torch.softmax(pred, -1), -1)
            print(pred_labels)
            # backward (get gradients)
            acc = (pred_labels == gt_labels).double().mean()
            loss = loss_criterion(pred, gt_labels)
            loss.backward()
            # optimize weights according to gradients
            optimizer.step()

            running_loss+=loss
            running_acc+=acc
            print(f"epoch {epoch}, batch {i_batch}: loss = {loss}, acc={acc}")
            if i_batch % 5 == 4:
                print(f"epoch {epoch}, batch {i_batch}: avg loss per batch = {running_loss/5.0}, avg acc = {running_acc/5.0}")
                running_loss = 0.0
                running_acc=0.0

if __name__ == "__main__":
    train()