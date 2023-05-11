import numpy as np
import os
from datetime import date

import torch
import torch.nn as nn
import torch.nn.functional as F

from data_utils import PlayingCardsSet

class Cnn(nn.Module):
    def __init__(self) -> None:
        super().__init__()
        #act = 
        self.layers = []
        self.layers.append(nn.Conv2d(3, 32, 3, stride=2))
        self.layers.append(nn.ReLU())
        self.layers.append(nn.MaxPool2d(2, 2))
        self.layers.append(nn.Conv2d(32, 128, 3, stride=2))
        self.layers.append(nn.ReLU())
        self.layers.append(nn.MaxPool2d(2, 2))
        self.layers.append(nn.Conv2d(64, 256, 3, padding=1, stride=1))
        self.layers.append(nn.ReLU())
        #self.layers.append(nn.MaxPool2d(2, 2))
        #self.layers.append(nn.Conv2d(128, 256, 3, padding=1))
        #self.layers.append(act)

        self.layers.append(nn.Linear(256*5*5,1000))
        self.layers.append(nn.ReLU())
        #self.layers.append(nn.Linear(8000,1000))
        #self.layers.append(act)
        self.layers.append(nn.Linear(1000,200))
        self.layers.append(nn.ReLU())
        self.layers.append(nn.Linear(200,53))
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
            #print(output.shape)
            #if not isinstance(layer, nn.MaxPool2d) and i_layer < len(self.layers):
            #    output = torch.nn.functional.relu(output)
        return output

def train(model, src_path, device):
    src_path = src_path#os.path.join(os.path.dirname(os.path.abspath(__file__)),"../../Data/PlayingCards")
    datasets = {}
    datasets["train"] = PlayingCardsSet(src_path, "train")
    datasets["test"] = PlayingCardsSet(src_path, "test")
    print([len(dsi) for dsi in datasets.values()])

    dataloaders = {}
    batch_size = 16
    dataloaders["train"] = torch.utils.data.DataLoader(datasets["train"], batch_size=batch_size,
                                         shuffle=True, num_workers=0)
    dataloaders["test"] = torch.utils.data.DataLoader(datasets["test"], batch_size=batch_size,
                                         shuffle=False, num_workers=0)
    #print(next(iter(dataloaders["train"]))["image"].shape)

    loss_criterion = nn.CrossEntropyLoss() # includes softmax on output of the network
    optimizer = torch.optim.Adam(model.parameters(), lr=0.0005) #0.000005

    num_epochs = 10
    for epoch in range(num_epochs):
        running_loss = 0.0
        running_acc = 0.0
        for i_batch, batch in enumerate(dataloaders["train"]):
            # get batch data
            gt_labels = batch["label"].to(device)
            images = batch["image"].to(device)
            print("gt\t", gt_labels)
            print([batch["filename"][i].split("/")[-2:] for i in range(len(batch["filename"]))])
            
            # reset gradients to zero
            optimizer.zero_grad()

            # forward (get prediction)
            pred = model(images)
            pred_labels = torch.argmax(torch.softmax(pred, -1), -1)
            #print("pred\t", pred_labels)
            # backward (get gradients)
            acc = (pred_labels == gt_labels).detach().double().mean()
            loss = loss_criterion(pred, gt_labels)
            loss.backward()
            # optimize weights according to gradients
            optimizer.step()

            running_loss+=loss.detach()
            running_acc+=acc.detach()
            print(f"epoch {epoch}, batch {i_batch}: loss = {torch.round(loss.detach(),decimals=3)}, acc={torch.round(acc.detach(),decimals=3)}")
            if i_batch % 5 == 4:
                print(f"epoch {epoch}, batch {i_batch}: avg loss per batch = {torch.round(running_loss/5.0, decimals=3)}, avg acc = {torch.round(running_acc/5.0, decimals=3)}")
                running_loss = 0.0
                running_acc=0.0

        if epoch % 1 == 0:
          torch.save({
            'epoch': epoch,
            'model_state_dict': model.state_dict(),
            'optimizer_state_dict': optimizer.state_dict(),
            'loss': loss,
            }, os.path.join(src_path, f"{date.today()}_model_e{epoch}_b{i_batch}.pth"))

    return model


def test(model, src_path, device):
    src_path = src_path#os.path.join(os.path.dirname(os.path.abspath(__file__)),"../../Data/PlayingCards")
    datasets = {}
    datasets["test"] = PlayingCardsSet(src_path, "test")
    print([len(dsi) for dsi in datasets.values()])

    dataloaders = {}
    batch_size = 32
    dataloaders["test"] = torch.utils.data.DataLoader(datasets["test"], batch_size=batch_size,
                                         shuffle=False, num_workers=1)
    running_acc = 0.0
    counter = 0
    for i_batch, batch in enumerate(dataloaders["test"]):
        imgs = batch["image"].to(device)
        gt_labels = batch["label"].to(device)
        pred = model(imgs)
        pred_labels = torch.argmax(torch.softmax(pred, -1), -1)
        acc = (pred_labels == gt_labels).double().mean()
        running_acc += acc
        counter += 1
    test_accuracy = running_acc/counter
    print(f"TEST ACCURACY: {test_accuracy}")

if __name__ == "__main__":
    device = "cuda" if torch.cuda.is_available() else "cpu"
    src_path = os.path.join(os.path.dirname(os.path.abspath(__file__)),"../../Data/CustomSet")
    model = Cnn()
    train(model, src_path, device)
    test(model, src_path, device)