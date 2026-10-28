#include <stdio.h>

int main(void) {
    double single_avail = 0.995; /* single SMP machine availability */
    double cluster_2node = 1.0 - (1.0 - single_avail) * (1.0 - single_avail);
    double p = 0.92; /* 92% parallelisable MRI reconstruction fraction */
    printf("Database Availability Comparison:\n");
    printf("  Single SMP Server Availability : %.4f%% (Downtime/yr: %.1f hours)\n",
           single_avail * 100.0, (1.0 - single_avail) * 8760.0);
    printf("  2-Node Active-Passive Cluster  : %.6f%% (Downtime/yr: %.2f hours)\n\n",
           cluster_2node * 100.0, (1.0 - cluster_2node) * 8760.0);
    printf("MRI Server SMP Speedup (Amdahl's Law, P = %.2f):\n", p);
    int cores[] = {1, 4, 8, 16, 32, 64};
    for (int i = 0; i < 6; i++) {
        int n = cores[i];
        double speedup = 1.0 / ((1.0 - p) + (p / n));
        printf("  N = %2d processors -> Speedup = %5.2fx\n", n, speedup);
    }
    return 0;
}
