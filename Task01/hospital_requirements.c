#include <stdio.h>

typedef struct {
    const char *env;
    int count_1hosp;
    double target_latency_ms;
    int safety_critical;
    const char *ranked_reqs;
} env_spec_t;

int main(void) {
    env_spec_t specs[] = {
        {"Bedside Patient Monitors", 500, 10.0, 1, "1.Response(<10ms) 2.Reliability 3.Security 4.Throughput 5.Battery(AC+UPS)"},
        {"Nurses Handheld Tablets",  120, 150.0, 0, "1.Battery(12h shift) 2.Security 3.Response 4.Reliability 5.Throughput"},
        {"Doctors Workstations",      80, 100.0, 0, "1.Response/UI 2.Throughput 3.Security 4.Reliability 5.Battery(N/A)"},
        {"Central Database Server",    2, 20.0,  1, "1.Reliability(99.999%) 2.Security 3.Throughput 4.Response 5.Battery(N/A)"},
        {"MRI Imaging Server",         1, 500.0, 0, "1.Throughput(Parallel) 2.Reliability 3.Security 4.Response 5.Battery(N/A)"},
        {"Partner Hospital Link",      3, 250.0, 0, "1.Security(mTLS/HIPAA) 2.Reliability 3.Throughput 4.Response 5.Battery(N/A)"}
    };
    printf("%-26s | %5s | %10s | %8s | %s\n", "Environment", "Units", "MaxLat(ms)", "Critical", "Ranked Requirements");
    printf("----------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < 6; i++) {
        printf("%-26s | %5d | %10.1f | %8s | %s\n",
               specs[i].env, specs[i].count_1hosp, specs[i].target_latency_ms,
               specs[i].safety_critical ? "YES" : "NO", specs[i].ranked_reqs);
    }
    return 0;
}
