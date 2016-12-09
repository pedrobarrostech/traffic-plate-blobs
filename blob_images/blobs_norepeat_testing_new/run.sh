#!/bin/bash



rm -rf out.csv

for f in *; do 
	if [ ! -d $f ]; then
		$(../TrafficSign/neuralNetworkTests $f >> out.csv) 

	fi
done




