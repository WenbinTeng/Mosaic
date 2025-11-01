# Mosaic

**Mosaic** is a research framework for **fine-grained FPGA virtualization** based on **nested dynamic partial reconfiguration**. It enables flexible, scalable, and efficient sharing of FPGA resources among multiple applications.

---

### Overview

Modern FPGA cloud platforms often suffer from low resource utilization due to coarse-grained partitioning and long reconfiguration overhead. Mosaic addresses these challenges by introducing:

- **Hierarchical** reconfigurable resource management: based on a tree-strutted allocator.
- Separate stream transmission: through a on-chip fat-tree structure.
- Compilation time trade-off: through dual-level paging optimization.

Mosaic serves as a foundation for exploring FPGA multi-tenancy, serverless acceleration, and runtime resource optimization.

---

### How to Use

##### For tenants

1. Develop your hardware accelerator kernels using Xilinx Vitis HLS. 

    - Use AXI-Stream interfaces for kernel-to-kernel communication. In HLS, we can use the INTERFACE pragma to bind an interface and implement it with stream FIFO.

      ```c
      #pragma HLS interface mode=axis port=<name> [OPTIONS]
      ```

   - Use AXI-Memory-Mapped interfaces for memory access. Likewise, in HLS, we can bind an interface to AXI using the INTERFACE pragma.

     ```c
     #pragma HLS interface mode=m_axi port=<name> offset=slave bundle=data [OPTIONS]
     ```

2. Synthesize and export the kernels as RTL IP cores through Vitis HLS.
3. Prepare a task graph description (e.g., in JSON or YAML) that defines the dataflow dependencies among kernels, including:
   - Kernel file
   - Batch size
   - Post-synthesized resource estimation
   - Dependency list for every kernel
   - ...
4. Submit the exported IPs and task graph to the vendor for integration into the Tetris framework.

##### For vendors

1. Generate wrappers for user HLS-designed kernels:

```bash
# for single file
python utility/wrapper/gen_wrapper.py \
	--config <yaml file> \
	--output <output path>

# for directory
bash scripts/auto_wrap.sh <config firectory> <output directory>
```

2. Generate latency-insensitive interface router used in micro-service region:

```bash
python utility/router/gen_router.py \
	--N_IN <number of LII input> \
	--N_OUT <number of LII output> \
	--output <output path>
```

3. Generate memory agent used in top service region:

```bash
python utility/memory-agent/gen_ma.py \
	--output <output path>
```

4. Generate memory management unit in top service region:

```bash
python utility/memory-management-unit/gen_mmu.py \
	--output <output path>
```

5. Generate tcl scripts for Xilinx Dynamic Function eXchange (DFX) flow:

```bash
python host/dfx/gen_tcl.py
```

6. Submit user application using scheduler's API.

---

### Experimental Environment

Software

- Windows 11
- AMD Vivado Design Suite 2023.2
- AMD Vitis HLS 2023.2
- AMD Vitis 2023.2

Hardware

- AMD Ryzen 9 5900X with 64GB RAM
- AMD Zynq UltraScale+ MPSoC ZCU102 Evaluation Kit

---

### License

This project is released under the **MIT License**. See the LICENSE file for details.

