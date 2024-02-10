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

## Test Result


| SoC / CPU                        | Kernel                           | 1e5 timespent  | Note |
| -------------------------------- | -------------------------------- | -------------- | - |
| Intel Core i9 13900K             | Debian 6.6.11-1 (2024-01-14)     | 203409168 ns   | |
| StarFive VisionFive2(Sifive U74) | Linux v6.8-rc2 + defconfig       | 454372218 ns   | Mainline OpenSBI v1.3 |
| LicheePi 4A TH1520               | [T-Head Linux SDK V1.4.2](https://github.com/revyos/thead-kernel/tree/Linux_SDK_V1.4.2) | 1311311167 ns | Mainline OpenSBI v1.3 |
| LicheePi 4A TH1520               | [th1520-v6.7.2](https://github.com/revyos/th1520-linux-kernel/tree/f5b4176686a0176f6e8a71e940e677bf2a58fac5) | 1697927600 ns | Mainline OpenSBI v1.3 |
| Milk-V Pioneer(SG2042)(1 cluster) | [a7e8fb0](https://github.com/cyyself/linux/tree/a7e8fb064dde871d43994a5f71b9c5a2d3a483a5) | 933509238 ns | [Modified OpenSBI](https://github.com/cyyself/opensbi/tree/ccc90c64f512f4d77e81e5213bd79612acad25a2) without sfence.vma errata fix |
| Milk-V Pioneer(SG2042)           | [a7e8fb0](https://github.com/cyyself/linux/tree/a7e8fb064dde871d43994a5f71b9c5a2d3a483a5) | 14843102479 ns | [Modified OpenSBI](https://github.com/cyyself/opensbi/tree/ccc90c64f512f4d77e81e5213bd79612acad25a2) without sfence.vma errata fix |