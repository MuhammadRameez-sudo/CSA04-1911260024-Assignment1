# CSA04-1911260024-Assignment1
## Evaluate and Justify the Operating-System Design of a Smart Hospital (CO1)

- **Student Name:** Tharun M (Alternative / Team Member: Muhammad Rameez)
- **Register Number:** `1911260024` (Alternative: `1924260015`)
- **Course:** CSA04 – Operating Systems (Assignment 1 – CO1, 50 Marks)
- **Institution:** SIMATS Engineering (Saveetha Institute of Medical and Technical Sciences)
- **Linux Distribution & Kernel:** Debian GNU/Linux 12 (bookworm), Kernel `Linux x86_64`
- **Compiler & Version:** `gcc (Debian 12.2.0-14+deb12u1) 12.2.0`
- **Standard Compile Command:** `gcc -std=c11 -Wall -Wextra -O2 -pthread <source.c> -o <binary>`

---

## Repository Structure & Task Summary

| Folder | Task Title | Source / Config Files | How to Compile & Run | Key Result Summary |
| :--- | :--- | :--- | :--- | :--- |
| `Task01/` | Requirements of Each Computing Environment | `hospital_requirements.c`, `output.txt`, `screenshot.png` | `gcc -std=c11 -Wall -Wextra hospital_requirements.c -o hospital_requirements && ./hospital_requirements` | Ranked response-time, reliability, throughput, battery, and security across all 6 hospital environments; proved single GP-OS cannot guarantee `< 10 ms` monitor deadline. |
| `Task02/` | Type of Operating System for Each Environment | `os_types_eval.c`, `output.txt`, `screenshot.png` | `gcc -std=c11 -Wall -Wextra os_types_eval.c -o os_types_eval && ./os_types_eval` | Evaluated Hard RTOS, Handheld OS, Desktop OS, Active-Passive Cluster (`99.9975%` availability vs `99.5%` single machine), SMP (`10.59x` speedup at 64 cores), and Distributed Link. |
| `Task03/` | Operating-System Structures | `kernel_struct_bench.c`, `output.txt`, `screenshot.png` | `gcc -std=c11 -Wall -Wextra kernel_struct_bench.c -o kernel_struct_bench && ./kernel_struct_bench` | Benchmarked monolithic syscall vs microkernel IPC round-trip; justified Microkernel (QNX/seL4) for patient monitors and Modular Monolithic (Linux) for the database server. |
| `Task04/` | Operating-System Services | `os_services_protection.c`, `output.txt`, `screenshot.png` | `gcc -std=c11 -Wall -Wextra os_services_protection.c -o os_services_protection && ./os_services_protection` | Demonstrated hardware MMU page-level protection (`mprotect` + `SIGSEGV` isolation) and service pruning for bedside monitors vs full database server services. |
| `Task05/` | Examine System Calls | `syscall_demo.c`, `output.txt`, `screenshot.png` | `gcc -std=c11 -Wall -Wextra syscall_demo.c -o syscall_demo && strace ./syscall_demo` | Traced `openat()`, `write()`, `lseek()`, `read()`, and `close()` system calls and compared unbuffered `O(n/b)` syscalls against `stdio` `O(n/4096)` buffering. |
| `Task06/` | Complete the Code: Cost of System Calls (`copy_file`) | `copy_file.c`, `output.txt`, `screenshot.png` | `gcc -std=c11 -Wall -Wextra copy_file.c -o copy_file && ./copy_file && strace -c ./copy_file` | Completed `TODO 1`: verified exact outputs (`10001`, `21`, `4` `read()` calls for 1 B, 512 B, 4096 B buffers) and validated total `10,026` `read()` syscalls under `strace -c`. |
| `Task07/` | Complete the Code: Process Control System Calls | `shell_jobs.c`, `shell_jobs_unflushed.c`, `output.txt`, `screenshot.png` | `gcc -std=c11 -Wall -Wextra shell_jobs.c -o shell_jobs && ./shell_jobs` | Completed `TODO 1`: executed `expr 6 * 7` (`42`, exit status `0`) and job 2 (exit status `3`); demonstrated duplicate `"shell: running job 2"` when `fflush(stdout)` is omitted and `_exit(3)` is replaced by `exit(3)` on file redirection. |
| `Task08/` | CPU States and Protection | `cpu_protection_test.c`, `output.txt`, `screenshot.png` | `gcc -std=c11 -Wall -Wextra cpu_protection_test.c -o cpu_protection_test && ./cpu_protection_test` | Verified Ring-3 hardware trap (`SIGSEGV`) when attempting privileged `cli` instruction; compared interrupt latency (`~5 us`) against polling CPU waste (`100%`). |
| `Task09/` | Memory Hierarchy and I/O Systems | `memory_dma_eval.c`, `output.txt`, `screenshot.png` | `gcc -std=c11 -Wall -Wextra memory_dma_eval.c -o memory_dma_eval && ./memory_dma_eval` | Computed 500 MB MRI transfer overhead (`134,217,728` interrupts / `268.44 s` CPU time with interrupt-driven I/O vs `1` completion interrupt / `50 us` with DMA) and cache EAT. |
| `Task10/` | Measure and Compare | `measure_compare.c`, `output.txt`, `screenshot.png` | `gcc -std=c11 -O2 -Wall -Wextra -pthread measure_compare.c -o measure_compare && bash -c 'time -p ./measure_compare'` | Measured 1 MB copy across buffer sizes (`1 B` to `64 KB`) and compared creating 1,000 processes with `fork()` vs 1,000 threads with `pthread_create()`. |
| `Task11/` | Recommend and Defend the Operating-System Plan | `hospital-cluster-config.yaml`, `verify_plan.sh`, `output.txt`, `screenshot.png` | `./verify_plan.sh` | Synthesized the complete OS architecture across all 6 environments and 3-hospital federation, comparing physical isolation against VM consolidation. |
| `Task12/` | Day-4 Design Challenge: Home Monitoring & Cloud Records | `edge_cloud_sim.c`, `Dockerfile`, `output.txt`, `screenshot.png` | `gcc -std=c11 -Wall -Wextra edge_cloud_sim.c -o edge_cloud_sim && ./edge_cloud_sim` | Evaluated impact of internet-connected home monitors and cloud EHR; defended **MODIFY** strategy (Embedded Edge RTOS + local alarm loop + distributed cloud EHR). |

---

## Pushing to Your GitHub Account

This repository already contains **13 progressive commits** (one per task plus README). To push it directly to your GitHub account with the full commit history intact:

```bash
cd CSA04-1911260024-Assignment1
git branch -M main
git remote add origin https://github.com/<your-github-username>/CSA04-1911260024-Assignment1.git
git push -u origin main
```
