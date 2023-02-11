#!/bin/bash
## Creating SPIR-V binary file
## -o: output file
/usr/local/bin/glslc shader_base.vert -o base_vert.spv
/usr/local/bin/glslc shader_base.frag -o base_frag.spv

sleep 30s
