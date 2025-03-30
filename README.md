# collectiveSim

A library for implementing collectives commonly used in machine learning tasks including allGather and allReduce, in simulation mode as well as regular use mode. 

## allGather

allGather lets you gather data distributed accross different chare array elements. The library provides 3 algorithms for doing the allGather operations, namely naive, hypercube and flooding. 

### How to use

declare allGather as an extern module in your `.ci` file and include the `allGather.hh` header file in your `cc/hh` file. Then create an AllGather array object and bind it to your chare array. eg.

```C++
CkArrayOptions opts(n);
opts.bindTo(sim);
AllGather_array = CProxy_AllGather::ckNew(k, n, (int)allGatherType::ALL_GATHER_DEFAULT, opts);
```

Here n refers to the size of the chare array, k refers to the number of data elements present in each chare array element and the third parameter lets you choose the algorithm you want to run. The algorithms are:

```C++
enum allGatherType {
  ALL_GATHER_DEFAULT,
  ALL_GATHER_HYPERCUBE,
  ALL_GATHER_FLOODING
};
```

##### [NOTE]: `ALL_GATHER_HYPERCUBE` only works when n(size of chare array) is a power of 2.

You must also declare a callback to a function which the library can return to after its done and it must only take a pointer to `allGatherMsg` as its argument. To start operations, each chare array element must call startGather:

```C++
AllGather_array({{CHARE_ARRAY_INDEX}}).startGather(data, k, cb);
void done(allGatherMsg *msg);
```

Here data refers to per chare array element data and cb refers to the callback. 

Once the library is done, it will call the callback function and give it the gathered data as an allGatherMsg. To extract data from it, simply call get_data and the data will always have a size of `n X k`. The structure of `allGatherMsg` is:

```C++
class allGatherMsg : public CMessage_allGatherMsg {
private:
  long int *data;
public:
  long int *get_data();
  allGatherMsg(long int *d);
};
```
