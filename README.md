# Tetris

**Tetris** is a research framework for **fine-grained FPGA virtualization** based on **nested dynamic partial reconfiguration**. It enables flexible, scalable, and efficient sharing of FPGA resources among multiple applications.

---

### Overview

Modern FPGA cloud platforms often suffer from low resource utilization due to coarse-grained partitioning and long reconfiguration overhead. Tetris addresses these challenges by introducing:

- **Hierarchical** reconfigurable resource management: based on a tree-strutted allocator.
- Separate stream transmission: through a on-chip fat-tree structure.
- Compilation time trade-off: through dual-level paging optimization.

Tetris serves as a foundation for exploring FPGA multi-tenancy, serverless acceleration, and runtime resource optimization.

---

### How to Use

##### For tenants

1. Develop your hardware accelerator kernels using Xilinx Vitis HLS. 

   - Use AXI-Stream interfaces for kernel-to-kernel communication. In HLS, we can use the INTERFACE pragma to bind an interface and implement it with stream FIFO.

     ```c++
     #pragma HLS interface mode=axis port=<name> [OPTIONS]
     ```

     For example, the `myfun` kernel:

     ```c++
     void myfun(
       hls::stream<din_t>& in_stream,
       hls::stream<dout_t>& out_stream
     ) {
     #pragma HLS INTERFACE ap_ctrl_none port=return
     #pragma HLS INTERFACE axis port=in_stream
     #pragma HLS INTERFACE axis port=out_stream
       ...
     }
     ```

   - Use AXI-Memory-Mapped interfaces for memory access. Likewise, in HLS, we can bind an interface to AXI using the INTERFACE pragma.

     ```c++
     #pragma HLS interface mode=m_axi port=<name> offset=slave bundle=data [OPTIONS]
     ```
     
     For example, the `top` kernel:
     
     ```c++
     void top(
       din_t in_data[IN_SIZE],
       dout_t out_data[OUT_SIZE]
     ) {
     #pragma HLS INTERFACE ap_ctrl_none port=return
     #pragma HLS INTERFACE m_axi port=in_data offset=slave bundle=data
     #pragma HLS INTERFACE m_axi port=out_data offset=slave bundle=data
     	... 
     }
     ```

2. Prepare a task graph definition in YAML format that defines the dataflow dependencies among kernels, including:

   - Kernel file
   - Batch size
   - Post-synthesized resource estimation
   - Dependency list for every kernel
   - ...

   An example definition can be seen in [lenet graph](app/lenet/kernel/graph.yaml).

3. Submit.

##### For vendors

1. Generate tcl scripts for HLS synthesizing.

   ```bash
   python host/hls/gen_tcl.py \
   	--output <output_path>
   ```

   Synthesize and export the kernels as RTL IP cores using Vitis HLS.

   ```bash
   cd <output_path>
   vitis_hls run_all.tcl
   ```

2. Generate wrappers for user HLS kernels:

    ```bash
    # for single file
    python utility/wrapper/gen_wrapper.py \
      --config <yaml file> \
      --output <output path>
    # OR for directory
    bash scripts/auto_wrap.sh <config firectory> <output directory>
    ```
    
3. Generate latency-insensitive interface router used in micro-service region:

    ```bash
    python utility/router/gen_router.py \
      --N_IN <number of LII input> \
      --N_OUT <number of LII output> \
      --output <output path>
    ```

4. Generate memory agent used in top service region:

    ```bash
    python utility/memory-agent/gen_ma.py \
      --output <output path>
    ```

5. Generate memory management unit in top service region:

    ```bash
    python utility/memory-management-unit/gen_mmu.py \
      --output <output path>
    ```

6. Generate tcl scripts for Xilinx Dynamic Function eXchange (DFX) flow:

    ```bash
    python host/dfx/gen_tcl.py
    ```

7. Submit user application using scheduler's API.

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

