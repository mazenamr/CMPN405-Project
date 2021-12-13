package cmpn405_project;
@license(LGPL);
//
// TODO documentation
//

import ned.IdealChannel;

network Network
{
    parameters:
        int n = default(6);
    submodules:
        nodes[n]: Node;
        coordinator: Coordinator;
    connections:
        for i=0..sizeof(nodes)-1, if i%2 == 0
        {
            nodes[i].out --> IdealChannel --> nodes[i+1].in;
            nodes[i+1].out --> IdealChannel --> nodes[i].in;
            nodes[i].coord <-- IdealChannel <-- coordinator.outs++;
            nodes[i+1].coord <-- IdealChannel <-- coordinator.outs++;            
        }
}

