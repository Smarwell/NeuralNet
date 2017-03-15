
from ctypes import *
import os

NN=cdll.LoadLibrary("NNWrapper.dll")
NN.init.argtypes=[c_int,c_int,c_int,c_int,c_int,c_int]
NN.eval.argtypes=[c_int, POINTER(c_float)]
NN.eval.restype=POINTER(c_float)
NN.score_net.argtypes=[c_int, c_float]
NN.save_net.argtypes=[c_int, c_bool]
NN.load_nets.argtypes=[POINTER(c_int),c_int]

class NeuralNet:
    def __init__(self, num_inputs, layer_height, num_outputs, num_layers=1,
                 num_nets=30, survivors=5, load_nets=False):
        self.num_inputs=num_inputs
        self.layer_height=layer_height
        self.num_outputs=num_outputs
        self.num_layers=num_layers
        self.num_nets=num_nets
        self.survivors=survivors

        NN.init(num_inputs, num_layers, layer_height, num_outputs, num_nets, survivors)

        if load_nets:
            net_files=[]
            for file in os.listdir():
                try:
                    if file.endswith('.n'):
                        net_files.append(int(file[3:6]))
                except:
                    continue
            if net_files:
                NN.load_nets((c_int*len(net_files))(*net_files),len(net_files))
            print("Loaded in {0} existing nets".format(len(net_files)))

    def run(self, net, inputs):
        if len(inputs)>self.num_inputs:
            raise IndexError("%d inputs expected, %d recieved"%(self.num_inputs,len(inputs)))
        elif len(inputs)<self.num_inputs:
            inputs = inputs+[0.]*(self.num_inputs-len(inputs)) #pad inputs with zeros
        if net<0 or net>=self.num_nets:
            raise IndexError("Cannot access net %d"%net)
        arr = (c_float * len(inputs))(*inputs)
        result = NN.eval(net,arr)
        return [result[i] for i in range(self.num_outputs)]

    
    def score_net(self, net, score):
        if net<0 or net>=self.num_nets:
            raise IndexError("Cannot access net %d"%net)
        NN.score_net(net,c_float(score))

    def reproduce(self):
        NN.reproduce()

    def save(self, net, visualize=False):
        NN.save_net(net, visualize)

