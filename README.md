# IPI Bench

Benchmark IPI performance on Linux Kernel using `on_each_cpu`

## How to use

```bash
make
sudo insmod ipibench.ko
# Test for nr_cpu*10**5 times
echo 100000 | sudo tee /sys/kernel/ipibench/status
cat /sys/kernel/ipibench/status
```

You will get result like this:

```console
32 of CPUs online.
IPI takes 204225607 ktime, 204225607 ns
```