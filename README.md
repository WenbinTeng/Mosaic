# Mosaic

**Mosaic** is a research framework for **fine-grained FPGA virtualization** based on **nested dynamic partial reconfiguration**. It enables flexible, scalable, and efficient sharing of FPGA resources among multiple applications.

---

### Overview

Modern FPGA cloud platforms often suffer from low resource utilization due to coarse-grained partitioning and long reconfiguration overhead. Mosaic addresses these challenges by introducing:

- Hierarchical resource management: based on a tree-strutted allocator.
- Separate stream transmission: through a on-chip fat-tree structure.
- Compilation time trade-off: through dual-level paging optimization.

Mosaic serves as a foundation for exploring FPGA multi-tenancy, serverless acceleration, and runtime resource optimization.

---

### How to Use

##### For tenants

1. Develop your hardware accelerator kernels using Xilinx Vitis HLS. For optimal performance, use AXI-Stream interfaces for inter-kernel communication and AXI-Memory-Mapped interfaces for memory access.
2. Compile the designed kernels in Xilinx Vitis HLS and export RTL.
3. Submit the exported RTL IPs and task graph definition to the vendor for integration into the framework.

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

### License

This project is released under the **MIT License**. See the LICENSE file for details.

