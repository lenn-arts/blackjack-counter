import numpy as np
import os
from datetime import date

import torch
import torch.nn as nn
import torch.nn.functional as F
from data_utils import transform
import matplotlib.pyplot as plt



from data_utils import PlayingCardsSet

class Cnn(nn.Module):
    def __init__(self) -> None:
        super().__init__()
        #act = 
        self.layers = []
        self.layers.append(nn.Conv2d(3, 32, 5, stride=1))
        self.layers.append(nn.ReLU())
        self.layers.append(nn.MaxPool2d(3, 3))
        self.layers.append(nn.Conv2d(32, 128, 3, stride=1))
        self.layers.append(nn.ReLU())
        self.layers.append(nn.MaxPool2d(2, 2))
        self.layers.append(nn.Conv2d(128, 256, 3, padding=1, stride=1))
        self.layers.append(nn.ReLU())
        self.layers.append(nn.MaxPool2d(2, 2))
        #self.layers.append(nn.MaxPool2d(2, 2))
        #self.layers.append(nn.Conv2d(128, 256, 3, padding=1))
        #self.layers.append(act)

        self.layers.append(nn.Linear(256*5*5,1000))
        self.layers.append(nn.ReLU())
        #self.layers.append(nn.Linear(8000,1000))
        #self.layers.append(act)
        self.layers.append(nn.Linear(1000,100))
        self.layers.append(nn.ReLU())
        self.layers.append(nn.Linear(100,13))
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
            #print(i_layer, output.shape)
            #if not isinstance(layer, nn.MaxPool2d) and i_layer < len(self.layers):
            #    output = torch.nn.functional.relu(output)
        return output

def train(model, src_path, device, chkpt=None):
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
    optimizer = torch.optim.Adam(model.parameters(), lr=0.0001) #0.000005

    if chkpt is not None:
        checkpoint = torch.load(chkpt)
        model.load_state_dict(checkpoint['model_state_dict'])
        optimizer.load_state_dict(checkpoint['optimizer_state_dict'])
        epoch = checkpoint['epoch']
        loss = checkpoint['loss']
        model.train()

    num_epochs = 20
    accuracy_list = []
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
            accuracy_list.append(loss.detach())
            print(f"epoch {epoch}, batch {i_batch}: loss = {torch.round(loss.detach(),decimals=3)}, acc={torch.round(acc.detach(),decimals=3)}")
            if i_batch % 5 == 4:
                print(f"epoch {epoch}, batch {i_batch}: avg loss per batch = {torch.round(running_loss/5.0, decimals=3)}, avg acc = {torch.round(running_acc/5.0, decimals=3)}")
                running_loss = 0.0
                running_acc=0.0

        if epoch % 1 == 0:
          save_img(accuracy_list, epoch)
          torch.save({
            'epoch': epoch,
            'model_state_dict': model.state_dict(),
            'optimizer_state_dict': optimizer.state_dict(),
            'loss': loss,
            }, os.path.join(src_path, f"{date.today()}_model_e{epoch}_b{i_batch}.pth"))

    return model

def save_img(input, j):
    fig_size = (15,7)
    figure, ax = plt.subplots(figsize=fig_size)
    ax.plot(input)
    ax.set_ylim(bottom=0.0)

    ax.set_xlabel('Iteration')
    ax.set_ylabel('Batch loss')
    ax.set_title('Training chart')
    #ax.legend()
    ax.grid()
    plt.savefig(os.path.dirname(__file__)+f"/test_{j}.png")
    plt.close()
    #else:
    #    plt.show()

def test(model, src_path, device, chkpt=None):
    src_path = src_path#os.path.join(os.path.dirname(os.path.abspath(__file__)),"../../Data/PlayingCards")
    datasets = {}
    datasets["test"] = PlayingCardsSet(src_path, "test")
    print([len(dsi) for dsi in datasets.values()])

    if chkpt is not None:
        checkpoint = torch.load(chkpt)
        model.load_state_dict(checkpoint['model_state_dict'])
        model.eval()

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
    chkpt_path = os.path.join(os.path.dirname(os.path.abspath(__file__)),"model.pth")
    
    #train(model, src_path, device, chkpt=chkpt_path)
    #test(model, src_path, device, chkpt=chkpt_path)

    checkpoint = torch.load(chkpt_path)
    model.load_state_dict(checkpoint['model_state_dict'])
    with open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'rgb888img'), 'r') as f:
        pixel_values = np.loadtxt(f)
    pixel_values = pixel_values.reshape(640,480,3).astype(np.uint8)
    # postprocess
    in_tensor = transform(pixel_values)
    in_tensor = torch.unsqueeze(in_tensor,0)
    print("my shape",in_tensor.shape)
    out = model(in_tensor)
    pred = torch.argmax(torch.softmax(out, -1), -1).detach().item()
    #color_map = {"club":0,"diamond":1,"heart":2,"spade":3}
    #color_map_inverse = {val:str(key) for key, val in color_map.items()}
    print("output:", pred)
    pred_map = {0: "ace", 1:2, 2:3, 3:4, 4:5, 5:6, 6:7, 7:8, 8:9, 9:10, 10:"jack", 11:"queen",12:"king"}
    point_map = {0: -1, 1: 1, 2:1, 3:1, 4:1, 5:1, 6:0, 7:0, 8:0, 9:-1, 10:-1, 11: -1, 12:-1}
    # add +1 : card 2-6
    # nothing: 7-9
    # subtract -1: 10-ace
    with open(os.path.join(os.path.dirname(os.path.abspath(__file__)),'output.txt'), 'a') as f:
        f.write(f"the current card is a {pred_map[pred]}, add {point_map[pred]} to score.")
    #
    
