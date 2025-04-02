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
