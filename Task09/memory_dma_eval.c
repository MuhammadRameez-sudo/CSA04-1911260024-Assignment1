#include <stdio.h>

int main(void) {
    long mri_bytes = 500L * 1024L * 1024L; /* 500 MB MRI scan */
    long word_size = 4; /* 4-byte word */
    long num_words = mri_bytes / word_size;
    double isr_cost_us = 2.0; /* 2 microseconds per interrupt service + context switch */
    double intr_cpu_sec = (num_words * isr_cost_us) / 1e6;
    double dma_cpu_sec = 0.00005; /* 1 setup + 1 completion interrupt (~50 us CPU time) */

    printf("MRI Image Transfer Analysis (500 MB = %ld bytes, 4-byte words):\n", mri_bytes);
    printf("  Interrupt-Driven I/O (1 interrupt/word):\n");
    printf("    Total Interrupts : %ld (1.342 x 10^8 interrupts)\n", num_words);
    printf("    CPU Time Spent   : %.2f seconds (at %.1f us per ISR)\n", intr_cpu_sec, isr_cost_us);
    printf("  Direct Memory Access (DMA):\n");
    printf("    Total Interrupts : 1 (transfer-completion interrupt)\n");
    printf("    CPU Time Spent   : %.5f seconds (50 us setup + completion)\n", dma_cpu_sec);
    printf("    CPU Overhead Reduction Factor : %.0fx\n\n", intr_cpu_sec / dma_cpu_sec);

    /* Effective Access Time with Cache */
    double t_cache = 10.0; /* 10 ns RAM page-cache hit */
    double t_ssd = 100000.0; /* 100,000 ns (100 us) NVMe SSD miss */
    double hit_ratios[] = {0.0, 0.50, 0.90, 0.99};
    printf("Database Record Lookup Effective Access Time (RAM Cache = 10 ns, NVMe SSD = 100,000 ns):\n");
    for (int i = 0; i < 4; i++) {
        double h = hit_ratios[i];
        double eat_ns = h * t_cache + (1.0 - h) * (t_cache + t_ssd);
        printf("  Hit Ratio %5.1f%% -> EAT = %9.1f ns (%6.2f us)\n", h * 100.0, eat_ns, eat_ns / 1000.0);
    }
    return 0;
}
