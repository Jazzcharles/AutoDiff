# AutoDiff
Auto differentiation with cpp

## Introduction
This is a toy example of auto differentiation in cpp. The basic idea is to build a static computation graph including all the extra nodes for calculating the gradients.
Basically, what we need are:
1) class Node, the nodes in the computation graph, including the input nodes, the operation type, ...
2) class Operation, to perform the detailed forward and backward computation.

## Supported operations:
1) Addition
2) Multiplication








Reference:
1. CSE599W: http://dlsys.cs.washington.edu/
