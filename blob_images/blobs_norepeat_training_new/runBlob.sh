#!/bin/bash


for f in *; do 
	if [ ! -d $f ]; then
		$(../TrafficSign/neuralNetworkTests $f) 

	fi
done




