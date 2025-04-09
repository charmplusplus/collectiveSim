# collectiveSim

A library for implementing collectives commonly used in machine learning tasks including allGather and allReduce, in simulation mode as well as regular use mode. 

## allGather

allGather lets you gather data distributed accross different chare array elements. The library provides 3 algorithms for doing the allGather operations, namely ring, hypercube and flooding.

### How to use

declare allGather as an extern module in your `.ci` file and include the `allGather.hh` header file in your `cc/hh` file. Then create an AllGather array object and bind it to your chare array. eg.

```C++
CkArrayOptions opts(n);
opts.bindTo(sim);
AllGather_array = CProxy_AllGather::ckNew(k, n, (int)allGatherType::ALL_GATHER_RING, opts);
```

Here n refers to the size of the chare array, k refers to the number of data elements present in each chare array element and the third parameter lets you choose the algorithm you want to run. The algorithms are:

```C++
enum allGatherType {
  ALL_GATHER_RING,
  ALL_GATHER_HYPERCUBE,
  ALL_GATHER_FLOODING
};
```

##### [NOTE]: `ALL_GATHER_HYPERCUBE` only works when n(size of chare array) is a power of 2.

You must also declare a callback to a function which the library can return to after its done and it must only take a pointer to `allGatherMsg` as its argument. To start operations, each chare array element must make a local pointer to the library chare array element bound to it and call init.

```C++
AllGather* libptr = AllGather_array({{CHARE_ARRAY_INDEX}}).ckLocal();
libptr->init(result, data, cb);
```
Here, result is a pointer to where the user wants the result of allGather operation to be stored(note that it must be of size n * k), data refers to per chare array element contributed data and cb refers to the callback.

Once the library is done, it will send an empty message (a kick if you will) telling the user that the result is now available in the destination that the user specified earlier.

### **Scaling the communication graph (fixed data: 40 bytes)**

#### 1 Node, 128 PEs
| Communication Graph | Trial 1   | Trial 2   | Trial 3   | Trial 4   | Average     |
|---------------------|-----------|-----------|-----------|-----------|-------------|
| Ring                | 0.199741  | 0.150521  | 0.183364  | 0.280327  | 0.20348825  |
| Hypercube           | 0.026346  | 0.039749  | 0.02717   | 0.027799  | 0.030266    |

#### 2 Nodes, 128 PEs each
| Communication Graph | Trial 1   | Trial 2   | Trial 3   | Trial 4   | Average     |
|---------------------|-----------|-----------|-----------|-----------|-------------|
| Ring                | 1.243352  | 0.912825  | 1.359307  | 0.833105  | 1.08714725  |
| Hypercube           | 0.480072  | 0.354723  | 0.302094  | 0.677252  | 0.45353525  |

#### 4 Nodes, 128 PEs each
| Communication Graph | Trial 1   | Trial 2   | Trial 3   | Trial 4   | Average     |
|---------------------|-----------|-----------|-----------|-----------|-------------|
| Ring                | 2.311041  | 2.53261   | 3.680528  | 2.596037  | 2.780054    |
| Hypercube           | 0.878991  | 0.793332  | 0.799686  | 1.024804  | 0.87420325  |

---

### **Scaling the amount of data (fixed topology: 1 Node, 128 PEs)**

#### Ring
| Data Size           | Trial 1   | Trial 2   | Trial 3   | Trial 4   | Average     |
|---------------------|-----------|-----------|-----------|-----------|-------------|
| 500×4               | 0.205591  | 0.158634  | 0.133589  | 0.421874  | 0.229922    |
| 1000×4              | 0.184694  | 0.293661  | 0.214799  | 0.149986  | 0.210785    |
| 2000×4              | 0.149573  | 0.222398  | 0.154197  | 0.285809  | 0.20299425  |
| 10000×4             | 0.058948  | 0.073159  | 0.067228  | 0.129617  | 0.082238    |
| 100000×4            | 0.298904  | 0.285255  | 0.280117  | 0.275075  | 0.28483775  |

#### Hypercube
| Data Size           | Trial 1   | Trial 2   | Trial 3   | Trial 4   | Average     |
|---------------------|-----------|-----------|-----------|-----------|-------------|
| 500×4               | 0.036408  | 0.037173  | 0.03821   | 0.036537  | 0.037082    |
| 10000×4             | 0.031082  | 0.030288  | 0.029472  | 0.037486  | 0.032082    |
| 100000×4            | 0.235187  | 0.228271  | 0.225347  | 0.240304  | 0.23227725  |
