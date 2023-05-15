# Intel Core i9 AVX throttling
Processor Speed may appear below expected values when monitored through Task Manager or similar applications under periods of heavy load.
System fans may not appear to increase as CPU load is applied.
This is working as designed based on the architecture of the Intel i9 processor. 

AVX workloads use wider registers and require higher power to switch than standard 64-bit registers. 
This issue is most frequently seen when using benchmarking software such as Prime95 (Prime 95 by default relies on AVX workloads to apply stress).  
For more details:
[throttling](https://www.dell.com/support/kbdoc/it-it/000184687/intel-i9-processor-throttling-under-avx-advanced-vector-extensions){:target="_blank"}
