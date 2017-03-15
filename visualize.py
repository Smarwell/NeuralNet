
DISPLAY=False

import pygame

pygame.init()

import sys
from math import log,e


def iterator(data):
    for line in data:
        try:
            yield int(line)
        except:
            yield floats(line)

def floats(line):
    return [float(num) for num in line.split(' ')]

def colorize(weight):
    aweight=abs(weight)
    hue=int(min(log(aweight+1,e)*91.97,250))
    if weight < 0:
        return (250,250-hue,250-hue)
    else:
        return (250-hue,250,250-hue)

if len(sys.argv) == 1:
    filename=input("Enter a file to load:  ")
else:
    filename=sys.argv[1]

try:
    with open(filename,'r') as infile:
        data=iterator([line.strip() for line in infile.readlines() if line.strip()])
except:
    print("Failed to open file")
    raise
    exit()

input_nodes=next(data)
num_layers=next(data)
layers=[]
for layer in range(int(num_layers)):
    num_neurons=next(data)
    layers.append([])
    for num in range(int(num_neurons)):
        num_weights=next(data)
        layers[layer].append(next(data))
        if len(layers[layer][num])!=num_weights:
            print("File formatted incorrectly")
            exit()

output_layer=[]
output_nodes=next(data)
for i in range(int(output_nodes)):
    num_weights=next(data)
    output_layer.append(next(data))

spacing=75

if DISPLAY:
    screen=pygame.display.set_mode((int(3*spacing + spacing*num_layers),int(spacing+spacing*len(layers[0]))))
else:
    screen=pygame.Surface((int(3*spacing + spacing*num_layers),int(spacing+spacing*len(layers[0]))))

screen.fill((245,245,245))

input_node_pos=[]
x=spacing
y=int(screen.get_size()[1]/2-spacing*(input_nodes-1)/2)
for node in range(input_nodes):
    input_node_pos.append((x,y))
    y+=spacing

x+=spacing
y=0
node_pos=[[]]
for firstnode in layers[0]:
    y+=spacing
    node_pos[0].append((x,y))
    for num,weight in enumerate(firstnode):
        if not weight:
            continue
        pygame.draw.line(screen,colorize(weight),(x,y),input_node_pos[num],3)

for num, node in enumerate(input_node_pos):
    if num!=input_nodes-1:
        pygame.draw.line(screen,(150,150,150),node,(node[0]-spacing,node[1]),2)
    pygame.draw.circle(screen,(20,20,20),node,15)

if len(layers)>1:
    for layernum, layer in enumerate(layers):
        if layernum==0:
            continue
        x+=spacing
        node_pos.append([])
        y=0
        for nodenum, node in enumerate(layer):
            y+=spacing
            node_pos[layernum].append((x,y))
            for weightnum, weight in enumerate(node):
                if not weight:
                    continue
                pygame.draw.line(screen,colorize(weight),(x,y),node_pos[layernum-1][weightnum],3)

x+=spacing
y=int(screen.get_size()[1]/2-spacing*(output_nodes-1)/2)
for outnode in output_layer:
    for num,weight in enumerate(outnode):
        if not weight:
            continue
        pygame.draw.line(screen,colorize(weight),(x,y),node_pos[len(node_pos)-1][num],3)
    pygame.draw.line(screen,(150,150,150),(x,y),(x+spacing,y),2)
    pygame.draw.circle(screen,(20,20,20),(x,y),15)
    y+=spacing

for layer in node_pos:
    for node in layer:
        pygame.draw.circle(screen,(20,20,20),node,15)

pygame.image.save(screen,filename.split('.')[0]+".png")

if DISPLAY:
    pygame.display.set_caption(filename)
    
    clock=pygame.time.Clock()
    running=True
    while running:
        for event in pygame.event.get():
            if event.type==pygame.QUIT:
                running=False
        pygame.display.update()
        clock.tick(20)

pygame.quit()



























    
